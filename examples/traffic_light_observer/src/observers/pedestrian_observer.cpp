#include "pedestrian_observer.h"
#include <iostream>

void PedestrianObserver::on_state_transition(TrafficState from,
                                             TrafficEvent event,
                                             TrafficState to) {
    // Handle pedestrian button press
    if (event == TrafficEvent::BUTTON_PRESSED) {
        if (!request_pending) {
            request_pending = true;
            request_time = std::chrono::steady_clock::now();
            log_debug("Pedestrian button pressed! Request registered.");
        } else {
            auto duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - request_time);
            log_debug("Pedestrian button pressed again (request already "
                      "pending for " +
                      std::to_string(duration.count()) + "ms)");
        }
        return;
    }

    // Clear request when walk phase ends
    if (from == TrafficState::WALK && to != TrafficState::WALK &&
        request_pending) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - request_time);
        log_debug("Walk phase ended. Clearing request (total wait: " +
                  std::to_string(duration.count()) + "ms)");
        clear_request();
    }

    // Log interesting transitions
    if (from != to) {
        if (to == TrafficState::WALK && request_pending) {
            auto duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - request_time);
            log_debug("Pedestrian can now walk! (waited " +
                      std::to_string(duration.count()) + "ms)");
        }
    }
}

void PedestrianObserver::clear_request() { request_pending = false; }

void PedestrianObserver::log_debug(const std::string &message) const {
    if (debug_mode) {
        std::cout << message << std::endl;
    }
}
