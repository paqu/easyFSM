#include "traffic_states.h"
#include "traffic_enum_utils.h"

std::ostream &operator<<(std::ostream &os, TrafficState state) {
    return os << TrafficEnumUtils::state_to_string(state);
}
