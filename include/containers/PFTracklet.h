#pragma once
#include <optional>
#include <array>

class PFTracklet {
public:
    using Point3D = std::array<float, 3>;

    PFTracklet(int trackletId, int particleId);

    // Accessors
    int getTrackletId() const;
    int getParticleId() const;
    std::optional<Point3D> getEndpoint0() const;
    std::optional<Point3D> getEndpoint1() const;

    // Mutators
    void setEndpoints(const Point3D& pt0, const Point3D& pt1);

private:
    int tracklet_id_;
    int particle_id_;
    std::optional<Point3D> endpoint_0_;
    std::optional<Point3D> endpoint_1_;
};
