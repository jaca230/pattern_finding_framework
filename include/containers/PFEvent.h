#pragma once

#include "PFPattern.h"
#include <unordered_set>
#include <memory>

class PFEvent {
public:
    PFEvent(int eventId);

    void addPattern(const std::shared_ptr<PFPattern>& pattern);
    const std::unordered_set<std::shared_ptr<PFPattern>>& getPatterns() const;
    int getEventId() const;

private:
    int event_id;
    std::unordered_set<std::shared_ptr<PFPattern>> patterns;
};
