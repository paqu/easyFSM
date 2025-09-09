#include "system_events.h"
#include "enum_utils.h"

std::ostream &operator<<(std::ostream &os, SystemEvent event) {
    return os << EnumUtils::event_to_string(event);
}
