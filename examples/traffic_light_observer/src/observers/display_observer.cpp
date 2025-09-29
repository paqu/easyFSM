#include "display_observer.h"
#include "light_timings.h"

DisplayObserver::DisplayObserver(
    std::unique_ptr<IDisplayService<TrafficContext>> service)
    : display_service(std::move(service)) {
    initialize_contexts();
}

void DisplayObserver::on_state_transition(TrafficState from, TrafficEvent event,
                                          TrafficState to) {
    (void)event;
    // Only update display if state actually changed
    if (from != to && display_service) {
        auto it = state_contexts.find(to);
        if (it != state_contexts.end()) {
            display_service->show_state(it->second);
        }
    }
}

void DisplayObserver::initialize_contexts() {
    // Initialize all state_contexts
    state_contexts[TrafficState::CAR_GREEN] = {
        "CAR_GREEN",
        LightTimings::GREEN_DURATION,
        {false, false, true},
        {true, false},
    };
    state_contexts[TrafficState::CAR_YELLOW] = {
        "CAR_YELLOW",
        LightTimings::YELLOW_DURATION,
        {false, true, false},
        {true, false},
    };
    state_contexts[TrafficState::CAR_RED] = {
        "CAR_RED",
        LightTimings::RED_DURATION,
        {true, false, false},
        {true, false},
    };
    state_contexts[TrafficState::WALK_PREP] = {
        "WALK_PREP",
        LightTimings::WALK_PREP_DURATION,
        {true, false, false},
        {true, false},
    };
    state_contexts[TrafficState::WALK] = {
        "WALK",
        LightTimings::WALK_DURATION,
        {true, false, false},
        {false, true},
    };
    state_contexts[TrafficState::WALK_FINISH] = {
        "WALK_FINISH",
        LightTimings::WALK_FINISH_DURATION,
        {true, false, false},
        {true, false},
    };
    state_contexts[TrafficState::CAR_RED_YELLOW] = {
        "CAR_RED_YELLOW",
        LightTimings::RED_YELLOW_DURATION,
        {true, true, false},
        {true, false},
    };
}
