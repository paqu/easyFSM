#pragma once
#include <cstdint>

/**
 * @brief Default timing constants for traffic light states
 */
struct LightTimings {
    static constexpr uint32_t RED_DURATION = 8;
    static constexpr uint32_t RED_YELLOW_DURATION = 2;
    static constexpr uint32_t GREEN_DURATION = 10;
    static constexpr uint32_t YELLOW_DURATION = 2;
    static constexpr uint32_t WALK_DURATION = 5;
    static constexpr uint32_t WALK_PREP_DURATION = 1;
    static constexpr uint32_t WALK_FINISH_DURATION = 2;
};
