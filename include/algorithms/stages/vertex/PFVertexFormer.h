#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFVertex.h"
#include "PFPipelineObjectContainer.h"
#include <set>
#include <memory>

class PFVertexFormer : public PFAlgorithmStage {
public:
    PFVertexFormer() {
        setOrderIndex(200);  // Set the order index specific to PFVertexFormer
        setId("vertex-former");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        std::shared_ptr<PFPipelineObjectContainer<std::set<PFTracklet>>> trackletContainer =
            std::dynamic_pointer_cast<PFPipelineObjectContainer<std::set<PFTracklet>>>(input);
        if (!trackletContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<std::set<PFTracklet>>");
        }

        std::set<PFVertex> vertices = form(trackletContainer->get());

        return std::make_shared<PFPipelineObjectContainer<std::set<PFVertex>>>(vertices);
    }

    virtual std::set<PFVertex> form(const std::set<PFTracklet>& tracklets) = 0;
};
