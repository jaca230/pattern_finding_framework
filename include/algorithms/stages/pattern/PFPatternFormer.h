#pragma once
#include "PFAlgorithmStage.h"
#include "PFPipelineObjectContainer.h"
#include "PFVertex.h"
#include "PFPattern.h"
#include <unordered_set>
#include <memory>

class PFPatternFormer : public PFAlgorithmStage {
public:
    PFPatternFormer() {
        setOrderIndex(300);  // Set the order index specific to PFPatternFormer
        setId("pattern-former");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        std::shared_ptr<PFPipelineObjectContainer<std::unordered_set<PFVertex>>> vertexContainer =
            std::dynamic_pointer_cast<PFPipelineObjectContainer<std::unordered_set<PFVertex>>>(input);
        if (!vertexContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<std::unordered_set<PFVertex>>");
        }

        const std::unordered_set<PFVertex>& vertexSet = vertexContainer->get();
        std::unordered_set<PFPattern> patterns = form(vertexSet);

        return std::make_shared<PFPipelineObjectContainer<std::unordered_set<PFPattern>>>(patterns);
    }

    virtual std::unordered_set<PFPattern> form(const std::unordered_set<PFVertex>& vertexSet) = 0;
};
