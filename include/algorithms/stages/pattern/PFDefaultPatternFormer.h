#pragma once
#include "PFPatternFormer.h"

class PFDefaultPatternFormer : public PFPatternFormer {
public:
    // Constructor
    PFDefaultPatternFormer();

    // Override the runImpl method to implement default behavior (returning an empty set for now)
    std::set<PFPattern> runImpl(const std::set<PFVertex>& input) override;
};
