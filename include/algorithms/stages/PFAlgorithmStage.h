#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "PFPipelineObject.h"

class PFAlgorithmStage {
public:
    virtual ~PFAlgorithmStage() = default;

    // Run method to process input and return output
    std::shared_ptr<PFPipelineObject> run(std::shared_ptr<PFPipelineObject> input);

    // Order for PFAlgorithmStage
    int getOrderIndex() const;
    void setOrderIndex(int index);

    std::string getId() const;
    void setId(std::string id);

    // Extra information
    const nlohmann::json& getExtraInfo() const;
    void setExtraInfo(const nlohmann::json& info);

protected:
    // Derived class must implement this method to convert input into output
    virtual std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) = 0;

private:
    int orderIndex_ = -1;
    std::string id_ = "default-stage-id";
    nlohmann::json extraInfo_;  // Store additional info about the stage
};
