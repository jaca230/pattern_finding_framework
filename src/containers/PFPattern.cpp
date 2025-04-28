#include "PFPattern.h"

// Constructor
PFPattern::PFPattern(int patternId)
    : pattern_id_(patternId) {}

// Add a single vertex to the pattern
void PFPattern::addVertex(const VertexPtr& vertex) {
    vertices_.insert(vertex);
}

// Add multiple vertices to the pattern
void PFPattern::addVertices(const std::unordered_set<VertexPtr>& vertices) {
    vertices_.insert(vertices.begin(), vertices.end());
}

// Get all vertices in the pattern
const std::unordered_set<PFPattern::VertexPtr>& PFPattern::getVertices() const {
    return vertices_;
}

// Get all unique tracklets across all vertices
std::unordered_set<std::shared_ptr<PFTracklet>> PFPattern::getUniqueTracklets() const {
    std::unordered_set<int> trackletIds;  // Store the IDs of tracklets we've already seen
    std::unordered_set<std::shared_ptr<PFTracklet>> uniqueTracklets;

    for (const auto& vertex : vertices_) {
        const auto& tracklets = vertex->getTracklets();
        for (const auto& tracklet : tracklets) {
            // Only insert the tracklet if its ID is not already in the set
            if (trackletIds.find(tracklet->getTrackletId()) == trackletIds.end()) {
                uniqueTracklets.insert(tracklet);
                trackletIds.insert(tracklet->getTrackletId());  // Mark the ID as seen
            }
        }
    }

    return uniqueTracklets;  // Return unique tracklet objects based on tracklet ID
}


// Get the pattern ID
int PFPattern::getId() const {
    return pattern_id_;
}
