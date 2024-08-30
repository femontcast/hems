#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP32Time.h>  //Libreria para usar el timer

ESP32Time rtc(-21600); 

hw_timer_t *timer = NULL;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme1; // I2C
Adafruit_BME280 bme2; // I2C

bool timer_flag = true;

void ARDUINO_ISR_ATTR onTimer() {
  timer_flag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  //rtc.setTime(00, 37, 5, 26, 8, 2024); //ESPTime 

  bool status;

  // Inicializar sensores
  status = bme1.begin(0x76) & bme2.begin(0x77);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Set timer frequency to 1Mhz
  timer = timerBegin(1000000);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
  timerAlarm(timer, 1000000, true, 0);
}

void loop() {
  // Ejecuta la función de impresión si la bandera está activa
  if (timer_flag) {
    printValues();    
    timer_flag = false; // Resetea la bandera
  }
}

void printValues() {
  // Imprime los datos del Sensor 1
  String now = rtc.getTime("%Y-%m-%d %H:%M:%S,");
  Serial.print(now);
  Serial.print("1,"); // Identificador del sensor
  Serial.print(bme1.readTemperature()); // Temperatura
  Serial.print(","); 
  Serial.print(bme1.readPressure() / 100.0F); // Presión en hPa
  Serial.print(",");
  Serial.print(bme1.readHumidity()); // Humedad
  Serial.println(); // Finaliza la línea
  // Imprime los datos del Sensor 2
  Serial.print(now);
  Serial.print("2,"); // Identificador del sensor
  Serial.print(bme2.readTemperature()); // Temperatura
  Serial.print(","); 
  Serial.print(bme2.readPressure() / 100.0F); // Presión en hPa
  Serial.print(",");
  Serial.print(bme2.readHumidity()); // Humedad
  Serial.println(); // Finaliza la línea
}