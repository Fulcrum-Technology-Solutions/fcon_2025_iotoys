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
- **LED not lighting**: Check Pin 13 connection, verify LED negative connected to ground rail, check LED polarity (longer leg = positive)
- **LED always on/off**: Check Pin 13 connection, verify ground connection, ensure code uploaded successfully
- **Irregular blinking**: Check power supply stability, verify all connections secure
- **LED very dim**: Normal for direct connection without resistor; verify connections

### Phase 2 - Button + Buzzer Issues
- **Button not responding**: Verify Pin 8 connection to ground, check INPUT_PULLUP configuration
- **LED not toggling**: Check Pin 13 connection, verify LED polarity and ground connection
- **Buzzer not working**: Check Pin 7 connection and polarity, verify buzzer is active type (not passive)
- **Buzzer too quiet**: Normal for small active buzzers; ensure good connections
- **Multiple triggers**: Normal button bounce behavior, not a bug

### Phase 3 - Proximity Alarm Issues
- **4-Digit display not working**: Check shift register connections (pins 4-6), verify common cathode connections (pins 9-12)
- **Display showing wrong numbers**: Check shift register wiring, verify multiplexing timing, ensure proper digit order
- **Display flickering**: Normal for multiplexed displays; ensure stable power and good ground connections
- **Ultrasonic sensor not reading**: Check Pin 2 (trigger) and Pin 3 (echo), verify 5V power and ground
- **Distance readings inconsistent**: Ensure sensor perpendicular to target, avoid soft/angled surfaces, check for obstacles
- **Sensor reads 0 constantly**: Check echo pin connection, verify 5V power to sensor
- **LED not lighting at ≤5cm**: Check Pin 13 connection, verify distance threshold and actual measured distance
- **Buzzer not working at ≤10cm**: Check Pin 7 connection, verify mute state (press button on Pin 8 to toggle)
- **Buzzer always on**: Check mute state, verify distance readings in Serial Monitor

## Error Message Explanations

### Phase 0 Messages
- **"System Status: READY"**: Power connectivity verified, system operational
- **No status messages**: Check serial connection, verify baud rate 9600

### Phase 1 Messages
- **"LED State: ON/OFF"**: Normal LED blink operation with timing
- **No LED state messages**: Check pin 13 connection, verify code upload

### Phase 2 Messages
- **"Button Pressed! LED: ON, Buzzer: BEEP"**: Normal button press, LED toggled on
- **"Button Pressed! LED: OFF, Buzzer: BEEP"**: Normal button press, LED toggled off
- **No button response**: Check Pin 8 connection, verify INPUT_PULLUP, check ground connection

### Phase 3 Messages
- **"Distance: XXX cm, Mute: OFF, LED: ON/OFF"**: Normal sensor operation
- **"Distance: XXX cm - BACKUP ALERT! Beep interval: XXXms"**: Buzzer active, shows beep timing
- **"Distance: 0 cm"**: Ultrasonic sensor timeout or no echo received (check connections)
- **"BACKUP ALERT! (MUTED)"**: Buzzer muted but alarm distance detected (≤10cm)
- **No distance readings**: Check Pin 2/3 connections, verify sensor VCC and ground

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
