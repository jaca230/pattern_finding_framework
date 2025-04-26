#pragma once
#include <nlohmann/json.hpp>

class PFJson : public PFPipelineObject {
public:
    PFJson() = default;
    PFJson(const nlohmann::json& json) : jsonData_(json) {}

    // Accessor for the raw JSON data
    const nlohmann::json& getJson() const { return jsonData_; }

    // Set the JSON data
    void setJson(const nlohmann::json& json) { jsonData_ = json; }

    // You can implement methods to extract specific data as needed, for example:
    template <typename T>
    T getValue(const std::string& key) const {
        return jsonData_.at(key).get<T>();
    }

    // Optionally, helper methods to handle specific types of data
    bool contains(const std::string& key) const {
        return jsonData_.contains(key);
    }

private:
    nlohmann::json jsonData_;
};
