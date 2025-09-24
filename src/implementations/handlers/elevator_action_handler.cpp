#include "elevator_action_handler.h"
#include "elevator_events.h"
#include "elevator_states.h"
#include "elevator_timings.h"

#include <chrono>
#include <iostream>

ElevatorActionHandler::ElevatorActionHandler(
    std::unique_ptr<IDisplayService> ds, std::unique_ptr<ITimerService> ts,
    int initial_floor)
    : display_service(std::move(ds)), timer_service(std::move(ts)),
      current_floor(initial_floor), target_floor(initial_floor),
      emergency_active(false), obstacle_present(false) {

    std::map<ElevatorState, ElevatorContext> states;
    std::unique_ptr<IDisplayService> display_service;
    std::unique_ptr<ITimerService> timer_service;

    // Initialize all elevator states
    states[ElevatorState::IDLE] = {"IDLE",
                                   ElevatorTimings::IDLE_TIMEOUT,
                                   {false, false, false}, // doors: closed
                                   {false, false, true},  // movement: stopped
                                   initial_floor,
                                   initial_floor};

    states[ElevatorState::DOORS_OPENING] = {
        "DOORS_OPENING",      ElevatorTimings::DOORS_OPENING_DURATION,
        {false, true, false}, // doors: opening
        {false, false, true}, // movement: stopped
        initial_floor,        initial_floor};

    states[ElevatorState::DOORS_OPEN] = {
        "DOORS_OPEN",         ElevatorTimings::DOORS_OPEN_DURATION,
        {true, false, false}, // doors: open
        {false, false, true}, // movement: stopped
        initial_floor,        initial_floor};

    states[ElevatorState::DOORS_CLOSING] = {
        "DOORS_CLOSING",      ElevatorTimings::DOORS_CLOSING_DURATION,
        {false, false, true}, // doors: closing
        {false, false, true}, // movement: stopped
        initial_floor,        initial_floor};

    states[ElevatorState::MOVING_UP] = {
        "MOVING_UP",           ElevatorTimings::FLOOR_TRAVEL_DURATION,
        {false, false, false}, // doors: closed
        {true, false, false},  // movement: moving up
        initial_floor,         initial_floor};

    states[ElevatorState::MOVING_DOWN] = {
        "MOVING_DOWN",         ElevatorTimings::FLOOR_TRAVEL_DURATION,
        {false, false, false}, // doors: closed
        {false, true, false},  // movement: moving down
        initial_floor,         initial_floor};

    states[ElevatorState::EMERGENCY_STOP] = {
        "EMERGENCY_STOP",      ElevatorTimings::EMERGENCY_TIMEOUT,
        {false, false, false}, // doors: closed
        {false, false, true},  // movement: stopped
        initial_floor,         initial_floor};
}

void ElevatorActionHandler::handle(ElevatorState current_state,
                                   ElevatorEvent event,
                                   ElevatorState next_state) {

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

    // Handle specific events
    switch (event) {
    case ElevatorEvent::FLOOR_REQUESTED:
        // Floor request is handled by controller
        break;
    case ElevatorEvent::DOORS_OPEN_REQUESTED:
        // Force doors to open if possible
        break;
    case ElevatorEvent::DOORS_CLOSE_REQUESTED:
        // Force doors to close if possible
        break;
    case ElevatorEvent::EMERGENCY_BUTTON:
        emergency_active = true;
        clear_all_requests();
        break;
    case ElevatorEvent::OBSTACLE_DETECTED:
        obstacle_present = true;
        break;
    case ElevatorEvent::FLOOR_REACHED:
        // Update current floor when reaching target
        current_floor = target_floor;
        break;
    case ElevatorEvent::TIMER_EXPIRED:
        // Handle timeout-specific logic
        if (current_state == ElevatorState::EMERGENCY_STOP) {
            emergency_active = false;
        }
        if (current_state == ElevatorState::DOORS_OPENING ||
            current_state == ElevatorState::DOORS_CLOSING) {
            obstacle_present = false;
        }
        break;
    }

    std::cout << "\n[" << now << "] Elevator Transition: " << current_state
              << " --[" << event << "]--> " << next_state << std::endl;
    std::cout << "Floor: " << current_floor << " -> " << target_floor
              << std::endl;

    display_elevator_state(next_state);
    start_state_timer(next_state);
}

void ElevatorActionHandler::display_elevator_state(ElevatorState state) {
    auto it = states.find(state);
    if (it != states.end()) {
        auto ctx = it->second;

        // Update context with current data
        ctx.current_floor = current_floor;
        ctx.target_floor = target_floor;
        ctx.pending_requests = pending_requests;
        ctx.emergency_active = emergency_active;
        ctx.obstacle_detected = obstacle_present;

        if (display_service) {
            // Note: We need to adapt display_service for elevator context
            // For now, this won't compile - we'll need generic display service
        }
    }
}

void ElevatorActionHandler::start_state_timer(ElevatorState state) {
    auto it = states.find(state);
    if (it != states.end()) {
        const auto &ctx = it->second;
        uint32_t duration_sec = ctx.duration;

        if (timer_service && duration_sec > 0) {
            timer_service->start_timeout(duration_sec);
        }
    }
}

void ElevatorActionHandler::set_state_timeout(ElevatorState state,
                                              uint32_t timeout) {
    states[state].duration = timeout;
}

void ElevatorActionHandler::configure_state(ElevatorState state,
                                            const ElevatorContext &config) {
    states[state] = config;
}
