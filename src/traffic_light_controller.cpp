#include "traffic_light_controller.h"

TrafficState SimpleStateTransition::get_from_state() const {
    return from_state;
}
SystemEvent SimpleStateTransition::get_trigger_event() const {
    return trigger_event;
}
TrafficState SimpleStateTransition::get_to_state() const { return to_state; }

bool SimpleStateTransition::can_transition(TrafficState current_state,
                                           SystemEvent event) const {
    return this->from_state == current_state && this->trigger_event == event;
}

RuntimeStateMachine::RuntimeStateMachine(TrafficState initial_state)
    : current_state(initial_state) {
    states.insert(initial_state);
}

TrafficState RuntimeStateMachine::get_current_state() const {
    return current_state;
}

void RuntimeStateMachine::set_state(TrafficState state) {
    states.insert(state);
    current_state = state;
}

void RuntimeStateMachine::add_transition(
    std::unique_ptr<StateTransition> transition) {
    states.insert(transition->get_from_state());
    states.insert(transition->get_to_state());
    events.insert(transition->get_trigger_event());
    transitions.push_back(std::move(transition));
}

TrafficState RuntimeStateMachine::get_next_state(TrafficState current_state,
                                                 SystemEvent event) const {
    auto it = std::find_if(
        transitions.begin(), transitions.end(),
        [&current_state, &event](const std::unique_ptr<StateTransition> &t) {
            return t->can_transition(current_state, event);
        });

    if (it != transitions.end()) {
        return (*it)->get_to_state();
    }
    return current_state;
}

bool RuntimeStateMachine::process_event(SystemEvent event) {

    TrafficState next_state = get_next_state(current_state, event);
    if (next_state != current_state) {
        current_state = next_state;
        return true;
    }
    return false;
}

std::vector<TrafficState> RuntimeStateMachine::get_all_states() const {
    return std::vector<TrafficState>(states.begin(), states.end());
}

std::vector<SystemEvent> RuntimeStateMachine::get_all_events() const {
    return std::vector<SystemEvent>(events.begin(), events.end());
}

TrafficLightController::TrafficLightController(
    std::shared_ptr<StateMachine> state_machine,
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
bool TrafficLightController::has_pedestrian_request() const {
    return pedestrian_request;
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
