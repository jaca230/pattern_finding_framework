#include "PFPattern.h"

PFPattern::PFPattern(int patternId)
    : pattern_id_(patternId) {}

void PFPattern::addVertex(const VertexPtr& vertex) {
    vertices_.insert(vertex);
}

const std::unordered_set<PFPattern::VertexPtr>& PFPattern::getVertices() const {
    return vertices_;
}

std::unordered_set<PFPattern::TrackletPtr> PFPattern::getUniqueTracklets() const {
    std::unordered_set<TrackletPtr> uniqueTracklets;
    for (const auto& vertex : vertices_) {
        const auto& tracklets = vertex->getTracklets();
        uniqueTracklets.insert(tracklets.begin(), tracklets.end());
    }
    return uniqueTracklets;
}

int PFPattern::getId() const {
    return pattern_id_;
}
