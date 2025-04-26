#include "PFTracklet.h"

PFTracklet::PFTracklet(int trackletId, int particleId)
    : tracklet_id_(trackletId), particle_id_(particleId) {}

int PFTracklet::getTrackletId() const {
    return tracklet_id_;
}

int PFTracklet::getParticleId() const {
    return particle_id_;
}

std::optional<PFTracklet::Point3D> PFTracklet::getEndpoint0() const {
    return endpoint_0_;
}

std::optional<PFTracklet::Point3D> PFTracklet::getEndpoint1() const {
    return endpoint_1_;
}

void PFTracklet::setEndpoints(const Point3D& pt0, const Point3D& pt1) {
    endpoint_0_ = pt0;
    endpoint_1_ = pt1;
}
