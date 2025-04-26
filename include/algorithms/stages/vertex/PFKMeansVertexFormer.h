#pragma once
#include "PFVertexFormer.h"

class PFKMeansVertexFormer : public PFVertexFormer {
public:
    // Constructor
    PFKMeansVertexFormer();

    // Override the form method to return an empty set for now
    std::set<PFVertex> form(const std::set<PFTracklet>& input) override;
};
