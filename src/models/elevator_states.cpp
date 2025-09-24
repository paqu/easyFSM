#include "elevator_states.h"
#include "enum_utils.h"

std::ostream &operator<<(std::ostream &os, ElevatorState state) {
    return os << EnumUtils::elevator_state_to_string(state);
}
