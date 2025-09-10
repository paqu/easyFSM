#include "state_machine.h"
#include "state_transition.h"
#include "system_events.h"
#include "traffic_states.h"

#include <memory>
#include <set>
#include <vector>

/**
 * @brief Runtime configurable state machine
 */
class RuntimeStateMachine : public IStateMachine {
  private:
    TrafficState current_state;
    std::vector<std::unique_ptr<IStateTransition>> transitions;
    std::set<TrafficState> states;
    std::set<SystemEvent> events;

  public:
    explicit RuntimeStateMachine(TrafficState initial_state);

    TrafficState get_current_state() const override;

    void set_state(TrafficState state) override;

    void add_transition(std::unique_ptr<IStateTransition> transition) override;

    TrafficState get_next_state(TrafficState current_state,
                                SystemEvent event) const override;

    bool process_event(SystemEvent event) override;

    std::vector<TrafficState> get_all_states() const override;

    std::vector<SystemEvent> get_all_events() const override;
};
