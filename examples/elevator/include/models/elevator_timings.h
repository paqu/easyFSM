#pragma once
#include <cstdint>

/**
 * @brief Default timing constants for elevator states
 */
struct ElevatorTimings {
    static constexpr uint32_t DOORS_OPENING_DURATION = 3;
    static constexpr uint32_t DOORS_OPEN_DURATION = 5;
    static constexpr uint32_t DOORS_CLOSING_DURATION = 3;
    static constexpr uint32_t FLOOR_TRAVEL_DURATION = 4;
    static constexpr uint32_t EMERGENCY_TIMEOUT = 30;
    static constexpr uint32_t IDLE_TIMEOUT = 0; // no timeout when idle
    static constexpr uint32_t OBSTACLE_RETRY_DURATION = 2;
};
