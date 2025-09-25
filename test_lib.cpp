#include <iostream>
#include <state_machine/state_machine.h>

enum class TestState { A, B };
enum class TestEvent { GO };

int main() {
    using namespace state_machine;

    auto sm =
        std::make_shared<StateMachine<TestState, TestEvent>>(TestState::A);
    std::cout << "Library works! Current state: "
              << (int)sm->get_current_state() << std::endl;
    return 0;
}
