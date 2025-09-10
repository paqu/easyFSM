#pragma once
#include "base_controller.h"

/**
 * @brief Traffic light specific controller
 */

class TrafficLightController : public BaseController {
  public:
    TrafficLightController(std::shared_ptr<IStateMachine> state_machine,
                           std::unique_ptr<IActionHandler> ah);

    void button_pressed();
    void timeout_expired();

  private:
    void handle_button_press();
    void handle_timeout();
};
