#include "traffic_light_controller.h"

TrafficLightController::TrafficLightController(
    std::shared_ptr<IStateMachine<TrafficState, SystemEvent>> sm,
    std::shared_ptr<IActionHandler<TrafficState, SystemEvent>> ah)
    : BaseController(sm, std::move(ah)) {}

void TrafficLightController::handle_button_press() {
    handle_event(SystemEvent::BUTTON_PRESSED);
}

void TrafficLightController::handle_timeout() {
    handle_event(SystemEvent::TIME_EXPIRED);
}

void TrafficLightController::button_pressed() { handle_button_press(); }

void TrafficLightController::timeout_expired() { handle_timeout(); }
