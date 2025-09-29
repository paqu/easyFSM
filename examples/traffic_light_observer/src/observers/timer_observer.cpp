#include "timer_observer.h"
#include "light_timings.h"

TimerObserver::TimerObserver(std::unique_ptr<ITimerService> service)
    : timer_service(std::move(service)) {
    initialize_durations();
}

void TimerObserver::on_state_transition(TrafficState from, TrafficEvent event,
                                        TrafficState to) {
    (void)event;

    // Start timer only if state actually changed
    if (from != to && timer_service) {
        auto it = durations.find(to);
        if (it != durations.end() && it->second > 0) {
            timer_service->start_timeout(it->second);
        }
    }
}

void TimerObserver::set_duration(TrafficState state, uint32_t seconds) {
    durations[state] = seconds;
}

void TimerObserver::initialize_durations() {
    durations[TrafficState::CAR_GREEN] = LightTimings::GREEN_DURATION;
    durations[TrafficState::CAR_YELLOW] = LightTimings::YELLOW_DURATION;
    durations[TrafficState::CAR_RED] = LightTimings::RED_DURATION;
    durations[TrafficState::CAR_RED_YELLOW] = LightTimings::RED_YELLOW_DURATION;
    durations[TrafficState::WALK_PREP] = LightTimings::WALK_PREP_DURATION;
    durations[TrafficState::WALK] = LightTimings::WALK_DURATION;
    durations[TrafficState::WALK_FINISH] = LightTimings::WALK_FINISH_DURATION;
}
