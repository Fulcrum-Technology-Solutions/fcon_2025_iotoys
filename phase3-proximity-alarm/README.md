# Phase 3: Proximity Alarm System

## Overview
Build a car-like proximity alarm that measures distance with an ultrasonic sensor, displays it on the 7-segment display, and beeps faster as objects get closer.

## Components Required
- Arduino Uno R3 (with all Phase 2 components still connected)
- HC-SR04 ultrasonic distance sensor
- Active buzzer
- Additional jumper wires

## Pin Assignments
```
From Phase 2 (keep connected):
Pin 7: Active Buzzer
Pin 8: Push Button
Pin 13: Red LED

New for Phase 3:
Pin 2: Ultrasonic Trigger (HC-SR04)
Pin 3: Ultrasonic Echo (HC-SR04)
Pins 4-6: 7-Segment Shift Register (data, latch, clock)
Pins 9-12: 7-Segment Common Cathodes (digits 1-4)
```

## Wiring Instructions
1. **Keep Phase 2 wiring intact** (LED, button, and buzzer)
2. **4-Digit 7-Segment Display**:
   - Shift register data → Pin 4
   - Shift register latch → Pin 5
   - Shift register clock → Pin 6
   - Common cathodes → Pins 9-12 (one per digit)
   - Segment connections via 220Ω resistors
3. **HC-SR04 Sensor**:
   - VCC → Breadboard 5V rail
   - GND → Breadboard ground rail
   - TRIG → Pin 2
   - ECHO → Pin 3

## What This Does
- Continuously measures distance (0-9999cm range)
- Displays distance on 4-digit 7-segment display
- LED lights up when distance ≤5cm
- Activates buzzer alarm when distance ≤10cm (car-like backup sensor)
- Beeping speed increases as distance decreases:
  - 10cm: beep every 800ms
  - 5cm: beep every ~400ms
  - 2cm: beep every 100ms
  - <2cm: solid beep (continuous)
- Button toggles mute/unmute (display and LED keep working)
- Serial output shows distance, alarm status, mute state, and beep interval

## Expected Behavior
1. Display shows distance 0-9999cm on 4 digits
2. LED lights up when distance ≤5cm
3. Buzzer starts beeping at ≤10cm
4. Beeping gets progressively faster as distance decreases
5. Solid beep when distance <2cm (danger zone)
6. Press button to mute/unmute buzzer
7. Distance display and LED always active

## Wiring Diagram
See `diagram_phase3_proximity_alarm.png` or `docs/wiring-diagrams/phase3-proximity-alarm-wiring.png`

## Upload Instructions
1. Add 4-digit display and HC-SR04 to existing Phase 2 circuit
2. Open `phase3_proximity_alarm.ino` in Arduino IDE
3. Upload to Arduino
4. Open Serial Monitor (9600 baud)
5. Move hand near sensor to test distance readings

## Troubleshooting
- No distance readings? Check HC-SR04 VCC/GND and Pin 2/3 connections
- Erratic readings? Sensor needs clear line of sight, avoid soft surfaces
- Buzzer not working? Verify Pin 7 connection and check mute state
- Display not showing numbers? Check shift register pins 4-6 and common cathodes 9-12
- LED not lighting? Check Pin 13 and verify distance is ≤5cm
- Button not muting? Check Pin 8 connection
- Display flickering? Normal for multiplexed displays, ensure proper ground connections

## Key Concepts
- Ultrasonic distance measurement with HC-SR04
- Pulse timing with `pulseIn()`
- Shift register control for 7-segment displays
- Display multiplexing for multi-digit displays
- Dynamic timing intervals (variable beep rates)
- State management (muted/unmuted)
- Multi-sensor integration
- Real-time sensor feedback

## Workshop Complete!
You've built a complete sensor system with:
- ✅ Digital output (LED)
- ✅ Multi-digit display control (4-digit 7-segment with shift registers)
- ✅ User input (button with debouncing)
- ✅ Distance sensing (ultrasonic HC-SR04)
- ✅ Audio feedback (buzzer with dynamic intervals)
- ✅ State management (mute toggle)
- ✅ Display multiplexing for smooth output
- ✅ Real-time distance monitoring

## Further Exploration
- Add colored LEDs for different distance zones (green/yellow/red)
- Implement different alarm patterns or melodies
- Add temperature/humidity sensors
- Create parking assist game modes
- Display distance in inches or feet
- Add maximum/minimum distance tracking

