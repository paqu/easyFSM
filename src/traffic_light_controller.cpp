#include "traffic_light_controller.h"

TrafficLightController::TrafficLightController(
    std::unique_ptr<IDisplayService> ds, std::unique_ptr<ITimerService> ts)
    : current_state(CAR_RED), pedestrian_request(false),
      displayService(std::move(ds)), timerService(std::move(ts)) {

    // Initialize all states
    states[CAR_GREEN] = {
        "CAR_GREEN",
        LightTimings::GREEN_DURATION,
        {false, false, true},
        {true, false},
    };
    states[CAR_YELLOW] = {
        "CAR_YELLOW",
        LightTimings::YELLOW_DURATION,
        {false, true, false},
        {true, false},
    };
    states[CAR_RED] = {
        "CAR_RED",
        LightTimings::RED_DURATION,
        {true, false, false},
        {true, false},
    };
    states[WALK_PREP] = {
        "WALK_PREP",
        LightTimings::WALK_PREP_DURATION,
        {true, false, false},
        {true, false},
    };
    states[WALK] = {
        "WALK",
        LightTimings::WALK_DURATION,
        {true, false, false},
        {false, true},
    };
    states[WALK_FINISH] = {
        "WALK_FINISH",
        LightTimings::WALK_FINISH_DURATION,
        {true, false, false},
        {true, false},
    };
    states[CAR_RED_YELLOW] = {
        "CAR_RED_YELLOW",
        LightTimings::RED_YELLOW_DURATION,
        {true, true, false},
        {true, false},
    };
}

void TrafficLightController::button_pressed() {
    bool waiting_to_be_processed = false;
    if (pedestrian_request != true) {
        pedestrian_request = true;
        displayService->showButtonState(waiting_to_be_processed);
    } else {
        waiting_to_be_processed = true;
        displayService->showButtonState(waiting_to_be_processed);
    }
}

void TrafficLightController::timeout_expired() {
    // Determine next state
    current_state = get_next_state(current_state, pedestrian_request);
    // Copy current state info into context
    StateContext context = states[current_state];

    displayService->showState(context);

    if (current_state == WALK_FINISH) {
        pedestrian_request = false;
    }

    // Start timeout for next state
    start_timeout(states[current_state].duration);
}

TrafficLightController::State
TrafficLightController::get_next_state(State s, bool ped_request) const {
    switch (s) {
    case CAR_GREEN:
        return CAR_YELLOW;
    case CAR_YELLOW:
        return ped_request ? WALK_PREP : CAR_RED;
    case CAR_RED:
        return CAR_RED_YELLOW;
    case WALK_PREP:
        return WALK;
    case WALK:
        return WALK_FINISH;
    case WALK_FINISH:
        return CAR_RED_YELLOW;
    case CAR_RED_YELLOW:
        return CAR_GREEN;
    default:
        return CAR_GREEN;
    }
}

void TrafficLightController::start_timeout(uint32_t duration) const {
    timerService->start_timeout(duration);
}
