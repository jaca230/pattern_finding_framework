#pragma once
#include "PFTracklet.h"
#include <unordered_set>
#include <memory>

class PFVertex {
public:
    PFVertex(int vertexId);

    // Accessor to get all tracklets
    const std::unordered_set<std::shared_ptr<PFTracklet>>& getTracklets() const;

    // Add a single tracklet to the vertex (shared_ptr version)
    void addTracklet(const std::shared_ptr<PFTracklet>& tracklet);

    // Add a single tracklet to the vertex (non-shared_ptr version)
    void addTracklet(PFTracklet tracklet);

    // Add multiple tracklets to the vertex (shared_ptr version)
    void addTracklets(const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets);

    // Add multiple tracklets to the vertex (non-shared_ptr version)
    void addTracklets(const std::unordered_set<PFTracklet>& tracklets);

    // Set tracklets (clear existing ones and add new)
    void setTracklets(const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets);

    // Set tracklets (clear existing ones and add new) (non-shared_ptr version)
    void setTracklets(const std::unordered_set<PFTracklet>& tracklets);

    // Get the vertex ID
    int getId() const;

    // Equality operator for unordered_set
    bool operator==(const PFVertex& other) const {
        return vertex_id_ == other.vertex_id_;
    }

private:
    int vertex_id_;
    std::unordered_set<std::shared_ptr<PFTracklet>> tracklets_;
};

// Hash specialization for PFVertex
namespace std {
    template <>
    struct hash<PFVertex> {
        std::size_t operator()(const PFVertex& vertex) const {
            return std::hash<int>()(vertex.getId());  // Hash based on vertex ID
        }
    };
}
