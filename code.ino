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
  unsigned status1 = bmp1.begin(0x77);             // SDO está desconectado              
  while (status1 == false){
    Serial.println("sensor 1 nao encontrado");
    delay(3000);
    status1 = bmp1.begin(0x77);
  }


  unsigned status2 = bmp2.begin(0x76);             // SDO está ligado ao Ground
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
  Serial.print("Tempo do Programa ");
  Serial.print(";");
  Serial.print(" Temperatura 1 ");
  Serial.print(";");
  Serial.print(" Pressao 1 ");
  Serial.print(";");
  Serial.print(" Altitude 1 ");
  Serial.print(";");
  Serial.print(" Temperatura 2 ");
  Serial.print(";");
  Serial.print(" Pressao 2 ");
  Serial.print(";");
  Serial.print(" Altitude 2 ");
  Serial.print(";");
  Serial.print(" Tensão do Circuito");
  Serial.print(";");
  Serial.print(" Corrente em milliAmperes");
  Serial.print(";");
  Serial.print(" Energia Criada");
  Serial.print(";");
  Serial.print(" Energia Utilizada");
  Serial.println("");

}

float pressaoMar = 1013.25; //Ajustar
float bmp280usage = 0.0027;  //mA para 1Hz
float ina219usage = 0.0019; //mA (pode estar errado) para 1Hz
float arduinousage = 25;  //mA (ajustar)
float currentusage = bmp280usage * 2 + ina219usage + arduinousage; //energia utilizada

void loop() {

  time_t t = now();                                                                                                                                                                                                      
  Serial.print(t);
  Serial.print(" s");
  Serial.print(";");

  Serial.print(bmp1.readTemperature());
  Serial.print(" C");
  Serial.print(";");

  Serial.print(bmp1.readPressure() / 100);
  Serial.print(" hPa");
  Serial.print(";");

  Serial.print(bmp1.readAltitude(pressaoMar));
  Serial.print(" m");
  Serial.print(";");

  Serial.print(bmp2.readTemperature());
  Serial.print(" C");
  Serial.print(";");

  Serial.print(bmp2.readPressure() / 100);
  Serial.print(" hPa");
  Serial.print(";");
  
  Serial.print(bmp2.readAltitude(pressaoMar));
  Serial.print(" m");
  Serial.print(";");

  float shuntvoltage = ina219.getShuntVoltage_mV(); // Voltagem entre as entradas V- e V+
  float busvoltage = ina219.getBusVoltage_V(); // Voltagem entre GND e V- 
  float loadvoltage = busvoltage + (shuntvoltage / 1000); // Tensão que passa no voltímetro
  Serial.print(loadvoltage);
  Serial.print(" V");
  Serial.print(";");

  float current_mA = ina219.getCurrent_mA(); // Corrente em milliAmperes
  Serial.print(current_mA); // Voltagem em milliAmperes
  Serial.print(" mA");
  Serial.print(";");

  float energycreated = current_mA/1000 * t;
  Serial.print(energycreated);
  Serial.print(" J");
  Serial.print(";");

  float energyused = currentusage/1000 * t;
  Serial.print(energyused);
  Serial.print(" J");
  Serial.println("");

  delay(1000);
}


//Energy (in joules) = Power (in watts) x Time (in seconds)