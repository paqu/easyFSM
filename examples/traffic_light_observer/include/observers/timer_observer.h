#pragma once
#include "traffic_events.h"
#include "traffic_states.h"
#include <map>
#include <memory>
#include <state_machine/state_machine.h>

using namespace state_machine;

class TimerObserver : public IObserver<TrafficState, TrafficEvent> {
  private:
    std::unique_ptr<ITimerService> timer_service;
    std::map<TrafficState, uint32_t> durations;

  public:
    explicit TimerObserver(std::unique_ptr<ITimerService> service);

    void on_state_transition(TrafficState from, TrafficEvent event,
                             TrafficState to) override;

    void set_duration(TrafficState state, uint32_t seconds);

  private:
    void initialize_durations();
};
