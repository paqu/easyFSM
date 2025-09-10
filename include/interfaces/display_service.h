#pragma once

struct StateContext;

/**
 * @brief Interface for displaying traffic light state information
 */
class IDisplayService {
  public:
    virtual ~IDisplayService() = default;
    virtual void show_state(const StateContext &ctx) = 0;
    virtual void show_button_state(bool waiting_to_be_processed) = 0;
};
