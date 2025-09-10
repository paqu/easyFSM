#pragma once

#include "display_service.h"
#include "timer_service.h"
#include "traffic_light_controller.h"
#include <memory>

/**
 * @brief Abstract factory for creating traffic light systems
 */
class TrafficLightFactory {
  public:
    virtual ~TrafficLightFactory() = default;

    virtual std::unique_ptr<TrafficLightController>
    create_controller(std::unique_ptr<IDisplayService> display_service,
                      std::unique_ptr<ITimerService> timer_service) = 0;

    virtual std::string get_factory_name() const = 0;
};

/**
 * @brief Factory for standard traffic lights (with RED_YELLOW state)
 */
class StandardTrafficLightFactory : public TrafficLightFactory {
  public:
    std::unique_ptr<TrafficLightController>
    create_controller(std::unique_ptr<IDisplayService> display_service,
                      std::unique_ptr<ITimerService> timer_service) override;

    std::string get_factory_name() const override;
};

/**
 * @brief Factory for simplified traffic lights (without RED_YELLOW state)
 */
class SimpleTrafficLightFactory : public TrafficLightFactory {
  public:
    std::unique_ptr<TrafficLightController>
    create_controller(std::unique_ptr<IDisplayService> display_service,
                      std::unique_ptr<ITimerService> timer_service) override;

    std::string get_factory_name() const override;
};
