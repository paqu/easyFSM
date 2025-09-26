#pragma once

#include "traffic_light_controller.h"
#include <state_machine/state_machine.h>

#include "traffic_context.h"
#include "traffic_events.h"
#include "traffic_states.h"
#include <memory>

using namespace state_machine;

/**
 * @brief Traffic light types supported by the factory
 */
enum class TrafficLightType {
    STANDARD, // With RED_YELLOW state
    SIMPLE    // Without RED_YELLOW state
};

/**
 * @brief Factory for creating different types of traffic light systems
 */
class TrafficLightFactory {
  public:
    /**
     * @brief Create a traffic light controller of specified type
     * @param type Type of traffic light to create
     * @param display_service Display service to use
     * @param timer_service Timer service to use
     * @return Unique pointer to created controller
     */
    static std::unique_ptr<TrafficLightController> create_controller(
        TrafficLightType type,
        std::unique_ptr<IDisplayService<TrafficContext>> display_service,
        std::unique_ptr<ITimerService> timer_service);

    /**
     * @brief Create a standard traffic light controller (with RED_YELLOW)
     */
    static std::unique_ptr<TrafficLightController> create_standard_controller(
        std::unique_ptr<IDisplayService<TrafficContext>> display_service,
        std::unique_ptr<ITimerService> timer_service);

    /**
     * @brief Create a simple traffic light controller (without RED_YELLOW)
     */
    static std::unique_ptr<TrafficLightController> create_simple_controller(
        std::unique_ptr<IDisplayService<TrafficContext>> display_service,
        std::unique_ptr<ITimerService> timer_service);

  private:
    // Helper methods for creating state machines
    static void setup_standard_transitions(
        std::shared_ptr<RuntimeStateMachine<TrafficState, TrafficEvent>>
            state_machine,
        std::function<bool()> ped_check);

    static void setup_simple_transitions(
        std::shared_ptr<RuntimeStateMachine<TrafficState, TrafficEvent>>
            state_machine,
        std::function<bool()> ped_check);
};
