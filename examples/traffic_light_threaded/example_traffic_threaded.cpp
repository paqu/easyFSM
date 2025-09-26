#include <iostream>
#include <memory>
#include <state_machine/state_machine.h>

// Traffic light domain types
#include "controllers/traffic_light_controller.h"
#include "factories/traffic_light_factory.h"
#include "models/traffic_events.h"
#include "services/ascii_display_service.h"

// Modern threading
#include "utils/traffic_executor.h"

using namespace state_machine;

class TrafficLightApp {
  private:
    std::unique_ptr<TrafficLightController> controller_;
    std::unique_ptr<TrafficExecutor> executor_;

  public:
    TrafficLightApp() {
        // Create timer service that integrates with TrafficExecutor
        auto timer_service = std::make_unique<FunctionTimerService>(
            [this](uint32_t duration_sec) {
                executor_->start_timer(std::chrono::seconds(duration_sec));
            });

        // Create controller using factory (like in demo())
        controller_ = TrafficLightFactory::create_controller(
            TrafficLightType::STANDARD, std::make_unique<AsciiDisplayService>(),
            std::move(timer_service));

        // Create executor with event/input handlers
        executor_ = std::make_unique<TrafficExecutor>(
            [this](TrafficEvent event) { handle_event(event); },
            [this](char input) { handle_input(input); });
    }

    void run_demo() {
        std::cout << "=== Demo Mode (from original main.cpp) ===" << std::endl;

        // Port demo() logic - show both standard and simple
        demo_standard();
        demo_simple();
    }

    void run_simulation() {
        std::cout << "=== Simulation Mode (from original main.cpp) ==="
                  << std::endl;
        std::cout << "Press any key for pedestrian button, 'q' to quit"
                  << std::endl;

        // Start threaded simulation (replaces original pthread code)
        executor_->start();

        // Start initial timer (replaces start_timeout(1) from main.cpp)
        executor_->start_timer(std::chrono::seconds(1));

        // Wait for completion (replaces pthread_join calls)
        executor_->wait_for_completion();

        std::cout << "Simulation ended." << std::endl;
    }

  private:
    void handle_event(TrafficEvent event) {
        // Replaces process_traffic_light() from main.cpp
        switch (event) {
        case TrafficEvent::TIME_EXPIRED:
            controller_->timeout_expired();
            break;
        case TrafficEvent::BUTTON_PRESSED:
            controller_->button_pressed();
            break;
        }
    }

    void handle_input(char input) {
        if (input != 'q' && input != 'Q') {
            std::cout << "Pedestrian button pressed!" << std::endl;
            executor_->send_button_event();
        }
    }

    void demo_standard() {
        // Port demo() function for standard traffic light
        std::cout << "\n=== Standard Traffic Light Demo ===" << std::endl;

        controller_->timeout_expired(); // GREEN -> YELLOW
        controller_->timeout_expired(); // YELLOW -> RED
        controller_->timeout_expired(); // RED -> RED_YELLOW
        controller_->timeout_expired(); // RED_YELLOW -> GREEN

        // Test pedestrian
        controller_->button_pressed();
        controller_->timeout_expired(); // GREEN -> YELLOW
        controller_->timeout_expired(); // YELLOW -> WALK_PREP (pedestrian)
        controller_->timeout_expired(); // WALK_PREP -> WALK
        controller_->timeout_expired(); // WALK -> WALK_FINISH
        controller_->timeout_expired(); // WALK_FINISH -> RED_YELLOW
    }

    void demo_simple() {
        std::cout << "\n=== Simple Traffic Light Demo ===" << std::endl;

        std::function<void(uint32_t)> simple_timer_func =
            [](uint32_t duration) {
                std::cout << "Simple timer: " << duration << "s" << std::endl;
            };
        // Create simple controller for comparison
        auto simple_controller = TrafficLightFactory::create_controller(
            TrafficLightType::SIMPLE, std::make_unique<AsciiDisplayService>(),
            std::make_unique<FunctionTimerService>(simple_timer_func));

        simple_controller->timeout_expired(); // GREEN -> YELLOW
        simple_controller->timeout_expired(); // YELLOW -> RED
        simple_controller->timeout_expired(); // RED -> GREEN (no RED_YELLOW)
    }
};

int main() {
    std::cout << "Choose mode:" << std::endl;
    std::cout << "1. Demo" << std::endl;
    std::cout << "2. Simulation" << std::endl;
    std::cout << "Enter choice (1 or 2): ";

    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        choice = 2;
    }

    try {
        TrafficLightApp app;

        if (choice == 1) {
            app.run_demo();
        } else {
            app.run_simulation();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
