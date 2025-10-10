let simulator = null;
let isRunning = false;
let currentTimer = null;
let timerInterval = null;
let stats = {
  totalTransitions: 0,
  pedestrianCrossings: 0,
};

const STATE_DURATIONS = {
  CAR_GREEN: 10,
  CAR_YELLOW: 2,
  CAR_RED: 8,
  CAR_RED_YELLOW: 2,
  WALK_PREP: 1,
  WALK: 5,
  WALK_FINISH: 2,
};

// Initialize when WASM module loads
createTrafficLightModule()
  .then((Module) => {
    console.log("✅ WebAssembly module loaded!");

    // Create simulator instance
    simulator = new Module.TrafficLightSimulator();

    // Set up callbacks
    Module.setDisplayCallback((stateName) => {
      updateDisplay(stateName);
    });

    // Initialize UI
    updateDisplay("CAR_GREEN");
    setupEventListeners();
  })
  .catch((err) => {
    console.error("❌ Failed to load WebAssembly:", err);
    alert("Failed to load simulation. Please refresh the page.");
  });

function updateDisplay(stateName) {
  // Update current state display
  document.getElementById("current-state").textContent = stateName;

  // Update traffic lights
  document.querySelectorAll(".light").forEach((light) => {
    light.classList.remove("active");
  });

  document.querySelectorAll(".ped-light").forEach((light) => {
    light.classList.remove("active");
  });

  // Activate appropriate lights
  if (stateName.includes("RED") && !stateName.includes("YELLOW")) {
    document.getElementById("red-light").classList.add("active");
  }
  if (stateName.includes("YELLOW")) {
    document.getElementById("yellow-light").classList.add("active");
    if (stateName.includes("RED")) {
      document.getElementById("red-light").classList.add("active");
    }
  }
  if (stateName.includes("GREEN")) {
    document.getElementById("green-light").classList.add("active");
  }

  // Pedestrian lights
  if (stateName === "WALK") {
    document.getElementById("ped-green").classList.add("active");
  } else {
    document.getElementById("ped-red").classList.add("active");
  }

  // Update timer
  if (STATE_DURATIONS[stateName]) {
    currentTimer = STATE_DURATIONS[stateName];
    document.getElementById("timer").textContent = currentTimer;
  }

  // Update stats
  stats.totalTransitions++;
  if (stateName === "WALK") {
    stats.pedestrianCrossings++;
  }
  updateStats();
}

function updateStats() {
  document.getElementById("total-transitions").textContent =
    stats.totalTransitions;
  document.getElementById("ped-crossings").textContent =
    stats.pedestrianCrossings;
}

function startSimulation() {
  if (isRunning || !simulator) return;

  isRunning = true;
  document.getElementById("start-btn").disabled = true;
  document.getElementById("stop-btn").disabled = false;
  document.getElementById("step-btn").disabled = true;

  // Start timer countdown
  timerInterval = setInterval(() => {
    if (currentTimer > 0) {
      currentTimer--;
      document.getElementById("timer").textContent = currentTimer;
    }

    if (currentTimer === 0) {
      simulator.timeoutExpired();
    }
  }, 1000);
}

function stopSimulation() {
  if (!isRunning) return;

  isRunning = false;
  document.getElementById("start-btn").disabled = false;
  document.getElementById("stop-btn").disabled = true;
  document.getElementById("step-btn").disabled = false;

  if (timerInterval) {
    clearInterval(timerInterval);
    timerInterval = null;
  }
}

function stepSimulation() {
  if (!simulator || isRunning) return;
  simulator.timeoutExpired();
}

function pressButton() {
  if (!simulator) return;

  simulator.buttonPressed();
  document.getElementById("ped-request").textContent = "Yes";

  // Visual feedback
  const btn = document.getElementById("ped-btn");
  btn.style.background = "#ff4444";
  setTimeout(() => {
    btn.style.background = "#667eea";

    // Check if request is still pending
    if (simulator && !simulator.hasPedestrianRequest()) {
      document.getElementById("ped-request").textContent = "No";
    }
  }, 300);
}

function setupEventListeners() {
  document
    .getElementById("start-btn")
    .addEventListener("click", startSimulation);
  document.getElementById("stop-btn").addEventListener("click", stopSimulation);
  document.getElementById("ped-btn").addEventListener("click", pressButton);
  document.getElementById("step-btn").addEventListener("click", stepSimulation);
}
