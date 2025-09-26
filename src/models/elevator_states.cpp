#include "elevator_states.h"
#include "elevator_enum_utils.h"

std::ostream &operator<<(std::ostream &os, ElevatorState state) {
    return os << ElevatorEnumUtils::elevator_state_to_string(state);
}
