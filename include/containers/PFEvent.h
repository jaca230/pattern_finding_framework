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

    void addPattern(const std::shared_ptr<PFPattern>& pattern);
    void setPatterns(const std::set<PFPattern>& patterns);

    const std::unordered_set<std::shared_ptr<PFPattern>>& getPatterns() const;
    int getEventId() const;

    void setPipeline(std::shared_ptr<PFAlgorithmPipeline> pipeline);
    std::shared_ptr<PFAlgorithmPipeline> getPipeline() const;

private:
    int event_id;
    std::unordered_set<std::shared_ptr<PFPattern>> patterns;
    std::shared_ptr<PFAlgorithmPipeline> pipeline_;
};
