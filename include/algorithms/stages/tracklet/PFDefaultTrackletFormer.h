#pragma once
#include "PFTrackletFormer.h"

class PFDefaultTrackletFormer : public PFTrackletFormer {
public:
    // Constructor
    PFDefaultTrackletFormer();

protected:
    // Override the form method to implement default tracklet formation logic
    std::unordered_set<PFTracklet> form(const nlohmann::json& inputJson) override;
};
