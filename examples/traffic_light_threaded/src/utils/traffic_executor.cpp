#include "utils/traffic_executor.h"
#include <iostream>

TrafficExecutor::TrafficExecutor(EventHandler event_handler,
                                 InputHandler input_handler)
    : event_handler_(std::move(event_handler)),
      input_handler_(std::move(input_handler)) {}

TrafficExecutor::~TrafficExecutor() { stop(); }

void TrafficExecutor::start() {
    if (running_)
        return;

    running_ = true;

    // Start threads (replaces pthread_create from main.cpp)
    worker_thread_ = std::thread(&TrafficExecutor::worker_loop, this);
    timer_thread_ = std::thread(&TrafficExecutor::timer_loop, this);

    if (input_handler_) {
        input_thread_ = std::thread(&TrafficExecutor::input_loop, this);
    }
}

void TrafficExecutor::stop() {
    running_ = false;

    // Wake up all threads
    event_cv_.notify_all();
    timer_cv_.notify_all();

    // Join threads (replaces pthread_join from main.cpp)
    if (worker_thread_.joinable())
        worker_thread_.join();
    if (input_thread_.joinable())
        input_thread_.join();
    if (timer_thread_.joinable())
        timer_thread_.join();
}

void TrafficExecutor::start_timer(std::chrono::seconds duration) {
    {
        std::lock_guard<std::mutex> lock(timer_mutex_);
        timer_duration_ = duration;
        timer_active_ = true;
    }
    timer_cv_.notify_one();
}

void TrafficExecutor::wait_for_completion() {
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}

// Private methods - modern C++ replacements for main.cpp pthread functions
void TrafficExecutor::worker_loop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(event_mutex_);
        event_cv_.wait(lock,
                       [this] { return !running_ || !event_queue_.empty(); });

        if (!running_)
            break;

        // Process all pending events
        while (!event_queue_.empty()) {
            auto event = event_queue_.front();
            event_queue_.pop();
            lock.unlock();

            if (event_handler_) {
                event_handler_(event);
            }

            lock.lock();
        }
    }
}

void TrafficExecutor::input_loop() {
    char input;
    while (running_ && std::cin >> input) {
        if (input_handler_) {
            input_handler_(input);
        }

        if (input == 'q' || input == 'Q') {
            running_ = false;
            event_cv_.notify_all();
            break;
        }
    }
}

void TrafficExecutor::timer_loop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(timer_mutex_);
        timer_cv_.wait(lock, [this] { return !running_ || timer_active_; });

        if (!running_)
            break;

        if (timer_active_) {
            auto duration = timer_duration_;
            timer_active_ = false;
            lock.unlock();

            std::this_thread::sleep_for(duration);

            if (running_) {
                // Send timeout event
                {
                    std::lock_guard<std::mutex> event_lock(event_mutex_);
                    event_queue_.push(TrafficEvent::TIME_EXPIRED);
                }
                event_cv_.notify_one();
            }
        }
    }
}

void TrafficExecutor::send_button_event() {
    {
        std::lock_guard<std::mutex> lock(event_mutex_);
        event_queue_.push(TrafficEvent::BUTTON_PRESSED);
    }
    event_cv_.notify_one();
}
