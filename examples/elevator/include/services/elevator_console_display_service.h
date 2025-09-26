#pragma once
#include "elevator_context.h"
#include <state_machine/state_machine.h>

using namespace state_machine;
/**
 * @brief Console-based implementation of display service for elevators
 */
class ElevatorConsoleDisplayService : public IDisplayService<ElevatorContext> {
  public:
    void show_state(const ElevatorContext &ctx) override;

    // Elevator specific display methods
    void show_elevator_state(const ElevatorContext &ctx);
    void show_floor_requests(const std::set<int> &requests);
};
