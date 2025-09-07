#pragma once

#include <functional>
#include <map>
#include <memory>
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

class TrafficLightController {
  public:
    enum State {
        CAR_GREEN,
        CAR_YELLOW,
        CAR_RED,
        WALK_PREP,
        WALK,
        WALK_FINISH,
        CAR_RED_YELLOW
    };

    TrafficLightController(std::unique_ptr<IDisplayService> ds,
                           std::unique_ptr<ITimerService> ts);

    void button_pressed();
    void timeout_expired();

  private:
    State current_state;
    bool pedestrian_request;
    std::map<State, StateContext> states;
    std::unique_ptr<IDisplayService> displayService;
    std::unique_ptr<ITimerService> timerService;

    State get_next_state(State s, bool ped_request) const;
    void start_timeout(uint32_t duration) const;
};
