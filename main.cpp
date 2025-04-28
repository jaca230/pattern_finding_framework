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

    // Specify sigma and n_iters when creating PFKMeansVertexFormer
    double sigma = 1.5;    // Example sigma value
    int n_iters = 4;       // Example number of tiers
    pipeline->registerStage(std::make_shared<PFKMeansVertexFormer>(sigma, n_iters));

    pipeline->registerStage(std::make_shared<PFDefaultPatternFormer>());

    event.setPipeline(pipeline);

    // Prepare input
    nlohmann::json input = {
        { "tracklet1", {
            { "particle_id", 100 },
            { "endpoint0", {0.0, 0.0, 0.0} },
            { "endpoint1", {1.0, 1.0, 1.0} }
        }},
        { "tracklet2", {
            { "particle_id", 101 },
            { "endpoint0", {1.05, 1.05, 1.05} },
            { "endpoint1", {2.0, 2.0, 2.0} }
        }},
        { "tracklet3", {
            { "particle_id", 102 },
            { "endpoint0", {2.05, 2.05, 2.05} },
            { "endpoint1", {3.0, 3.0, 3.0} }
        }}
    };
    
    std::shared_ptr<PFPipelineObjectContainer<nlohmann::json>> pipelineInput =
        std::make_shared<PFPipelineObjectContainer<nlohmann::json>>(input);

    // Run pipeline
    std::shared_ptr<PFPipelineObject> output = event.getPipeline()->runPipeline(pipelineInput);

    // Cast and set patterns
    std::shared_ptr<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFPattern>>>> patternContainer =
        std::dynamic_pointer_cast<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFPattern>>>>(output);

    if (!patternContainer) {
        std::cerr << "Error: Final output is not a pattern set." << std::endl;
        return 1;
    }

    event.setPatterns(patternContainer->get());

    // Show results
    std::cout << "Event ID: " << event.getEventId() << std::endl;
    std::cout << "Number of patterns: " << event.getPatterns().size() << std::endl;

    // Print unique tracklets in each pattern
    for (const auto& pattern : event.getPatterns()) {
        std::cout << "Pattern ID: " << pattern->getId() << std::endl;  // Use `->` instead of `.`
        const auto& uniqueTracklets = pattern->getUniqueTracklets();  // Use `->` instead of `.`

        std::cout << "Unique Tracklets in this pattern:" << std::endl;
        for (const auto& tracklet : uniqueTracklets) {
            std::cout << "  Tracklet ID: " << tracklet->getTrackletId() << std::endl;
        }
    }

    return 0;
}
