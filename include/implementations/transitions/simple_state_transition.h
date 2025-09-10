#pragma once
#include "state_transition.h"
#include "system_events.h"
#include "traffic_states.h"

/**
 * @brief Simple unconditional state transition
 */
class SimpleStateTransition : public IStateTransition {
  private:
    TrafficState from_state;
    SystemEvent trigger_event;
    TrafficState to_state;

  public:
    SimpleStateTransition(TrafficState from_state, SystemEvent trigger_event,
                          TrafficState to_state);

    TrafficState get_from_state() const override;
    SystemEvent get_trigger_event() const override;
    TrafficState get_to_state() const override;

    bool can_transition(TrafficState current_state,
                        SystemEvent event) const override;
};
