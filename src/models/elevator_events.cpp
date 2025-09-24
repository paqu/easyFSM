#include "elevator_events.h"
#include "enum_utils.h"

std::ostream &operator<<(std::ostream &os, ElevatorEvent event) {
    return os << EnumUtils::elevator_event_to_string(event);
}
