#include "PFKMeansVertexFormer.h"
#include <unordered_map>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

// Struct to store BIC and other details from each iteration
struct KMeansResult {
    double bic;
    int k;
    std::vector<std::vector<double>> vertices;
    int iteration;
};

// Constructor to initialize parameters
PFKMeansVertexFormer::PFKMeansVertexFormer(double sigma, int nIters) 
    : sigma_(sigma), nIters_(nIters) {}

// Main method to form vertices from tracklets
std::unordered_set<PFVertex> PFKMeansVertexFormer::form(const std::unordered_set<PFTracklet>& input) {
    // Collect endpoints from tracklets
    std::vector<std::vector<std::vector<double>>> trackletEndPointsVec;
    for (const auto& tracklet : input) {
        // Retrieve endpoints from tracklet
        auto [endpoint0, endpoint1] = tracklet.getEndpoints();
        // Populate endpoints
        std::vector<std::vector<double>> endpoints = {
            {endpoint0[0], endpoint0[1], endpoint0[2]},  // Convert to vector of doubles
            {endpoint1[0], endpoint1[1], endpoint1[2]}   // Convert to vector of doubles
        };
        trackletEndPointsVec.push_back(endpoints);
    }

    // Collect extra information and append to existing data
    nlohmann::json extraInfo = getExtraInfo();  // Get current extra info
    extraInfo["input_tracklets_count"] = input.size();
    extraInfo["input_endpoints_count"] = trackletEndPointsVec.size();
    extraInfo["sigma"] = sigma_;
    extraInfo["n_iters"] = nIters_;
    setExtraInfo(extraInfo);  // Append new info to existing extraInfo_

    // Run constrained k-means clustering with required parameters
    auto [centroids, vertexEndpointMap] = constrainedKMeans(trackletEndPointsVec, nIters_);

    // Create vertices from the vertex-endpoint map
    return createVerticesFromMap(vertexEndpointMap, input);
}

// Compute Bayesian Information Criterion (BIC) for the clustering solution
double PFKMeansVertexFormer::computeBIC(double sigma, int k, const std::vector<std::vector<double>>& clusterCenters, 
                                        const std::vector<std::vector<std::vector<double>>>& endPoints, const std::vector<std::vector<int>>& vertexEndpointMap) {
    size_t N = endPoints.size();  // N now represents the number of tracklets (pairs of endpoints)
    size_t d = 3;  // Dimension of each endpoint (x, y, z)

    // Check for any unassigned vertex before performing any calculations or printing
    for (size_t i = 0; i < vertexEndpointMap.size(); ++i) {
        // Each tracklet has two endpoints, so check both assignments
        int assignedVertexIdx1 = vertexEndpointMap[i][0];  // First endpoint of the tracklet
        int assignedVertexIdx2 = vertexEndpointMap[i][1];  // Second endpoint of the tracklet
        
        if (assignedVertexIdx1 == -1 || assignedVertexIdx2 == -1) {
            // If any endpoint is unassigned, return max double to indicate invalid clustering
            return std::numeric_limits<double>::infinity();  // Exit early with max BIC
        }
    }

    // Compute the sum of squared distances (based on vertex assignment)
    double vertex_to_endpoints_distances = 0.0;
    
    // Loop over each tracklet and calculate the distance to its assigned cluster center for both endpoints
    for (size_t i = 0; i < N; ++i) {
        const auto& tracklet = endPoints[i];  // This contains two endpoints for tracklet i
        
        // Find the clusters assigned to this tracklet's endpoints
        int assignedVertexIdx1 = vertexEndpointMap[i][0];  // First endpoint's assigned vertex
        int assignedVertexIdx2 = vertexEndpointMap[i][1];  // Second endpoint's assigned vertex
        
        // Compute squared distances to the assigned cluster centers for both endpoints
        const auto& clusterCenter1 = clusterCenters[assignedVertexIdx1];
        double distSquared1 = std::pow(tracklet[0][0] - clusterCenter1[0], 2) + 
                             std::pow(tracklet[0][1] - clusterCenter1[1], 2) + 
                             std::pow(tracklet[0][2] - clusterCenter1[2], 2);
        
        const auto& clusterCenter2 = clusterCenters[assignedVertexIdx2];
        double distSquared2 = std::pow(tracklet[1][0] - clusterCenter2[0], 2) + 
                             std::pow(tracklet[1][1] - clusterCenter2[1], 2) + 
                             std::pow(tracklet[1][2] - clusterCenter2[2], 2);
        
        // Add the square roots of the squared distances to the term2 sum
        vertex_to_endpoints_distances += std::sqrt(distSquared1);
        vertex_to_endpoints_distances += std::sqrt(distSquared2);
    }
    
    double term1 = -(N * d / 2.0) * std::log(2 * M_PI * sigma * sigma);
    double term2 = -1.0 / (2.0 * sigma * sigma) * vertex_to_endpoints_distances;
    double logL = term1 + term2;
    return k * std::log(N) - 2 * logL;
}







// Assign tracklet endpoints to the nearest vertices
std::vector<std::vector<int>> PFKMeansVertexFormer::assignVertices(
    const std::vector<std::vector<double>>& verticesVec, 
    const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec) 
{
    size_t nTracklets = trackletEndPointsVec.size();
    size_t nVertices = verticesVec.size();

    std::vector<std::vector<int>> verticesEndPointsMap(nTracklets);
    std::vector<std::vector<double>> minDistEndPointsMap(nTracklets);

    // Temporary vectors to store distance and index information
    for (size_t i = 0; i < nTracklets; ++i) {
        const auto& trackletEndPoints = trackletEndPointsVec[i];
        
        verticesEndPointsMap[i].resize(trackletEndPoints.size());
        minDistEndPointsMap[i].resize(trackletEndPoints.size(), 0.0);

        std::vector<double> distMap; // Stores the distances for sorting
        std::vector<std::pair<int, int>> distIndexMap; // Stores the (endpoint, vertex) index pairs

        // Calculate distances for all pairs (tracklet endpoint to vertex)
        for (size_t j = 0; j < trackletEndPoints.size(); ++j) {
            for (size_t k = 0; k < nVertices; ++k) {
                double dist = std::sqrt(std::pow(trackletEndPoints[j][0] - verticesVec[k][0], 2) + 
                                        std::pow(trackletEndPoints[j][1] - verticesVec[k][1], 2) + 
                                        std::pow(trackletEndPoints[j][2] - verticesVec[k][2], 2));

                distMap.push_back(dist);
                distIndexMap.push_back({j, k});
            }
        }

        // Sort distances, from smallest to largest
        std::vector<int> sortedIndices(distMap.size());
        std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
        std::sort(sortedIndices.begin(), sortedIndices.end(), [&distMap](int a, int b) {
            return distMap[a] < distMap[b];
        });

        // Track which endpoints and vertices have been assigned
        std::vector<bool> jUsed(trackletEndPoints.size(), false);
        std::vector<bool> kUsed(nVertices, false);

        // Assign the closest vertices to endpoints, ensuring unique assignments
        for (int idx : sortedIndices) {
            int j = distIndexMap[idx].first;
            int k = distIndexMap[idx].second;

            if (!jUsed[j] && !kUsed[k]) {
                verticesEndPointsMap[i][j] = k;
                minDistEndPointsMap[i][j] = distMap[idx];
                jUsed[j] = true;
                kUsed[k] = true;
            }
        }
    }

    return verticesEndPointsMap;
}




// Compute new vertex positions as the mean of assigned tracklet endpoints
std::vector<std::vector<double>> PFKMeansVertexFormer::computeNewVertices(
    const std::vector<std::vector<double>>& verticesVec, 
    const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec, 
    const std::vector<std::vector<int>>& verticesEndPointsMap) 
{
    std::vector<std::vector<double>> newVertices(verticesVec.size(), std::vector<double>(3, 0.0));
    std::vector<int> nVerticesVec(verticesVec.size(), 0);

    for (size_t i = 0; i < trackletEndPointsVec.size(); ++i) {
        const auto& trackletEndPoints = trackletEndPointsVec[i];
        
        for (size_t j = 0; j < trackletEndPoints.size(); ++j) {
            int vertexIdx = verticesEndPointsMap[i][j];
            
            newVertices[vertexIdx][0] += trackletEndPoints[j][0];
            newVertices[vertexIdx][1] += trackletEndPoints[j][1];
            newVertices[vertexIdx][2] += trackletEndPoints[j][2];
            nVerticesVec[vertexIdx]++;
        }
    }

    // Compute the average position for each vertex
    for (size_t k = 0; k < newVertices.size(); ++k) {
        if (nVerticesVec[k] != 0) {
            newVertices[k][0] /= nVerticesVec[k];
            newVertices[k][1] /= nVerticesVec[k];
            newVertices[k][2] /= nVerticesVec[k];
        }
    }

    return newVertices;
}



// Check if a vertex is empty (no assigned endpoints)
bool PFKMeansVertexFormer::checkIfEmptyVertex(int k, const std::unordered_set<int>& indices) {
    return indices.size() != static_cast<size_t>(k);
}

// Generate random vertex guesses
std::vector<std::vector<double>> PFKMeansVertexFormer::createVertexGuess(int k, const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec) {
    return createVertexGuessRandom(k, trackletEndPointsVec);
}

// Generate random vertex guesses within the bounding box of all endpoints
std::vector<std::vector<double>> PFKMeansVertexFormer::createVertexGuessRandom(int k, const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec) {
    std::vector<double> xValues, yValues, zValues;
    
    for (const auto& endpoints : trackletEndPointsVec) {
        for (const auto& endpoint : endpoints) {
            xValues.push_back(endpoint[0]);
            yValues.push_back(endpoint[1]);
            zValues.push_back(endpoint[2]);
        }
    }

    double minX = *std::min_element(xValues.begin(), xValues.end());
    double maxX = *std::max_element(xValues.begin(), xValues.end());
    double minY = *std::min_element(yValues.begin(), yValues.end());
    double maxY = *std::max_element(yValues.begin(), yValues.end());
    double minZ = *std::min_element(zValues.begin(), zValues.end());
    double maxZ = *std::max_element(zValues.begin(), zValues.end());

    std::vector<std::vector<double>> vertices(k, std::vector<double>(3));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(minX, maxX);
    std::uniform_real_distribution<> disY(minY, maxY);
    std::uniform_real_distribution<> disZ(minZ, maxZ);

    for (auto& vertex : vertices) {
        vertex[0] = disX(gen);
        vertex[1] = disY(gen);
        vertex[2] = disZ(gen);
    }

    return vertices;
}

// Main method to run constrained k-means clustering
std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> 
PFKMeansVertexFormer::constrainedKMeans(const std::vector<std::vector<std::vector<double>>>& trackletEndPointsVec, int nIters) {
    size_t nEndPoints = 0;
    std::vector<std::vector<double>> endPoints;

    // Flatten the tracklet endpoints
    for (const auto& trackletEndPoints : trackletEndPointsVec) {
        for (const auto& endPoint : trackletEndPoints) {
            endPoints.push_back(endPoint);
            ++nEndPoints;
        }
    }

    double minBIC = std::numeric_limits<double>::max();
    int minK = -1;
    int minIter = 0;
    std::vector<std::vector<double>> minVerticesVec;

    // Store all results in extraInfo_ as a JSON object
    nlohmann::json allResultsJson = nlohmann::json::array();

    // Run K-means for different values of k
    for (int k = 1; k <= nEndPoints; ++k) {
        auto verticesVec = createVertexGuessRandom(k, trackletEndPointsVec);
        auto verticesEndPointsMap = assignVertices(verticesVec, trackletEndPointsVec);

        for (int iter = 0; iter < nIters; ++iter) {
            verticesVec = computeNewVertices(verticesVec, trackletEndPointsVec, verticesEndPointsMap);
            verticesEndPointsMap = assignVertices(verticesVec, trackletEndPointsVec);

            std::vector<int> verticesIndices;
            for (const auto& vertexEndPoints : verticesEndPointsMap) {
                verticesIndices.insert(verticesIndices.end(), vertexEndPoints.begin(), vertexEndPoints.end());
            }

            std::unordered_set<int> uniqueVertexIndices(verticesIndices.begin(), verticesIndices.end());
            if (checkIfEmptyVertex(k, uniqueVertexIndices)) {
                continue;  // Skip empty vertices
            }

            auto verticesMap = verticesVec;
            double bic = computeBIC(sigma_, k, verticesVec, trackletEndPointsVec, verticesEndPointsMap);

            // Create a JSON object for the current result
            nlohmann::json resultJson = {
                {"bic", bic},
                {"k", k},
                {"vertices", verticesVec},
                {"iteration", iter + 1}
            };

            // Append the result to the existing extraInfo_ (merge new data)
            nlohmann::json currentExtraInfo = getExtraInfo();
            currentExtraInfo["results"].push_back(resultJson);
            setExtraInfo(currentExtraInfo);  // Update extraInfo_

            if (bic < minBIC) {
                minBIC = bic;
                minK = k;
                minVerticesVec = verticesVec;
                minIter = iter + 1;
            }
        }
    }

    // Map the final vertices to endpoints
    auto verticesEndPointsMap = assignVertices(minVerticesVec, trackletEndPointsVec);
    return {minVerticesVec, verticesEndPointsMap};  // Return the pair of results
}



std::unordered_set<PFVertex> PFKMeansVertexFormer::createVerticesFromMap(const std::vector<std::vector<int>>& vertexEndpointMap, const std::unordered_set<PFTracklet>& tracklets) {
    std::unordered_map<int, std::unordered_set<std::shared_ptr<PFTracklet>>> vertexToTracklets;

    // Map tracklets to corresponding vertex indices
    auto trackletIter = tracklets.begin();
    for (size_t i = 0; i < vertexEndpointMap.size(); ++i) {
        for (int vIdx : vertexEndpointMap[i]) {
            vertexToTracklets[vIdx].insert(std::make_shared<PFTracklet>(*trackletIter));  // Create a shared pointer to the PFTracklet
        }
        ++trackletIter;
    }

    // Create vertices and associate them with tracklets
    std::unordered_set<PFVertex> vertices;
    for (const auto& [vertexIdx, trackletSet] : vertexToTracklets) {
        PFVertex vertex(vertexIdx);  // Create a PFVertex with the given vertex index
        for (const auto& tracklet : trackletSet) {
            vertex.addTracklet(tracklet);  // Add the shared pointer to the vertex
            //TODO: This creates a shared pointer for some tracklets multiple times
            // In other words, we making copies for the same tracklet into and putting a pointer 
            // to that copy in multiple vertices.
            // It would be better to use a shared pointer to avoid this.
            // We could make each step of the pipeline deal with sets of shared pointers instead as one fix
            // But for now, this is fine, it at most creates a factor of 2 overhead
        }
        vertices.insert(vertex);  // Insert the vertex into the set
    }

    return vertices;
}

