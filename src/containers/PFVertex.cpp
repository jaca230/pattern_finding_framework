#include "PFVertex.h"

PFVertex::PFVertex(int vertexId)
    : vertex_id_(vertexId) {}

void PFVertex::addTracklet(const TrackletPtr& tracklet) {
    tracklets_.insert(tracklet);
}

const std::unordered_set<PFVertex::TrackletPtr>& PFVertex::getTracklets() const {
    return tracklets_;
}

int PFVertex::getId() const {
    return vertex_id_;
}
