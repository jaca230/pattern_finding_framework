#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFTrackletSet.h"
#include "PFJson.h"  // Include the PFJson class
#include <set>
#include <memory>

class PFTrackletFormer : public PFAlgorithmStage {
public:
    PFTrackletFormer() {
        setOrderIndex(100);  // Set the order index specific to PFTrackletFormer
    }

protected:
    // The runImpl method will handle the conversion and call the abstract method.
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        // Cast the input to PFJson (assuming the input is a PFJson object)
        std::shared_ptr<PFJson> jsonInput = std::dynamic_pointer_cast<PFJson>(input);
        if (!jsonInput) {
            throw std::invalid_argument("Input must be of type PFJson");
        }

        // Call the abstract method to form the tracklets using the PFJson object
        std::set<PFTracklet> tracklets = form(jsonInput->getJson());

        // Return the result as a PFTrackletSet
        return std::make_shared<PFTrackletSet>(tracklets);
    }

    // Abstract method that will be implemented by derived classes to form tracklets
    virtual std::set<PFTracklet> form(const nlohmann::json& inputJson) = 0;
};
