#pragma once

#include "action_handler.h"
#include "state_machine.h"
#include "system_events.h"

#include <memory>

/**
 * @brief Base controller providing common functionality
 */
class BaseController {
  private:
    std::shared_ptr<IStateMachine> state_machine;
    std::unique_ptr<IActionHandler> action_handler;

  public:
    BaseController(std::shared_ptr<IStateMachine> state_machine,
                   std::unique_ptr<IActionHandler> action_handler);
    virtual ~BaseController() = default;

  protected:
    void handle_event(SystemEvent event);

    std::shared_ptr<IStateMachine> get_state_machine() const;
    IActionHandler *get_action_handler() const;
};
