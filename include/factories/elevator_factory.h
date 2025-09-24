#pragma once

#include "display_service.h"
#include "elevator_controller.h"
#include "runtime_state_machine.h"
#include "timer_service.h"

#include "elevator_events.h"
#include "elevator_states.h"
#include <memory>

/**
 * @brief Elevator types supported by the factory
 */
enum class ElevatorType {
    BASIC,   // Simple elevator with basic functionality
    ADVANCED // Advanced elevator with safety features
};

/**
 * @brief Factory for creating different types of elevator systems
 */
class ElevatorFactory {
  public:
    /**
     * @brief Create an elevator controller of specified type
     * @param type Type of elevator to create
     * @param display_service Display service to use
     * @param timer_service Timer service to use
     * @param min_floor Minimum floor number
     * @param max_floor Maximum floor number
     * @return Unique pointer to created controller
     */
    static std::unique_ptr<ElevatorController>
    create_controller(ElevatorType type,
                      std::unique_ptr<IDisplayService> display_service,
                      std::unique_ptr<ITimerService> timer_service,
                      int min_floor = 0, int max_floor = 10);

    /**
     * @brief Create a basic elevator controller
     */
    static std::unique_ptr<ElevatorController>
    create_basic_controller(std::unique_ptr<IDisplayService> display_service,
                            std::unique_ptr<ITimerService> timer_service,
                            int min_floor = 0, int max_floor = 10);

    /**
     * @brief Create an advanced elevator controller with safety features
     */
    static std::unique_ptr<ElevatorController>
    create_advanced_controller(std::unique_ptr<IDisplayService> display_service,
                               std::unique_ptr<ITimerService> timer_service,
                               int min_floor = 0, int max_floor = 10);

  private:
    // Helper methods for creating state machines
    static void setup_basic_transitions(
        std::shared_ptr<RuntimeStateMachine<ElevatorState, ElevatorEvent>>
            state_machine,
        std::function<bool()> has_requests,
        std::function<bool()> should_move_up,
        std::function<bool()> should_move_down);

    static void setup_advanced_transitions(
        std::shared_ptr<RuntimeStateMachine<ElevatorState, ElevatorEvent>>
            state_machine,
        std::function<bool()> has_requests,
        std::function<bool()> should_move_up,
        std::function<bool()> should_move_down,
        std::function<bool()> obstacle_detected);
};
