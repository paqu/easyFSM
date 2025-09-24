#pragma once

#include "action_handler.h"
#include "display_service.h"
#include "timer_service.h"
#include "traffic_context.h"
#include <map>
#include <memory>

/**
 * @brief Action handler specific to traffic light logic
 */
class TrafficLightActionHandler
    : public IActionHandler<TrafficState, SystemEvent> {
  private:
    std::map<TrafficState, StateContext> states;
    bool pedestrian_request = false;
    std::unique_ptr<IDisplayService<StateContext>> display_service;
    std::unique_ptr<ITimerService> timer_service;

    void display_traffic_state(TrafficState state);
    void start_state_timer(TrafficState state);

  public:
    TrafficLightActionHandler(std::unique_ptr<IDisplayService<StateContext>> ds,
                              std::unique_ptr<ITimerService> ts);

    // Implementation of IActionHandler interface
    void handle(TrafficState current_state, SystemEvent event,
                TrafficState next_state) override;

    // Traffic light specific methods
    bool has_pedestrian_request() const;
    void handle_button_press_event();
    void set_state_timeout(TrafficState state, uint32_t timeout);
    void configure_state(TrafficState state, const StateContext &config);
};
