#pragma once
#include "base_controller.h"

#include "system_events.h"
#include "traffic_states.h"

/**
 * @brief Traffic light specific controller
 */
class TrafficLightController
    : public BaseController<TrafficState, SystemEvent> {
  public:
    TrafficLightController(
        std::shared_ptr<IStateMachine<TrafficState, SystemEvent>> state_machine,
        std::shared_ptr<IActionHandler<TrafficState, SystemEvent>> ah);

    void button_pressed();
    void timeout_expired();

  private:
    void handle_button_press();
    void handle_timeout();
};
