#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <TimeLib.h>
#include <Adafruit_INA219.h>

Adafruit_BMP280 bmp1;
Adafruit_BMP280 bmp2;
Adafruit_INA219 ina219;

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);
  delay(100);

  unsigned status1 = bmp1.begin(0x77);             // SDO is desconected; isolated              
  while (status1 == false){
    Serial1.println("sensor 1 not found");
    delay(3000);
    status1 = bmp1.begin(0x77);
  }


  unsigned status2 = bmp2.begin(0x76);             // SDO is connect to Ground; interior
  while (status2 == false){
    Serial1.println("sensor 2 not found");
    delay(3000);
    status2 = bmp2.begin(0x76);
  }

  unsigned status3 = ina219.begin();
  while (status3 == false){
    Serial.println("sensor ina219 not found");
    delay(3000);
    status2 = ina219.begin(0x40);
  }

  // Header Excel:
  Serial1.print("Time ");
  Serial1.print(";");
  Serial1.print(" External Temperature ");
  Serial1.print(";");
  Serial1.print(" External Pressure ");
  Serial1.print(";");
  Serial1.print(" External Altitude ");
  Serial1.print(";");
  Serial1.print(" Internal Temperature ");
  Serial1.print(";");
  Serial1.print(" Internal Pressure ");
  Serial1.print(";");
  Serial1.print(" Internal Altitude ");
  Serial1.print(";");
  Serial1.print(" Current in milliAmperes ");
  Serial1.print(";");
  Serial1.print(" Power Created microW ");
  Serial1.print(";");
  Serial1.print(" Power used microW ");
  Serial1.println("");

}

float pressaoMar = 1013.25; //ADJUST
float bmp280usage = 0.0054;  //mA 
float ina219usage = 0.0038; //mA 
float arduinousage = 25;  //mA
float antennausage = 30; //mA 
float currentusagecomponents = bmp280usage * 2 + ina219usage; //energy used with 8.76V in our battery
float t1 = millis();

void loop() {
  Serial1.flush();
  float t2 = millis();
  float t = (t2 - t1)/1000;
  Serial1.print(t);
  Serial1.print(";");

  Serial1.print(bmp1.readTemperature());
  Serial1.print(";");

  Serial1.print(bmp1.readPressure() / 100);
  Serial1.print(";");

  Serial1.print(bmp1.readAltitude(pressaoMar));
  Serial1.print(";");

  Serial1.print(bmp2.readTemperature());
  Serial1.print(";");

  Serial1.print(bmp2.readPressure() / 100);
  Serial1.print(";");
  
  Serial1.print(bmp2.readAltitude(pressaoMar));
  Serial1.print(";");

  float shuntvoltage = ina219.getShuntVoltage_mV(); // Voltage between V- e V+
  float busvoltage = ina219.getBusVoltage_V(); // Voltage between GND e V- 
  float loadvoltage = busvoltage * 1000 + shuntvoltage; // Voltage that passes on the multimeter

  t = now();
  
  float current_mA = ina219.getCurrent_mA(); // Current in mA
  Serial1.print(current_mA);
  Serial1.print(";");

  float powercreated = loadvoltage * current_mA;
  Serial1.print(powercreated);
  Serial1.print(";");
  
  float powerused = 5 * 1000 * currentusagecomponents + 8.76 * 1000 * arduinousage ;
  Serial1.print(powerused);
  Serial1.print(";");
  Serial1.println("");


  delay(500);
}