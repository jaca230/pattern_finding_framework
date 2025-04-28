#include "PFDefaultPatternFormer.h"
#include <stack>
#include <iostream>

// Constructor
PFDefaultPatternFormer::PFDefaultPatternFormer() {
    // Constructor implementation (empty for now)
}

// Form patterns by identifying connected components
std::unordered_set<PFPattern> PFDefaultPatternFormer::form(const std::unordered_set<PFVertex>& vertexSet) {
    std::unordered_set<PFPattern> patterns;
    std::unordered_set<std::shared_ptr<PFVertex>> unvisited;

    // Convert the input vertex set to shared_ptr
    for (const auto& vertex : vertexSet) {
        unvisited.insert(std::make_shared<PFVertex>(vertex));
    }

    int pattern_id = 0;  // Start pattern ID counter

    // Visit each vertex in the set exactly once
    while (!unvisited.empty()) {
        auto start = *unvisited.begin();  // Start at some vertex in the set
        unvisited.erase(unvisited.begin());  // Remove it from the unvisited set

        // Find all connected vertices (component) using DFS
        auto component = dfs(start, unvisited);

        // Create a pattern with the current pattern_id
        PFPattern pattern(pattern_id);

        // Debug: Print the tracklet IDs in each vertex of the component
        std::cout << "Pattern ID: " << pattern_id << std::endl;
        for (const auto& vertex : component) {
            std::cout << "Vertex ID: " << vertex->getId() << " Tracklets: ";
            const auto& tracklets = vertex->getTracklets();
            for (const auto& tracklet : tracklets) {
                std::cout << tracklet->getTrackletId() << " ";
            }
            std::cout << std::endl;
        }

        // Add vertices to the pattern in two steps
        pattern.addVertices(component);  // Add all the connected vertices

        // Add the pattern to the set of patterns
        patterns.insert(pattern);

        pattern_id++;
    }

    // Add extra info about the pattern formation process
    nlohmann::json info = getExtraInfo();  // Get current extra info
    info["total_patterns"] = patterns.size();
    info["total_vertices"] = vertexSet.size();  // Number of vertices passed in
    setExtraInfo(info);

    return patterns;
}


// Helper method for depth-first search to find connected vertices
std::unordered_set<std::shared_ptr<PFVertex>> PFDefaultPatternFormer::dfs(
    std::shared_ptr<PFVertex> start, 
    std::unordered_set<std::shared_ptr<PFVertex>>& unvisited
) {
    std::unordered_set<std::shared_ptr<PFVertex>> connected;
    std::stack<std::shared_ptr<PFVertex>> stack;
    stack.push(start);
    connected.insert(start);

    while (!stack.empty()) {
        auto current = stack.top();
        stack.pop();

        // Find vertices connected to 'current' based on shared tracklets
        std::unordered_set<std::shared_ptr<PFVertex>> to_link;
        for (const auto& vertex : unvisited) {
            // Check if tracklets are shared (disjoint check)
            if (!current->getTracklets().empty() && !vertex->getTracklets().empty()) {
                if (!isDisjoint(current, vertex)) {
                    to_link.insert(vertex);  // Add to_link if not disjoint
                }
            }
        }

        // Add new vertices to stack and update the connected set
        for (const auto& vertex : to_link) {
            stack.push(vertex);
            connected.insert(vertex);
            unvisited.erase(vertex);  // Remove the visited vertex from unvisited set
        }
    }

    return connected;
}

// Helper method to check if two sets of tracklets are disjoint
bool PFDefaultPatternFormer::isDisjoint(std::shared_ptr<PFVertex> v1, std::shared_ptr<PFVertex> v2) const {
    for (const auto& tracklet1 : v1->getTracklets()) {
        for (const auto& tracklet2 : v2->getTracklets()) {
            if (tracklet1->getTrackletId() == tracklet2->getTrackletId()) {
                return false;  // Found a common tracklet, sets are not disjoint
            }
        }
    }
    return true;  // No common tracklets found, sets are disjoint
}
