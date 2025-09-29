#include <chrono>
#include <iostream>
#include <state_machine/state_machine.h>
#include <thread>

#include "traffic_controller.h"
/*
#include "factories/traffic_light_factory.h"
*/
#include "console_display_service.h"
#include "console_logger_observer.h"
#include "display_observer.h"
#include "file_logger_observer.h"
#include "pedestrian_observer.h"
#include "timer_observer.h"
#include "traffic_events.h"
#include "traffic_states.h"

using namespace state_machine;

auto create_state_machine() {
    auto state_machine =
        std::make_shared<RuntimeStateMachine<TrafficState, TrafficEvent>>(
            TrafficState::CAR_GREEN);

    std::cout << "Creating state machine with initial state: "
              << TrafficState::CAR_GREEN << std::endl;

    return state_machine;
}
auto create_controller(
    std::shared_ptr<RuntimeStateMachine<TrafficState, TrafficEvent>> sm) {
    auto controller = std::make_unique<TrafficController>(sm);

    std::cout << "Traffic controller created" << std::endl;
    std::cout << "Current state: " << controller->current_state() << std::endl;

    return controller;
}

auto create_display_service() {
    auto display_service = std::make_unique<ConsoleDisplayService>();
    std::cout << "Display service created" << std::endl;
    return display_service;
}

// Create timer service
auto create_timer_service() {
    std::function<void(uint32_t)> timer_func = [](uint32_t duration) {
        std::this_thread::sleep_for(std::chrono::seconds(duration));
    };

    auto timer_service = std::make_unique<FunctionTimerService>(timer_func);
    std::cout << "Timer service created" << std::endl;
    return timer_service;
}

// Create all observers
auto create_observers(std::unique_ptr<ConsoleDisplayService> display_service,
                      std::unique_ptr<FunctionTimerService> timer_service) {

    std::cout << "Creating observers..." << std::endl;

    // Console logger
    auto console_logger = std::make_shared<ConsoleLoggerObserver>("", false);
    std::cout << "   Console logger observer" << std::endl;
    // Display observer
    auto display_observer =
        std::make_shared<DisplayObserver>(std::move(display_service));
    std::cout << "   Display observer" << std::endl;

    // Timer observer
    auto timer_observer =
        std::make_shared<TimerObserver>(std::move(timer_service));
    std::cout << "   Timer observer" << std::endl;
    // Pedestrian observer
    auto pedestrian_observer = std::make_shared<PedestrianObserver>(true);
    std::cout << "   Pedestrian observer" << std::endl;
    // File logger observer
    auto file_logger =
        std::make_shared<FileLoggerObserver>("traffic_observer_demo.log");
    std::cout << "   File logger observer" << std::endl;

    return std::make_tuple(console_logger, display_observer, timer_observer,
                           pedestrian_observer, file_logger);
}

// Setup state transitions
void setup_transitions(
    std::shared_ptr<RuntimeStateMachine<TrafficState, TrafficEvent>>
        state_machine,
    std::shared_ptr<PedestrianObserver> pedestrian_obs) {

    std::cout << "Setting up state transitions..." << std::endl;

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_GREEN, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_YELLOW));

    // YELLOW -> RED or WALK (conditional based on pedestrian request)
    auto ped_check = [pedestrian_obs]() -> bool {
        return pedestrian_obs->has_request();
    };

    state_machine->add_transition(
        std::make_unique<
            ConditionalStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_YELLOW, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED, TrafficState::WALK_PREP, ped_check));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_RED, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED_YELLOW));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::CAR_RED_YELLOW, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_GREEN));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK_PREP, TrafficEvent::TIME_EXPIRED,
            TrafficState::WALK));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK, TrafficEvent::TIME_EXPIRED,
            TrafficState::WALK_FINISH));

    state_machine->add_transition(
        std::make_unique<SimpleStateTransition<TrafficState, TrafficEvent>>(
            TrafficState::WALK_FINISH, TrafficEvent::TIME_EXPIRED,
            TrafficState::CAR_RED_YELLOW));

    std::cout << "State transitions configured" << std::endl;
}

void attach_observers(TrafficController *controller,
                      std::shared_ptr<ConsoleLoggerObserver> console_logger,
                      std::shared_ptr<DisplayObserver> display_observer,
                      std::shared_ptr<TimerObserver> timer_observer,
                      std::shared_ptr<PedestrianObserver> pedestrian_observer,
                      std::shared_ptr<FileLoggerObserver> file_logger) {

    std::cout << "Attaching observers to controller..." << std::endl;

    controller->add_observer(console_logger);
    controller->add_observer(display_observer);
    controller->add_observer(pedestrian_observer);
    controller->add_observer(file_logger);
    controller->add_observer(timer_observer);

    std::cout << "All observers attached" << std::endl;
}

void demo_pedestrian_cycle(TrafficController *controller) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "DEMO: Pedestrian Crossing" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    controller->timer_expired();
    controller->timer_expired();
    controller->timer_expired();
    controller->button_pressed();
    controller->button_pressed();
    controller->timer_expired();
    controller->timer_expired();
    controller->timer_expired();
    controller->timer_expired();
    controller->timer_expired();
    controller->timer_expired();
    controller->timer_expired();

    std::cout << "\nPedestrian cycle completed!" << std::endl;
}

void demo_pedestrian_cycle_with_timing(TrafficController *controller) {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "DEMO: Pedestrian Crossing with Timing Analysis" << std::endl;
    std::cout << std::string(70, '=') << std::endl;

    // Storage for timing data
    std::vector<std::chrono::steady_clock::time_point> timestamps;
    std::vector<std::string> actions;

    // Helper lambda to record action with timestamp
    auto record_action = [&](const std::string &action, auto func) {
        timestamps.push_back(std::chrono::steady_clock::now());
        actions.push_back(action);
        func();
    };

    // Record the start time
    timestamps.push_back(std::chrono::steady_clock::now());
    actions.push_back("START");

    // Execute actions with timing
    record_action("timer_expired() #1", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #2", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #3", [&]() { controller->timer_expired(); });
    record_action("button_pressed() #1",
                  [&]() { controller->button_pressed(); });
    record_action("button_pressed() #2",
                  [&]() { controller->button_pressed(); });
    record_action("timer_expired() #4", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #5", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #6", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #7", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #8", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #9", [&]() { controller->timer_expired(); });
    record_action("timer_expired() #10",
                  [&]() { controller->timer_expired(); });

    // Print timing analysis
    std::cout << "\n" << std::string(70, '-') << std::endl;
    std::cout << "TIMING ANALYSIS" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    // Table header
    std::cout << std::left << std::setw(15) << "Time [HH:MM:SS:ms]"
              << " | " << std::setw(20) << "Action"
              << " | "
              << "Seconds from previous" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    // Convert start time for proper time formatting

    auto system_start = std::chrono::system_clock::now();

    for (size_t i = 0; i < timestamps.size(); ++i) {
        // Calculate wall clock time for this timestamp
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            timestamps[i] - timestamps[0]);

        auto wall_time = system_start + elapsed;
        auto time_t = std::chrono::system_clock::to_time_t(wall_time);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      wall_time.time_since_epoch()) %
                  1000;

        // Format time as HH:MM:SS:ms
        std::cout << std::left << std::setw(15);
        std::cout << std::put_time(std::localtime(&time_t), "%H:%M:%S");
        std::cout << ":" << std::setfill('0') << std::setw(3) << ms.count();
        std::cout << std::setfill(' ');

        std::cout << " | " << std::setw(20) << actions[i] << " | ";

        // Calculate seconds from previous
        if (i == 0) {
            std::cout << "0.000 (start)";
        } else {
            auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    timestamps[i] - timestamps[i - 1]);
            double seconds = duration.count() / 1000000.0;
            std::cout << std::fixed << std::setprecision(3) << seconds;
        }
        std::cout << std::endl;
    }

    // Summary statistics
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        timestamps.back() - timestamps[0]);
    double total_seconds = total_duration.count() / 1000000.0;

    std::cout << std::string(70, '-') << std::endl;
    std::cout << "SUMMARY:" << std::endl;
    std::cout << "  Total actions: " << (actions.size() - 1) << std::endl;
    std::cout << "  Total time: " << std::fixed << std::setprecision(3)
              << total_seconds << " seconds" << std::endl;
    std::cout << "  Average time per action: " << std::fixed
              << std::setprecision(3) << (total_seconds / (actions.size() - 1))
              << " seconds" << std::endl;

    std::cout << "\n✅ Pedestrian cycle with timing completed!" << std::endl;
}

int main() {
    std::cout << "=== Traffic Light Observer Example (using state_machine "
                 "library) ==="
              << std::endl;
    try {
        std::cout << "\n Step 1: Creating core components" << std::endl;
        auto state_machine = create_state_machine();
        auto controller = create_controller(state_machine);

        std::cout << "\n Step 2: Creating services" << std::endl;
        auto display_service = create_display_service();
        auto timer_service = create_timer_service();

        std::cout << "\n Step 3: Creating observers" << std::endl;
        auto [console_logger, display_observer, timer_observer,
              pedestrian_observer, file_logger] =
            create_observers(std::move(display_service),
                             std::move(timer_service));

        std::cout << "\n Step 4: Setting up transitions" << std::endl;
        setup_transitions(state_machine, pedestrian_observer);

        // Step 5: Attach observers
        std::cout << "\n Step 5: Attaching observers" << std::endl;
        attach_observers(controller.get(), console_logger, display_observer,
                         timer_observer, pedestrian_observer, file_logger);

        std::cout << "\n All components ready! Starting demos..." << std::endl;

        demo_pedestrian_cycle_with_timing(controller.get());
    } catch (const std::exception &e) {
        std::cerr << "\n Error: " << e.what() << std::endl;
        return 1;
    }
    /*
    // Create timer function
    std::function<void(uint32_t)> timer_func = [](uint32_t duration) {
        std::cout << "Timer started for " << duration << " seconds"
                  << std::endl;
    };

    // Create controller using factory
    auto controller = TrafficLightFactory::create_controller(
        TrafficLightType::STANDARD, std::make_unique<ConsoleDisplayService>(),
        std::make_unique<FunctionTimerService>(timer_func));

    // Test basic operation
    std::cout << "\n=== Testing State Transitions ===" << std::endl;
    controller->timeout_expired(); // GREEN -> YELLOW
    controller->timeout_expired(); // YELLOW -> RED
    controller->timeout_expired(); // RED -> RED_YELLOW
    controller->timeout_expired(); // RED_YELLOW -> GREEN

    std::cout << "\n=== Testing Pedestrian Button ===" << std::endl;
    controller->button_pressed();  // Pedestrian request
    controller->timeout_expired(); // GREEN -> YELLOW
    controller->timeout_expired(); // YELLOW -> WALK_PREP (because pedestrian
                                   // requested)
    controller->timeout_expired(); // WALK_PREP -> WALK
    controller->timeout_expired(); // WALK -> WALK_FINISH
    controller->timeout_expired(); // WALK_FINISH -> RED_YELLOW

    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    */

    return 0;
}
