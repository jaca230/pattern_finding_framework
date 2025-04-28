#include "PFDefaultTrackletFormer.h"

PFDefaultTrackletFormer::PFDefaultTrackletFormer() {
    // Nothing needed for now
}

std::unordered_set<PFTracklet> PFDefaultTrackletFormer::form(const nlohmann::json& inputJson) {
    std::unordered_set<PFTracklet> result;  // Change to unordered_set
    int trackletCounter = 0;

    for (const auto& [key, value] : inputJson.items()) {
        if (!value.contains("particle_id") || !value.contains("endpoint0") || !value.contains("endpoint1")) {
            continue;  // Skip invalid entries
        }

        int particleId = value["particle_id"].get<int>();
        std::vector<float> endpoint0_vec = value["endpoint0"].get<std::vector<float>>();
        std::vector<float> endpoint1_vec = value["endpoint1"].get<std::vector<float>>();

        if (endpoint0_vec.size() != 3 || endpoint1_vec.size() != 3) {
            continue;  // Skip entries with wrong endpoint size
        }

        PFTracklet tracklet(trackletCounter++, particleId);
        tracklet.setEndpoints(
            {endpoint0_vec[0], endpoint0_vec[1], endpoint0_vec[2]},
            {endpoint1_vec[0], endpoint1_vec[1], endpoint1_vec[2]}
        );

        result.insert(tracklet);  // Insert into unordered_set
    }

    // Save some extra info
    nlohmann::json info;
    info["input_tracklets_count"] = result.size();
    info["input_json_keys"] = inputJson.size();
    setExtraInfo(info);

    return result;  // Return unordered_set
}
