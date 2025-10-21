# Phase 0: Power Test

## Overview
Initial power validation and Arduino setup verification. This phase ensures your hardware is properly configured before starting the workshop.

## Components Required
- Arduino Uno R3
- USB cable
- Breadboard (60 columns)
- Power module (5V/3.3V)
- Jumper wires

## Power Configuration
```
Arduino 5V  → Breadboard top rail (red/+)
Arduino GND → Breadboard bottom rail (blue/-)
```

## What This Does
- Tests USB power connection
- Validates breadboard power distribution
- Confirms Serial Monitor communication
- Verifies Arduino IDE setup

## Expected Behavior
- Serial Monitor shows power test messages
- Confirms voltage levels
- Validates all connections

## Wiring Diagram
See `diagram_phase0_power_testpng` or `docs/wiring-diagrams/phase0-power-wiring.png`

## Upload Instructions
1. Connect Arduino via USB
2. Open `phase0_power_test.ino` in Arduino IDE
3. Select **Board**: Arduino Uno
4. Select **Port**: (your Arduino's port)
5. Click Upload
6. Open Serial Monitor (9600 baud)

## Troubleshooting
- No serial output? Check baud rate is 9600
- Upload fails? Verify correct board/port selected
- Power issues? Check breadboard rail connections

## Next Phase
**Phase 1**: LED Blink System - Add a blinking LED to learn basic output control

