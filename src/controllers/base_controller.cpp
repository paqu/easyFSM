#include "base_controller.h"

BaseController::BaseController(std::shared_ptr<IStateMachine> sm,
                               std::shared_ptr<IActionHandler> ah)
    : state_machine(sm), action_handler(ah) {}

void BaseController::handle_event(SystemEvent event) {

    TrafficState current_state = state_machine->get_current_state();
    TrafficState new_state = current_state;
    bool ret = state_machine->process_event(event);
    if (ret) { // true means that event rised state change
        new_state = state_machine->get_current_state();
    }

    action_handler->handle(current_state, event, new_state);
}

std::shared_ptr<IStateMachine> BaseController::get_state_machine() const {
    return state_machine;
}

IActionHandler *BaseController::get_action_handler() const {
    return action_handler.get();
}
