#pragma once
#include "elevator_context.h"
#include "elevator_events.h"
#include "elevator_states.h"
#include <map>
#include <memory>
#include <set>
#include <state_machine/state_machine.h>

using namespace state_machine;
/**
 * @brief Action handler specific to elevator logic
 */
class ElevatorActionHandler
    : public IActionHandler<ElevatorState, ElevatorEvent> {
  private:
    std::map<ElevatorState, ElevatorContext> states;
    std::unique_ptr<IDisplayService<ElevatorContext>> display_service;
    std::unique_ptr<ITimerService> timer_service;

    int current_floor;
    int target_floor;
    std::set<int> pending_requests;
    bool emergency_active;
    bool obstacle_present;

    void display_elevator_state(ElevatorState state);
    void start_state_timer(ElevatorState state);

  public:
    ElevatorActionHandler(std::unique_ptr<IDisplayService<ElevatorContext>> ds,
                          std::unique_ptr<ITimerService> ts,
                          int initial_floor = 0);

    void handle(ElevatorState current_state, ElevatorEvent event,
                ElevatorState next_state) override;

    void set_current_floor(int floor) { current_floor = floor; }
    void set_target_floor(int floor) { target_floor = floor; }
    void add_floor_request(int floor) { pending_requests.insert(floor); }
    void remove_floor_request(int floor) { pending_requests.erase(floor); }
    void clear_all_requests() { pending_requests.clear(); }

    int get_current_floor() const { return current_floor; }
    int get_target_floor() const { return target_floor; }
    bool has_pending_requests() const { return !pending_requests.empty(); }
    bool is_emergency_active() const { return emergency_active; }
    bool is_obstacle_present() const { return obstacle_present; }

    void set_state_timeout(ElevatorState state, uint32_t timeout);
    void configure_state(ElevatorState state, const ElevatorContext &config);
};
