#pragma once
#include "display_service.h"
#include "traffic_context.h"

/**
 * @brief Console-based implementation of display service
 */
class ConsoleDisplayService : public IDisplayService<TrafficContext> {
  public:
    void show_state(const TrafficContext &ctx) override;
};
