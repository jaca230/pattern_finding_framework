#pragma once
#include "PFVertex.h"
#include <unordered_set>
#include <memory>

class PFPattern {
public:
    using VertexPtr = std::shared_ptr<PFVertex>;
    using TrackletPtr = std::shared_ptr<PFTracklet>;

    PFPattern(int patternId);

    // Accessors
    int getId() const;
    const std::unordered_set<VertexPtr>& getVertices() const;

    // Add a single vertex to the pattern
    void addVertex(const VertexPtr& vertex);

    // Add multiple vertices to the pattern
    void addVertices(const std::unordered_set<VertexPtr>& vertices);

    // Get all unique tracklets across all vertices
    std::unordered_set<TrackletPtr> getUniqueTracklets() const;

    // Equality operator based on pattern_id_
    bool operator==(const PFPattern& other) const {
        return pattern_id_ == other.pattern_id_;
    }

private:
    int pattern_id_;
    std::unordered_set<VertexPtr> vertices_;
};

namespace std {
    template <>
    struct hash<PFPattern> {
        std::size_t operator()(const PFPattern& pattern) const {
            return std::hash<int>()(pattern.getId());  // Hash based on pattern_id_
        }
    };
}
