#pragma once
#include "models/traffic_events.h"
#include <state_machine/state_machine.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

// Domain-specific threading wrapper for Traffic Light
class TrafficExecutor {
  public:
    using EventHandler = std::function<void(TrafficEvent)>;
    using InputHandler = std::function<void(char)>;

  private:
    // Modern C++17 threading (replaces pthread mess from main.cpp)
    std::atomic<bool> running_{false};
    std::mutex event_mutex_;
    std::condition_variable event_cv_;
    std::queue<TrafficEvent> event_queue_;

    std::thread worker_thread_;
    std::thread input_thread_;
    std::thread timer_thread_;

    // Callbacks
    EventHandler event_handler_;
    InputHandler input_handler_;

    // Timer state (replaces global variables from main.cpp)
    std::atomic<bool> timer_active_{false};
    std::chrono::steady_clock::duration timer_duration_;
    std::mutex timer_mutex_;
    std::condition_variable timer_cv_;

  public:
    TrafficExecutor(EventHandler event_handler, InputHandler input_handler);
    ~TrafficExecutor();

    // API that replaces main.cpp functions
    void start(); // Replaces pthread_create calls
    void stop();  // Clean shutdown
    void start_timer(std::chrono::seconds duration); // Replaces start_timeout()
    void wait_for_completion(); // Replaces pthread_join calls
    void send_button_event();

  private:
    // Modern C++ replacements for main.cpp pthread functions
    void worker_loop(); // Replaces worker()
    void input_loop();  // Replaces read_char()
    void timer_loop();  // Replaces timeout_generator()
};
