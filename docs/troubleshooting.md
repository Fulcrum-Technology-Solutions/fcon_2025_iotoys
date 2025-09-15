# Troubleshooting Guide

## Common Hardware Connection Issues

### Serial Monitor Problems
- **Issue**: No serial output visible
- **Solution**: Check baud rate is set to 9600, verify USB connection, ensure correct COM port selected

### Component Not Working
- **Issue**: LED/Display/Sensor not responding
- **Solution**: Verify pin connections, check power supply (5V/GND), ensure proper resistor values

### Upload Errors
- **Issue**: Code won't upload to Arduino
- **Solution**: Check Arduino IDE settings, verify board selection (Arduino Uno), ensure no other programs using COM port

## Phase-Specific Troubleshooting

### Phase 0 - Power Test Issues
- **No serial output**: Check USB connection, verify COM port, ensure Arduino is powered
- **Power module not working**: Verify input voltage (7V-10V), check jumper settings, ensure 5V output enabled
- **Breadboard connections loose**: Ensure jumper wires fully inserted, check power rail continuity

### Phase 1 - LED Blink Issues
- **LED not lighting**: Check pin 13 connection, verify 220Ω resistor, check LED polarity (longer leg = positive)
- **LED always on/off**: Check pin 13 connection, verify ground connection, ensure code uploaded successfully
- **Irregular blinking**: Check power supply stability, verify all connections secure

### Phase 2 - Button + Buzzer Issues
- **Button not responding**: Verify pin 8 connection to ground, check INPUT_PULLUP configuration
- **LED not toggling**: Check pin 13 connection, verify LED polarity and resistor
- **Buzzer not working**: Check pin 7 connection and polarity, verify buzzer is active type
- **Multiple triggers**: Normal button behavior, ensure clean press and release

### Phase 3 - Proximity Alarm Issues
- **4-Digit display not working**: Check shift register connections (pins 4-6), verify common cathode connections (pins 9-12)
- **Display showing wrong numbers**: Check shift register wiring, verify multiplexing timing
- **Ultrasonic sensor not reading**: Check pins 2 (trigger) and 3 (echo), verify 5V power and ground
- **Distance readings inconsistent**: Ensure sensor perpendicular to target, check for obstacles
- **LED not lighting at ≤5cm**: Check pin 13 connection, verify distance threshold in code
- **Buzzer not working**: Check pin 7 connection, verify mute button (pin 8) functionality

## Error Message Explanations

### Phase 0 Messages
- **"System Status: READY"**: Power connectivity verified, system operational
- **No status messages**: Check serial connection, verify baud rate 9600

### Phase 1 Messages
- **"LED State: ON/OFF"**: Normal LED blink operation with timing
- **No LED state messages**: Check pin 13 connection, verify code upload

### Phase 2 Messages
- **"Button Pressed! LED: ON/OFF, Buzzer: BEEP"**: Normal button operation
- **No button response**: Check pin 8 connection, verify INPUT_PULLUP

### Phase 3 Messages
- **"Distance: XXX cm, Mute: ON/OFF, LED: ON/OFF"**: Normal sensor operation
- **"Distance: 0 cm"**: Ultrasonic sensor timeout or connection issue
- **"BACKUP ALERT! (MUTED)"**: Buzzer muted but alarm conditions detected
- **No distance readings**: Check pins 2-3 connections, verify sensor power

## Solutions and Workarounds

### Power Issues
- Use breadboard power rails for clean distribution
- Ensure all components share common ground
- Check 5V supply voltage with multimeter
- Verify power module jumper settings (5V output enabled)

### Connection Issues
- Use solid core wire for breadboard connections
- Verify all connections with continuity tester
- Double-check pin assignments against documentation
- Ensure shift register connections are secure (Phase 3)

### Code Issues
- Verify all pin numbers match hardware connections
- Check Serial Monitor for error messages
- Ensure proper function implementations
- Confirm folder name matches .ino file name (Arduino IDE requirement)

### Progressive Phase Testing
- Test each phase individually before moving to next
- Verify Phase 0 power connectivity before proceeding
- Confirm Phase 1 LED works before adding Phase 2 components
- Test Phase 2 button/buzzer before adding Phase 3 sensors
- Use Serial Monitor to verify each phase's expected output
