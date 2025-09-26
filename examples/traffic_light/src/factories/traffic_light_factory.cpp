#include "traffic_light_factory.h"
#include "light_timings.h"
#include "traffic_light_action_handler.h"
#include <state_machine/state_machine.h>

std::unique_ptr<TrafficLightController> TrafficLightFactory::create_controller(
    TrafficLightType type,
    std::unique_ptr<IDisplayService<TrafficContext>> display_service,
    std::unique_ptr<ITimerService> timer_service) {

    switch (type) {
    case TrafficLightType::STANDARD:
        return create_standard_controller(std::move(display_service),
                                          std::move(timer_service));
    case TrafficLightType::SIMPLE:
        return create_simple_controller(std::move(display_service),
                                        std::move(timer_service));
    default:
        return create_standard_controller(std::move(display_service),
                                          std::move(timer_service));
    }
}

std::unique_ptr<TrafficLightController>
TrafficLightFactory::create_standard_controller(
    std::unique_ptr<IDisplayService<TrafficContext>> display_service,
    std::unique_ptr<ITimerService> timer_service) {

    auto state_machine =
        std::make_shared<RuntimeStateMachine<TrafficState, TrafficEvent>>(
            TrafficState::CAR_GREEN);

    auto action_handler = std::make_shared<TrafficLightActionHandler>(
        std::move(display_service), std::move(timer_service));

    auto ped_check = [action_handler]() -> bool {
        return action_handler->has_pedestrian_request();
    };

    setup_standard_transitions(state_machine, ped_check);

    return std::make_unique<TrafficLightController>(state_machine,
                                                    action_handler);
}

std::unique_ptr<TrafficLightController>
TrafficLightFactory::create_simple_controller(
    std::unique_ptr<IDisplayService<TrafficContext>> display_service,
    std::unique_ptr<ITimerService> timer_service) {

    auto state_machine =
        std::make_shared<RuntimeStateMachine<TrafficState, TrafficEvent>>(
            TrafficState::CAR_GREEN);

    auto action_handler = std::make_shared<TrafficLightActionHandler>(
        std::move(display_service), std::move(timer_service));

    // Configure modified timing for YELLOW state (longer duration instead of
    // RED_YELLOW)
    action_handler->set_state_timeout(TrafficState::CAR_YELLOW,
                                      LightTimings::YELLOW_DURATION +
                                          LightTimings::RED_YELLOW_DURATION);

    auto ped_check = [action_handler]() -> bool {
        return action_handler->has_pedestrian_request();
    };

    setup_simple_transitions(state_machine, ped_check);

    return std::make_unique<TrafficLightController>(state_machine,
                                                    std::move(action_handler));
}

void TrafficLightFactory::setup_standard_transitions(
    std::shared_ptr<RuntimeStateMachine<TrafficState, TrafficEvent>>
        state_machine,
    std::function<bool()> ped_check) {

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_GREEN, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_YELLOW));

    state_machine->add_transition(
        std::make_unique<
            ConditionalStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_YELLOW, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED, TrafficState::WALK_PREP, ped_check));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_RED, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED_YELLOW));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_RED_YELLOW, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_GREEN));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK_PREP, TrafficEvent::TIME_EXPIRED,
            TrafficState::WALK));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK, TrafficEvent::TIME_EXPIRED,
            TrafficState::WALK_FINISH));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK_FINISH, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED_YELLOW));
}

void TrafficLightFactory::setup_simple_transitions(
    std::shared_ptr<RuntimeStateMachine<TrafficState, TrafficEvent>>
        state_machine,
    std::function<bool()> ped_check) {

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_GREEN, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_YELLOW));

    state_machine->add_transition(
        std::make_unique<
            ConditionalStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_YELLOW, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED, TrafficState::WALK_PREP, ped_check));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_RED, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_GREEN));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK_PREP, TrafficEvent::TIME_EXPIRED,
            TrafficState::WALK));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK, TrafficEvent::TIME_EXPIRED,
            TrafficState::WALK_FINISH));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK_FINISH, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_GREEN));
}
