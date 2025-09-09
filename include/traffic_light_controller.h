#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdint.h>
#include <string>

struct LightTimings {
    static constexpr uint32_t RED_DURATION = 8;
    static constexpr uint32_t RED_YELLOW_DURATION = 2;
    static constexpr uint32_t GREEN_DURATION = 10;
    static constexpr uint32_t YELLOW_DURATION = 2;
    static constexpr uint32_t WALK_DURATION = 5;
    static constexpr uint32_t WALK_PREP_DURATION = 1;
    static constexpr uint32_t WALK_FINISH_DURATION = 2;
};

struct TrafficLights {
    bool red;
    bool yellow;
    bool green;
};

struct PedestrianLights {
    bool red;
    bool green;
};

struct StateContext {
    std::string name;
    uint32_t duration;
    TrafficLights carLights;
    PedestrianLights pedLights;
};

class IDisplayService {
  public:
    virtual ~IDisplayService() = default;
    virtual void showState(const StateContext &ctx) = 0;
    virtual void showButtonState(bool waiting_to_be_processed) = 0;
};

class ITimerService {
  public:
    virtual ~ITimerService() = default;
    virtual void start_timeout(uint32_t duration_sec) = 0;
};

enum class TrafficState {
    CAR_GREEN,
    CAR_YELLOW,
    CAR_RED,
    WALK_PREP,
    WALK,
    WALK_FINISH,
    CAR_RED_YELLOW
};

enum class SystemEvent { TIME_EXPIRED, BUTTON_PRESSED };

class EnumUtils {
  public:
    static std::string state_to_string(TrafficState state);
    static std::string event_to_string(SystemEvent event);
};

class StateTransition {
  public:
    virtual ~StateTransition() = default;
    virtual TrafficState get_from_state() const = 0;
    virtual SystemEvent get_trigger_event() const = 0;
    virtual TrafficState get_to_state() const = 0;
    virtual bool can_transition(TrafficState current_state,
                                SystemEvent event) const = 0;
};

class StateMachine {
  public:
    virtual ~StateMachine() = default;
    virtual TrafficState get_current_state() const = 0;
    virtual bool process_event(SystemEvent event) = 0;
    virtual TrafficState get_next_state(TrafficState current_state,
                                        SystemEvent event) const = 0;
    virtual void
    add_transition(std::unique_ptr<StateTransition> transition) = 0;
    virtual void set_state(TrafficState state) = 0;
    virtual std::vector<TrafficState> get_all_states() const = 0;
    virtual std::vector<SystemEvent> get_all_events() const = 0;
};

class SimpleStateTransition : public StateTransition {
  private:
    TrafficState from_state;
    SystemEvent trigger_event;
    TrafficState to_state;

  public:
    SimpleStateTransition(TrafficState from_state, SystemEvent trigger_event,
                          TrafficState to_state)
        : from_state(from_state), trigger_event(trigger_event),
          to_state(to_state) {}

    TrafficState get_from_state() const override;
    SystemEvent get_trigger_event() const override;
    TrafficState get_to_state() const override;

    bool can_transition(TrafficState current_state,
                        SystemEvent event) const override;
};
// ========== STATE MACHINE IMPLEMENTATION ==========

class RuntimeStateMachine : public StateMachine {
  private:
    TrafficState current_state;
    std::vector<std::unique_ptr<StateTransition>> transitions;
    std::set<TrafficState> states;
    std::set<SystemEvent> events;

  public:
    explicit RuntimeStateMachine(TrafficState initial_state);

    TrafficState get_current_state() const override;

    void set_state(TrafficState state) override;

    void add_transition(std::unique_ptr<StateTransition> transition) override;

    TrafficState get_next_state(TrafficState current_state,
                                SystemEvent event) const override;

    bool process_event(SystemEvent event) override;

    std::vector<TrafficState> get_all_states() const override;

    std::vector<SystemEvent> get_all_events() const override;
};

// ========== TRAFFIC LIGHT ACTION HANDLER ==========

class TrafficLightTransition : public StateTransition {
  private:
    TrafficState from_state;
    SystemEvent trigger_event;
    TrafficState to_state_normal;
    TrafficState to_state_with_pedestrian;
    std::function<bool()> has_pedestrian_request;
    bool check_pedestrian_request;

  public:
    // Simple transition constructor
    TrafficLightTransition(TrafficState from, SystemEvent event,
                           TrafficState to)
        : from_state(from), trigger_event(event), to_state_normal(to),
          to_state_with_pedestrian(to), check_pedestrian_request(false) {}

    // Conditional transition constructor
    TrafficLightTransition(TrafficState from, SystemEvent event,
                           TrafficState to_normal, TrafficState to_pedestrian,
                           std::function<bool()> ped_check)
        : from_state(from), trigger_event(event), to_state_normal(to_normal),
          to_state_with_pedestrian(to_pedestrian),
          has_pedestrian_request(ped_check), check_pedestrian_request(true) {}

    TrafficState get_from_state() const override { return from_state; }
    SystemEvent get_trigger_event() const override { return trigger_event; }

    TrafficState get_to_state() const override {
        if (check_pedestrian_request && has_pedestrian_request &&
            has_pedestrian_request()) {
            return to_state_with_pedestrian;
        }
        return to_state_normal;
    }

    bool can_transition(TrafficState current_state,
                        SystemEvent event) const override {
        return this->from_state == current_state &&
               this->trigger_event == event;
    }
};

class ActionHandler {
  public:
    virtual ~ActionHandler() = default;
    virtual void handle(TrafficState current_state, SystemEvent event,
                        TrafficState next_state) = 0;
};

class TrafficLightController {
  public:
    TrafficLightController(std::shared_ptr<StateMachine> state_machine,
                           std::unique_ptr<ActionHandler> ah);

    void button_pressed();
    void timeout_expired();
    void handle_event(SystemEvent event);
    void handle_timeout();
    void handle_button_press();

  private:
    std::shared_ptr<StateMachine> state_machine;
    std::unique_ptr<ActionHandler> action_handler;
};

class TrafficLightActionHandler : public ActionHandler {
  private:
    std::map<TrafficState, StateContext> states;
    bool pedestrian_request = false;
    std::unique_ptr<IDisplayService> displayService;
    std::unique_ptr<ITimerService> timerService;

    void display_traffic_state(TrafficState state);
    void start_state_timer(TrafficState state);

  public:
    TrafficLightActionHandler(std::unique_ptr<IDisplayService> ds,
                              std::unique_ptr<ITimerService> ts);

    void handle(TrafficState current_state, SystemEvent event,
                TrafficState next_state) override;
    bool has_pedestrian_request() const;
    void handle_button_press();
    void set_state_timeout(const TrafficState state, uint32_t timeout);

    void configure_state(TrafficState state, const StateContext &config);
};
