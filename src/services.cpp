#include "services.h"
#include <iostream>

void ConsoleDisplayService::showState(const StateContext &ctx) {
    std::cout << "\n------------------------ \n"
              << "Traffic Light State:\n"
              << "------------------------ \n";

    std::cout << "State: " << ctx.name << "\n"
              << "Duration: " << ctx.duration << "s\n"
              << "Car Lights: [R:" << ctx.carLights.red
              << ", Y:" << ctx.carLights.yellow << ", G:" << ctx.carLights.green
              << "]\n"
              << "Pedestrian Lights: [R:" << ctx.pedLights.red
              << ", G:" << ctx.pedLights.green << "]\n";

    std::cout << std::endl;
}

void ConsoleDisplayService::showButtonState(bool waiting_to_be_processed) {
    if (waiting_to_be_processed == false) {
        std::cout << "Button Pressed" << std::endl;
    } else {
        std::cout << "Request is waiting to be processed." << std::endl;
    }
}

FunctionTimerService::FunctionTimerService(std::function<void(uint32_t)> func)
    : timer_function(std::move(func)) {}

FunctionTimerService::FunctionTimerService(void (*func)(uint32_t))
    : timer_function(func) {}

void FunctionTimerService::start_timeout(uint32_t duration_sec) {
    timer_function(duration_sec);
}
