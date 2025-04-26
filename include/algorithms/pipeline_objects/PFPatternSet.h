#pragma once
#include "PFPipelineObject.h"
#include "PFPattern.h"
#include <set>
#include <memory>

class PFPatternSet : public PFPipelineObject {
public:
    PFPatternSet(const std::set<PFPattern>& patterns) : patterns_(patterns) {}

    const std::set<PFPattern>& getPatterns() const { return patterns_; }

private:
    std::set<PFPattern> patterns_;
};
