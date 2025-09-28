#pragma once
#include <memory>

namespace state_machine {

template <typename StateType, typename EventType> class IObserver {
  public:
    virtual ~IObserver() = default;
    virtual void on_state_transition(StateType from_state, EventType event,
                                     StateType to_state) = 0;
};

template <typename StateType, typename EventType> class ISubject {
  public:
    virtual ~ISubject() = default;
    virtual void
    add_observer(std::shared_ptr<IObserver<StateType, EventType>> observer) = 0;
    virtual void remove_observer(
        std::shared_ptr<IObserver<StateType, EventType>> observer) = 0;

  protected:
    virtual void notify_observers(StateType from_state, EventType event,
                                  StateType to_state) = 0;
};

} // namespace state_machine
