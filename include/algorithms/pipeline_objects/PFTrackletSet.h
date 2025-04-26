#pragma once
#include "PFPipelineObject.h"
#include "PFTracklet.h"
#include <set>
#include <memory>

class PFTrackletSet : public PFPipelineObject {
public:
    PFTrackletSet(const std::set<PFTracklet>& tracklets) : tracklets_(tracklets) {}

    const std::set<PFTracklet>& getTracklets() const { return tracklets_; }

private:
    std::set<PFTracklet> tracklets_;
};
