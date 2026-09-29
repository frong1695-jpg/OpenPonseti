#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>

#include "secrets.h"

// =====================================================
// OpenPonseti v0.1
// Single-Foot Integrated Prototype
//
// Sensors:
// - FSR pressure/contact sensor
// - MPU6050 IMU
// - SHT3X temperature/humidity sensor
// - Hall sensor for buckle detection
// - microSD module
//
// IMPORTANT:
// This is a research prototype.
// Sensor thresholds are NOT clinical thresholds.
// =====================================================


// =====================================================
// PIN SETTINGS
// Keep current tested wiring
// =====================================================

const int FSR_PIN  = 34;
const int HALL_PIN = 27;

// I2C
const int SDA_PIN = 21;
const int SCL_PIN = 22;

// microSD SPI
const int SD_CS   = 5;
const int SD_SCK  = 18;
const int SD_MISO = 19;
const int SD_MOSI = 23;


// =====================================================
// I2C ADDRESSES
// =====================================================

const uint8_t MPU6050_ADDR = 0x68;

uint8_t sht3xAddr = 0x44;


// =====================================================
// SYSTEM STATUS
// =====================================================

bool mpuReady = false;
bool shtReady = false;
bool sdReady  = false;


// =====================================================
// TIMING
// =====================================================

// Debug / serial output interval
const unsigned long SAMPLE_INTERVAL = 500;

// SD logging interval
const unsigned long LOG_INTERVAL = 1000;

unsigned long lastSampleTime = 0;
unsigned long lastLogTime    = 0;


// =====================================================
// SENSOR RECORD
// One unified data structure
// =====================================================

struct SensorRecord {

  unsigned long timestamp_ms;

  int fsr_raw;
  int hall_raw;

  float temperature_c;
  float humidity_percent;

  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;

  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;

  bool mpu_ok;
  bool sht_ok;
};


// =====================================================
// I2C DEVICE CHECK
// =====================================================

bool i2cDeviceExists(uint8_t address) {

  Wire.beginTransmission(address);

  return Wire.endTransmission() == 0;
}


// =====================================================
// READ MPU6050
//
// Read:
// accelerometer
// internal temperature registers (ignored)
// gyroscope
// =====================================================

bool readMPU6050(
  int16_t &accX,
  int16_t &accY,
  int16_t &accZ,
  int16_t &gyroX,
  int16_t &gyroY,
  int16_t &gyroZ
) {

  Wire.beginTransmission(MPU6050_ADDR);

  // Start at ACCEL_XOUT_H
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  // 14 bytes:
  // Acc X/Y/Z = 6
  // Temp       = 2
  // Gyro X/Y/Z= 6
  Wire.requestFrom(
    MPU6050_ADDR,
    (uint8_t)14,
    (uint8_t)true
  );

  if (Wire.available() < 14) {
    return false;
  }

  accX =
    ((int16_t)Wire.read() << 8) |
    Wire.read();

  accY =
    ((int16_t)Wire.read() << 8) |
    Wire.read();

  accZ =
    ((int16_t)Wire.read() << 8) |
    Wire.read();

  // Skip MPU6050 internal temperature
  Wire.read();
  Wire.read();

  gyroX =
    ((int16_t)Wire.read() << 8) |
    Wire.read();

  gyroY =
    ((int16_t)Wire.read() << 8) |
    Wire.read();

  gyroZ =
    ((int16_t)Wire.read() << 8) |
    Wire.read();

  return true;
}


// =====================================================
// READ SHT3X
// =====================================================

bool readSHT3X(
  float &temperature,
  float &humidity
) {

  Wire.beginTransmission(sht3xAddr);

  // High repeatability measurement
  Wire.write(0x24);
  Wire.write(0x00);

  if (Wire.endTransmission() != 0) {
    return false;
  }

  delay(20);

  Wire.requestFrom(
    sht3xAddr,
    (uint8_t)6
  );

  if (Wire.available() < 6) {
    return false;
  }

  uint16_t rawTemperature =
    ((uint16_t)Wire.read() << 8) |
    Wire.read();

  // CRC byte
  Wire.read();

  uint16_t rawHumidity =
    ((uint16_t)Wire.read() << 8) |
    Wire.read();

  // CRC byte
  Wire.read();

  temperature =
    -45.0 +
    175.0 *
    ((float)rawTemperature / 65535.0);

  humidity =
    100.0 *
    ((float)rawHumidity / 65535.0);

  return true;
}


// =====================================================
// FSR PROTOTYPE STATUS
// NOT CLINICALLY VALIDATED
// =====================================================

const char* getFSRStatus(int fsrValue) {

  if (fsrValue < 300) {
    return "NO_CONTACT";
  }

  else if (fsrValue < 2200) {
    return "CONTACT";
  }

  else {
    return "HIGH_CONTACT_SIGNAL";
  }
}


// =====================================================
// BUCKLE STATUS
//
// Current tested Hall behavior:
// magnet close = 1
// magnet far   = 0
// =====================================================

const char* getBuckleStatus(int hallState) {

  if (hallState == 1) {
    return "BUCKLE_CLOSED";
  }

  return "BUCKLE_OPEN";
}


// =====================================================
// INITIALIZE SD
// =====================================================

void initializeSD() {

  Serial.println();
  Serial.println("Checking SD card...");

  SPI.begin(
    SD_SCK,
    SD_MISO,
    SD_MOSI,
    SD_CS
  );

  // Lower SPI speed for prototype/breadboard stability
  if (!SD.begin(SD_CS, SPI, 1000000)) {

    Serial.println("SD unavailable.");
    Serial.println(
      "System will continue without SD logging."
    );

    sdReady = false;
    return;
  }

  if (SD.cardType() == CARD_NONE) {

    Serial.println("No SD card detected.");
    Serial.println(
      "System will continue without SD logging."
    );

    sdReady = false;
    return;
  }

  sdReady = true;

  Serial.println("SD CARD READY");

  Serial.print("Card size: ");

  Serial.print(
    SD.cardSize() /
    (1024 * 1024)
  );

  Serial.println(" MB");


  // Create CSV if it does not exist

  if (!SD.exists("/openponseti.csv")) {

    File file =
      SD.open(
        "/openponseti.csv",
        FILE_WRITE
      );

    if (file) {

      file.println(
        "timestamp_ms,"
        "fsr_raw,"
        "fsr_status,"
        "hall_raw,"
        "buckle_status,"
        "temperature_c,"
        "humidity_percent,"
        "acc_x_raw,"
        "acc_y_raw,"
        "acc_z_raw,"
        "gyro_x_raw,"
        "gyro_y_raw,"
        "gyro_z_raw"
      );

      file.close();

      Serial.println(
        "Created openponseti.csv"
      );
    }
  }
}


// =====================================================
// READ ALL SENSORS
// =====================================================

SensorRecord readSensors() {

  SensorRecord record;

  record.timestamp_ms = millis();

  // FSR
  record.fsr_raw =
    analogRead(FSR_PIN);

  // Hall
  record.hall_raw =
    digitalRead(HALL_PIN);

  // MPU6050
  record.mpu_ok = false;

  record.acc_x  = 0;
  record.acc_y  = 0;
  record.acc_z  = 0;

  record.gyro_x = 0;
  record.gyro_y = 0;
  record.gyro_z = 0;

  if (mpuReady) {

    record.mpu_ok =
      readMPU6050(
        record.acc_x,
        record.acc_y,
        record.acc_z,
        record.gyro_x,
        record.gyro_y,
        record.gyro_z
      );
  }


  // SHT3X
  record.sht_ok = false;

  record.temperature_c = 0.0;
  record.humidity_percent = 0.0;

  if (shtReady) {

    record.sht_ok =
      readSHT3X(
        record.temperature_c,
        record.humidity_percent
      );
  }

  return record;
}


// =====================================================
// SERIAL CSV OUTPUT
// =====================================================

void printCSVRecord(
  const SensorRecord &r
) {

  Serial.print(r.timestamp_ms);
  Serial.print(",");

  Serial.print(r.fsr_raw);
  Serial.print(",");

  Serial.print(
    getFSRStatus(r.fsr_raw)
  );
  Serial.print(",");

  Serial.print(r.hall_raw);
  Serial.print(",");

  Serial.print(
    getBuckleStatus(r.hall_raw)
  );
  Serial.print(",");


  if (r.sht_ok) {

    Serial.print(
      r.temperature_c,
      2
    );

    Serial.print(",");

    Serial.print(
      r.humidity_percent,
      2
    );
  }

  else {

    Serial.print("ERROR,ERROR");
  }

  Serial.print(",");


  if (r.mpu_ok) {

    Serial.print(r.acc_x);
    Serial.print(",");

    Serial.print(r.acc_y);
    Serial.print(",");

    Serial.print(r.acc_z);
    Serial.print(",");

    Serial.print(r.gyro_x);
    Serial.print(",");

    Serial.print(r.gyro_y);
    Serial.print(",");

    Serial.print(r.gyro_z);
  }

  else {

    Serial.print(
      "ERROR,ERROR,ERROR,"
      "ERROR,ERROR,ERROR"
    );
  }

  Serial.println();
}


// =====================================================
// SD CSV OUTPUT
// =====================================================

void saveCSVRecord(
  const SensorRecord &r
) {

  if (!sdReady) {
    return;
  }

  File file =
    SD.open(
      "/openponseti.csv",
      FILE_APPEND
    );

  if (!file) {

    Serial.println(
      "# WARNING: SD write failed"
    );

    return;
  }

  file.print(r.timestamp_ms);
  file.print(",");

  file.print(r.fsr_raw);
  file.print(",");

  file.print(
    getFSRStatus(r.fsr_raw)
  );
  file.print(",");

  file.print(r.hall_raw);
  file.print(",");

  file.print(
    getBuckleStatus(r.hall_raw)
  );
  file.print(",");


  if (r.sht_ok) {

    file.print(
      r.temperature_c,
      2
    );

    file.print(",");

    file.print(
      r.humidity_percent,
      2
    );
  }

  else {

    file.print(
      "ERROR,ERROR"
    );
  }

  file.print(",");


  if (r.mpu_ok) {

    file.print(r.acc_x);
    file.print(",");

    file.print(r.acc_y);
    file.print(",");

    file.print(r.acc_z);
    file.print(",");

    file.print(r.gyro_x);
    file.print(",");

    file.print(r.gyro_y);
    file.print(",");

    file.print(r.gyro_z);
  }

  else {

    file.print(
      "ERROR,ERROR,ERROR,"
      "ERROR,ERROR,ERROR"
    );
  }

  file.println();

  file.close();
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println(
    "========================================"
  );

  Serial.println(
    "OpenPonseti v0.1 - Single Foot Prototype"
  );

  Serial.println(
    "========================================"
  );


  // GPIO
  pinMode(HALL_PIN, INPUT);


  // I2C
  Wire.begin(
    SDA_PIN,
    SCL_PIN
  );


  // MPU6050
  if (
    i2cDeviceExists(
      MPU6050_ADDR
    )
  ) {

    mpuReady = true;

    Serial.println(
      "MPU6050 found at 0x68"
    );

    // Wake MPU6050
    Wire.beginTransmission(
      MPU6050_ADDR
    );

    Wire.write(0x6B);
    Wire.write(0x00);

    Wire.endTransmission(true);
  }

  else {

    Serial.println(
      "WARNING: MPU6050 not found"
    );
  }


  // SHT3X
  if (
    i2cDeviceExists(0x44)
  ) {

    sht3xAddr = 0x44;

    shtReady = true;

    Serial.println(
      "SHT3X found at 0x44"
    );
  }

  else if (
    i2cDeviceExists(0x45)
  ) {

    sht3xAddr = 0x45;

    shtReady = true;

    Serial.println(
      "SHT3X found at 0x45"
    );
  }

  else {

    Serial.println(
      "WARNING: SHT3X not found"
    );
  }


  // SD
  initializeSD();


  Serial.println();

  Serial.println(
    "timestamp_ms,"
    "fsr_raw,"
    "fsr_status,"
    "hall_raw,"
    "buckle_status,"
    "temperature_c,"
    "humidity_percent,"
    "acc_x_raw,"
    "acc_y_raw,"
    "acc_z_raw,"
    "gyro_x_raw,"
    "gyro_y_raw,"
    "gyro_z_raw"
  );

  Serial.println(
    "# System started."
  );
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  unsigned long now =
    millis();

  if (
    now - lastSampleTime
    < SAMPLE_INTERVAL
  ) {
    return;
  }

  lastSampleTime = now;


  // Read everything once
  SensorRecord record =
    readSensors();


  // Serial output
  printCSVRecord(record);


  // SD logging
  if (
    now - lastLogTime
    >= LOG_INTERVAL
  ) {

    lastLogTime = now;

    saveCSVRecord(record);
  }
}
