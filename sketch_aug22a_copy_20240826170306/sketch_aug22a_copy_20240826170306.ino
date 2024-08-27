#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP32Time.h>  //Libreria para usar el timer

ESP32Time rtc; 


#define SEALEVELPRESSURE_HPA (1013.25)
#define TIMER_INTERVAL 1000 // Intervalo de tiempo en milisegundos (1 segundo)

Adafruit_BME280 bme1; // I2C
Adafruit_BME280 bme2; // I2C

unsigned long previousMillis = 0; // Almacena el último momento en que se actualizó
bool timer_flag = false; // Bandera para controlar la temporización

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

  Serial.println("-- Default Test --");
}

void loop() {
  unsigned long currentMillis = millis(); // Obtiene el tiempo actual

  
  // Comprueba si ha pasado el intervalo
  if (currentMillis - previousMillis >= TIMER_INTERVAL) {
    // Actualiza el último tiempo y establece la bandera
    previousMillis = currentMillis;
    timer_flag = true;
  }

  // Ejecuta la función de impresión si la bandera está activa
  if (timer_flag) {
    printTime();
    printValues();
    
    timer_flag = false; // Resetea la bandera
  }
}
void printTime() {
  String now = rtc.getTime("%A, %B %d %Y %H:%M:%S");
  Serial.println(now);

}
void printValues() {
  // Imprime los datos del Sensor 1
  
  
  Serial.print("1,"); // Identificador del sensor
  Serial.print(bme1.readTemperature()); // Temperatura
  Serial.print(","); 
  Serial.print(bme1.readPressure() / 100.0F); // Presión en hPa
  Serial.print(",");
  Serial.print(bme1.readHumidity()); // Humedad
  Serial.println(); // Finaliza la línea

  // Imprime los datos del Sensor 2
  Serial.print("2,"); // Identificador del sensor
  Serial.print(bme2.readTemperature()); // Temperatura
  Serial.print(","); 
  Serial.print(bme2.readPressure() / 100.0F); // Presión en hPa
  Serial.print(",");
  Serial.print(bme2.readHumidity()); // Humedad
  Serial.println(); // Finaliza la línea
}