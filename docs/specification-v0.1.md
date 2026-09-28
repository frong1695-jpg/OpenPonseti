# OpenPonseti v0.1 Specification

## 1. Project Overview

**Project Name:** OpenPonseti  
**Version:** v0.1  
**Project Type:** Open-source Ponseti Brace Monitoring Research Prototype

OpenPonseti is an open-source modular monitoring system for Ponseti Foot Abduction Braces.

The project adds a low-cost sensing and data-logging system to an existing Ponseti brace. The goal is to make everyday brace use more observable and recordable.

The final system is designed for both feet. Current single-foot testing is only used to validate sensors and electronics before building the complete dual-foot system.

---

## 2. Main Research Question

**How can a low-cost, open-source sensing system make everyday Ponseti brace use more observable, recordable and understandable?**

OpenPonseti v0.1 focuses on data collection rather than clinical diagnosis or treatment decisions.

---

## 3. What the System Measures

OpenPonseti v0.1 collects data related to:

- Heel contact and relative force
- Shoe-to-bar connection state
- Shoe temperature
- Shoe humidity
- Overall brace movement
- Wearing-related time data

The system does not yet determine whether the brace is being worn in a clinically correct or effective way.

---

## 4. Final Hardware Architecture

### Left Shoe

- 1 × FSR pressure sensor at the heel
- 1 × SHT31 temperature and humidity sensor
- 1 × Hall sensor for shoe-to-bar connection detection

### Center Bar

- 1 × ESP32
- 1 × MPU6050 IMU
- 1 × microSD SPI module

### Right Shoe

- 1 × FSR pressure sensor at the heel
- 1 × SHT31 temperature and humidity sensor
- 1 × Hall sensor for shoe-to-bar connection detection

---

## 5. Sensor Functions

### FSR

Two FSR sensors are used, one for each heel.

They collect raw ADC values related to heel contact and relative force.

Current experimental observations include:

- No pressure: approximately 0
- Light finger pressure: approximately 1500
- Higher finger pressure: approximately 3000

These values are raw prototype readings and are not calibrated physical pressure measurements or medical safety thresholds.

### SHT31

Two SHT31 sensors are used, one in each shoe.

They measure:

- Temperature
- Relative humidity

The purpose is to observe the microclimate inside the brace shoes.

### Hall Sensors

Two Hall sensors are used, one at each shoe-to-bar connection.

They are used to detect:

- Connected state
- Disconnected state
- Donning events
- Doffing events

### MPU6050

One MPU6050 is mounted at the center of the bar.

It measures:

- Acceleration X, Y, Z
- Gyroscope X, Y, Z

The MPU6050 measures movement of the overall brace system rather than the independent movement of each foot.

### microSD

The microSD module is a required component.

It stores time-series sensor data locally in CSV format.

---

## 6. Communication

OpenPonseti v0.1 supports:

- Serial output for debugging
- microSD storage for long-term recording
- Wi-Fi for structured data transmission

BLE is not required in v0.1.

The final dashboard, mobile app, cloud platform and API are not yet defined.

---

## 7. Data Structure

A v0.1 data record should contain:

- timestamp
- left_pressure_raw
- right_pressure_raw
- left_temperature_c
- right_temperature_c
- left_humidity_rh
- right_humidity_rh
- left_hall_raw
- right_hall_raw
- accel_x
- accel_y
- accel_z
- gyro_x
- gyro_y
- gyro_z

Derived values such as wear time or connected state should remain separate from the original raw sensor data.

---

## 8. Sampling Modes

### Debug Mode

Recommended sampling interval:

100–500 ms

This mode is used for sensor testing and hardware debugging.

### Long-term Logging Mode

Initial recording interval:

5–60 seconds

The final interval will be determined through later prototype testing.

---

## 9. v0.1 Success Criteria

OpenPonseti v0.1 is complete when:

1. ESP32 operates reliably.
2. Both FSR sensors can be read.
3. Both SHT31 sensors can be read.
4. Both Hall sensors can detect connection changes.
5. MPU6050 outputs motion data.
6. All sensors can operate together in one program.
7. Sensor data follows one unified format.
8. microSD can store CSV data.
9. Serial Monitor can display debug data.
10. ESP32 can transmit structured data through Wi-Fi.
11. Hardware and wiring instructions are documented.
12. Firmware is published in the repository.

---

## 10. Not Included in v0.1

OpenPonseti v0.1 does not include:

- Abduction angle measurement
- Dorsiflexion angle measurement
- Heart rate
- Body temperature
- SpO2
- AI diagnosis
- Relapse prediction
- Medical pressure thresholds
- Clinical determination of correct brace wearing
- Automatic brace adjustment

These may be explored in future versions if there is sufficient technical and clinical evidence.

---

## 11. Target Users

OpenPonseti is open to different users, but the primary target groups are:

- Doctors and healthcare organizations
- Hospitals and NGOs
- Researchers
- Designers
- Engineering and design students
- Makers
- Open-source developers

Children and caregivers are important users and stakeholders of the final system, but the project does not assume that parents need to build or program the electronics themselves.

---

## 12. Project Principle

OpenPonseti aims to make the system reproducible and extendable.

The project will gradually publish:

- Hardware architecture
- Bill of Materials
- Wiring diagrams
- ESP32 firmware
- Data format
- Sensor testing procedures
- Known limitations
- Future development modules

The intended open-source workflow is:

Understand → Build → Test → Collect Data → Modify → Contribute
