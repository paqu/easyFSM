#include <stdint.h>

#include <iostream>
#include <map>

#include <pthread.h>
#include <unistd.h>

char shared_char;
bool char_available = false;
bool timeout_expired = false;
bool timeout_set = false;
uint32_t shared_timeout_sec = 0;

pthread_mutex_t worker_mutex;
pthread_mutex_t timeout_mutex;
pthread_cond_t worker_convar;
pthread_cond_t start_timeout_convar;

void process_traffic_light(bool timeout_expired, bool button_pressed);

/**
 * @brief Reading console
 * Read the char from console and store the char in the @ref shared_char
 * variable and then signal the conditional variable @ref worker_convar.
 * In case the pressed char is a 'q' the thread will end.
 *
 * @param arg not used
 * @return void* not used
 */
void *read_char(void *arg) {
    while (true) {
        char input_char;
        std::cin >> input_char;
        pthread_mutex_lock(&worker_mutex);
        char_available = true;
        shared_char = input_char;
        pthread_cond_signal(&worker_convar);
        pthread_mutex_unlock(&worker_mutex);

        if ('q' == input_char) {
            break;
        }
    }
    return nullptr;
}

/**
 * @brief Function used to start generation of the timeout
 * This function will store the required timeout value in @ref
 * shared_timeout_sec varaible and signal the @ref start_timeout_convar
 * conditional variable to start the @ref timeout_generator thread. This thread
 * will then signal the @ref worker_convar conditional variable after the
 * timeout expired.
 *
 * @param timeout_sec the timeout value in seconds
 */
void start_timeout(uint32_t timeout_sec) {
    pthread_mutex_lock(&timeout_mutex);
    shared_timeout_sec = timeout_sec;
    timeout_set = true;
    pthread_cond_signal(&start_timeout_convar);
    pthread_mutex_unlock(&timeout_mutex);
}

/**
 * @brief This thread is used to generate timeouts.
 * When the @ref start_timeout() function has been called. This thread will
 * signal the @ref worker_convar conditional variable after given number of
 * seconds.
 * @param arg not used
 * @return void* not used
 */
void *timeout_generator(void *arg) {
    while (true) {
        uint32_t local_timeout_sec = 0;
        pthread_mutex_lock(&timeout_mutex);
        while (!timeout_set) {
            pthread_cond_wait(&start_timeout_convar, &timeout_mutex);
        }
        local_timeout_sec = shared_timeout_sec;
        shared_timeout_sec = 0;
        timeout_expired = false;
        timeout_set = false;
        pthread_mutex_unlock(&timeout_mutex);
        std::cout << "Start timeout " << local_timeout_sec << std::endl;
        sleep(local_timeout_sec); // Wait for the timeout duration
        std::cout << "Timeout " << local_timeout_sec << " seconds" << std::endl;
        pthread_mutex_lock(&worker_mutex);
        timeout_expired = true;
        pthread_cond_signal(&worker_convar);
        pthread_mutex_unlock(&worker_mutex);

        // If 'q' is entered, stop the timeout thread as well
        if (shared_char == 'q') {
            break;
        }
    }
    return nullptr;
}

/**
 * @brief Main worker thread
 * Receives the chars from the console and timeouts using @ref worker_convar
 * conditional variable
 * TODO: implement traffic light functionality here.
 * @param arg
 * @return void*
 */
void *worker(void *arg) {
    std::cout << "Starting worker thread" << std::endl;
    start_timeout(1);
    while (true) {

        pthread_mutex_lock(&worker_mutex);
        while ((!timeout_expired) && (!char_available)) {
            pthread_cond_wait(&worker_convar, &worker_mutex);
        }

        process_traffic_light(timeout_expired, char_available);

        timeout_expired = false;
        char_available = false;

        pthread_mutex_unlock(&worker_mutex);

        // If 'q' is entered, stop the timeout thread as well
        if (shared_char == 'q') {
            start_timeout(0);
            break;
        }
    }
    std::cout << "End worker" << std::endl;
    return nullptr;
}

struct LightTimings {
    static constexpr uint32_t RED_DURATION = 8;
    static constexpr uint32_t RED_YELLOW_DURATION = 2;
    static constexpr uint32_t GREEN_DURATION = 10;
    static constexpr uint32_t YELLOW_DURATION = 2;
    static constexpr uint32_t WALK_DURATION = 5;
    static constexpr uint32_t WALK_PREP_DURATION = 1;
    static constexpr uint32_t WALK_FINISH_DURATION = 2;
};

struct TrafficLights {
    bool red;
    bool yellow;
    bool green;
};
struct PedestrianLights {
    bool red;
    bool green;
};

struct StateContext {
    std::string name;
    uint32_t duration;
    TrafficLights carLights;
    PedestrianLights pedLights;
};

class TrafficLightController {
  public:
    enum State {
        CAR_GREEN,
        CAR_YELLOW,
        CAR_RED,
        WALK_PREP,
        WALK,
        WALK_FINISH,
        CAR_RED_YELLOW
    };

    TrafficLightController()
        : current_state(CAR_RED), pedestrian_request(false) {
        // Initialize all states
        states[CAR_GREEN] = {
            "CAR_GREEN",
            LightTimings::GREEN_DURATION,
            {false, false, true},
            {true, false},
        };
        states[CAR_YELLOW] = {
            "CAR_YELLOW",
            LightTimings::YELLOW_DURATION,
            {false, true, false},
            {true, false},
        };
        states[CAR_RED] = {
            "CAR_RED",
            LightTimings::RED_DURATION,
            {true, false, false},
            {true, false},
        };
        states[WALK_PREP] = {
            "WALK_PREP",
            LightTimings::WALK_PREP_DURATION,
            {true, false, false},
            {true, false},
        };
        states[WALK] = {
            "WALK",
            LightTimings::WALK_DURATION,
            {true, false, false},
            {false, true},
        };
        states[WALK_FINISH] = {
            "WALK_FINISH",
            LightTimings::WALK_FINISH_DURATION,
            {true, false, false},
            {true, false},
        };
        states[CAR_RED_YELLOW] = {
            "CAR_RED_YELLOW",
            LightTimings::RED_YELLOW_DURATION,
            {true, true, false},
            {true, false},
        };
    }

    void button_pressed() {
        if (pedestrian_request != true) {
            pedestrian_request = true;
            std::cout << "Button Pressed" << std ::endl;
        } else {
            std::cout << "Request is processed." << std::endl;
        }
    }

    void timeout_expired() {
        // Determine next state
        current_state = get_next_state(current_state, pedestrian_request);
        // Copy current state info into context
        StateContext context = states[current_state];

        // Call print function
        print_state_info(context);

        if (current_state == WALK_FINISH) {
            pedestrian_request = false;
        }

        // Start timeout for next state
        start_timeout(states[current_state].duration);
    }

  private:
    State current_state;
    bool pedestrian_request;
    std::map<State, StateContext> states;

    State get_next_state(State s, bool ped_request) const {
        switch (s) {
        case CAR_GREEN:
            return CAR_YELLOW;
        case CAR_YELLOW:
            return ped_request ? WALK_PREP : CAR_RED;
        case CAR_RED:
            return CAR_RED_YELLOW;
        case WALK_PREP:
            return WALK;
        case WALK:
            return WALK_FINISH;
        case WALK_FINISH:
            return CAR_RED_YELLOW;
        case CAR_RED_YELLOW:
            return CAR_GREEN;
        default:
            return CAR_GREEN;
        }
    }

    void print_state_info(const StateContext &ctx) const {
        std::cout << "------------------------ \n"
                  << "State: " << ctx.name << "\n"
                  << "Duration: " << ctx.duration << "s\n"
                  << "Car Lights: [R:" << ctx.carLights.red
                  << ", Y:" << ctx.carLights.yellow
                  << ", G:" << ctx.carLights.green << "]\n"
                  << "Pedestrian Lights: [R:" << ctx.pedLights.red
                  << ", G:" << ctx.pedLights.green << "]\n\n";
    }

    void start_timeout(uint32_t duration) const {
        ::start_timeout(duration); // your external timeout function
    }
};

TrafficLightController controller;
/**
 * Main function for traffic light state machine
 * @param timeout_expired true when the last timeout started with @ref
 * start_timeout() has expired
 * @param button_pressed true when the user has pressed the button
 */
void process_traffic_light(bool timeout_expired, bool button_pressed) {

    if (button_pressed)
        controller.button_pressed();
    if (timeout_expired)
        controller.timeout_expired();
}

int main() {
    pthread_t reader_thread, worker_thread, timeout_thread;

    pthread_mutex_init(&worker_mutex, nullptr);
    pthread_mutex_init(&timeout_mutex, nullptr);
    pthread_cond_init(&worker_convar, nullptr);
    pthread_cond_init(&start_timeout_convar, nullptr);

    pthread_create(&reader_thread, nullptr, read_char, nullptr);
    pthread_create(&worker_thread, nullptr, worker, nullptr);
    pthread_create(&timeout_thread, nullptr, timeout_generator, nullptr);

    pthread_join(reader_thread, nullptr);
    pthread_join(worker_thread, nullptr);
    pthread_join(timeout_thread, nullptr);

    pthread_mutex_destroy(&worker_mutex);
    pthread_mutex_destroy(&timeout_mutex);
    pthread_cond_destroy(&worker_convar);
    pthread_cond_destroy(&start_timeout_convar);

    return 0;
}
