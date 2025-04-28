#pragma once
#include "PFPatternFormer.h"
#include "PFVertex.h"
#include <unordered_set>
#include <memory>

class PFDefaultPatternFormer : public PFPatternFormer {
public:
    // Constructor
    PFDefaultPatternFormer();

    // Override the form method to implement default pattern formation logic
    std::unordered_set<std::shared_ptr<PFPattern>> form(
        const std::unordered_set<std::shared_ptr<PFVertex>>& vertexSet) override;

private:
    // Helper method to perform depth-first search to find connected vertices
    std::unordered_set<std::shared_ptr<PFVertex>> dfs(
        std::shared_ptr<PFVertex> start,
        std::unordered_set<std::shared_ptr<PFVertex>>& unvisited
    );

    // Helper method to check if two vertices' tracklets are disjoint
    bool isDisjoint(std::shared_ptr<PFVertex> v1, std::shared_ptr<PFVertex> v2) const;
};
