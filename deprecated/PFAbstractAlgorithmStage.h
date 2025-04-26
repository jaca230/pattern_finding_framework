#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

class PFAbstractAlgorithmStage {
public:
    virtual ~PFAbstractAlgorithmStage() = default;

    // Run method to process input and return new output
    virtual std::unique_ptr<void> run(std::unique_ptr<void> input) = 0;

    virtual int getOrderIndex() const = 0;
    virtual void setOrderIndex(int index) = 0;

    virtual int getId() const = 0;
    virtual void setId(int id) = 0;

    // Methods for extra information related to the stage
    virtual const nlohmann::json& getExtraInfo() const = 0;
    virtual void setExtraInfo(const nlohmann::json& info) = 0;
};
