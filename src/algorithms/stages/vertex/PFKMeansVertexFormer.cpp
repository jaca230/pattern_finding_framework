#include "PFKMeansVertexFormer.h"

// Constructor implementation
PFKMeansVertexFormer::PFKMeansVertexFormer() {
    // No changes to order index; keeping it the same
}

// Override the form method to return an empty set for now
std::set<PFVertex> PFKMeansVertexFormer::form(const std::set<PFTracklet>& input) {
    // Add some extra info to the stage's extraInfo_
    nlohmann::json info;
    info["processed_tracklets_count"] = input.size();
    setExtraInfo(info);  // Store extra information in the base class

    // For now, just return an empty set
    return std::set<PFVertex>(); 
}
