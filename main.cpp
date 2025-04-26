#include <iostream>
#include <memory>
#include "PFAlgorithmPipeline.h"
#include "PFDefaultPatternFormer.h"
#include "PFDefaultTrackletFormer.h"
#include "PFKMeansVertexFormer.h"

int main() {
    // Create pipeline
    PFAlgorithmPipeline<nlohmann::json, std::set<PFPattern>> pipeline;

    // Register stages in the pipeline (types are already set in the classes themselves)
    pipeline.registerStage(std::make_shared<PFDefaultPatternFormer>());
    pipeline.registerStage(std::make_shared<PFDefaultTrackletFormer>());
    pipeline.registerStage(std::make_shared<PFKMeansVertexFormer>());

    // Prepare initial input for the pipeline (JSON for TrackletFormer)
    nlohmann::json input = { /* JSON content for tracklet formation */ };

    // Run the pipeline
    std::unique_ptr<std::set<PFPattern>> output = pipeline.runPipeline(std::make_unique<nlohmann::json>(input));

    // Display results (just a dummy output for this example)
    std::cout << "Number of patterns generated: " << output->size() << std::endl;

    return 0;
}

