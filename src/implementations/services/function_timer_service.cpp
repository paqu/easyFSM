#include "function_timer_service.h"

FunctionTimerService::FunctionTimerService(std::function<void(uint32_t)> func)
    : timer_function(std::move(func)) {}

FunctionTimerService::FunctionTimerService(void (*func)(uint32_t))
    : timer_function(func) {}

void FunctionTimerService::start_timeout(uint32_t duration_sec) {
    timer_function(duration_sec);
}
