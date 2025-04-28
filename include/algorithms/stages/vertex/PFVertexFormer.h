#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFVertex.h"
#include "PFPipelineObjectContainer.h"
#include <unordered_set>
#include <memory>

class PFVertexFormer : public PFAlgorithmStage {
public:
    PFVertexFormer() {
        setOrderIndex(200);  // Set the order index specific to PFVertexFormer
        setId("vertex-former");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        std::shared_ptr<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFTracklet>>>> trackletContainer =
            std::dynamic_pointer_cast<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFTracklet>>>>(input);
        if (!trackletContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFTracklet>>>");
        }

        std::unordered_set<std::shared_ptr<PFVertex>> vertices = form(trackletContainer->get());

        return std::make_shared<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFVertex>>>>(vertices);
    }

    virtual std::unordered_set<std::shared_ptr<PFVertex>> form(const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets) = 0;
};
