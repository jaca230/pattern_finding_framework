#include <iostream>
#include <memory>
#include "PFAlgorithmPipeline.h"
#include "PFDefaultPatternFormer.h"
#include "PFDefaultTrackletFormer.h"
#include "PFKMeansVertexFormer.h"
#include "PFPipelineObjectContainer.h"
#include "PFTracklet.h"
#include "PFVertex.h"
#include "PFPattern.h"
#include <nlohmann/json.hpp>

int main() {
    // Create pipeline
    PFAlgorithmPipeline pipeline;

    // Register stages (orderIndex is set in constructors)
    pipeline.registerStage(std::make_shared<PFDefaultTrackletFormer>());
    pipeline.registerStage(std::make_shared<PFKMeansVertexFormer>());
    pipeline.registerStage(std::make_shared<PFDefaultPatternFormer>());

    // Initial JSON input
    nlohmann::json input = {
        { "tracklet1", {1, 2, 3} },
        { "tracklet2", {4, 5, 6} }
    };

    // Wrap the input in a container
    std::shared_ptr<PFPipelineObjectContainer<nlohmann::json>> pipelineInput =
        std::make_shared<PFPipelineObjectContainer<nlohmann::json>>(input);

    // Run the pipeline step by step
    std::shared_ptr<PFPipelineObject> current = pipelineInput;
    int stageNumber = 0;

    while (pipeline.doNextStage(current)) {
        const std::map<int, nlohmann::json>& extraInfo = pipeline.getExtraInfo();
        if (extraInfo.count(stageNumber)) {
            std::cout << "Stage " << stageNumber << " extra info:\n"
                      << extraInfo.at(stageNumber).dump(4) << std::endl;
        } else {
            std::cout << "Stage " << stageNumber << " did not produce extra info.\n";
        }
        stageNumber++;
    }

    // Try to get the final output as a pattern container
    std::shared_ptr<PFPipelineObjectContainer<std::set<PFPattern>>> patternContainer =
        std::dynamic_pointer_cast<PFPipelineObjectContainer<std::set<PFPattern>>>(current);

    if (patternContainer) {
        const std::set<PFPattern>& patterns = patternContainer->get();
        std::cout << "Number of patterns generated: " << patterns.size() << std::endl;
    } else {
        std::cerr << "Pipeline did not return a valid pattern container." << std::endl;
    }

    return 0;
}
