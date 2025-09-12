/*
 * Arduino IoT Workshop - Phase 1: Basic Setup and LED Control
 * 
 * Description: Basic LED control with startup sequence and alternating blink pattern
 * Hardware: Arduino Uno R3, Green LED (Pin 5), Red LED (Pin 6)
 * 
 * Pin Assignments:
 * Pin 5: Green LED (success/positive feedback)
 * Pin 6: Red LED (error/negative feedback)
 * 
 * Wiring Diagram:
 * 
 * Arduino Uno R3
 * ┌─────────────────┐
 * │                 │
 * │  Pin 5 ────[R]──┼── Green LED ──── GND
 * │                 │     (220Ω)
 * │  Pin 6 ────[R]──┼── Red LED ────── GND
 * │                 │     (220Ω)
 * │                 │
 * └─────────────────┘
 * 
 * Expected Behavior:
 * 1. Startup: Both LEDs blink 3 times simultaneously
 * 2. Main loop: Alternating pattern - Green 500ms, Red 500ms, repeat
 * 3. Serial output for debugging and status
 * 
 * Troubleshooting:
 * - If LEDs don't light: Check wiring and resistor values
 * - If timing is off: Verify non-blocking delay implementation
 * - If serial output missing: Check baud rate (9600)
 * 
 * Known Limitations:
 * - No sensor integration yet
 * - No LCD display yet
 * - Basic timing only
 * 
 * Upgrade Path to Phase 2:
 * - Add LCD display integration
 * - Maintain LED functionality
 * - Add I2C communication
 * 
 * Library Requirements:
 * - No external libraries required for Phase 1
 * - Uses built-in Arduino functions only
 * 
 * Version: 1.0
 * Created: 2025
 * Compatible: Arduino IDE 1.8.x / 2.x
 */

// Pin Definitions
#define GREEN_LED_PIN 5
#define RED_LED_PIN 6

// Timing Constants
#define STARTUP_BLINK_COUNT 3
#define STARTUP_BLINK_DURATION 200
#define ALTERNATING_DURATION 500

// Global Variables
unsigned long lastBlinkTime = 0;
bool currentLED = false; // false = green, true = red
bool startupComplete = false;
int startupBlinkCount = 0;
unsigned long lastStartupBlink = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== Arduino IoT Workshop - Phase 1 ===");
  Serial.println("Initializing LED control system...");
  
  // Initialize LED pins as outputs
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Turn off both LEDs initially
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("LED pins initialized successfully");
  Serial.println("Starting startup sequence...");
  
  // Initialize timing variables
  lastBlinkTime = millis();
  lastStartupBlink = millis();
}

void loop() {
  // Handle startup sequence first
  if (!startupComplete) {
    blinkStartup();
  } else {
    // Main alternating blink pattern
    alternatingBlink();
  }
}

void blinkStartup() {
  unsigned long currentTime = millis();
  
  // Check if it's time for next startup blink
  if (currentTime - lastStartupBlink >= STARTUP_BLINK_DURATION) {
    if (startupBlinkCount < STARTUP_BLINK_COUNT * 2) { // *2 for on/off cycles
      if (startupBlinkCount % 2 == 0) {
        // Turn on both LEDs
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("Startup blink ON");
      } else {
        // Turn off both LEDs
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        Serial.println("Startup blink OFF");
      }
      startupBlinkCount++;
    } else {
      // Startup sequence complete
      startupComplete = true;
      Serial.println("Startup sequence complete - entering alternating mode");
    }
    lastStartupBlink = currentTime;
  }
}

void alternatingBlink() {
  unsigned long currentTime = millis();
  
  // Check if it's time to switch LEDs
  if (currentTime - lastBlinkTime >= ALTERNATING_DURATION) {
    if (currentLED) {
      // Switch to green LED
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, HIGH);
      Serial.println("Green LED active");
      currentLED = false;
    } else {
      // Switch to red LED
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("Red LED active");
      currentLED = true;
    }
    lastBlinkTime = currentTime;
  }
}
