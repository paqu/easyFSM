#pragma once
#include "state_machine.h"
#include "subject.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace state_machine {

/**
 * @brief Observable controller that notifies observers about state transitions
 * Alternative to BaseController for modern observer-based architecture
 */
template <typename StateType, typename EventType>
class ObservableController : public ISubject<StateType, EventType> {
  private:
    std::shared_ptr<IStateMachine<StateType, EventType>> state_machine;
    std::vector<std::weak_ptr<IObserver<StateType, EventType>>> observers;

  public:
    explicit ObservableController(
        std::shared_ptr<IStateMachine<StateType, EventType>> sm)
        : state_machine(sm) {}

    virtual ~ObservableController() = default;

    // ISubject implementation
    void add_observer(
        std::shared_ptr<IObserver<StateType, EventType>> observer) override {
        observers.push_back(observer);
    }

    void remove_observer(
        std::shared_ptr<IObserver<StateType, EventType>> observer) override {
        observers.erase(
            std::remove_if(
                observers.begin(), observers.end(),
                [&observer](const std::weak_ptr<IObserver<StateType, EventType>>
                                &weak_obs) {
                    return weak_obs.lock() == observer;
                }),
            observers.end());
    }

  protected:
    void handle_event(EventType event) {
        StateType current_state = state_machine->get_current_state();
        StateType new_state = current_state;

        bool state_changed = state_machine->process_event(event);
        if (state_changed) {
            new_state = state_machine->get_current_state();
        }

        // Always notify observers (even if state didn't change)
        notify_observers(current_state, event, new_state);
    }

    void notify_observers(StateType from_state, EventType event,
                          StateType to_state) override {
        cleanup_expired_observers();

        for (auto &weak_obs : observers) {
            if (auto obs = weak_obs.lock()) {
                obs->on_state_transition(from_state, event, to_state);
            }
        }
    }

    std::shared_ptr<IStateMachine<StateType, EventType>>
    get_state_machine() const {
        return state_machine;
    }

  private:
    void cleanup_expired_observers() {
        observers.erase(
            std::remove_if(
                observers.begin(), observers.end(),
                [](const std::weak_ptr<IObserver<StateType, EventType>>
                       &weak_obs) { return weak_obs.expired(); }),
            observers.end());
    }
};

} // namespace state_machine
