#include "elevator_controller.h"

ElevatorController::ElevatorController(
    std::shared_ptr<IStateMachine<ElevatorState, ElevatorEvent>> sm,
    std::shared_ptr<IActionHandler<ElevatorState, ElevatorEvent>> ah,
    int min_floor, int max_floor)
    : BaseController(sm, ah), current_floor(0), target_floor(0),
      min_floor(min_floor), max_floor(max_floor) {}

void ElevatorController::request_floor(int floor) {
    if (floor >= min_floor && floor <= max_floor && floor != current_floor) {
        floor_requests.insert(floor);
        handle_floor_request(floor);
    }
}

void ElevatorController::open_doors() { handle_doors_open_request(); }

void ElevatorController::close_doors() { handle_doors_close_request(); }

void ElevatorController::emergency_stop() {
    floor_requests.clear();
    handle_emergency();
}

void ElevatorController::timer_expired() { handle_timer(); }

void ElevatorController::floor_reached() {
    // Remove current floor from requests if it was requested
    floor_requests.erase(current_floor);
    handle_floor_reached();
}

void ElevatorController::obstacle_detected() { handle_obstacle(); }

// Private methods
void ElevatorController::handle_floor_request(int floor) {
    update_target_floor();
    handle_event(ElevatorEvent::FLOOR_REQUESTED);
}

void ElevatorController::handle_doors_open_request() {
    handle_event(ElevatorEvent::DOORS_OPEN_REQUESTED);
}

void ElevatorController::handle_doors_close_request() {
    handle_event(ElevatorEvent::DOORS_CLOSE_REQUESTED);
}

void ElevatorController::handle_emergency() {
    handle_event(ElevatorEvent::EMERGENCY_BUTTON);
}

void ElevatorController::handle_timer() {
    handle_event(ElevatorEvent::TIMER_EXPIRED);
}

void ElevatorController::handle_floor_reached() {
    current_floor = target_floor;
    update_target_floor();
    handle_event(ElevatorEvent::FLOOR_REACHED);
}

void ElevatorController::handle_obstacle() {
    handle_event(ElevatorEvent::OBSTACLE_DETECTED);
}

void ElevatorController::update_target_floor() {
    if (floor_requests.empty()) {
        target_floor = current_floor;
        return;
    }

    // Simple algorithm: go to closest floor first
    int closest_distance = std::abs(*floor_requests.begin() - current_floor);
    target_floor = *floor_requests.begin();

    for (int floor : floor_requests) {
        int distance = std::abs(floor - current_floor);
        if (distance < closest_distance) {
            closest_distance = distance;
            target_floor = floor;
        }
    }
}

bool ElevatorController::should_move_up() const {
    return target_floor > current_floor;
}

bool ElevatorController::should_move_down() const {
    return target_floor < current_floor;
}
