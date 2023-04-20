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

  unsigned status1 = bmp1.begin(0x77);             // SDO está desconectado; 3 piso              
  while (status1 == false){
    Serial.println("sensor 1 nao encontrado");
    delay(3000);
    status1 = bmp1.begin(0x77);
  }


  unsigned status2 = bmp2.begin(0x76);             // SDO está ligado ao Ground; 2 piso
  while (status2 == false){
    Serial.println("sensor 2 nao encontrado");
    delay(3000);
    status2 = bmp2.begin(0x76);
  }

  unsigned status3 = ina219.begin();
  while (status3 == false){
    Serial.println("sensor ina219 nao encontrado");
    delay(3000);
    status2 = ina219.begin(0x40);
  }

  // Header Excel:
  Serial1.print("Tempo do Programa ");
  Serial1.print(";");
  Serial1.print(" Temperatura 1 ");
  Serial1.print(";");
  Serial1.print(" Pressao 1 ");
  Serial1.print(";");
  Serial1.print(" Altitude 1 ");
  Serial1.print(";");
  Serial1.print(" Temperatura 2 ");
  Serial1.print(";");
  Serial1.print(" Pressao 2 ");
  Serial1.print(";");
  Serial1.print(" Altitude 2 ");
  Serial1.print(";");
  Serial1.print(" Tensão do Circuito");
  Serial1.print(";");
  Serial1.print(" Corrente em milliAmperes");
  Serial1.print(";");
  Serial1.print(" Energia Criada");
  Serial1.print(";");
  Serial1.print(" Energia Utilizada");
  Serial1.println("");

}

float pressaoMar = 1013.25; //AJUSTAR
float bmp280usage = 0.0054;  //mA 
float ina219usage = 0.0038; //mA 
float arduinousage = 25;  //mA 
float currentusage = bmp280usage * 2 + ina219usage + arduinousage; //energia utilizada
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

  float shuntvoltage = ina219.getShuntVoltage_mV(); // Voltagem entre as entradas V- e V+
  float busvoltage = ina219.getBusVoltage_V(); // Voltagem entre GND e V- 
  float loadvoltage = busvoltage + (shuntvoltage / 1000); // Tensão que passa no voltímetro
  t = now();
  Serial1.print(loadvoltage);
  Serial1.print(";");

  float current_mA = ina219.getCurrent_mA(); // Corrente em milliAmperes
  Serial1.print(current_mA); // Voltagem em milliAmperes
  Serial1.print(";");

  float energycreated = current_mA/1000 * t;
  Serial1.print(energycreated);
  Serial1.print(";");

  float energyused = currentusage/1000 * t;
  Serial1.print(energyused);
  Serial1.print(";");
  Serial1.println("");

  delay(500);
}


//Energy (in joules) = Power (in watts) x Time (in seconds)