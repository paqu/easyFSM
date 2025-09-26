#pragma once
#include "traffic_enum_utils.h"
#include <iostream>

/**
 * @brief System events that trigger state transitions
 */
enum class TrafficEvent { TIME_EXPIRED, BUTTON_PRESSED };

std::ostream &operator<<(std::ostream &os, TrafficEvent event);
