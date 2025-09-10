#include "traffic_light_factory.h"
#include "light_timings.h"
#include "runtime_state_machine.h"
#include "simple_state_transition.h"
#include "traffic_light_action_handler.h"
#include "traffic_light_transition.h"

std::unique_ptr<TrafficLightController>
StandardTrafficLightFactory::create_controller(
    std::unique_ptr<IDisplayService> display_service,
    std::unique_ptr<ITimerService> timer_service) {

    // Create state machine starting with CAR_GREEN
    auto state_machine =
        std::make_shared<RuntimeStateMachine>(TrafficState::CAR_GREEN);
    // Create action handler
    auto action_handler = std::make_shared<TrafficLightActionHandler>(
        std::move(display_service), std::move(timer_service));

    // Get reference to check pedestrian requests
    auto ped_check = [action_handler]() -> bool {
        return action_handler->has_pedestrian_request();
    };

    // GREEN -> YELLOW (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_GREEN, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_YELLOW));

    // YELLOW -> RED (timeout) - conditional based on pedestrian request
    state_machine->add_transition(std::make_unique<TrafficLightTransition>(
        TrafficState::CAR_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED, TrafficState::WALK_PREP, ped_check));

    // RED -> RED_YELLOW (timeout) - only if no pedestrian sequence
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED_YELLOW));

    // RED_YELLOW -> GREEN (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_GREEN));

    // Pedestrian sequence transitions
    // WALK_PREP -> WALK (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_PREP, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK));

    // WALK -> WALK_FINISH (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK_FINISH));

    // WALK_FINISH -> RED_YELLOW (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_FINISH, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED_YELLOW));

    return std::make_unique<TrafficLightController>(state_machine,
                                                    action_handler);
}

std::string StandardTrafficLightFactory::get_factory_name() const {
    return "Standard Traffic Light (with RED_YELLOW)";
}

std::unique_ptr<TrafficLightController>
SimpleTrafficLightFactory::create_controller(
    std::unique_ptr<IDisplayService> display_service,
    std::unique_ptr<ITimerService> timer_service) {

    // Create state machine starting with CAR_GREEN
    auto state_machine =
        std::make_shared<RuntimeStateMachine>(TrafficState::CAR_GREEN);

    // Create action handler
    auto action_handler = std::make_shared<TrafficLightActionHandler>(
        std::move(display_service), std::move(timer_service));

    // Configure modified timing for YELLOW state (longer duration instead of
    // RED_YELLOW)
    action_handler->set_state_timeout(TrafficState::CAR_YELLOW,
                                      LightTimings::YELLOW_DURATION +
                                          LightTimings::RED_YELLOW_DURATION);

    // Add transitions for simple traffic light (WITHOUT RED_YELLOW)

    // GREEN -> YELLOW (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_GREEN, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_YELLOW));

    state_machine->add_transition(std::make_unique<TrafficLightTransition>(
        TrafficState::CAR_YELLOW, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_RED, TrafficState::WALK_PREP,
        [action_handler]() -> bool {
            return action_handler->has_pedestrian_request();
        }));

    // RED -> GREEN directly (timeout) - NO RED_YELLOW state
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::CAR_RED, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_GREEN));

    // Pedestrian sequence transitions
    // WALK_PREP -> WALK (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_PREP, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK));

    // WALK -> WALK_FINISH (timeout)
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK, SystemEvent::TIME_EXPIRED,
        TrafficState::WALK_FINISH));

    // WALK_FINISH -> GREEN directly (timeout) - NO RED_YELLOW state
    state_machine->add_transition(std::make_unique<SimpleStateTransition>(
        TrafficState::WALK_FINISH, SystemEvent::TIME_EXPIRED,
        TrafficState::CAR_GREEN));

    return std::make_unique<TrafficLightController>(state_machine,
                                                    action_handler);
}

std::string SimpleTrafficLightFactory::get_factory_name() const {
    return "Simple Traffic Light (without RED_YELLOW)";
}
