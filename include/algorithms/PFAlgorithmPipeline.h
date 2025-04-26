#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "PFAlgorithmStage.h"
#include "PFPipelineObject.h"

class PFAlgorithmPipeline {
public:
    PFAlgorithmPipeline() = default;
    ~PFAlgorithmPipeline() = default;

    // Register an algorithm stage
    void registerStage(std::shared_ptr<PFAlgorithmStage> stage);

    // Run the entire pipeline with an input and return the final output
    std::shared_ptr<PFPipelineObject> runPipeline(std::shared_ptr<PFPipelineObject> input);

    // Run one stage at a time, returns true if a stage is processed
    bool doNextStage(std::shared_ptr<PFPipelineObject>& currentInput);

    // Get extra information collected from the pipeline
    const nlohmann::json& getExtraInfo() const;

private:
    std::map<int, std::shared_ptr<PFAlgorithmStage>> stages_;  // Map stages by their orderIndex
    nlohmann::json extraInfo_;  // Collect all extra information as a single JSON object
    size_t currentStageIndex_ = 0;  // Keeps track of the next stage to execute
};
