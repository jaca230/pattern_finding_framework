#pragma once
#include "PFTracklet.h"
#include <unordered_set>
#include <memory>

class PFVertex {
public:
    using TrackletPtr = std::shared_ptr<PFTracklet>;

    PFVertex(int vertexId);

    // Add a tracklet to the vertex
    void addTracklet(const TrackletPtr& tracklet);

    // Get all tracklets
    const std::unordered_set<TrackletPtr>& getTracklets() const;

    // Get the vertex ID
    int getId() const;

    // Equality operator for unordered_set
    bool operator==(const PFVertex& other) const {
        return vertex_id_ == other.vertex_id_;
    }

private:
    int vertex_id_;
    std::unordered_set<TrackletPtr> tracklets_;
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