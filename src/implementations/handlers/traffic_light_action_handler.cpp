#include "traffic_light_action_handler.h"
#include "light_timings.h"
#include "system_events.h"
#include "traffic_states.h"

#include <chrono>
#include <iostream>

TrafficLightActionHandler::TrafficLightActionHandler(
    std::unique_ptr<IDisplayService> ds, std::unique_ptr<ITimerService> ts)
    : pedestrian_request(false), display_service(std::move(ds)),
      timer_service(std::move(ts)) {

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

void TrafficLightActionHandler::handle(TrafficState current_state,
                                       SystemEvent event,
                                       TrafficState next_state) {

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    if (event == SystemEvent::BUTTON_PRESSED) {
        handle_button_press_event();
        return;
    } else if (event == SystemEvent::TIME_EXPIRED &&
               next_state == TrafficState::WALK_FINISH) {
        pedestrian_request = false;
    }
    std::cout << current_state << std::endl;

    std::cout << "\n[" << now << "] Transition: " << current_state << " --["
              << event << "]--> " << next_state << std::endl;

    display_traffic_state(next_state);
    start_state_timer(next_state);
};

bool TrafficLightActionHandler::has_pedestrian_request() const {
    return pedestrian_request;
}

void TrafficLightActionHandler::display_traffic_state(TrafficState state) {
    /* Lookup guard */
    auto it = states.find(state);
    if (it != states.end()) {
        const auto &ctx = it->second;

        if (display_service) {
            display_service->show_state(ctx);
        }
    }
}

void TrafficLightActionHandler::start_state_timer(TrafficState state) {
    /* Lookup guard */
    auto it = states.find(state);
    if (it != states.end()) {
        const auto &ctx = it->second;
        uint32_t duration_sec = ctx.duration;

        if (timer_service) {

            timer_service->start_timeout(duration_sec);
        }
    }
}

void TrafficLightActionHandler::handle_button_press_event() {
    bool waiting_to_be_processed = false;
    if (!pedestrian_request) {
        pedestrian_request = true;

        if (display_service) {
            display_service->show_button_state(waiting_to_be_processed);
        }
    } else {
        waiting_to_be_processed = true;
        if (display_service) {
            display_service->show_button_state(waiting_to_be_processed);
        }
    }
}
void TrafficLightActionHandler::set_state_timeout(const TrafficState state,
                                                  uint32_t timeout) {
    states[state].duration = timeout;
}

void TrafficLightActionHandler::configure_state(TrafficState state,
                                                const StateContext &config) {
    states[state] = config;
}
