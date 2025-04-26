#pragma once
#include "PFAlgorithmStage.h"
#include "PFVertexSet.h"
#include "PFPatternSet.h"
#include <memory>

class PFPatternFormer : public PFAlgorithmStage {
public:
    PFPatternFormer() {
        setOrderIndex(300);  // Set the order index specific to PFPatternFormer
    }

protected:
    // The runImpl method will handle the conversion and call the abstract method.
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        // Cast the input to PFVertexSet (assuming the input is a PFVertexSet object)
        std::shared_ptr<PFVertexSet> vertexSetInput = std::dynamic_pointer_cast<PFVertexSet>(input);
        if (!vertexSetInput) {
            throw std::invalid_argument("Input must be of type PFVertexSet");
        }

        // Call the abstract method to form the patterns
        std::set<PFPattern> patterns = form(vertexSetInput->getVertices());

        // Return the result as a PFPatternSet
        return std::make_shared<PFPatternSet>(patterns);
    }

    // Abstract method that will be implemented by derived classes to form patterns
    virtual std::set<PFPattern> form(const std::set<PFVertex>& vertexSet) = 0;
};
