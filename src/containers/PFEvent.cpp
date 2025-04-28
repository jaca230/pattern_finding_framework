#include "PFEvent.h"

PFEvent::PFEvent(int eventId) : event_id(eventId) {}

void PFEvent::addPattern(const std::shared_ptr<PFPattern>& pattern) {
    patterns.insert(pattern);
}

void PFEvent::addPattern(PFPattern pattern) {
    addPattern(std::make_shared<PFPattern>(std::move(pattern)));  // Convert to shared_ptr and insert
}

void PFEvent::addPatterns(const std::unordered_set<std::shared_ptr<PFPattern>>& inputPatterns) {
    for (const auto& pattern : inputPatterns) {
        addPattern(pattern);  // Delegate to shared_ptr version of addPattern
    }
}

void PFEvent::addPatterns(const std::unordered_set<PFPattern>& inputPatterns) {
    for (const auto& pattern : inputPatterns) {
        addPattern(pattern);  // Delegate to shared_ptr version of addPattern after casting
    }
}

void PFEvent::setPatterns(const std::unordered_set<std::shared_ptr<PFPattern>>& inputPatterns) {
    patterns.clear();  // Clear previous patterns
    addPatterns(inputPatterns);  // Delegate to addPatterns for logic
}

void PFEvent::setPatterns(const std::unordered_set<PFPattern>& inputPatterns) {
    patterns.clear();  // Clear previous patterns
    addPatterns(inputPatterns);  // Delegate to addPatterns for logic
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
