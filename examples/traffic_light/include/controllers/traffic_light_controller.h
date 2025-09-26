#pragma once
#include <state_machine/state_machine.h>

#include "traffic_events.h"
#include "traffic_states.h"

using namespace state_machine;
/**
 * @brief Traffic light specific controller
 */
class TrafficLightController
    : public BaseController<TrafficState, TrafficEvent> {
  public:
    TrafficLightController(
        std::shared_ptr<IStateMachine<TrafficState, TrafficEvent>>
            state_machine,
        std::shared_ptr<IActionHandler<TrafficState, TrafficEvent>> ah);

    void button_pressed();
    void timeout_expired();

  private:
    void handle_button_press();
    void handle_timeout();
};
