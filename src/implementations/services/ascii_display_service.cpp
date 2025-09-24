#include "ascii_display_service.h"

#include "traffic_context.h"
#include <iomanip>
#include <iostream>
#include <sstream>

// ANSI color codes
const std::string AsciiDisplayService::RED_COLOR = "\033[91m";
const std::string AsciiDisplayService::YELLOW_COLOR = "\033[93m";
const std::string AsciiDisplayService::GREEN_COLOR = "\033[92m";
const std::string AsciiDisplayService::WHITE_COLOR = "\033[97m";
const std::string AsciiDisplayService::GRAY_COLOR = "\033[90m";
const std::string AsciiDisplayService::RESET_COLOR = "\033[0m";
const std::string AsciiDisplayService::BOLD = "\033[1m";

void AsciiDisplayService::show_state(const StateContext &ctx) {
    std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top

    std::cout << create_separator() << std::endl;
    std::cout << (color_enabled ? BOLD + WHITE_COLOR : "")
              << "🚦 TRAFFIC LIGHT SIMULATOR 🚦"
              << (color_enabled ? RESET_COLOR : "") << std::endl;
    std::cout << create_separator() << std::endl;

    // Current state info
    std::cout << (color_enabled ? BOLD : "") << "Current State: " << ctx.name
              << " (" << ctx.duration << "s)"
              << (color_enabled ? RESET_COLOR : "") << std::endl;

    std::cout << create_separator() << std::endl;

    // Main traffic light display
    std::cout << create_traffic_light_frame() << std::endl;
    std::cout << create_car_light(ctx.carLights.red, ctx.carLights.yellow,
                                  ctx.carLights.green)
              << std::endl;

    std::cout << std::endl;

    // Pedestrian crossing display
    std::cout << create_separator() << std::endl;
    std::cout << create_pedestrian_light(ctx.pedLights.red, ctx.pedLights.green)
              << std::endl;
    std::cout << create_separator() << std::endl;
}

std::string AsciiDisplayService::create_traffic_light_frame() const {
    return R"(
    ┌───────────────────┐
    │                   │)";
}

std::string AsciiDisplayService::create_car_light(bool red, bool yellow,
                                                  bool green) const {
    std::ostringstream oss;

    // Red light
    oss << "    │    ";
    if (red) {
        oss << (color_enabled ? RED_COLOR + BOLD : "") << "●●●●●"
            << (color_enabled ? RESET_COLOR : "");
    } else {
        oss << (color_enabled ? GRAY_COLOR : "") << "○○○○○"
            << (color_enabled ? RESET_COLOR : "");
    }
    oss << "          │" << std::endl;

    // Yellow light
    oss << "    │    ";
    if (yellow) {
        oss << (color_enabled ? YELLOW_COLOR + BOLD : "") << "●●●●●"
            << (color_enabled ? RESET_COLOR : "");
    } else {
        oss << (color_enabled ? GRAY_COLOR : "") << "○○○○○"
            << (color_enabled ? RESET_COLOR : "");
    }
    oss << "          │" << std::endl;

    // Green light
    oss << "    │    ";
    if (green) {
        oss << (color_enabled ? GREEN_COLOR + BOLD : "") << "●●●●●"
            << (color_enabled ? RESET_COLOR : "");
    } else {
        oss << (color_enabled ? GRAY_COLOR : "") << "○○○○○"
            << (color_enabled ? RESET_COLOR : "");
    }
    oss << "          │" << std::endl;
    oss << "    │                   │" << std::endl;
    oss << "    └───────────────────┘";

    return oss.str();
}

std::string AsciiDisplayService::create_pedestrian_light(bool red,
                                                         bool green) const {
    std::ostringstream oss;

    oss << "  PEDESTRIAN CROSSING" << std::endl;
    oss << "  ┌─────────┐  ┌─────────┐" << std::endl;

    // Red pedestrian light
    oss << "  │  ";
    if (red) {
        oss << (color_enabled ? RED_COLOR + BOLD : "") << "STOP"
            << (color_enabled ? RESET_COLOR : "");
    } else {
        oss << (color_enabled ? GRAY_COLOR : "") << "----"
            << (color_enabled ? RESET_COLOR : "");
    }
    oss << "   │  ";

    // Green pedestrian light
    oss << "│  ";
    if (green) {
        oss << (color_enabled ? GREEN_COLOR + BOLD : "") << "WALK"
            << (color_enabled ? RESET_COLOR : "");
    } else {
        oss << (color_enabled ? GRAY_COLOR : "") << "----"
            << (color_enabled ? RESET_COLOR : "");
    }
    oss << "   │" << std::endl;

    oss << "  │   🚫    │  │   🚶    │" << std::endl;
    oss << "  └─────────┘  └─────────┘";

    return oss.str();
}

std::string AsciiDisplayService::create_separator() const {
    return "════════════════════════════════════════════════";
}

void AsciiDisplayService::set_color_enabled(bool enabled) {

    color_enabled = enabled;
}
