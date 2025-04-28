#include "PFPattern.h"

// Constructor
PFPattern::PFPattern(int patternId)
    : pattern_id_(patternId) {}

// Add a single vertex to the pattern (shared_ptr version)
void PFPattern::addVertex(const std::shared_ptr<PFVertex>& vertex) {
    vertices_.insert(vertex);
}

// Add a single vertex to the pattern (non-shared_ptr version)
void PFPattern::addVertex(PFVertex vertex) {
    vertices_.insert(std::make_shared<PFVertex>(std::move(vertex)));  // Convert to shared_ptr and insert
}

// Add multiple vertices to the pattern (shared_ptr version)
void PFPattern::addVertices(const std::unordered_set<std::shared_ptr<PFVertex>>& vertices) {
    for (const auto& vertex : vertices) {
        addVertex(vertex);  // Reuse the shared_ptr version to centralize logic
    }
}

// Add multiple vertices to the pattern (non-shared_ptr version)
void PFPattern::addVertices(const std::unordered_set<PFVertex>& vertices) {
    for (const auto& vertex : vertices) {
        addVertex(vertex);  // Reuse the non-shared_ptr version to centralize logic
    }
}

// Set vertices (clear and then add new) (shared_ptr version)
void PFPattern::setVertices(const std::unordered_set<std::shared_ptr<PFVertex>>& vertices) {
    vertices_.clear();  // Clear existing vertices
    addVertices(vertices);  // Add new vertices
}

// Set vertices (clear and then add new) (non-shared_ptr version)
void PFPattern::setVertices(const std::unordered_set<PFVertex>& vertices) {
    vertices_.clear();  // Clear existing vertices
    addVertices(vertices);  // Add new vertices
}

// Get all vertices in the pattern
const std::unordered_set<std::shared_ptr<PFVertex>>& PFPattern::getVertices() const {
    return vertices_;
}

// Get all unique tracklets across all vertices
std::unordered_set<std::shared_ptr<PFTracklet>> PFPattern::getUniqueTracklets() const {
    std::unordered_set<std::shared_ptr<PFTracklet>> uniqueTracklets;

    for (const auto& vertex : vertices_) {
        const auto& tracklets = vertex->getTracklets();
        for (const auto& tracklet : tracklets) {
            uniqueTracklets.insert(tracklet);  // insert does nothing if already present
        }
    }

    return uniqueTracklets;
}

// Get the pattern ID
int PFPattern::getId() const {
    return pattern_id_;
}
