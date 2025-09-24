#include "traffic_events.h"
#include "enum_utils.h"

std::ostream &operator<<(std::ostream &os, TrafficEvent event) {
    return os << EnumUtils::event_to_string(event);
}
