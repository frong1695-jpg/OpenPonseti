# OpenPonseti Hardware Pin Map

## Current Tested Prototype

**Controller:** ESP32 DevKit 32E  
**Prototype:** Single-foot integrated prototype

This document records the pin assignment that has been physically connected and successfully tested in the current OpenPonseti prototype.

The final OpenPonseti system will use two shoes, but the current hardware prototype uses one side for integration testing.

---

## Tested Pin Assignment

| Component | Module Pin | ESP32 Pin | Interface | Status |
|---|---|---|---|---|
| FSR | Signal | GPIO 34 | ADC1 | Tested |
| Hall Sensor | DO | GPIO 27 | Digital Input | Tested |
| MPU6050 | SDA | GPIO 21 | I2C | Tested |
| MPU6050 | SCL | GPIO 22 | I2C | Tested |
| SHT31 | SDA | GPIO 21 | I2C | Tested |
| SHT31 | SCL | GPIO 22 | I2C | Tested |
| microSD | SCK | GPIO 18 | SPI | Module integrated |
| microSD | MISO | GPIO 19 | SPI | Module integrated |
| microSD | MOSI | GPIO 23 | SPI | Module integrated |
| microSD | CS | GPIO 5 | SPI | Module integrated |

---

## I2C Bus

The current prototype uses one I2C bus:

- SDA: GPIO 21
- SCL: GPIO 22

Connected devices:

- MPU6050
- SHT31

The two devices can share the same I2C bus because they use different I2C addresses.

Current addresses:

- MPU6050: 0x68
- SHT31: typically 0x44 or 0x45

---

## FSR

The FSR is connected to:

- GPIO 34

GPIO 34 is an ADC1 input.

ADC1 is preferred because the final OpenPonseti system will also use Wi-Fi.

FSR values are currently treated as raw ADC readings.

They are not calibrated physical pressure values.

---

## Hall Sensor

The Hall sensor uses the digital output:

- DO → GPIO 27

The analog output is not currently used.

Current tested behavior:

- Magnet close: 1
- Magnet far: 0

This sensor is intended to detect the connection state between the shoe and brace bar.

---

## microSD Module

The microSD SPI interface currently uses:

- SCK → GPIO 18
- MISO → GPIO 19
- MOSI → GPIO 23
- CS → GPIO 5

The microSD module has been integrated into the firmware.

Actual SD card file writing has not yet been tested because no dedicated microSD card is currently inserted.

---

## Current Prototype Status

The following components have successfully operated together on one ESP32:

- FSR
- Hall sensor
- MPU6050
- SHT31

The microSD interface is included in the same firmware.

Wi-Fi transmission has not yet been tested.

---

## Future Dual-Foot Expansion

The final OpenPonseti system is intended to include:

- 2 × FSR
- 2 × SHT31
- 2 × Hall sensors
- 1 × MPU6050
- 1 × ESP32
- 1 × microSD module

The exact GPIO assignment for the second shoe will be documented after the dual-foot hardware configuration has been physically tested.

Untested future pin assignments should not be treated as final.
