#include "traffic_events.h"
#include "traffic_enum_utils.h"

std::ostream &operator<<(std::ostream &os, TrafficEvent event) {
    return os << TrafficEnumUtils::event_to_string(event);
}
