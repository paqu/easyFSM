#include "elevator_factory.h"
#include "conditional_state_transation.h"
#include "elevator_action_handler.h"
#include "runtime_state_machine.h"
#include "simple_state_transition.h"

std::unique_ptr<ElevatorController> ElevatorFactory::create_controller(
    ElevatorType type,
    std::unique_ptr<IDisplayService<ElevatorContext>> display_service,
    std::unique_ptr<ITimerService> timer_service, int min_floor,
    int max_floor) {

    switch (type) {
    case ElevatorType::BASIC:
        return create_basic_controller(std::move(display_service),
                                       std::move(timer_service), min_floor,
                                       max_floor);
    case ElevatorType::ADVANCED:
        return create_advanced_controller(std::move(display_service),
                                          std::move(timer_service), min_floor,
                                          max_floor);
    default:
        return create_basic_controller(std::move(display_service),
                                       std::move(timer_service), min_floor,
                                       max_floor);
    }
}

std::unique_ptr<ElevatorController> ElevatorFactory::create_basic_controller(
    std::unique_ptr<IDisplayService<ElevatorContext>> display_service,
    std::unique_ptr<ITimerService> timer_service, int min_floor,
    int max_floor) {

    auto state_machine =
        std::make_shared<RuntimeStateMachine<ElevatorState, ElevatorEvent>>(
            ElevatorState::IDLE);

    auto action_handler = std::make_shared<ElevatorActionHandler>(
        std::move(display_service), std::move(timer_service), min_floor);

    // Create controller first to get access to its methods
    auto controller = std::make_unique<ElevatorController>(
        state_machine, action_handler, min_floor, max_floor);

    // Get raw pointer for lambda capture (safe because controller owns the
    // shared_ptr)
    auto controller_ptr = controller.get();

    auto has_requests = [controller_ptr]() -> bool {
        return controller_ptr->has_pending_requests();
    };

    auto should_move_up = [controller_ptr]() -> bool {
        return controller_ptr->get_target_floor() >
               controller_ptr->get_current_floor();
    };

    auto should_move_down = [controller_ptr]() -> bool {
        return controller_ptr->get_target_floor() <
               controller_ptr->get_current_floor();
    };

    setup_basic_transitions(state_machine, has_requests, should_move_up,
                            should_move_down);

    return controller;
}

std::unique_ptr<ElevatorController> ElevatorFactory::create_advanced_controller(
    std::unique_ptr<IDisplayService<ElevatorContext>> display_service,
    std::unique_ptr<ITimerService> timer_service, int min_floor,
    int max_floor) {

    auto state_machine =
        std::make_shared<RuntimeStateMachine<ElevatorState, ElevatorEvent>>(
            ElevatorState::IDLE);

    auto action_handler = std::make_shared<ElevatorActionHandler>(
        std::move(display_service), std::move(timer_service), min_floor);

    auto controller = std::make_unique<ElevatorController>(
        state_machine, action_handler, min_floor, max_floor);

    auto controller_ptr = controller.get();
    auto handler_ptr = action_handler.get();

    auto has_requests = [controller_ptr]() -> bool {
        return controller_ptr->has_pending_requests();
    };

    auto should_move_up = [controller_ptr]() -> bool {
        return controller_ptr->get_target_floor() >
               controller_ptr->get_current_floor();
    };

    auto should_move_down = [controller_ptr]() -> bool {
        return controller_ptr->get_target_floor() <
               controller_ptr->get_current_floor();
    };

    auto obstacle_detected = [handler_ptr]() -> bool {
        return handler_ptr->is_obstacle_present();
    };

    setup_advanced_transitions(state_machine, has_requests, should_move_up,
                               should_move_down, obstacle_detected);

    return controller;
}

void ElevatorFactory::setup_basic_transitions(
    std::shared_ptr<RuntimeStateMachine<ElevatorState, ElevatorEvent>>
        state_machine,
    std::function<bool()> has_requests, std::function<bool()> should_move_up,
    std::function<bool()> should_move_down) {

    // From IDLE
    state_machine->add_transition(
        std::make_unique<
            ConditionalStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::IDLE, ElevatorEvent::FLOOR_REQUESTED,
            ElevatorState::DOORS_OPENING, ElevatorState::DOORS_OPENING,
            has_requests));

    // From DOORS_OPENING
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_OPENING, ElevatorEvent::TIMER_EXPIRED,
            ElevatorState::DOORS_OPEN));

    // From DOORS_OPEN
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_OPEN, ElevatorEvent::TIMER_EXPIRED,
            ElevatorState::DOORS_CLOSING));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_OPEN, ElevatorEvent::DOORS_CLOSE_REQUESTED,
            ElevatorState::DOORS_CLOSING));

    // From DOORS_CLOSING
    state_machine->add_transition(
        std::make_unique<
            ConditionalStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_CLOSING, ElevatorEvent::TIMER_EXPIRED,
            ElevatorState::IDLE, ElevatorState::MOVING_UP, should_move_up));

    state_machine->add_transition(
        std::make_unique<
            ConditionalStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_CLOSING, ElevatorEvent::TIMER_EXPIRED,
            ElevatorState::IDLE, ElevatorState::MOVING_DOWN, should_move_down));

    // From MOVING_UP
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::MOVING_UP, ElevatorEvent::FLOOR_REACHED,
            ElevatorState::DOORS_OPENING));

    // From MOVING_DOWN
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::MOVING_DOWN, ElevatorEvent::FLOOR_REACHED,
            ElevatorState::DOORS_OPENING));

    // Emergency from any state
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::IDLE, ElevatorEvent::EMERGENCY_BUTTON,
            ElevatorState::EMERGENCY_STOP));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::EMERGENCY_STOP, ElevatorEvent::TIMER_EXPIRED,
            ElevatorState::IDLE));
}

void ElevatorFactory::setup_advanced_transitions(
    std::shared_ptr<RuntimeStateMachine<ElevatorState, ElevatorEvent>>
        state_machine,
    std::function<bool()> has_requests, std::function<bool()> should_move_up,
    std::function<bool()> should_move_down,
    std::function<bool()> obstacle_detected) {

    // Start with basic transitions
    setup_basic_transitions(state_machine, has_requests, should_move_up,
                            should_move_down);

    // Add advanced safety features
    // Obstacle detection during door closing
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_CLOSING, ElevatorEvent::OBSTACLE_DETECTED,
            ElevatorState::DOORS_OPENING));

    // Re-open doors if obstacle detected during opening
    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<ElevatorState, ElevatorEvent>>(
            ElevatorState::DOORS_OPENING, ElevatorEvent::OBSTACLE_DETECTED,
            ElevatorState::DOORS_OPENING)); // Restart opening cycle
}
