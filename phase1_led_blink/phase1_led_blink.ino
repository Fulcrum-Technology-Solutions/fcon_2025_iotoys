/*
 * Arduino IoT Workshop - Phase 1: LED Blink
 * 
 * Components Used:
 * - Arduino Uno R3
 * - Green LED with 220Ω resistor
 * - Breadboard with power rails
 * - Breadboard power supply module
 * 
 * Pin Assignments:
 * - Pin 13: Green LED (via 220Ω resistor to ground)
 * - Arduino 5V: Breadboard top rail (positive power)
 * - Arduino GND: Breadboard bottom rail (ground)
 * 
 * Wiring Diagram:
 * Arduino 5V  ----->  Breadboard Top Rail (Red/Positive)
 * Arduino GND ----->  Breadboard Bottom Rail (Blue/Negative)
 * Pin 13      ----->  LED Positive Lead (longer leg)
 * LED Negative ----->  220Ω Resistor ----->  Breadboard Ground Rail
 * 
 * Expected Behavior:
 * - LED blinks continuously: 500ms ON, 500ms OFF
 * - Serial output shows LED state changes with timestamps
 * - System information displayed on startup
 * - Non-blocking millis() timing used
 * - Ready for Phase 2 (Button + Buzzer)
 * 
 * Serial Output Examples:
 * "=== Arduino IoT Workshop - Phase 1 ==="
 * "LED Blink System Initialized"
 * "LED State: ON (Elapsed: 1234ms)"
 * "LED State: OFF (Elapsed: 1734ms)"
 */

// Pin definitions
#define LED_PIN 13

// Timing constants
#define BLINK_INTERVAL 500  // 500ms on/off cycle

// Global variables
unsigned long previousBlinkTime = 0;
bool ledState = false;
unsigned long startTime = 0;
unsigned long lastSerialUpdate = 0;
const unsigned long serialUpdateInterval = 500; // Update serial every 500ms

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Wait for Serial to initialize
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Configure LED pin as output
  pinMode(LED_PIN, OUTPUT);
  
  // Record start time
  startTime = millis();
  
  // Display workshop header and system information
  displayWorkshopHeader();
  
  // Initialize LED to OFF state
  digitalWrite(LED_PIN, LOW);
  ledState = false;
  
  Serial.println("LED Blink System Ready!");
  Serial.println("================================");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Non-blocking LED blink control using millis()
  blinkLED();
  
  // Update serial output with current status (every 500ms)
  if (currentTime - lastSerialUpdate >= serialUpdateInterval) {
    updateSerial();
    lastSerialUpdate = currentTime;
  }
  
  // Small delay for stability
  delay(10);
}

void blinkLED() {
  unsigned long currentTime = millis();
  
  // Check if it's time to change LED state
  if (currentTime - previousBlinkTime >= BLINK_INTERVAL) {
    // Toggle LED state
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    
    // Update timing for next blink
    previousBlinkTime = currentTime;
  }
}

void updateSerial() {
  unsigned long elapsedTime = millis() - startTime;
  
  // Display LED state changes
  if (ledState) {
    Serial.print("LED State: ON  (Elapsed: ");
  } else {
    Serial.print("LED State: OFF (Elapsed: ");
  }
  Serial.print(elapsedTime);
  Serial.println("ms)");
}

void displayWorkshopHeader() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("=== Arduino IoT Workshop - Phase 1 ===");
  Serial.println("========================================");
  Serial.println("LED Blink System Initialized");
  Serial.println();
  Serial.println("Hardware Configuration:");
  Serial.print("Pin ");
  Serial.print(LED_PIN);
  Serial.println(": Green LED");
  Serial.println("Breadboard: Power distribution");
  Serial.println("Resistor: 220Ω (current limiting)");
  Serial.println();
  Serial.println("Blink Pattern: 500ms ON, 500ms OFF");
  Serial.println("Timing: Non-blocking millis() based");
  Serial.println();
  Serial.println("Next Phase: Button + Buzzer (Phase 2)");
  Serial.println("========================================");
  Serial.println();
}

/*
 * Common Issues and Solutions:
 * 
 * 1. LED not lighting:
 *    - Check LED polarity (longer leg = positive)
 *    - Verify 220Ω resistor connection
 *    - Ensure ground connection to breadboard rail
 *    - Check pin 13 connection
 * 
 * 2. LED always on/off:
 *    - Check pin 13 connection
 *    - Verify code uploaded successfully
 *    - Check for loose breadboard connections
 *    - Ensure proper power supply
 * 
 * 3. Irregular blinking:
 *    - Check power supply stability
 *    - Verify all connections are secure
 *    - Ensure no interference from other components
 *    - Check for loose jumper wires
 * 
 * 4. No serial output:
 *    - Verify baud rate is set to 9600
 *    - Check USB connection and COM port
 *    - Ensure Serial Monitor is open in Arduino IDE
 *    - Verify Arduino is properly powered
 * 
 * 5. LED dim or flickering:
 *    - Check resistor value (should be 220Ω)
 *    - Verify power supply voltage (5V)
 *    - Check for loose connections
 *    - Ensure proper breadboard power rail connections
 */