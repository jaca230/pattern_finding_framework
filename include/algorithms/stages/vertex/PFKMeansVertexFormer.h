#pragma once
#include "PFVertexFormer.h"

class PFKMeansVertexFormer : public PFVertexFormer {
public:
    // Constructor
    PFKMeansVertexFormer();

    // Override the runImpl method to return an empty set for now
    std::set<PFVertex> runImpl(const std::set<PFTracklet>& input) override;

};
