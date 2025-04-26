#include "PFAlgorithmPipeline.h"
#include <algorithm>
#include <memory>

void PFAlgorithmPipeline::registerStage(std::shared_ptr<PFAbstractAlgorithmStage> stage) {
    // Add the stage to the pipeline, maintaining the order based on the order index
    stages_.push_back(stage);
    // Sort the stages by their order index to ensure correct execution order
    std::sort(stages_.begin(), stages_.end(), [](const auto& a, const auto& b) {
        return a->getOrderIndex() < b->getOrderIndex();
    });
}

std::unique_ptr<void> PFAlgorithmPipeline::runPipeline(std::unique_ptr<void> input) {
    std::unique_ptr<void> currentInput = std::move(input);

    // Execute the pipeline stage by stage using doNextStage
    while (doNextStage(currentInput)) {
        // Continue processing if there are more stages
    }

    // Return the final output after all stages are completed
    return currentInput;
}

bool PFAlgorithmPipeline::doNextStage(std::unique_ptr<void>& currentInput) {
    if (currentStageIndex_ < stages_.size()) {
        auto& stage = stages_[currentStageIndex_];

        // Update the "stage" node with the current stage info
        extraInfo_["stage"] = {
            {"id", stage->getId()},
            {"orderIndex", stage->getOrderIndex()}
        };

        // Run the stage
        currentInput = stage->run(std::move(currentInput));

        // Store detailed info under the stage's ID
        extraInfo_[std::to_string(stage->getId())] = {
            {"id", stage->getId()},
            {"orderIndex", stage->getOrderIndex()},
            {"extraInfo", stage->getExtraInfo()}
        };

        ++currentStageIndex_;
        return true;
    }

    return false;
}

