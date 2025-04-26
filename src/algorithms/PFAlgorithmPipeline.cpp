#include "PFAlgorithmPipeline.h"

void PFAlgorithmPipeline::registerStage(std::shared_ptr<PFAlgorithmStage> stage) {
    // Insert the stage into the map, automatically sorted by orderIndex
    stages_.emplace(stage->getOrderIndex(), stage);
}

std::shared_ptr<PFPipelineObject> PFAlgorithmPipeline::runPipeline(std::shared_ptr<PFPipelineObject> input) {
    std::shared_ptr<PFPipelineObject> currentOutput = input;
    // Iterate over stages in order of their orderIndex
    for (const auto& stagePair : stages_) {
        auto& stage = stagePair.second;  // Get the stage from the map
        currentOutput = stage->run(currentOutput);  // Run the stage with the current input
    }
    return currentOutput;  // Final output after all stages
}

bool PFAlgorithmPipeline::doNextStage(std::shared_ptr<PFPipelineObject>& currentInput) {
    if (currentStageIndex_ < stages_.size()) {
        // Retrieve the stage from the map using the index as an iterator
        auto it = stages_.begin();
        std::advance(it, currentStageIndex_);
        auto& stage = it->second;

        // Execute the current stage with the input
        auto output = stage->run(currentInput);

        // Store extra information for the current stage
        extraInfo_[currentStageIndex_] = stage->getExtraInfo();

        // Prepare for the next stage
        currentInput = output;
        currentStageIndex_++;
        return true;
    }
    return false;  // All stages have been processed
}

const std::map<int, nlohmann::json>& PFAlgorithmPipeline::getExtraInfo() const {
    return extraInfo_;
}
