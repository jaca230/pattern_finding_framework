#include "PFVertex.h"

// Constructor
PFVertex::PFVertex(int vertexId)
    : vertex_id_(vertexId) {}

// Add a single tracklet to the vertex (shared_ptr version)
void PFVertex::addTracklet(const std::shared_ptr<PFTracklet>& tracklet) {
    tracklets_.insert(tracklet);  // Insert the shared_ptr directly
}

// Add a single tracklet to the vertex (non-shared_ptr version)
void PFVertex::addTracklet(PFTracklet tracklet) {
    addTracklet(std::make_shared<PFTracklet>(std::move(tracklet)));  // Convert to shared_ptr and delegate
}

// Add multiple tracklets to the vertex (shared_ptr version)
void PFVertex::addTracklets(const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets) {
    for (const auto& tracklet : tracklets) {
        addTracklet(tracklet);  // Delegate to addTracklet(shared_ptr)
    }
}

// Add multiple tracklets to the vertex (non-shared_ptr version)
void PFVertex::addTracklets(const std::unordered_set<PFTracklet>& tracklets) {
    for (const auto& tracklet : tracklets) {
        addTracklet(tracklet);  // Delegate to addTracklet(non-shared_ptr)
    }
}

// Set tracklets (clear existing ones and add new) (shared_ptr version)
void PFVertex::setTracklets(const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets) {
    tracklets_.clear();  // Clear existing tracklets first
    addTracklets(tracklets);  // Delegate to addTracklets(shared_ptr)
}

// Set tracklets (clear existing ones and add new) (non-shared_ptr version)
void PFVertex::setTracklets(const std::unordered_set<PFTracklet>& tracklets) {
    tracklets_.clear();  // Clear existing tracklets first
    addTracklets(tracklets);  // Delegate to addTracklets(non-shared_ptr)
}

// Get the vertex ID
int PFVertex::getId() const {
    return vertex_id_;
}

const std::unordered_set<std::shared_ptr<PFTracklet>>& PFVertex::getTracklets() const {
    return tracklets_;
}
