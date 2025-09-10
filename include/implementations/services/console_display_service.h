#pragma once
#include "display_service.h"

/**
 * @brief Console-based implementation of display service
 */
class ConsoleDisplayService : public IDisplayService {
  public:
    void show_state(const StateContext &ctx) override;
    void show_button_state(bool is_new_request) override;
};
