#include <iostream>
#include <memory>
#include "PFAlgorithmPipeline.h"
#include "PFDefaultPatternFormer.h"
#include "PFDefaultTrackletFormer.h"
#include "PFKMeansVertexFormer.h"
#include "PFJson.h"

int main() {
    // Create pipeline
    PFAlgorithmPipeline pipeline;

    // Register stages in the pipeline (ensure orderIndex is set correctly in each class constructor)
    pipeline.registerStage(std::make_shared<PFDefaultTrackletFormer>());
    pipeline.registerStage(std::make_shared<PFKMeansVertexFormer>());
    pipeline.registerStage(std::make_shared<PFDefaultPatternFormer>());

    // Prepare initial input for the pipeline (JSON for TrackletFormer)
    nlohmann::json input = {
        { "tracklet1", {1, 2, 3} },
        { "tracklet2", {4, 5, 6} }
    };

    // Create the PFJson object from the input JSON
    std::shared_ptr<PFJson> pipelineInput = std::make_shared<PFJson>(input);

    // Run the pipeline one stage at a time
    std::shared_ptr<PFPipelineObject> current = pipelineInput;
    int stageNumber = 0;
    while (pipeline.doNextStage(current)) {
        const auto& extraInfo = pipeline.getExtraInfo();
        if (extraInfo.count(stageNumber)) {
            std::cout << "Stage " << stageNumber << " extra info:\n"
                      << extraInfo.at(stageNumber).dump(4) << std::endl;
        } else {
            std::cout << "Stage " << stageNumber << " did not produce extra info.\n";
        }
        stageNumber++;
    }

    // Final output casting
    std::shared_ptr<PFPatternSet> patternSetOutput = std::dynamic_pointer_cast<PFPatternSet>(current);
    if (patternSetOutput) {
        std::cout << "Number of patterns generated: "
                  << patternSetOutput->getPatterns().size() << std::endl;
    } else {
        std::cerr << "Pipeline did not return a valid PFPatternSet." << std::endl;
    }

    return 0;
}
