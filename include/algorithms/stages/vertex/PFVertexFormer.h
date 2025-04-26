#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFVertex.h"
#include <set>

class PFVertexFormer : public PFAlgorithmStage<std::set<PFTracklet>, std::set<PFVertex>> {
public:
    PFVertexFormer() {
        setOrderIndex(200);  // Set the order index specific to PFVertexFormer
    }

protected:
    // Pure virtual method to be implemented by derived classes.
    std::set<PFVertex> runImpl(const std::set<PFTracklet>& input) override = 0; 
};
