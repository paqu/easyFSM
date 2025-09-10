#include "traffic_states.h"
#include "enum_utils.h"

std::ostream &operator<<(std::ostream &os, TrafficState state) {
    return os << EnumUtils::state_to_string(state);
}
