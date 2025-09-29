#pragma once
#include "traffic_events.h"
#include "traffic_states.h"
#include <chrono>
#include <state_machine/state_machine.h>

using namespace state_machine;

class PedestrianObserver : public IObserver<TrafficState, TrafficEvent> {
  private:
    bool request_pending;
    std::chrono::steady_clock::time_point request_time;
    bool debug_mode;

  public:
    explicit PedestrianObserver(bool debug = true)
        : request_pending(false), debug_mode(debug) {}

    void on_state_transition(TrafficState from, TrafficEvent event,
                             TrafficState to) override;

    bool has_request() const { return request_pending; }
    void clear_request();

    void set_debug_mode(bool enabled) { debug_mode = enabled; }

  private:
    void log_debug(const std::string &message) const;
};
