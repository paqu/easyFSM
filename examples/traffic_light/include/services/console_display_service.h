#pragma once
#include <state_machine/state_machine.h>

#include "traffic_context.h"

using namespace state_machine;

/**
 * @brief Console-based implementation of display service
 */
class ConsoleDisplayService : public IDisplayService<TrafficContext> {
  public:
    void show_state(const TrafficContext &ctx) override;
};
