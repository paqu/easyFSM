#pragma once

#include "action_handler.h"
#include "display_serivce.h"
#include "states_context.h"
#include "timer_service.h"
#include <map>
#include <memory>

/**
 * @brief Action handler specific to traffic light logic
 */

class TrafficLightActionHandler : public IActionHandler {
  private:
    std::map<TrafficState, StateContext> states;
    bool pedestrian_request = false;
    std::unique_ptr<IDisplayService> displayService;
    std::unique_ptr<ITimerService> timerService;

    void display_traffic_state(TrafficState state);
    void start_state_timer(TrafficState state);

  public:
    TrafficLightActionHandler(std::unique_ptr<IDisplayService> ds,
                              std::unique_ptr<ITimerService> ts);

    void handle(TrafficState current_state, SystemEvent event,
                TrafficState next_state) override;
    bool has_pedestrian_request() const;
    void handle_button_press();
    void set_state_timeout(const TrafficState state, uint32_t timeout);

    void configure_state(TrafficState state, const StateContext &config);
};
