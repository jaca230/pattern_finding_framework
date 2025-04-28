#pragma once
#include <array>
#include <unordered_set>

class PFTracklet {
public:
    using Point3D = std::array<float, 3>;

    PFTracklet(int trackletId, int particleId);

    // Accessors
    int getTrackletId() const;
    int getParticleId() const;
    std::pair<const Point3D&, const Point3D&> getEndpoints() const;  // New single getter

    // Mutators
    void setEndpoints(const Point3D& pt0, const Point3D& pt1);

    // Equality operator based on tracklet_id_
    bool operator==(const PFTracklet& other) const {
        return tracklet_id_ == other.tracklet_id_;
    }

private:
    int tracklet_id_;
    int particle_id_;
    Point3D endpoint_0_;
    Point3D endpoint_1_;
};

namespace std {
    template <>
    struct hash<PFTracklet> {
        std::size_t operator()(const PFTracklet& tracklet) const {
            return std::hash<int>()(tracklet.getTrackletId());  // Hash based on tracklet_id_
        }
    };
}
