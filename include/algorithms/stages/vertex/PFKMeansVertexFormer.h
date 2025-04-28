#pragma once

#include "PFVertexFormer.h"
#include "PFTracklet.h"
#include "PFVertex.h"

#include <unordered_set>
#include <vector>
#include <memory>

class PFKMeansVertexFormer : public PFVertexFormer {
public:
    // Constructor
    PFKMeansVertexFormer(double sigma, int nIters);

    // Override: form vertices from tracklets
    std::unordered_set<std::shared_ptr<PFVertex>> form(const std::unordered_set<std::shared_ptr<PFTracklet>>& input) override;

private:
    double sigma_;
    int nIters_;

    // Internal helper methods
    double computeBIC(
        double sigma,
        int numVertices,
        const std::vector<std::vector<double>>& vertexCenters,
        const std::vector<std::vector<std::vector<double>>>& trackletEndpoints,
        const std::vector<std::vector<int>>& vertexEndpointAssignments
    );

    std::vector<std::vector<int>> assignVertices(
        const std::vector<std::vector<double>>& vertexCenters,
        const std::vector<std::vector<std::vector<double>>>& trackletEndpoints
    );

    std::vector<std::vector<double>> computeNewVertices(
        const std::vector<std::vector<double>>& oldCenters,
        const std::vector<std::vector<std::vector<double>>>& trackletEndpoints,
        const std::vector<std::vector<int>>& vertexEndpointAssignments
    );

    bool checkIfEmptyVertex(int vertexIndex, const std::unordered_set<int>& assignedEndpoints) const;

    std::vector<std::vector<double>> createVertexGuess(
        int numVertices,
        const std::vector<std::vector<std::vector<double>>>& trackletEndpoints
    );

    std::vector<std::vector<double>> createVertexGuessRandom(
        int numVertices,
        const std::vector<std::vector<std::vector<double>>>& trackletEndpoints
    );

    std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> constrainedKMeans(
        const std::vector<std::vector<std::vector<double>>>& trackletEndpoints,
        int maxIterations
    );

    std::unordered_set<std::shared_ptr<PFVertex>> createVerticesFromMap(
        const std::vector<std::vector<int>>& vertexEndpointAssignments,
        const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets
    );
};
