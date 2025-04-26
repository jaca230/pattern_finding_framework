#pragma once
#include "PFTrackletFormer.h"

class PFDefaultTrackletFormer : public PFTrackletFormer {
public:
    // Constructor
    PFDefaultTrackletFormer();

    // Override the runImpl method to implement default behavior (returning an empty set for now)
    std::set<PFTracklet> runImpl(const nlohmann::json& input) override;
};
