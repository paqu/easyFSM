#pragma once

#include "traffic_light_controller.h"
#include <functional>

class ConsoleDisplayService : public IDisplayService {
  public:
    void showState(const StateContext &ctx) override;
    void showButtonState(bool waiting_to_be_processed) override;
};

class FunctionTimerService : public ITimerService {
  private:
    std::function<void(uint32_t)> timer_function;

  public:
    // Constructor that accepts any callable (function pointer, lambda,
    // std::function)
    explicit FunctionTimerService(std::function<void(uint32_t)> func);

    // Convenience constructor for function pointers
    explicit FunctionTimerService(void (*func)(uint32_t));

    void start_timeout(uint32_t duration_sec) override;
};
