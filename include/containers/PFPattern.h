#pragma once
#include "PFVertex.h"
#include <unordered_set>
#include <memory>

class PFPattern {
public:
    PFPattern(int patternId);

    // Accessors
    int getId() const;
    const std::unordered_set<std::shared_ptr<PFVertex>>& getVertices() const;

    // Add a single vertex to the pattern (shared_ptr version)
    void addVertex(const std::shared_ptr<PFVertex>& vertex);

    // Add a single vertex to the pattern (non-shared_ptr version)
    void addVertex(PFVertex vertex);

    // Add multiple vertices to the pattern (shared_ptr version)
    void addVertices(const std::unordered_set<std::shared_ptr<PFVertex>>& vertices);

    // Add multiple vertices to the pattern (non-shared_ptr version)
    void addVertices(const std::unordered_set<PFVertex>& vertices);

    // Set vertices (clear and then add new)
    void setVertices(const std::unordered_set<std::shared_ptr<PFVertex>>& vertices);

    // Set vertices (clear and then add new) (non-shared_ptr version)
    void setVertices(const std::unordered_set<PFVertex>& vertices);

    // Get all unique tracklets across all vertices
    std::unordered_set<std::shared_ptr<PFTracklet>> getUniqueTracklets() const;

    // Equality operator based on pattern_id_
    bool operator==(const PFPattern& other) const {
        return pattern_id_ == other.pattern_id_;
    }

private:
    int pattern_id_;
    std::unordered_set<std::shared_ptr<PFVertex>> vertices_;
};

namespace std {
    template <>
    struct hash<PFPattern> {
        std::size_t operator()(const PFPattern& pattern) const {
            return std::hash<int>()(pattern.getId());  // Hash based on pattern_id_
        }
    };
}
