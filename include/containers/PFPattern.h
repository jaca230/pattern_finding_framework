#pragma once
#include "PFVertex.h"
#include <unordered_set>
#include <memory>

class PFPattern {
public:
    using VertexPtr = std::shared_ptr<PFVertex>;
    using TrackletPtr = std::shared_ptr<PFTracklet>;

    PFPattern(int patternId);

    // Add a vertex to the pattern
    void addVertex(const VertexPtr& vertex);

    // Get all vertices in the pattern
    const std::unordered_set<VertexPtr>& getVertices() const;

    // Get all unique tracklets across all vertices
    std::unordered_set<TrackletPtr> getUniqueTracklets() const;

    // Get pattern ID
    int getId() const;

private:
    int pattern_id_;
    std::unordered_set<VertexPtr> vertices_;
};
