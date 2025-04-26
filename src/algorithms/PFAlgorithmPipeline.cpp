#include "PFAlgorithmPipeline.h"

void PFAlgorithmPipeline::registerStage(std::shared_ptr<PFAlgorithmStage> stage) {
    // Insert the stage into the map, automatically sorted by orderIndex
    stages_.emplace(stage->getOrderIndex(), stage);
}

std::shared_ptr<PFPipelineObject> PFAlgorithmPipeline::runPipeline(std::shared_ptr<PFPipelineObject> input) {
    std::shared_ptr<PFPipelineObject> currentInput = input;

    // Iterate over stages and call doNextStage for each stage
    for (std::map<int, std::shared_ptr<PFAlgorithmStage>>::iterator stagePair = stages_.begin(); stagePair != stages_.end(); ++stagePair) { // Loop over all stages
        // Call doNextStage to run the current stage
        if (!doNextStage(currentInput)) {
            // If doNextStage returns false, stop processing further stages
            // This should never happen in a well-formed pipeline
            break;
        }
    }

    return currentInput;  // Final output after all stages
}

bool PFAlgorithmPipeline::doNextStage(std::shared_ptr<PFPipelineObject>& currentInput) {
    if (currentStageIndex_ < stages_.size()) {
        // Retrieve the stage from the map using the index as an iterator
        std::map<int, std::shared_ptr<PFAlgorithmStage>>::iterator it = stages_.begin();
        std::advance(it, currentStageIndex_);
        std::shared_ptr<PFAlgorithmStage> stage = it->second;

        // Execute the current stage with the input
        std::shared_ptr<PFPipelineObject> output = stage->run(currentInput);

        // Append the extra information from the current stage to the `extraInfo_` JSON
        extraInfo_[stage->getId()] = stage->getExtraInfo();

        // Prepare for the next stage
        currentInput = output;
        currentStageIndex_++;
        return true;
    }
    return false;  // All stages have been processed
}

const nlohmann::json& PFAlgorithmPipeline::getExtraInfo() const {
    return extraInfo_;
}
