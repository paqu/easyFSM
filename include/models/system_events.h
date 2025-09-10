#pragma once
#include "enum_utils.h"
#include <iostream>

/**
 * @brief System events that trigger state transitions
 */
enum class SystemEvent { TIME_EXPIRED, BUTTON_PRESSED };

std::ostream &operator<<(std::ostream &os, SystemEvent event);
