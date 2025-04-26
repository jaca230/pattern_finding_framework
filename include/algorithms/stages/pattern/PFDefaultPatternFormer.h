#pragma once
#include "PFPatternFormer.h"

class PFDefaultPatternFormer : public PFPatternFormer {
public:
    // Constructor
    PFDefaultPatternFormer();

protected:
    // Override the form method to implement default pattern formation logic
    std::set<PFPattern> form(const std::set<PFVertex>& vertexSet) override;
};
