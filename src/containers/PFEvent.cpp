#include "PFEvent.h"

PFEvent::PFEvent(int eventId) : event_id(eventId) {}

void PFEvent::addPattern(const std::shared_ptr<PFPattern>& pattern) {
    patterns.insert(pattern);
}

const std::unordered_set<std::shared_ptr<PFPattern>>& PFEvent::getPatterns() const {
    return patterns;
}

int PFEvent::getEventId() const {
    return event_id;
}
