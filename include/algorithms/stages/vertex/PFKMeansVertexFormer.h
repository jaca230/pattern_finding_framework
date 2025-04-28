#pragma once
#include "PFVertexFormer.h"
#include "PFTracklet.h"
#include "PFVertex.h"
#include <unordered_set>
#include <vector>
#include <memory>

class PFKMeansVertexFormer : public PFVertexFormer {
public:
    // Constructor with parameters
    PFKMeansVertexFormer(double sigma, int nIters);

    // Override the form method
    std::unordered_set<PFVertex> form(const std::unordered_set<PFTracklet>& input) override;

private:
    double sigma_;
    int nIters_;

    // Helper methods for KMeans
    double computeBIC(double sigma, int k, const std::vector<std::vector<double>>& clusterCenters, 
                    const std::vector<std::vector<std::vector<double>>>& endPoints, 
                    const std::vector<std::vector<int>>& vertexEndpointMap);
    std::vector<std::vector<int>> assignVertices(const std::vector<std::vector<double>>& verticesVec, const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec);
    std::vector<std::vector<double>> computeNewVertices(const std::vector<std::vector<double>>& verticesVec, const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec, const std::vector<std::vector<int>>& verticesEndPointsMap);
    bool checkIfEmptyVertex(int k, const std::unordered_set<int>& indices);
    std::vector<std::vector<double>> createVertexGuess(int k, const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec);
    std::vector<std::vector<double>> createVertexGuessRandom(int k, const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec);
    std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> constrainedKMeans(const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec, int nIters);
    std::unordered_set<PFVertex> createVerticesFromMap(const std::vector<std::vector<int>>& vertexEndpointMap, const std::unordered_set<PFTracklet>& tracklets);
};
