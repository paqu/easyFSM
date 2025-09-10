#pragma once
#include <memory>
#include <vector>

// Forward declarations
enum class TrafficState;
enum class SystemEvent;
class IStateTransition;

/**
 * @brief Generic state machine interface
 */
class IStateMachine {
  public:
    virtual ~IStateMachine() = default;
    virtual TrafficState get_current_state() const = 0;
    virtual bool process_event(SystemEvent event) = 0;
    virtual TrafficState get_next_state(TrafficState current_state,
                                        SystemEvent event) const = 0;
    virtual void
    add_transition(std::unique_ptr<IStateTransition> transition) = 0;
    virtual void set_state(TrafficState state) = 0;
    virtual std::vector<TrafficState> get_all_states() const = 0;
    virtual std::vector<SystemEvent> get_all_events() const = 0;
};
