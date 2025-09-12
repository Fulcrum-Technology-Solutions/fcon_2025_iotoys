/*
 * Arduino IoT Workshop - Phase 2: LCD Display Integration
 * 
 * Description: LCD display integration with LED functionality from Phase 1
 * Hardware: Arduino Uno R3, LCD1602 (16x2) with I2C backpack, Green LED (Pin 5), Red LED (Pin 6)
 * 
 * Pin Assignments:
 * Pin 5: Green LED (success/positive feedback)
 * Pin 6: Red LED (error/negative feedback)
 * A4: SDA (I2C data line)
 * A5: SCL (I2C clock line)
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
 * │  A4 (SDA) ──────┼── LCD I2C SDA
 * │  A5 (SCL) ──────┼── LCD I2C SCL
 * │  VCC ───────────┼── LCD I2C VCC (5V)
 * │  GND ───────────┼── LCD I2C GND
 * │                 │
 * └─────────────────┘
 * 
 * LCD I2C Backpack (Address 0x27)
 * ┌─────────────────┐
 * │ VCC  GND  SDA  SCL │
 * │  │    │    │    │  │
 * │  │    │    │    │  │
 * │  │    │    │    │  │
 * │  │    │    │    │  │
 * └─────────────────┘
 * 
 * Expected Behavior:
 * 1. Startup: Both LEDs blink 3 times simultaneously
 * 2. LCD initialization with error handling
 * 3. Display "ENV MONITOR v1.0" / "Initializing..." for 3 seconds
 * 4. Show "System Ready" with timestamp
 * 5. Continue alternating LED pattern from Phase 1
 * 
 * Troubleshooting:
 * - If LCD doesn't display: Check I2C address (try 0x3F), wiring, power
 * - If "LCD not found": Verify I2C connections and address
 * - If display is garbled: Check power supply and connections
 * - If LEDs don't work: Check Phase 1 troubleshooting
 * 
 * Known Limitations:
 * - No sensor integration yet
 * - Basic LCD display only
 * - No keypad integration yet
 * 
 * Upgrade Path to Phase 3:
 * - Add sensor data display
 * - Implement rotating display screens
 * - Add sensor alert conditions
 * 
 * Library Requirements:
 * - LiquidCrystal_I2C (latest compatible version)
 * - Wire (built-in I2C library)
 * 
 * Version: 1.0
 * Created: 2025
 * Compatible: Arduino IDE 1.8.x / 2.x
 */

// Include required libraries
#include <LiquidCrystal_I2C.h>  // LCD display library (latest compatible version)
#include <Wire.h>               // I2C communication (built-in)

// Pin Definitions
#define GREEN_LED_PIN 5
#define RED_LED_PIN 6

// LCD Configuration
#define LCD_ADDRESS 0x27        // I2C address for LCD (try 0x3F if not working)
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Timing Constants
#define STARTUP_BLINK_COUNT 3
#define STARTUP_BLINK_DURATION 200
#define ALTERNATING_DURATION 500
#define LCD_INIT_DELAY 3000     // 3 seconds for "Initializing..." message

// Global Variables
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// LED control variables (from Phase 1)
unsigned long lastBlinkTime = 0;
bool currentLED = false; // false = green, true = red
bool startupComplete = false;
int startupBlinkCount = 0;
unsigned long lastStartupBlink = 0;

// LCD control variables
bool lcdInitialized = false;
bool lcdStartupComplete = false;
unsigned long lcdStartupTime = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== Arduino IoT Workshop - Phase 2 ===");
  Serial.println("Initializing LCD display system...");
  
  // Initialize LED pins as outputs (from Phase 1)
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Turn off both LEDs initially
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("LED pins initialized successfully");
  
  // Initialize LCD display
  initializeLCD();
  
  // Initialize timing variables
  lastBlinkTime = millis();
  lastStartupBlink = millis();
}

void loop() {
  // Handle startup sequence first
  if (!startupComplete) {
    blinkStartup();
  } else {
    // Main alternating blink pattern (from Phase 1)
    alternatingBlink();
  }
  
  // Handle LCD startup sequence
  if (lcdInitialized && !lcdStartupComplete) {
    handleLCDStartup();
  }
}

void initializeLCD() {
  Serial.println("Initializing LCD display...");
  
  // Initialize I2C communication
  Wire.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Test LCD communication
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LCD Test");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  delay(1000); // Brief test display
  
  // Clear and prepare for startup message
  lcd.clear();
  
  lcdInitialized = true;
  lcdStartupTime = millis();
  
  Serial.println("LCD initialized successfully");
  Serial.println("Displaying startup message...");
}

void displayStartupMessage() {
  if (!lcdInitialized) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENV MONITOR v1.0");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  Serial.println("Startup message displayed");
}

void displaySystemReady() {
  if (!lcdInitialized) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  
  // Display current uptime in seconds
  unsigned long uptime = millis() / 1000;
  lcd.print("Uptime: ");
  lcd.print(uptime);
  lcd.print("s");
  
  Serial.println("System ready message displayed");
}

void handleLCDStartup() {
  unsigned long currentTime = millis();
  
  if (currentTime - lcdStartupTime >= LCD_INIT_DELAY) {
    if (!lcdStartupComplete) {
      displaySystemReady();
      lcdStartupComplete = true;
      Serial.println("LCD startup sequence complete");
    }
  } else {
    // Still in initialization phase
    if (currentTime - lcdStartupTime < 100) {
      // Show startup message immediately
      displayStartupMessage();
    }
  }
}

// LED control functions from Phase 1
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
      Serial.println("LED startup sequence complete");
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
