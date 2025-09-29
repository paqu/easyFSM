#pragma once
#include "traffic_events.h"
#include "traffic_states.h"
#include <state_machine/state_machine.h>

using namespace state_machine;

class TrafficController
    : public ObservableController<TrafficState, TrafficEvent> {
  public:
    explicit TrafficController(
        std::shared_ptr<IStateMachine<TrafficState, TrafficEvent>> sm)
        : ObservableController(sm) {}

    void button_pressed() { handle_event(TrafficEvent::BUTTON_PRESSED); }

    void timer_expired() { handle_event(TrafficEvent::TIME_EXPIRED); }

    TrafficState current_state() const {
        return get_state_machine()->get_current_state();
    }
};
