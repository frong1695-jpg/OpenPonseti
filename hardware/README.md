# OpenPonseti Hardware

This folder contains the hardware documentation for OpenPonseti.

OpenPonseti v0.1 uses a modular sensor architecture based on ESP32.

The final system is designed for a dual-foot Ponseti Foot Abduction Brace.

Current development may use single-side testing to validate individual sensors before the full dual-foot system is assembled.

---

## Hardware Architecture

The system is divided into three physical areas:

### Left Shoe

- 1 × FSR pressure sensor
- 1 × SHT31 temperature and humidity sensor
- 1 × Hall sensor

### Center Bar

- 1 × ESP32 development board
- 1 × MPU6050 IMU
- 1 × microSD card module using SPI

### Right Shoe

- 1 × FSR pressure sensor
- 1 × SHT31 temperature and humidity sensor
- 1 × Hall sensor

---

## Module Overview

| Module | Component | Quantity | Function |
|---|---|---:|---|
| Controller | ESP32 | 1 | Sensor reading, processing and Wi-Fi communication |
| Pressure Module | FSR | 2 | Heel contact and relative force sensing |
| Environment Module | SHT31 | 2 | Shoe temperature and humidity sensing |
| Motion Module | MPU6050 | 1 | Overall brace movement sensing |
| Connection Module | Hall Sensor | 2 | Shoe-to-bar connection detection |
| Storage Module | microSD SPI Module | 1 | Local CSV data storage |

---

## Current Prototype Hardware

The currently available hardware includes:

- ESP32
- 3 × FSR pressure sensors
- MPU6050
- SHT31 sensors
- Hall sensors
- microSD SPI module
- Breadboard
- Jumper wires
- Resistors

Only two FSR sensors are planned for the final v0.1 dual-foot system.

The third FSR is reserved for future testing or expansion.

---

## Hardware Development Strategy

The development process follows this order:

1. Test each sensor independently.
2. Test one-side sensor configuration.
3. Connect multiple sensors to one ESP32.
4. Add microSD data logging.
5. Add Wi-Fi data transmission.
6. Build the complete dual-foot prototype.

This approach reduces debugging complexity before full system integration.

---

## Current Sensor Placement

### FSR

Planned location:

Heel / heel cup area of each shoe.

Purpose:

- Detect heel contact
- Record relative force changes
- Support future wear-state analysis

### SHT31

Planned location:

Inside each shoe.

Purpose:

- Measure local temperature
- Measure relative humidity
- Record shoe microclimate

### Hall Sensor

Planned location:

At each shoe-to-bar connection.

Purpose:

- Detect connected state
- Detect disconnected state
- Record donning and doffing events

### MPU6050

Planned location:

Center of the brace bar.

Purpose:

- Measure overall brace acceleration
- Measure angular velocity
- Record brace movement

### microSD Module

Planned location:

Near the ESP32 in the central electronics enclosure.

Purpose:

- Store time-series sensor data
- Record CSV files
- Support offline data collection

---

## Important Notes

FSR readings are currently treated as raw ADC values.

They are not calibrated physical pressure measurements.

Temperature and humidity measurements are environmental measurements and are not measurements of human body temperature.

The hardware system is currently a research prototype and is not a certified medical device.
