#pragma once
#include "display_service.h"

/**
 * @brief Console-based implementation of display service
 */
class ConsoleDisplayService : public IDisplayService<StateContext> {
  public:
    void show_state(const StateContext &ctx) override;
};
