#pragma once

#include "PFPattern.h"
#include "PFAlgorithmPipeline.h"
#include "PFPipelineObject.h"
#include <unordered_set>
#include <memory>
#include <set>

class PFEvent {
public:
    PFEvent(int eventId);

    // Accessors
    int getEventId() const;
    const std::unordered_set<std::shared_ptr<PFPattern>>& getPatterns() const;

    // Mutators
    void addPattern(const std::shared_ptr<PFPattern>& pattern);  // Add shared pointer pattern
    void addPattern(PFPattern pattern);  // Add non-pointer PFPattern (converted to shared_ptr)
    void addPatterns(const std::unordered_set<std::shared_ptr<PFPattern>>& patterns);  // Add shared pointer set
    void addPatterns(const std::unordered_set<PFPattern>& patterns);  // Add non-pointer PFPattern set (converted to shared_ptr)
    
    void setPatterns(const std::unordered_set<std::shared_ptr<PFPattern>>& patterns);  // Set shared pointer set
    void setPatterns(const std::unordered_set<PFPattern>& patterns);  // Set non-pointer PFPattern set (converted to shared_ptr)

    void setPipeline(std::shared_ptr<PFAlgorithmPipeline> pipeline);
    std::shared_ptr<PFAlgorithmPipeline> getPipeline() const;

    // Equality operator based on event_id_
    bool operator==(const PFEvent& other) const {
        return event_id == other.event_id;
    }

private:
    int event_id;
    std::unordered_set<std::shared_ptr<PFPattern>> patterns;
    std::shared_ptr<PFAlgorithmPipeline> pipeline_;
};

namespace std {
    template <>
    struct hash<PFEvent> {
        std::size_t operator()(const PFEvent& event) const {
            return std::hash<int>()(event.getEventId());  // Hash based on event_id_
        }
    };
}
