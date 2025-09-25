#pragma once

// Core interfaces
#include "core/action_handler.h"
#include "core/base_controller.h"
#include "core/state_machine.h"
#include "core/state_transition.h"

// Implementations
#include "implementations/conditional_state_transition.h"
#include "implementations/runtime_state_machine.h"
#include "implementations/simple_state_transition.h"

// Services
#include "services/display_service.h"
#include "services/function_timer_service.h"
#include "services/timer_service.h"

namespace state_machine {
// Convenience aliases
template <typename StateType, typename EventType>
using StateMachine = RuntimeStateMachine<StateType, EventType>;

template <typename StateType, typename EventType>
using Controller = BaseController<StateType, EventType>;
} // namespace state_machine
