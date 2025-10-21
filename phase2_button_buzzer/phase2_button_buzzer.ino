/*
 * Arduino IoT Workshop - Phase 2: Button + Buzzer Control
 * 
 * Components Used:
 * - Arduino Uno R3
 * - Red LED (direct connection from Phase 1)
 * - Active Buzzer
 * - Push Button
 * - Breadboard with power rails
 * - Breadboard power supply module
 * 
 * Pin Assignments:
 * - Pin 13: Red LED (direct connection to ground rail)
 * - Pin 7: Active Buzzer (positive lead, negative to ground)
 * - Pin 8: Push Button (with internal pull-up resistor)
 * - Arduino 5V: Breadboard top rail (positive power)
 * - Arduino GND: Breadboard bottom rail (ground)
 * 
 * Wiring Diagram:
 * Arduino 5V  ----->  Breadboard Top Rail (Red/Positive)
 * Arduino GND ----->  Breadboard Bottom Rail (Blue/Negative)
 * Pin 13      ----->  LED Positive Lead (longer leg)
 * LED Negative ----->  Breadboard Ground Rail (direct connection)
 * Pin 7       ----->  Buzzer Positive Lead
 * Buzzer Negative ----->  Breadboard Ground Rail
 * Pin 8       ----->  Push Button (other terminal to GND)
 * 
 * Expected Behavior:
 * - LED starts in OFF state
 * - Button press: LED turns ON + buzzer beeps once
 * - Button press again: LED turns OFF + buzzer beeps once
 * - Serial output shows button presses and LED state changes
 * - Ready for Phase 3 (Full Proximity Alarm System)
 * 
 * Serial Output Examples:
 * "=== Arduino IoT Workshop - Phase 2 ==="
 * "Button + Buzzer System Initialized"
 * "Button Pressed! LED: ON, Buzzer: BEEP"
 * "Button Pressed! LED: OFF, Buzzer: BEEP"
 */

// Pin definitions
#define LED_PIN 13
#define BUZZER_PIN 7
#define BUTTON_PIN 8

// Timing constants
#define BUZZER_BEEP_DURATION 200  // 200ms beep duration

// Global variables
bool ledState = false;
bool lastButtonState = HIGH;
unsigned long startTime = 0;
unsigned long lastBeepTime = 0;
bool buzzerActive = false;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Wait for Serial to initialize
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Configure pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize outputs to OFF state
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  ledState = false;
  
  // Record start time
  startTime = millis();
  
  // Display workshop header and system information
  displayWorkshopHeader();
  
  Serial.println("Button + Buzzer System Ready!");
  Serial.println("Press button to toggle LED and trigger buzzer");
  Serial.println("================================");
}

void loop() {
  // Read button state
  readButton();
  
  // Handle buzzer timing
  handleBuzzer();
  
  // Small delay for stability
  delay(10);
}

void readButton() {
  // Read the button state
  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  
  // Check for button press (transition from not pressed to pressed)
  if (buttonPressed && !lastButtonState) {
    // Button was just pressed - toggle LED and trigger buzzer
    toggleLED();
    triggerBuzzer();
  }
  
  // Update last button state
  lastButtonState = buttonPressed;
}

void toggleLED() {
  // Toggle LED state
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  
  // Serial output
  Serial.print("Button Pressed! LED: ");
  Serial.print(ledState ? "ON" : "OFF");
  Serial.println(", Buzzer: BEEP");
}

void triggerBuzzer() {
  // Start buzzer beep
  digitalWrite(BUZZER_PIN, HIGH);
  buzzerActive = true;
  lastBeepTime = millis();
}

void handleBuzzer() {
  // Check if buzzer should be turned off
  if (buzzerActive && (millis() - lastBeepTime >= BUZZER_BEEP_DURATION)) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerActive = false;
  }
}

void displayWorkshopHeader() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("=== Arduino IoT Workshop - Phase 2 ===");
  Serial.println("========================================");
  Serial.println("Button + Buzzer Control System Initialized");
  Serial.println();
  Serial.println("Hardware Configuration:");
  Serial.print("Pin ");
  Serial.print(LED_PIN);
  Serial.println(": Red LED");
  Serial.print("Pin ");
  Serial.print(BUZZER_PIN);
  Serial.println(": Active Buzzer");
  Serial.print("Pin ");
  Serial.print(BUTTON_PIN);
  Serial.println(": Push Button (INPUT_PULLUP)");
  Serial.println();
  Serial.println("Functionality:");
  Serial.println("- Button press toggles LED on/off");
  Serial.println("- Each button press triggers buzzer beep");
  Serial.println("- LED state persists until next button press");
  Serial.println("- Buzzer beeps for 200ms on each press");
  Serial.println();
  Serial.println("Next Phase: Proximity Alarm System (Phase 3)");
  Serial.println("========================================");
  Serial.println();
}

/*
 * Common Issues and Solutions:
 * 
 * 1. Button not responding:
 *    - Check button connected between pin 8 and ground
 *    - Verify INPUT_PULLUP mode in code
 *    - Test button continuity with multimeter
 *    - Check for loose breadboard connections
 * 
 * 2. LED not toggling:
 *    - Check pin 13 connection
 *    - Verify LED polarity (longer leg = positive)
 *    - Verify LED negative connected directly to ground rail
 *    - Ensure ground connection to breadboard rail
 * 
 * 3. Buzzer not working:
 *    - Check pin 7 connection and polarity
 *    - Verify buzzer is active type (not passive)
 *    - Test buzzer directly with 5V and GND
 *    - Check buzzer ground connection
 * 
 * 4. Multiple triggers on single press:
 *    - Button may be bouncing - this is normal behavior
 *    - Ensure clean button press and release
 *    - Check button quality and mounting
 * 
 * 5. No serial output:
 *    - Verify baud rate is set to 9600
 *    - Check USB connection and COM port
 *    - Ensure Serial Monitor is open in Arduino IDE
 *    - Verify Arduino is properly powered
 * 
 * 6. LED and buzzer not synchronized:
 *    - This is expected - LED toggles, buzzer beeps on each press
 *    - LED state persists, buzzer only beeps briefly
 *    - Check that both components are connected to correct pins
 */
