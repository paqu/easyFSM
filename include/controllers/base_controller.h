#pragma once

#include "action_handler.h"
#include "state_machine.h"

#include <memory>

/**
 * @brief Base controller providing common functionality
 */
#pragma once
#include "action_handler.h"
#include "state_machine.h"
#include <memory>

/**
 * @brief Base controller providing common functionality
 */
template <typename StateType, typename EventType> class BaseController {
  private:
    std::shared_ptr<IStateMachine<StateType, EventType>> state_machine;
    std::shared_ptr<IActionHandler<StateType, EventType>> action_handler;

  public:
    BaseController(std::shared_ptr<IStateMachine<StateType, EventType>> sm,
                   std::shared_ptr<IActionHandler<StateType, EventType>> ah)
        : state_machine(sm), action_handler(ah) {}

    virtual ~BaseController() = default;

  protected:
    void handle_event(EventType event) {
        StateType current_state = state_machine->get_current_state();
        StateType new_state = current_state;
        bool ret = state_machine->process_event(event);
        if (ret) { // true means that event raised state change
            new_state = state_machine->get_current_state();
        }
        action_handler->handle(current_state, event, new_state);
    }

    std::shared_ptr<IStateMachine<StateType, EventType>>
    get_state_machine() const {
        return state_machine;
    }

    IActionHandler<StateType, EventType> *get_action_handler() const {
        return action_handler.get();
    }
};
