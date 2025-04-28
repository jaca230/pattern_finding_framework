#pragma once
#include "PFTrackletFormer.h"
#include <memory>

class PFDefaultTrackletFormer : public PFTrackletFormer {
public:
    PFDefaultTrackletFormer();

protected:
    std::unordered_set<std::shared_ptr<PFTracklet>> form(const nlohmann::json& inputJson) override;
};
