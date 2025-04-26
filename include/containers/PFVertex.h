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

private:
    int vertex_id_;
    std::unordered_set<TrackletPtr> tracklets_;
};
