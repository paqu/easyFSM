#pragma once
#include <cstdint>
#include <set>
#include <string>

/**
 * @brief Elevator door configuration
 */
struct ElevatorDoors {
    bool open;
    bool opening;
    bool closing;

    ElevatorDoors(bool is_open = false, bool is_opening = false,
                  bool is_closing = false)
        : open(is_open), opening(is_opening), closing(is_closing) {}
};

/**
 * @brief Elevator movement configuration
 */
struct ElevatorMovement {
    bool moving_up;
    bool moving_down;
    bool stopped;

    ElevatorMovement(bool up = false, bool down = false, bool stop = true)
        : moving_up(up), moving_down(down), stopped(stop) {}
};

/**
 * @brief Complete elevator state context with timing and configuration
 */
struct ElevatorContext {
    std::string name;
    uint32_t duration;
    ElevatorDoors doors;
    ElevatorMovement movement;
    int current_floor;
    int target_floor;
    std::set<int> pending_requests;
    bool emergency_active;
    bool obstacle_detected;

    ElevatorContext() = default;
    ElevatorContext(const std::string &n, uint32_t dur,
                    const ElevatorDoors &door_state,
                    const ElevatorMovement &move_state, int curr_floor = 0,
                    int tgt_floor = 0)
        : name(n), duration(dur), doors(door_state), movement(move_state),
          current_floor(curr_floor), target_floor(tgt_floor),
          emergency_active(false), obstacle_detected(false) {}
};
