#pragma once

struct StateContext;

/**
 * @brief Interface for displaying traffic light state information
 */
class IDisplayService {
  public:
    virtual ~IDisplayService() = default;
    virtual void showState(const StateContext &ctx) = 0;
    virtual void showButtonState(bool waiting_to_be_processed) = 0;
};
