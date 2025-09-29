#include "console_logger_observer.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void ConsoleLoggerObserver::on_state_transition(TrafficState from,
                                                TrafficEvent event,
                                                TrafficState to) {
    std::string timestamp = show_timestamps ? get_timestamp() + " " : "";

    std::cout << timestamp << prefix;

    if (from != to) {
        std::cout << from << " --[" << event << "]--> " << to << " ✓"
                  << std::endl;
    } else {
        std::cout << "Event " << event << " in state " << from << " (no change)"
                  << std::endl;
    }
}

std::string ConsoleLoggerObserver::get_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) %
              1000;

    std::ostringstream oss;
    oss << "[" << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "]";
    return oss.str();
}
