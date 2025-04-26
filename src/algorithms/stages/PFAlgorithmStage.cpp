#include "PFAlgorithmStage.h"

// This method executes the stage logic and returns a new PFPipelineObject
std::shared_ptr<PFPipelineObject> PFAlgorithmStage::run(std::shared_ptr<PFPipelineObject> input) {
    return runImpl(input);  // Call the abstract method implemented in derived classes
}

void PFAlgorithmStage::setOrderIndex(int index) {
    orderIndex_ = index;
}

int PFAlgorithmStage::getOrderIndex() const {
    return orderIndex_;
}

void PFAlgorithmStage::setId(std::string id) {
    id_ = id;
}

std::string PFAlgorithmStage::getId() const {
    return id_;
}

void PFAlgorithmStage::setExtraInfo(const nlohmann::json& info) {
    extraInfo_ = info;
}

const nlohmann::json& PFAlgorithmStage::getExtraInfo() const {
    return extraInfo_;
}
