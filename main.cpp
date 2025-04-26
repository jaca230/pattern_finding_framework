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
#include "PFEvent.h"
#include <nlohmann/json.hpp>

int main() {
    // Create event
    PFEvent event(42);

    // Create and configure pipeline
    std::shared_ptr<PFAlgorithmPipeline> pipeline = std::make_shared<PFAlgorithmPipeline>();
    pipeline->registerStage(std::make_shared<PFDefaultTrackletFormer>());
    pipeline->registerStage(std::make_shared<PFKMeansVertexFormer>());
    pipeline->registerStage(std::make_shared<PFDefaultPatternFormer>());

    event.setPipeline(pipeline);

    // Prepare input
    nlohmann::json input = {
        { "tracklet1", {1, 2, 3} },
        { "tracklet2", {4, 5, 6} }
    };
    std::shared_ptr<PFPipelineObjectContainer<nlohmann::json>> pipelineInput =
        std::make_shared<PFPipelineObjectContainer<nlohmann::json>>(input);

    // Run pipeline
    std::shared_ptr<PFPipelineObject> output = event.getPipeline()->runPipeline(pipelineInput);

    // Cast and set patterns
    std::shared_ptr<PFPipelineObjectContainer<std::set<PFPattern>>> patternContainer =
        std::dynamic_pointer_cast<PFPipelineObjectContainer<std::set<PFPattern>>>(output);

    if (!patternContainer) {
        std::cerr << "Error: Final output is not a pattern set." << std::endl;
        return 1;
    }

    event.setPatterns(patternContainer->get());

    // Show results
    std::cout << "Event ID: " << event.getEventId() << std::endl;
    std::cout << "Number of patterns: " << event.getPatterns().size() << std::endl;

    // Dump entire extra info as a JSON
    const nlohmann::json& extraInfo = event.getPipeline()->getExtraInfo();
    std::cout << "Pipeline Extra Info:\n" << extraInfo.dump(4) << std::endl;

    return 0;
}
