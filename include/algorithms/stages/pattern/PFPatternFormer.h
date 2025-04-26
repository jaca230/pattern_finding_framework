#pragma once
#include "PFAlgorithmStage.h"
#include "PFPipelineObjectContainer.h"
#include "PFVertex.h"
#include "PFPattern.h"
#include <set>
#include <memory>

class PFPatternFormer : public PFAlgorithmStage {
public:
    PFPatternFormer() {
        setOrderIndex(300);  // Set the order index specific to PFPatternFormer
        setId("pattern-former");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        std::shared_ptr<PFPipelineObjectContainer<std::set<PFVertex>>> vertexContainer =
            std::dynamic_pointer_cast<PFPipelineObjectContainer<std::set<PFVertex>>>(input);
        if (!vertexContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<std::set<PFVertex>>");
        }

        const std::set<PFVertex>& vertexSet = vertexContainer->get();
        std::set<PFPattern> patterns = form(vertexSet);

        return std::make_shared<PFPipelineObjectContainer<std::set<PFPattern>>>(patterns);
    }

    virtual std::set<PFPattern> form(const std::set<PFVertex>& vertexSet) = 0;
};
