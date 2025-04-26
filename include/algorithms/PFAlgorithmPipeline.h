#pragma once
#include <vector>
#include <memory>
#include <map>
#include "PFAlgorithmStage.h"
#include <nlohmann/json.hpp>  // Assuming you're using nlohmann/json for the extra info

template<typename InputType, typename OutputType>
class PFAlgorithmPipeline {
public:
    PFAlgorithmPipeline() = default;
    ~PFAlgorithmPipeline() = default;

    // Register an algorithm stage
    void registerStage(std::shared_ptr<PFAlgorithmStage<InputType, OutputType>> stage) {
        stages_.push_back(stage);
    }

    // Run the entire pipeline with an input and return the final output
    std::unique_ptr<OutputType> runPipeline(std::unique_ptr<InputType> input) {
        std::unique_ptr<OutputType> currentOutput = std::move(input);
        while (doNextStage(currentOutput)) {
            // Process each stage
        }
        return currentOutput;  // Final output after all stages
    }

    // Run one stage at a time, returns true if a stage is processed
    bool doNextStage(std::unique_ptr<InputType>& currentInput) {
        if (currentStageIndex_ < stages_.size()) {
            // Execute the current stage with the input
            auto& stage = stages_[currentStageIndex_];
            std::unique_ptr<OutputType> output = stage->run(std::move(currentInput));

            // Store extra information for the current stage
            extraInfo_[currentStageIndex_] = stage->getExtraInfo();

            // Prepare for the next stage
            currentInput = std::move(output);
            currentStageIndex_++;
            return true;
        }
        return false;  // All stages have been processed
    }

    // Get extra information collected from the pipeline
    const std::map<int, nlohmann::json>& getExtraInfo() const { return extraInfo_; }

private:
    std::vector<std::shared_ptr<PFAlgorithmStage<InputType, OutputType>>> stages_;
    std::map<int, nlohmann::json> extraInfo_;  // Collect information from each stage
    size_t currentStageIndex_ = 0;  // Keeps track of the next stage to execute
};
