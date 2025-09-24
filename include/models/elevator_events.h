#pragma once
#include <iostream>

/**
 * @brief System events that trigger elevator state transitions
 */
enum class ElevatorEvent {
    FLOOR_REQUESTED,       // floor button pressed
    DOORS_OPEN_REQUESTED,  // open doors button pressed
    DOORS_CLOSE_REQUESTED, // close doors button pressed
    TIMER_EXPIRED,         // timeout occurred (e.g., auto door closing)
    FLOOR_REACHED,         // elevator reached requested floor
    EMERGENCY_BUTTON,      // emergency button pressed
    OBSTACLE_DETECTED      // obstacle detected in doors
};

std::ostream &operator<<(std::ostream &os, ElevatorEvent event);
