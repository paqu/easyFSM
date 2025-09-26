#pragma once
#include <iostream>

/**
 * @brief All possible elevator states
 */
enum class ElevatorState {
    IDLE,          // standing still, doors closed
    DOORS_OPENING, // doors are opening
    DOORS_OPEN,    // doors are open
    DOORS_CLOSING, // doors are closing
    MOVING_UP,     // moving upward
    MOVING_DOWN,   // moving downward
    EMERGENCY_STOP // emergency stop activated
};

std::ostream &operator<<(std::ostream &os, ElevatorState state);
