#pragma once

namespace state_machine {
/**
 * @brief Interface for displaying traffic light state information
 */

template <typename SystemContext> class IDisplayService {
  public:
    virtual ~IDisplayService() = default;
    virtual void show_state(const SystemContext &ctx) = 0;
};
} // namespace state_machine
