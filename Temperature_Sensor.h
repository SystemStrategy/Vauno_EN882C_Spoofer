#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

// Define your sensor type based on a runtime condition
#define AHT_SENSOR false
#define DS18B20_SENSOR true
#define ONE_WIRE_BUS 16

class TemperatureSensor {
public:
  TemperatureSensor()
    : Temp_Sensor_Error(false), last_Temp_Measure(0) {
    if (AHT_SENSOR) {
      aht = new Adafruit_AHTX0();
      ds18b20 = nullptr;  // DS18B20 is not used
    } else if (DS18B20_SENSOR) {
      oneWire = new OneWire(ONE_WIRE_BUS);
      ds18b20 = new DallasTemperature(oneWire);
      ds18b20->begin();
      deviceCount = ds18b20->getDeviceCount();
      deviceAddress = new DeviceAddress[deviceCount];
      ds18b20->requestTemperatures();
      ds18b20->getAddress(deviceAddress[0], 0);  // Get the unique address of the first DS18B20
      ds18b20->setResolution(deviceAddress[0], 11);
    }
  }

  bool initialize() {
    if (AHT_SENSOR) {
      if (aht->begin()) {
        return aht->getStatus() != 255;
      }
    } else if (DS18B20_SENSOR) {
      ds18b20->requestTemperatures();
      tempC = ds18b20->getTempC(deviceAddress[0]);
      if ((tempC == -127) || isnan(tempC)) {
        Temp_Sensor_Error = true;
        ds18b20->getAddress(deviceAddress[0], 0);  // Get the unique address of the first DS18B20
        ds18b20->setResolution(deviceAddress[0], 11);
      } else {
        last_Temp_Measure = millis();
        Temp_Sensor_Error = false;
        Actual_Temp = ((tempC * 9 / 5) + 32) - 3.1;  // convert from C to F
      }
    }
    return false;
  }

  bool hasError() const {
    return Temp_Sensor_Error;
  }

  void measureTemperature() {
    if (AHT_SENSOR) {
      sensors_event_t humidity, temp;
      aht->getEvent(&humidity, &temp);

      if (aht->getStatus() == 255) {
        Temp_Sensor_Error = true;
      } else {
        last_Temp_Measure = millis();
        Temp_Sensor_Error = false;
        Actual_Temp = ((temp.temperature * 9 / 5) + 32);  // convert from C to F
      }
    } else if (DS18B20_SENSOR) {
      ds18b20->requestTemperatures();
      tempC = ds18b20->getTempC(deviceAddress[0]);
      if ((tempC == -127) || isnan(tempC)) {
        Temp_Sensor_Error = true;
        ds18b20->getAddress(deviceAddress[0], 0);  // Get the unique address of the first DS18B20
        ds18b20->setResolution(deviceAddress[0], 11);
      } else {
        last_Temp_Measure = millis();
        Temp_Sensor_Error = false;
        Actual_Temp = ((tempC * 9 / 5) + 32) - 3.1;  // convert from C to F
      }
    }
  }

  float getTemperature() const {
    return Actual_Temp;
  }
  float getTemperature_C() const {
    return tempC;
  }

  unsigned long getlastupdate() const {
    return millis() - last_Temp_Measure;
  }

private:
  float Actual_Temp;
  float tempC;
  bool Temp_Sensor_Error;
  unsigned long last_Temp_Measure;
  Adafruit_AHTX0 *aht;
  OneWire *oneWire;
  DallasTemperature *ds18b20;
  DeviceAddress *deviceAddress;
  int deviceCount;
};
