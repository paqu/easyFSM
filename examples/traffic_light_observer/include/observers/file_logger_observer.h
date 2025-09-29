#pragma once
#include "traffic_events.h"
#include "traffic_states.h"
#include <fstream>
#include <state_machine/state_machine.h>
#include <string>

using namespace state_machine;

class FileLoggerObserver : public IObserver<TrafficState, TrafficEvent> {
  private:
    std::ofstream log_file;
    std::string filename;
    bool auto_flush;

  public:
    FileLoggerObserver(const std::string &log_filename,
                       bool flush_immediately = true);
    ~FileLoggerObserver();

    void on_state_transition(TrafficState from, TrafficEvent event,
                             TrafficState to) override;

    void flush() {
        if (log_file.is_open())
            log_file.flush();
    }

  private:
    std::string format_timestamp() const;
    void write_session_marker(const std::string &marker);
};
