#pragma once
#include "PFAlgorithmStage.h"
#include "PFVertex.h"
#include "PFPattern.h"
#include <set>

class PFPatternFormer : public PFAlgorithmStage<std::set<PFVertex>, std::set<PFPattern>> {
public:
    PFPatternFormer() {
        setOrderIndex(300);  // Set the order index specific to PatternFormer
    }

protected:
    // Pure virtual method to be implemented by derived classes.
    std::set<PFPattern> runImpl(const std::set<PFVertex>& input) override = 0;
};
