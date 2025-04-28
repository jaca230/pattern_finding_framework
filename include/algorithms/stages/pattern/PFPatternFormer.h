#pragma once
#include "PFAlgorithmStage.h"
#include "PFPipelineObjectContainer.h"
#include "PFVertex.h"
#include "PFPattern.h"
#include <unordered_set>
#include <memory>
#include <stdexcept>

class PFPatternFormer : public PFAlgorithmStage {
public:
    PFPatternFormer() {
        setOrderIndex(300);  // Set the order index specific to PFPatternFormer
        setId("pattern-former");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        // Dynamically cast the input to the appropriate container type
        std::shared_ptr<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFVertex>>>> vertexContainer =
            std::dynamic_pointer_cast<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFVertex>>>>(input);
        if (!vertexContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFVertex>>>");
        }

        // Retrieve the set of shared_ptr<PFVertex> objects from the container
        const std::unordered_set<std::shared_ptr<PFVertex>>& vertexSet = vertexContainer->get();
        
        // Call the form method to create a set of shared_ptr<PFPattern>
        std::unordered_set<std::shared_ptr<PFPattern>> patterns = form(vertexSet);

        // Return a new container holding the patterns
        return std::make_shared<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFPattern>>>>(patterns);
    }

    // The form method should now accept and return shared pointers
    virtual std::unordered_set<std::shared_ptr<PFPattern>> form(
        const std::unordered_set<std::shared_ptr<PFVertex>>& vertexSet) = 0;
};
