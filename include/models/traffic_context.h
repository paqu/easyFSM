#pragma once
#include <cstdint>
#include <string>

/**
 * @brief Car traffic light configuration
 */
struct TrafficLights {
    bool red;
    bool yellow;
    bool green;

    TrafficLights(bool r = false, bool y = false, bool g = false)
        : red(r), yellow(y), green(g) {}
};

/**
 * @brief Pedestrian light configuration
 */
struct PedestrianLights {
    bool red;
    bool green;

    PedestrianLights(bool r = true, bool g = false) : red(r), green(g) {}
};

/**
 * @brief Complete state context with timing and light configuration
 */
struct TrafficContext {
    std::string name;
    uint32_t duration;
    TrafficLights carLights;
    PedestrianLights pedLights;

    TrafficContext() = default;
    TrafficContext(const std::string &n, uint32_t dur, const TrafficLights &car,
                   const PedestrianLights &ped)
        : name(n), duration(dur), carLights(car), pedLights(ped) {}
};
