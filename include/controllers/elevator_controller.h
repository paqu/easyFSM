#pragma once
#include "base_controller.h"
#include "elevator_events.h"
#include "elevator_states.h"
#include <set>

/**
 * @brief Elevator specific controller
 */
class ElevatorController : public BaseController<ElevatorState, ElevatorEvent> {
  private:
    int current_floor;
    int target_floor;
    std::set<int> floor_requests;
    int min_floor;
    int max_floor;

  public:
    ElevatorController(
        std::shared_ptr<IStateMachine<ElevatorState, ElevatorEvent>>
            state_machine,
        std::shared_ptr<IActionHandler<ElevatorState, ElevatorEvent>>
            action_handler,
        int min_floor = 0, int max_floor = 10);

    void request_floor(int floor);
    void open_doors();
    void close_doors();
    void emergency_stop();
    void timer_expired();
    void floor_reached();
    void obstacle_detected();

    int get_current_floor() const { return current_floor; }
    int get_target_floor() const { return target_floor; }
    bool has_pending_requests() const { return !floor_requests.empty(); }
    std::set<int> get_pending_requests() const { return floor_requests; }

  private:
    void handle_floor_request(int floor);
    void handle_doors_open_request();
    void handle_doors_close_request();
    void handle_emergency();
    void handle_timer();
    void handle_floor_reached();
    void handle_obstacle();

    void update_target_floor();
    bool should_move_up() const;
    bool should_move_down() const;
};
