#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include <nlohmann/json.hpp>
#include <set>

class PFTrackletFormer : public PFAlgorithmStage<nlohmann::json, std::set<PFTracklet>> {
public:
    PFTrackletFormer() {
        setOrderIndex(100);  // Set the order index specific to TrackletFormer
    }

protected:
    // Pure virtual method to be implemented by derived classes.
    std::set<PFTracklet> runImpl(const nlohmann::json& input) override = 0; 
};
