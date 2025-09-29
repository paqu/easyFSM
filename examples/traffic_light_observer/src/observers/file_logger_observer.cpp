#include "file_logger_observer.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>

FileLoggerObserver::FileLoggerObserver(const std::string &log_filename,
                                       bool flush_immediately)
    : filename(log_filename), auto_flush(flush_immediately) {

    log_file.open(filename, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        throw std::runtime_error("Cannot open log file: " + filename);
    }

    write_session_marker("SESSION_START");
}

FileLoggerObserver::~FileLoggerObserver() {
    if (log_file.is_open()) {
        write_session_marker("SESSION_END");
        log_file.close();
    }
}

void FileLoggerObserver::on_state_transition(TrafficState from,
                                             TrafficEvent event,
                                             TrafficState to) {
    if (!log_file.is_open())
        return;

    log_file << format_timestamp() << " | ";
    log_file << std::setw(8) << from << " --[" << std::setw(12) << event
             << "]--> ";
    log_file << std::setw(8) << to;

    if (from == to) {
        log_file << " (no change)";
    }

    log_file << std::endl;

    if (auto_flush) {
        log_file.flush();
    }
}

std::string FileLoggerObserver::format_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) %
              1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void FileLoggerObserver::write_session_marker(const std::string &marker) {
    if (!log_file.is_open())
        return;

    log_file << std::string(60, '=') << std::endl;
    log_file << format_timestamp() << " | " << marker << std::endl;
    log_file << std::string(60, '=') << std::endl;

    if (auto_flush) {
        log_file.flush();
    }
}
