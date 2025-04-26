#pragma once
#include "PFPipelineObject.h"
#include "PFVertex.h"
#include <set>

class PFVertexSet : public PFPipelineObject {
public:
    PFVertexSet() = default;
    PFVertexSet(const std::set<PFVertex>& vertices) : vertices_(vertices) {}

    // Getter for vertices
    const std::set<PFVertex>& getVertices() const { return vertices_; }

    // Setter for vertices
    void setVertices(const std::set<PFVertex>& vertices) { vertices_ = vertices; }

private:
    std::set<PFVertex> vertices_;  // Store the set of PFVertex objects
};
