
#define TX_Pin 18

#define DATA_SIZE 6  // Size of the data packet

int device_id = 245;
int channel = 4;
int battery_low = 0;  // Assuming battery is OK
float temperature_C = -143.4;
int humidity = 18;

unsigned long Uptime = 1;

#include "Temperature_Sensor.h";
TemperatureSensor tempSensor;
#include "EN882C.h";

unsigned long now;
unsigned long lastMsg = 0;
unsigned long lastUptime = 0;


void printBinaryByte_to_pin(uint8_t byte, int bits) {
  for (int i = (bits - 1); i >= 0; --i) {
    if (((byte >> i) & 1) == 1) {
      digitalWrite(TX_Pin, LOW);
      delayMicroseconds(4000);
      digitalWrite(TX_Pin, HIGH);
      //Serial.print("1");
      delayMicroseconds(700);
    } else {
      digitalWrite(TX_Pin, LOW);
      delayMicroseconds(2000);
      digitalWrite(TX_Pin, HIGH);
      //Serial.print("0");
      delayMicroseconds(700);
    }
  }
}

void setup() {
  Serial.begin(115200);
  tempSensor.initialize();
  pinMode(TX_Pin, OUTPUT);
  delay(2000);
  // put your setup code here, to run once:

  // uint8_t* assembled_bits = assemble_bits(device_id, channel, battery_low, temperature_C, humidity);

  // for (int j = 0; j < 5; j++) {  // Writes data 5x
  //   for (int i = 0; i < DATA_SIZE; ++i) {
  //     int bits = 8;
  //     if (i == 5) bits = 2;
  //     printBinaryByte_to_pin(assembled_bits[i], bits);  // Print the binary representation of each byte
  //   }
  // }
}

void loop() {
  now = millis();

  if (now - lastUptime > 3600000) {
    lastUptime = now;
    Uptime++;
    if (Uptime > 127) Uptime = 1;
  }

  //Temperature Sensor Updates
  if (now - lastMsg > 60000) {
    lastMsg = now;

    tempSensor.measureTemperature();
    Serial.println(tempSensor.getTemperature_C());
    uint8_t* assembled_bits = assemble_bits(device_id, channel, tempSensor.hasError(), tempSensor.getTemperature_C(), Uptime);

    for (int j = 0; j < 5; j++) {  // Writes data 5x
      digitalWrite(TX_Pin, HIGH);
      delayMicroseconds(700);
      for (int i = 0; i < DATA_SIZE; ++i) {
        int bits = 8;
        if (i == 5) bits = 2;
        printBinaryByte_to_pin(assembled_bits[i], bits);  // Print the binary representation of each byte
      }
      digitalWrite(TX_Pin, LOW);
      delayMicroseconds(8600);
      digitalWrite(TX_Pin, HIGH);
      delayMicroseconds(700);
      digitalWrite(TX_Pin, LOW);
      delayMicroseconds(8600);
      digitalWrite(TX_Pin, HIGH);
      delayMicroseconds(700);
      digitalWrite(TX_Pin, LOW);
      delayMicroseconds(8600);
    }
  }
}
