/*
 * Arduino IoT Workshop - Phase 3: Proximity Alarm System
 * 
 * Components Used:
 * - Arduino Uno R3
 * - 4-digit 7-segment display (common cathode) with shift registers
 * - HC-SR04 Ultrasonic Distance Sensor
 * - Active Buzzer
 * - Push Button
 * - LED (direct connection)
 * - 220Ω resistors for 7-segment segments
 * - Breadboard for power distribution
 * 
 * Pin Assignments:
 * - Pins 4-6: Shift Register Control (data, latch, clock)
 * - Pins 9-12: 4-Digit 7-Segment Display Common Cathodes
 * - Pin 2: Ultrasonic Trigger (HC-SR04)
 * - Pin 3: Ultrasonic Echo (HC-SR04)
 * - Pin 7: Active Buzzer
 * - Pin 8: Push Button (with internal pull-up resistor)
 * - Pin 13: LED (direct connection to ground rail)
 * - 5V: Breadboard top rail (positive power)
 * - GND: Breadboard bottom rail (ground)
 * 
 * Wiring Diagram:
 * Arduino 5V  ----->  Breadboard Top Rail (Red/Positive)
 * Arduino GND ----->  Breadboard Bottom Rail (Blue/Negative)
 * Pin 4       ----->  Shift Register Data
 * Pin 5       ----->  Shift Register Latch
 * Pin 6       ----->  Shift Register Clock
 * Pin 9-12    ----->  4-Digit Common Cathodes
 * Pin 2       ----->  HC-SR04 TRIG
 * Pin 3       ----->  HC-SR04 ECHO
 * Pin 7       ----->  Buzzer Positive (negative to GND)
 * Pin 8       ----->  Push Button (other terminal to GND)
 * Pin 13      ----->  LED Positive Lead (negative to GND rail)
 * HC-SR04 VCC ----->  5V Rail
 * HC-SR04 GND ----->  Ground Rail
 * 7-Seg Common Cathodes ----->  Ground Rail
 * 
 * Expected Behavior:
 * - Distance measurement every 100ms
 * - Display distance 0-9999cm on 4-digit 7-segment display
 * - LED lights up when distance ≤5cm
 * - Buzzer alarm when distance ≤10cm with car-like beeping
 * - Button toggles buzzer mute/unmute
 * - Faster beeping as distance approaches 2cm
 * 
 * Serial Output Examples:
 * "Distance: 5cm, Mute: OFF, LED: ON"
 * "Distance: 1cm, Mute: OFF, LED: ON - BACKUP ALERT! Beep interval: 400ms"
 * "Mute: ON"
 */

// Pin definitions for 7-segment display
int CA_1 = 12;  // Common cathode digit 1
int CA_2 = 11;  // Common cathode digit 2  
int CA_3 = 10;  // Common cathode digit 3
int CA_4 = 9;   // Common cathode digit 4
int clk = 6;    // Shift register clock
int latch = 5;  // Shift register latch
int data = 4;   // Shift register data

// Pin definitions for ultrasonic sensor
int trigPin = 2;
int echoPin = 3;

// Pin definition for buzzer
int buzzerPin = 7;

// Pin definition for mute button
int muteButtonPin = 8;

// Pin definition for LED
int ledPin = 13;

// Display variables
int count = 0;
int digits[4];
int CAS[4] = {12, 11, 10, 9};
long distance = 0;

// Buzzer control variables
bool buzzerState = false;
bool muteEnabled = false;

// Button control variables
bool lastButtonState = HIGH;

// Timing variables
unsigned long previousDisplayMillis = 0;
unsigned long previousSensorMillis = 0;
unsigned long previousBuzzerMillis = 0;
const unsigned long displayInterval = 2;    // 2ms between digit updates
const unsigned long sensorInterval = 100;   // 100ms between distance readings
unsigned long buzzerInterval = 200;         // Variable buzzer beep interval (changes based on distance)

// Byte combinations for each number 0-9 (segments A-G)
byte numbers[10] = {
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110  // 9
};

void setup() {
  Serial.begin(9600);
  
  // Configure 7-segment display pins
  pinMode(CA_1, OUTPUT);
  pinMode(CA_2, OUTPUT);
  pinMode(CA_3, OUTPUT);
  pinMode(CA_4, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
  
  // Configure ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Configure buzzer pin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Start with buzzer off
  
  // Configure mute button pin with internal pull-up
  pinMode(muteButtonPin, INPUT_PULLUP);
  
  // Configure LED pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
  
  // Turn off all digits initially
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
  
  // Display workshop header and system information
  displayWorkshopHeader();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Read button for mute toggle
  readMuteButton();
  
  // Update distance reading every 100ms
  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;
    distance = measureDistance();
    
    // Limit distance to 4 digits (0-9999 cm)
    if (distance > 9999) {
      distance = 9999;
    }
    
    break_number(distance);
    
    // Control LED based on distance
    if (distance <= 5) {
      digitalWrite(ledPin, HIGH);  // Turn on LED
    } else {
      digitalWrite(ledPin, LOW);   // Turn off LED
    }
    
    // Debug output to serial with beep speed info
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.print(", Mute: ");
    Serial.print(muteEnabled ? "ON" : "OFF");
    Serial.print(", LED: ");
    Serial.print((distance <= 5) ? "ON" : "OFF");
    if (distance <= 10 && distance >= 2 && !muteEnabled) {
      Serial.print(" - BACKUP ALERT! Beep interval: ");
      Serial.print(buzzerInterval);
      Serial.print("ms");
    } else if (muteEnabled && distance <= 10 && distance >= 2) {
      Serial.print(" - BACKUP ALERT! (MUTED)");
    }
    Serial.println();
  }
  
  // Update display every 2ms for smooth multiplexing
  if (currentMillis - previousDisplayMillis >= displayInterval) {
    previousDisplayMillis = currentMillis;
    display_number();
  }
  
  // Handle proximity buzzer with variable interval (only if not muted)
  if (currentMillis - previousBuzzerMillis >= buzzerInterval) {
    previousBuzzerMillis = currentMillis;
    handleCarBackupBuzzer();
  }
}

long measureDistance() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echo pulse duration
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  
  // Calculate distance in centimeters
  // Speed of sound = 343 m/s = 0.0343 cm/microsecond
  // Distance = (duration * 0.0343) / 2 (divide by 2 for round trip)
  long dist = duration * 0.0343 / 2;
  
  // Return 0 if no echo received (timeout)
  if (duration == 0) {
    return 0;
  }
  
  return dist;
}

void break_number(long num) {
  // Separate the number into 4 single digits
  digits[0] = num / 1000;                    // Thousands
  digits[1] = (num / 100) % 10;             // Hundreds  
  digits[2] = (num / 10) % 10;              // Tens
  digits[3] = num % 10;                     // Ones
}

void display_number() {
  // Multiplexing - display one digit at a time rapidly
  cathode_high(); // Turn off all digits
  
  digitalWrite(latch, LOW); // Prepare shift register for data
  shiftOut(data, clk, LSBFIRST, numbers[digits[count]]); // Send digit pattern
  digitalWrite(latch, HIGH); // Latch the data
  
  digitalWrite(CAS[count], LOW); // Turn on current digit
  
  count++; // Move to next digit
  if (count == 4) {
    count = 0; // Reset to first digit
  }
}

void cathode_high() {
  // Turn off all 4 digits
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);
}

void readMuteButton() {
  // Simple button reading - check for press (LOW = pressed)
  bool buttonPressed = (digitalRead(muteButtonPin) == LOW);
  
  // Check for button press (transition from not pressed to pressed)
  if (buttonPressed && !lastButtonState) {
    toggleMute();
  }
  
  // Update last button state
  lastButtonState = buttonPressed;
}

void toggleMute() {
  // Toggle mute state
  muteEnabled = !muteEnabled;
  
  // Turn off buzzer immediately when muting
  if (muteEnabled) {
    digitalWrite(buzzerPin, LOW);
    buzzerState = false;
  }
  
  // Simple feedback
  Serial.print("Mute: ");
  Serial.println(muteEnabled ? "ON" : "OFF");
}

void handleCarBackupBuzzer() {
  // Only activate buzzer if not muted
  if (muteEnabled) {
    // Turn off buzzer when muted
    buzzerState = false;
    digitalWrite(buzzerPin, LOW);
    return;
  }
  
  // Car backup sensor behavior: beep from 10cm to 2cm minimum distance
  if (distance <= 10 && distance >= 2) {
    // Calculate buzzer interval based on distance
    // Closer distance = faster beeping
    // 10cm = 800ms interval, 2cm = 100ms interval
    buzzerInterval = map(distance, 2, 10, 100, 800);
    
    // Ensure minimum beep interval
    if (buzzerInterval < 100) {
      buzzerInterval = 100;
    }
    
    // Toggle buzzer on/off for beeping effect
    buzzerState = !buzzerState;
    digitalWrite(buzzerPin, buzzerState);
  } else if (distance < 2 && distance > 0) {
    // Continuous buzzer when closer than 2cm (danger zone)
    digitalWrite(buzzerPin, HIGH);
    buzzerState = true;
  } else {
    // Turn off buzzer when distance > 10cm or no reading
    buzzerState = false;
    digitalWrite(buzzerPin, LOW);
    buzzerInterval = 200; // Reset to default interval
  }
}

void displayWorkshopHeader() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("=== Arduino IoT Workshop - Phase 3 ===");
  Serial.println("========================================");
  Serial.println("Proximity Alarm System Initialized");
  Serial.println();
  Serial.println("Hardware Configuration:");
  Serial.println("Pins 4-6: Shift Register Control");
  Serial.println("Pins 9-12: 4-Digit 7-Segment Display");
  Serial.println("Pin 2: Ultrasonic Trigger");
  Serial.println("Pin 3: Ultrasonic Echo");
  Serial.println("Pin 7: Active Buzzer");
  Serial.println("Pin 8: Push Button (Mute Toggle)");
  Serial.println("Pin 13: LED (Distance ≤5cm)");
  Serial.println();
  Serial.println("Features:");
  Serial.println("- Distance measurement every 100ms");
  Serial.println("- Display: 0-9999cm on 4-digit display");
  Serial.println("- LED: Lights up when distance ≤5cm");
  Serial.println("- Alarm: Activates at ≤10cm");
  Serial.println("- Beep Pattern: Faster as distance decreases");
  Serial.println("- Mute Function: Button toggles buzzer");
  Serial.println();
  Serial.println("Project Complete! All phases implemented.");
  Serial.println("========================================");
  Serial.println();
  Serial.println("Ultrasonic Distance Display with Car Backup Sensor");
  Serial.println("Distance shown in cm (0-9999)");
  Serial.println("LED lights up when distance ≤ 5cm");
  Serial.println("Buzzer will sound when distance ≤ 10cm");
  Serial.println("Beeping gets faster as distance decreases (minimum 2cm)");
  Serial.println("Press button on Pin 8 to mute/unmute buzzer");
  Serial.println("LED on Pin 13 lights up when distance ≤ 5cm");
}

/*
 * Common Issues and Solutions:
 * 
 * 1. Ultrasonic sensor not reading distance:
 *    - Check VCC (5V) and GND connections
 *    - Verify TRIG (Pin 2) and ECHO (Pin 3) connections
 *    - Ensure sensor is perpendicular to target
 *    - Check for obstacles blocking sensor
 * 
 * 2. Distance readings inconsistent:
 *    - Ensure stable power supply
 *    - Check for loose connections
 *    - Verify sensor mounting is secure
 *    - Test with known distances (e.g., ruler)
 * 
 * 3. Buzzer not working:
 *    - Check pin 7 connection and polarity
 *    - Verify buzzer is active type (not passive)
 *    - Test buzzer directly with 5V and GND
 *    - Check mute function (button toggle)
 * 
 * 4. Display showing wrong numbers or flickering:
 *    - Check shift register connections (pins 4-6)
 *    - Verify common cathode connections (pins 9-12)
 *    - Ensure all segment resistors are connected
 *    - Check power supply stability
 * 
 * 5. Button not toggling mute:
 *    - Check pin 8 connection and ground
 *    - Verify INPUT_PULLUP mode in code
 *    - Test button continuity
 *    - Ensure proper button press detection
 * 
 * 6. LED not lighting up:
 *    - Check pin 13 connection
 *    - Verify LED polarity (longer leg = positive)
 *    - Verify LED negative connected directly to ground rail
 *    - Check if distance is actually ≤5cm
 * 
 * 7. Alarm not activating at correct distance:
 *    - Verify buzzer threshold (≤10cm)
 *    - Check ultrasonic sensor calibration
 *    - Test with ruler at known distances
 *    - Ensure sensor is clean and unobstructed
 */