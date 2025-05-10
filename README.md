# ESP32 Heater Control System

This project implements a basic heater control system using an ESP32, DHT22 temperature sensor, and simulated heater (LED). It operates based on temperature thresholds and tracks system states such as Idle, Heating, Stabilizing, Target Reached, and Overheat. The system also includes BLE advertising, buzzer alerts, and FreeRTOS-based task handling.

## Features

- ✅ Temperature-based state machine (Idle, Heating, Stabilizing, Target Reached, Overheat)
- ✅ Automatic heater (LED) control
- ✅ Buzzer alert for Overheat state
- ✅ BLE advertising of system status
- ✅ Serial logging of temperature and state
- ✅ FreeRTOS-based periodic operations

## Hardware Requirements

- ESP32 development board  
- DHT22 temperature sensor  
- LED (simulated heater)  
- Buzzer (optional)  
- Resistors (if needed)  
- Breadboard + jumper wires  

### Pin Connections

| Component     | ESP32 Pin |
|---------------|-----------|
| DHT22 (Data)  | GPIO 15   |
| LED (Heater)  | GPIO 2    |
| Buzzer        | GPIO 4    |
| DHT22 (VCC)   | 3.3V      |
| DHT22 (GND)   | GND       |

## State Machine Overview

| Temperature | System State      | Heater | Buzzer |
|-------------|-------------------|--------|--------|
| < 28°C      | Heating            | ON     | OFF    |
| 28–29.5°C   | Stabilizing        | ON     | OFF    |
| 29.5–30.5°C | Target Reached     | OFF    | OFF    |
| > 30.5°C    | Overheat           | OFF    | ON     |
| Sensor Fail | Idle               | OFF    | OFF    |

## 🧪 Simulation

You can simulate this project using [Wokwi](https://wokwi.com/).  
🔗 **Simulation Link**: https://wokwi.com/projects/430532436168855553
