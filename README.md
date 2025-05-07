# Smart Heater Control System

This project simulates a smart heater controller using ESP32 on Wokwi. It includes:
- NTC temperature sensing
- Relay-based heater simulation
- State machine for 5 heating states
- FreeRTOS tasks for control and alerts
- BLE state advertising (mocked)

## Features
- ✅ States: Idle, Heating, Stabilizing, Target Reached, Overheat
- ✅ Relay and LED simulate heater ON/OFF
- ✅ Buzzer for overheat alerts
- ✅ Feedback LED for heating/stabilizing
- ✅ FreeRTOS used for real-time task scheduling
- ✅ BLE simulation via Serial logs

## Requirements
- Wokwi ESP32 Dev Board
- Arduino Framework

## Simulation
Run it on [Wokwi](https://wokwi.com/) using `diagram.json` and `sketch.ino`.

## System Design
See `Part1-SystemDesign.pdf` for block diagram, sensors, and roadmap.
