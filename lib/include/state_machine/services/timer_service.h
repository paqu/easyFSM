#pragma once
#include <cstdint>

namespace state_machine {

/**
 * @brief Interface for timer functionality
 */
class ITimerService {
  public:
    virtual ~ITimerService() = default;
    virtual void start_timeout(uint32_t duration_sec) = 0;
};
} // namespace state_machine
