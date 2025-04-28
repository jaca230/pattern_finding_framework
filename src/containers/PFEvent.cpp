#include "PFEvent.h"

PFEvent::PFEvent(int eventId) : event_id(eventId) {}

void PFEvent::addPattern(const std::shared_ptr<PFPattern>& pattern) {
    patterns.insert(pattern);
}

void PFEvent::setPatterns(const std::unordered_set<PFPattern>& inputPatterns) {
    patterns.clear();  // Optional: Clear any previous patterns first
    for (const PFPattern& pattern : inputPatterns) {
        patterns.insert(std::make_shared<PFPattern>(pattern));
    }
}

const std::unordered_set<std::shared_ptr<PFPattern>>& PFEvent::getPatterns() const {
    return patterns;
}

int PFEvent::getEventId() const {
    return event_id;
}

void PFEvent::setPipeline(std::shared_ptr<PFAlgorithmPipeline> pipeline) {
    pipeline_ = pipeline;
}

std::shared_ptr<PFAlgorithmPipeline> PFEvent::getPipeline() const {
    return pipeline_;
}
