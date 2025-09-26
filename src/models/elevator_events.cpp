#include "elevator_events.h"
#include "elevator_enum_utils.h"

std::ostream &operator<<(std::ostream &os, ElevatorEvent event) {
    return os << ElevatorEnumUtils::elevator_event_to_string(event);
}
