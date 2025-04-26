#pragma once
#include <memory>
#include <nlohmann/json.hpp>

template<typename InputType, typename OutputType>
class PFAlgorithmStage {
public:
    virtual ~PFAlgorithmStage() = default;

    // Run method to process input and return output
    std::unique_ptr<OutputType> run(std::unique_ptr<InputType> input) {
        OutputType output = runImpl(*input);  // Process input into output
        return std::make_unique<OutputType>(output);
    }

    // Order for PFBaseAlgorithmStage stage
    int getOrderIndex() const { return orderIndex_; }
    void setOrderIndex(int index) { orderIndex_ = index; }

    int getId() const { return id_; }
    void setId(int id) { id_ = id; }

    // Implement extra info methods for metadata
    const nlohmann::json& getExtraInfo() const { return extraInfo_; }
    void setExtraInfo(const nlohmann::json& info) { extraInfo_ = info; }

protected:
    // Derived class must implement this method to convert input into output
    virtual OutputType runImpl(const InputType& input) = 0;

private:
    int orderIndex_ = -1;  // Default order index, can be overridden
    int id_ = -1;
    nlohmann::json extraInfo_;  // Store additional info about the stage
};
