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
Pins 2-8: 7-segment display
Pin 10: Push button

New for Phase 3:
Pin 11: Ultrasonic Trigger (HC-SR04)
Pin 12: Ultrasonic Echo (HC-SR04)
Pin 13: Active Buzzer
```

## Wiring Instructions
1. **Keep Phase 2 wiring intact** (display and button)
2. **HC-SR04 Sensor**:
   - VCC → Breadboard 5V rail
   - GND → Breadboard ground rail
   - TRIG → Pin 11
   - ECHO → Pin 12
3. **Active Buzzer**:
   - Positive (+) → Pin 13
   - Negative (-) → Breadboard ground rail

## What This Does
- Continuously measures distance (0-9cm range)
- Displays distance on 7-segment (blank if >9cm)
- Activates buzzer alarm when ≤2cm
- Beeping speed increases as distance decreases:
  - 2cm: beep every 800ms
  - 1cm: beep every 400ms
  - 0cm: beep every 200ms
- Button toggles mute/unmute (display keeps working)
- Serial output shows distance, alarm status, mute state

## Expected Behavior
1. Display shows distance 0-9cm
2. Display blank when >9cm
3. Buzzer starts beeping at ≤2cm
4. Beeping gets faster closer to 0cm
5. Press button to mute/unmute buzzer
6. Distance display always active

## Wiring Diagram
See `diagram_phase3_proximity_alarm.png` or `docs/wiring-diagrams/phase3-proximity-alarm-wiring.png`

## Upload Instructions
1. Add HC-SR04 and buzzer to existing Phase 2 circuit
2. Open `phase3_proximity_alarm.ino` in Arduino IDE
3. Upload to Arduino
4. Open Serial Monitor (9600 baud)
5. Move hand near sensor to test

## Troubleshooting
- No distance readings? Check HC-SR04 VCC/GND and TRIG/ECHO pins
- Erratic readings? Sensor needs clear line of sight, avoid soft surfaces
- Buzzer not working? Verify polarity (+ to Pin 13, - to GND)
- Continuous beeping? Check mute state via Serial Monitor
- Display issues? Verify Phase 2 connections still intact
- Button not muting? Check Pin 10 connection

## Key Concepts
- Ultrasonic distance measurement
- Pulse timing with `pulseIn()`
- Dynamic timing intervals
- State management (muted/unmuted)
- Multi-sensor integration
- Real-time sensor feedback

## Workshop Complete!
You've built a complete sensor system with:
- ✅ Digital output (LED)
- ✅ Display control (7-segment)
- ✅ User input (button)
- ✅ Distance sensing (ultrasonic)
- ✅ Audio feedback (buzzer)
- ✅ State management (mute toggle)

## Further Exploration
- Extend range beyond 9cm with multi-digit display
- Add colored LEDs for different distance zones
- Implement different alarm patterns
- Add sound effects with tone() function
- Create parking assist game modes

