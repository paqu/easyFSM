#pragma once
#include "display_service.h"
#include "traffic_context.h"
#include <string>

/**
 * @brief ASCII art-based implementation of display service
 */
class AsciiDisplayService : public IDisplayService<TrafficContext> {
  private:
    // ASCII art components
    std::string create_traffic_light_frame() const;
    std::string create_car_light(bool red, bool yellow, bool green) const;
    std::string create_pedestrian_light(bool red, bool green) const;
    std::string create_separator() const;

  protected:
    // Color codes for terminal
    static const std::string RED_COLOR;
    static const std::string YELLOW_COLOR;
    static const std::string GREEN_COLOR;
    static const std::string WHITE_COLOR;
    static const std::string GRAY_COLOR;
    static const std::string RESET_COLOR;
    static const std::string BOLD;
    bool color_enabled = true;

  public:
    void show_state(const TrafficContext &ctx) override;

    // Optional: disable colors for terminals that don't support them
    void set_color_enabled(bool enabled);
};
