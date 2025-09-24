#pragma once
#include "display_service.h"
#include "elevator_context.h"

/**
 * @brief Console-based implementation of display service for elevators
 */
class ElevatorConsoleDisplayService : public IDisplayService {
  public:
    void show_state(const StateContext &ctx) override;
    void show_button_state(bool is_new_request) override;

    // Elevator specific display methods
    void show_elevator_state(const ElevatorContext &ctx);
    void show_floor_requests(const std::set<int> &requests);
};
