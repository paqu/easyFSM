#pragma once
#include "traffic_context.h"
#include "traffic_events.h"
#include "traffic_states.h"
#include <map>
#include <memory>
#include <state_machine/state_machine.h>

using namespace state_machine;

class DisplayObserver : public IObserver<TrafficState, TrafficEvent> {
  private:
    std::unique_ptr<IDisplayService<TrafficContext>> display_service;
    std::map<TrafficState, TrafficContext> state_contexts;

  public:
    explicit DisplayObserver(
        std::unique_ptr<IDisplayService<TrafficContext>> service);

    void on_state_transition(TrafficState from, TrafficEvent event,
                             TrafficState to) override;

  private:
    void initialize_contexts();
};
