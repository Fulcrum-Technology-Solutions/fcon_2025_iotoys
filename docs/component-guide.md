# Component Guide

## Component Specifications and Ratings

### Arduino Uno R3
- **Microcontroller**: ATmega328P
- **Operating Voltage**: 5V
- **Input Voltage**: 7-12V (recommended)
- **Digital I/O Pins**: 14 (6 PWM)
- **Analog Input Pins**: 6
- **Flash Memory**: 32KB
- **SRAM**: 2KB
- **EEPROM**: 1KB
- **Clock Speed**: 16MHz

### Green LED (Phase 1-3)
- **Type**: Standard 5mm LED
- **Forward Voltage**: 2.1V (typical)
- **Forward Current**: 20mA (typical)
- **Connection**: Direct connection (no resistor needed)
- **Pin Assignment**: Pin 13 (positive), Ground rail (negative)

### Active Buzzer (Phase 2-3)
- **Operating Voltage**: 5V DC
- **Operating Current**: 30mA (typical)
- **Sound Output**: 85dB @ 10cm
- **Frequency**: 2.5kHz ± 500Hz
- **Type**: Piezoelectric
- **Pin Assignment**: Pin 7 (Phase 2-3)

### Push Button (Phase 2-3)
- **Type**: Momentary SPST (Single Pole Single Throw)
- **Operating Voltage**: 5V
- **Contact Rating**: 50mA @ 12V DC
- **Configuration**: INPUT_PULLUP (internal resistor)
- **Pin Assignment**: Pin 8 (Phase 2-3)

### HC-SR04 Ultrasonic Sensor (Phase 3)
- **Operating Voltage**: 5V DC
- **Operating Current**: 15mA
- **Measuring Range**: 2cm - 400cm
- **Measuring Angle**: 15 degrees
- **Resolution**: 0.3cm
- **Trigger Pulse**: 10μs minimum
- **Echo Pulse**: Up to 38ms
- **Pin Assignment**: Pin 2 (Trigger), Pin 3 (Echo)

### 4-Digit 7-Segment Display (Phase 3)
- **Type**: Common Cathode with Shift Register
- **Operating Voltage**: 2.1V per segment
- **Forward Current**: 10-20mA per segment
- **Pin Configuration**: Shift register control + 4 common cathodes
- **Resistor Required**: 220Ω per segment
- **Pin Assignment**: Pin 4 (Data), Pin 5 (Latch), Pin 6 (Clock), Pins 9-12 (Common Cathodes)

### Breadboard Power Supply Module (All Phases)
- **Input Voltage**: 7V - 10V DC
- **Output Voltage**: 5V and 3.3V (selectable)
- **Output Current**: 1A maximum
- **Features**: LED indicators, jumper selection
- **Purpose**: Clean power distribution to breadboard rails

### Breadboard
- **Type**: 0-60 columns, A-J rows
- **Power Rails**: 2 (top/bottom)
- **Connection Points**: 830 total
- **Voltage Rating**: 300V max
- **Current Rating**: 1A per connection

### Resistors (220Ω)
- **Type**: Carbon Film
- **Power Rating**: 1/4W
- **Tolerance**: ±5%
- **Color Code**: Red-Red-Brown-Gold OR Red-Red-Black-Black-Brown

## Pinout Diagrams and Connections

### Arduino Uno Pinout
```
                    ┌─────────┐
              Reset │ 1    30 │ GND
              (3V3) │ 2    29 │ AREF
               (5V) │ 3    28 │ A0
              (GND) │ 4    27 │ A1
              (GND) │ 5    26 │ A2
              (VIN) │ 6    25 │ A3
                D13 │ 7    24 │ A4
                D12 │ 8    23 │ A5
                D11 │ 9    22 │ A6
                D10 │ 10   21 │ A7
                 D9 │ 11   20 │ D8
                 D8 │ 12   19 │ D7
                 D7 │ 13   18 │ D6
                 D6 │ 14   17 │ D5
                 D5 │ 15   16 │ D4
                    └─────────┘
```

### 4-Digit 7-Segment Display Pinout (Common Cathode with Shift Register)
```
        ┌───┐
        │ A │
    ┌───┼───┼───┐
    │ F │   │ B │
    └───┼───┼───┘
        │ G │
    ┌───┼───┼───┐
    │ E │   │ C │
    └───┼───┼───┘
        │ D │
        └───┘

Shift Register Connections:
Pin 4: Data Input
Pin 5: Latch (Clock Enable)
Pin 6: Clock Input
Pins 9-12: Common Cathodes (Digit 1-4)
```

### HC-SR04 Ultrasonic Sensor Pinout
```
    ┌─────────────┐
    │ VCC  GND    │
    │ TRIG ECHO   │
    └─────────────┘
```

## Power Requirements

### Total Current Consumption by Phase
- **Phase 0**: ~15mA (Arduino Uno only - power test)
- **Phase 1**: ~20mA (LED only)
- **Phase 2**: ~50mA (LED + buzzer + button)
- **Phase 3**: ~200mA (4-digit display + sensor + buzzer + LED + button)

### Power Distribution
- **Arduino 5V**: Powers all components
- **Arduino GND**: Common ground for all components
- **Breadboard Rails**: Clean power distribution
- **USB Power**: Sufficient for all phases

## Safety Considerations

### Electrical Safety
- Never exceed 5V on digital pins
- Use appropriate resistors to limit current
- Avoid short circuits between power rails
- Disconnect power when making connections

### Component Protection
- Handle components by edges only
- Avoid static discharge (use anti-static mat if available)
- Don't force connections - gentle pressure only
- Keep components away from moisture

### Workspace Safety
- Work in well-lit area
- Keep workspace clean and organized
- Use proper tools for connections
- Have first aid kit available

## Component Testing Procedures

### LED Testing
1. Connect LED directly to 5V and GND (longer leg = positive)
2. Verify forward polarity (longer leg = positive)
3. Check brightness and color

### 4-Digit 7-Segment Display Testing (Phase 3)
1. Connect shift register data, latch, and clock pins
2. Connect common cathodes (pins 9-12) to GND
3. Test shift register with known patterns
4. Verify multiplexing displays all 4 digits
5. Test all digits 0-9 on each position

### Button Testing
1. Connect one terminal to digital pin
2. Connect other terminal to GND
3. Use INPUT_PULLUP mode
4. Verify LOW when pressed, HIGH when released

### Ultrasonic Sensor Testing
1. Connect VCC to 5V, GND to GND
2. Connect TRIG and ECHO to digital pins
3. Send 10μs trigger pulse
4. Measure echo pulse duration
5. Verify distance calculations

### Buzzer Testing
1. Connect positive terminal to digital pin
2. Connect negative terminal to GND
3. Send HIGH signal to activate
4. Verify audible sound output
