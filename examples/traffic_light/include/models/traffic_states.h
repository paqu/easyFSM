#pragma once
#include <iostream>
/**
 * @brief All possible traffic light states
 */
enum class TrafficState {
    CAR_GREEN,
    CAR_YELLOW,
    CAR_RED,
    WALK_PREP,
    WALK,
    WALK_FINISH,
    CAR_RED_YELLOW
};

std::ostream &operator<<(std::ostream &os, TrafficState state);
