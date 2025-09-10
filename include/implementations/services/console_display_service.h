#pragma once
#include "display_service.h"

/**
 * @brief Console-based implementation of display service
 */
class ConsoleDisplayService : public IDisplayService {
  public:
    void showState(const StateContext &ctx) override;
    void showButtonState(bool is_new_request) override;
};
