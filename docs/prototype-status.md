# OpenPonseti Prototype Status

## Current Prototype

**Version:** OpenPonseti v0.1  
**Prototype:** Single-foot integrated hardware prototype

The current prototype is used to validate the sensing and data architecture before building the final dual-foot system.

---

## Current Hardware Status

| Module | Status | Notes |
|---|---|---|
| ESP32 DevKit 32E | PASS | Main controller |
| FSR pressure sensor | PASS | Heel contact / raw ADC sensing |
| MPU6050 | PASS | Integrated with the main system |
| SHT31 | PASS | Temperature and humidity sensing |
| Hall sensor | PASS | Buckle / connection detection |
| Multiple sensors running together | PASS | Integrated on one ESP32 |
| microSD module interface | PASS | Module and code integrated |
| microSD card writing | NOT TESTED | No dedicated microSD card currently inserted |
| Wi-Fi connection | PASS | ESP32 successfully connects to a Wi-Fi network |
| Wi-Fi data transmission | NOT TESTED | Structured sensor data transmission is the next step |
| Dual-foot integration | NOT TESTED | Future development step |

---

## Current Single-Foot Wiring

| Component | ESP32 Pin |
|---|---|
| FSR | GPIO 34 |
| Hall Sensor DO | GPIO 27 |
| I2C SDA | GPIO 21 |
| I2C SCL | GPIO 22 |
| microSD SCK | GPIO 18 |
| microSD MISO | GPIO 19 |
| microSD MOSI | GPIO 23 |
| microSD CS | GPIO 5 |

The MPU6050 and SHT31 currently share the same I2C bus.

---

## Current Data

The integrated prototype currently collects:

- FSR raw ADC value
- Hall sensor state
- Temperature
- Relative humidity
- MPU6050 acceleration
- MPU6050 gyroscope data

The system uses a unified time-series data structure so that the same data can later be sent to:

- Serial Monitor
- microSD
- Wi-Fi
- Future dashboard software

---

## microSD Status

The microSD module is already integrated into the firmware.

The current firmware is designed so that the rest of the OpenPonseti system continues running even when an SD card is unavailable.

Actual CSV file writing and long-term storage have not yet been validated because no dedicated microSD card is currently being used.

---

## Current Development Stage

The current development sequence is:

1. Individual sensor testing — COMPLETE
2. Single-foot hardware integration — COMPLETE
3. Unified firmware integration — COMPLETE
4. Pin-map documentation — COMPLETE
5. Wi-Fi connection test — COMPLETE
6. Wi-Fi sensor data transmission — NEXT
7. microSD physical write test — PENDING
8. Dual-foot integration — PENDING
9. Dashboard development — FUTURE
