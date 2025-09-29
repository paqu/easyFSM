#pragma once
#include "traffic_events.h"
#include "traffic_states.h"
#include <state_machine/state_machine.h>

using namespace state_machine;

class ConsoleLoggerObserver : public IObserver<TrafficState, TrafficEvent> {
  private:
    bool show_timestamps;
    std::string prefix;

  public:
    ConsoleLoggerObserver(const std::string &log_prefix = "",
                          bool timestamps = true)
        : show_timestamps(timestamps), prefix(log_prefix) {}

    void on_state_transition(TrafficState from, TrafficEvent event,
                             TrafficState to) override;

    void set_timestamps(bool enabled) { show_timestamps = enabled; }
    void set_prefix(const std::string &new_prefix) { prefix = new_prefix; }

  private:
    std::string get_timestamp() const;
};
