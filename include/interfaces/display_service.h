#pragma once

/**
 * @brief Interface for displaying traffic light state information
 */

template <typename SystemContext> class IDisplayService {
  public:
    virtual ~IDisplayService() = default;
    virtual void show_state(const SystemContext &ctx) = 0;
};
