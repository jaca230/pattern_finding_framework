#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFPipelineObjectContainer.h"
#include <unordered_set>
#include <memory>
#include <nlohmann/json.hpp>

class PFTrackletFormer : public PFAlgorithmStage {
public:
    PFTrackletFormer() {
        setOrderIndex(100);  // Set the order index specific to PFTrackletFormer
        setId("tracklet-former");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        std::shared_ptr<PFPipelineObjectContainer<nlohmann::json>> jsonContainer =
            std::dynamic_pointer_cast<PFPipelineObjectContainer<nlohmann::json>>(input);
        if (!jsonContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<nlohmann::json>");
        }

        const nlohmann::json& inputJson = jsonContainer->get();
        std::unordered_set<std::shared_ptr<PFTracklet>> tracklets = form(inputJson);

        return std::make_shared<PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFTracklet>>>>(tracklets);
    }

    virtual std::unordered_set<std::shared_ptr<PFTracklet>> form(const nlohmann::json& inputJson) = 0;
};
