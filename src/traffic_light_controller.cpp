#include "traffic_light_controller.h"
#include <iostream>
#include <memory>

// Stream operators for easy printing
std::ostream &operator<<(std::ostream &os, TrafficState state) {
    return os << EnumUtils::state_to_string(state);
}

std::ostream &operator<<(std::ostream &os, SystemEvent event) {
    return os << EnumUtils::event_to_string(event);
}

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
    std::shared_ptr<StateMachine> sm, std::unique_ptr<IDisplayService> ds,
    std::unique_ptr<ITimerService> ts, std::unique_ptr<ActionHandler> ah)
    : pedestrian_request(false), displayService(std::move(ds)),
      timerService(std::move(ts)), state_machine(sm),
      action_handler(std::move(ah)) {

    // Initialize all states
    states[TrafficState::CAR_GREEN] = {
        "CAR_GREEN",
        LightTimings::GREEN_DURATION,
        {false, false, true},
        {true, false},
    };
    states[TrafficState::CAR_YELLOW] = {
        "CAR_YELLOW",
        LightTimings::YELLOW_DURATION,
        {false, true, false},
        {true, false},
    };
    states[TrafficState::CAR_RED] = {
        "CAR_RED",
        LightTimings::RED_DURATION,
        {true, false, false},
        {true, false},
    };
    states[TrafficState::WALK_PREP] = {
        "WALK_PREP",
        LightTimings::WALK_PREP_DURATION,
        {true, false, false},
        {true, false},
    };
    states[TrafficState::WALK] = {
        "WALK",
        LightTimings::WALK_DURATION,
        {true, false, false},
        {false, true},
    };
    states[TrafficState::WALK_FINISH] = {
        "WALK_FINISH",
        LightTimings::WALK_FINISH_DURATION,
        {true, false, false},
        {true, false},
    };
    states[TrafficState::CAR_RED_YELLOW] = {
        "CAR_RED_YELLOW",
        LightTimings::RED_YELLOW_DURATION,
        {true, true, false},
        {true, false},
    };
}
void TrafficLightController::handle_button_press() {
    handle_event(SystemEvent::BUTTON_PRESSED);
}

void TrafficLightController::handle_timeout() {
    handle_event(SystemEvent::TIME_EXPIRED);
}
void TrafficLightController::handle(TrafficState _current_state,
                                    SystemEvent _event,
                                    TrafficState _next_state) {

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    if (_event == SystemEvent::BUTTON_PRESSED) {
        bool waiting_to_be_processed = false;
        if (pedestrian_request != true) {
            pedestrian_request = true;
            displayService->showButtonState(waiting_to_be_processed);
        } else {
            waiting_to_be_processed = true;
            displayService->showButtonState(waiting_to_be_processed);
        }
        return;
    } else if (_event == SystemEvent::TIME_EXPIRED &&
               _next_state == TrafficState::WALK_FINISH) {
        std::cout << "Clearing pedestrian request after walk phase"
                  << std::endl;
        pedestrian_request = false;
    }
    std::cout << "\n[" << now << "] Transition: " << _current_state << " --["
              << _event << "]--> " << _next_state << std::endl;

    StateContext context = states[_next_state];

    displayService->showState(context);

    // Start timeout for next state
    start_timeout(states[_next_state].duration);
}

void TrafficLightController::button_pressed() { handle_button_press(); }

void TrafficLightController::timeout_expired() { handle_timeout(); }

bool TrafficLightController::has_pedestrian_request() const {
    return pedestrian_request;
}

void TrafficLightController::handle_event(SystemEvent event) {
    TrafficState new_state = state_machine->get_current_state();
    TrafficState current_state = state_machine->get_current_state();

    bool ret = state_machine->process_event(event);

    if (ret) { // true means that event rised state change
        new_state = state_machine->get_current_state();
    }

    action_handler->handle(current_state, event, new_state);

    handle(current_state, event, new_state);
}

void TrafficLightController::start_timeout(uint32_t duration) const {
    timerService->start_timeout(duration);
}

std::string EnumUtils::state_to_string(TrafficState state) {
    switch (state) {
    case TrafficState::CAR_GREEN:
        return "CAR_GREEN";
    case TrafficState::CAR_YELLOW:
        return "CAR_YELLOW";
    case TrafficState::CAR_RED:
        return "CAR_RED";
    case TrafficState::WALK_PREP:
        return "WALK_PREP";
    case TrafficState::WALK:
        return "WALK";
    case TrafficState::WALK_FINISH:
        return "WALK_FINISH";
    case TrafficState::CAR_RED_YELLOW:
        return "CAR_RED_YELLOW";
    default:
        return "UNKNOWN_STATE";
    }
}

std::string EnumUtils::event_to_string(SystemEvent event) {
    switch (event) {
    case SystemEvent::TIME_EXPIRED:
        return "TIME_EXPIRED";
    case SystemEvent::BUTTON_PRESSED:
        return "BUTTON_PRESSED";
    default:
        return "UNKNOWN_EVENT";
    }
}

TrafficLightActionHandler::TrafficLightActionHandler() {
    std::cout << "Consturctor executed";
}

void TrafficLightActionHandler::handle(TrafficState current_state,
                                       SystemEvent event,
                                       TrafficState next_state) {
    (void)current_state;
    (void)event;
    (void)next_state;
    std::cout << "Injected handler executed" << std::endl;
};
