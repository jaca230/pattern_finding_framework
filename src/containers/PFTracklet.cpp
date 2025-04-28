#include "PFTracklet.h"

PFTracklet::PFTracklet(int trackletId, int particleId)
    : tracklet_id_(trackletId), particle_id_(particleId),
      endpoint_0_({0.0f, 0.0f, 0.0f}), endpoint_1_({0.0f, 0.0f, 0.0f}) {}

int PFTracklet::getTrackletId() const {
    return tracklet_id_;
}

int PFTracklet::getParticleId() const {
    return particle_id_;
}

// New single getter method
std::pair<const PFTracklet::Point3D&, const PFTracklet::Point3D&> PFTracklet::getEndpoints() const {
    return {endpoint_0_, endpoint_1_};
}

void PFTracklet::setEndpoints(const Point3D& pt0, const Point3D& pt1) {
    endpoint_0_ = pt0;
    endpoint_1_ = pt1;
}
