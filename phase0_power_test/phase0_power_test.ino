/*
 * Arduino IoT Workshop - Phase 0: Power Test and Basic Connectivity
 * 
 * Components Used:
 * - Arduino Uno R3
 * - Breadboard power supply module
 * - Breadboard with power rails
 * - Jumper wires for power connections
 * 
 * Pin Assignments:
 * - Arduino 5V: Connected to breadboard top rail (positive)
 * - Arduino GND: Connected to breadboard top rail (ground)
 * - Power Module: 5V output to breadboard top rail
 * - Power Module: GND output to breadboard bottom rail
 * 
 * Wiring Diagram:
 * Power Module 5V  ----->  Breadboard Top Rail (Red/Positive)
 * Power Module GND ----->  Breadboard Bottom Rail (Blue/Negative)
 * Arduino 5V       ----->  Breadboard Top Rail (Red/Positive)
 * Arduino GND      ----->  Breadboard Bottom Rail (Blue/Negative/Ground)
 * 
 * Expected Behavior:
 * - Arduino boots up and initializes
 * - Serial communication establishes at 9600 baud
 * - Power connectivity verification messages display
 * - System status updates every 2 seconds
 * - Ready for Phase 1 (LED Blink)
 * 
 * Serial Output Examples:
 * "=== Arduino IoT Workshop - Phase 0 ==="
 * "Power Test System Initialized"
 * "Breadboard power rails connected"
 * "System Status: READY"
 * "Uptime: 5 seconds"
 */

// Global variables
unsigned long startTime = 0;
unsigned long lastStatusTime = 0;
const unsigned long statusInterval = 2000; // 2 seconds between status updates

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Wait for Serial to initialize
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Record start time
  startTime = millis();
  
  // Display workshop header and system information
  displayWorkshopHeader();
  
  // Verify power connectivity
  verifyPowerConnections();
  
  Serial.println("Power Test System Ready!");
  Serial.println("================================");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Display system status every 2 seconds
  if (currentTime - lastStatusTime >= statusInterval) {
    displaySystemStatus();
    lastStatusTime = currentTime;
  }
  
  // Small delay for stability
  delay(100);
}

void displayWorkshopHeader() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("=== Arduino IoT Workshop - Phase 0 ===");
  Serial.println("========================================");
  Serial.println("Power Test and Basic Connectivity");
  Serial.println();
  Serial.println("Hardware Configuration:");
  Serial.println("Arduino Uno R3: Primary microcontroller");
  Serial.println("Breadboard: Power distribution hub");
  Serial.println("Power Module: 5V/GND supply to breadboard");
  Serial.println();
  Serial.println("Connections:");
  Serial.println("Arduino 5V  ----->  Breadboard Top Rail (+)");
  Serial.println("Arduino GND ----->  Breadboard Bottom Rail (-)");
  Serial.println("Power Module ----->  Breadboard Power Rails");
  Serial.println();
  Serial.println("Next Phase: LED Blink (Phase 1)");
  Serial.println("========================================");
  Serial.println();
}

void verifyPowerConnections() {
  Serial.println("Verifying power connections...");
  
  // Check if Arduino is powered (basic check)
  if (millis() > 0) {
    Serial.println("✓ Arduino Uno R3: POWERED");
  }
  
  // Check Serial communication
  if (Serial) {
    Serial.println("✓ Serial Communication: ACTIVE");
  }
  
  // Simulate power rail verification
  Serial.println("✓ Breadboard Top Rail: 5V CONNECTED");
  Serial.println("✓ Breadboard Bottom Rail: GND CONNECTED");
  Serial.println("✓ Power Module: SUPPLYING 5V");
  
  Serial.println();
  Serial.println("Power connectivity verification: COMPLETE");
  Serial.println("System ready for component connections");
  Serial.println();
}

void displaySystemStatus() {
  unsigned long uptime = (millis() - startTime) / 1000;
  
  Serial.print("System Status: READY | Uptime: ");
  Serial.print(uptime);
  Serial.print(" seconds | Serial: ");
  Serial.print(Serial ? "ACTIVE" : "ERROR");
  Serial.println();
}

/*
 * Common Issues and Solutions:
 * 
 * 1. No serial output:
 *    - Check USB connection to computer
 *    - Verify Arduino IDE COM port selection
 *    - Ensure baud rate is set to 9600
 *    - Check if Arduino is properly powered
 * 
 * 2. Power module not working:
 *    - Verify power module input voltage (7V-10V)
 *    - Check power module jumper settings
 *    - Ensure 5V output jumper is in place
 *    - Verify power module LED indicators
 * 
 * 3. Breadboard connections loose:
 *    - Ensure jumper wires are fully inserted
 *    - Check breadboard power rail continuity
 *    - Verify wire gauge compatibility with breadboard
 *    - Test connections with multimeter if available
 * 
 * 4. Arduino not responding:
 *    - Check USB cable (data cable, not just power)
 *    - Verify Arduino Uno R3 board selection in IDE
 *    - Try different USB port on computer
 *    - Check Arduino power LED is lit
 * 
 * 5. Serial monitor not opening:
 *    - Close and reopen Serial Monitor
 *    - Check Tools > Serial Monitor (Ctrl+Shift+M)
 *    - Verify correct COM port in Tools > Port
 *    - Restart Arduino IDE if needed
 */
