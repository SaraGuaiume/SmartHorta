#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define sensorLuz A0
#define sensorUmidade A1
#define sensorTemperatura 2
#define DHTTYPE DHT22

DHT dht(sensorTemperatura, DHTTYPE);

void lerTemperatura(){
  double temperatura = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  delay(1000);
}

void lerLuminosidade(){
  int valorluminosidade = analogRead(sensorLuz);
  long porcentagemLuminosidade = (long)valorluminosidade*100/1023;
  
  Serial.print("Luminosidade: ");
  Serial.print(porcentagemLuminosidade);
  Serial.println('%');

  delay(1000);
}

void lerUmidade(){
  int valorUmidade = analogRead(sensorUmidade);
  float porcentagemUmidade = (1023 - valorUmidade) * 100.0 / 723.0;

  Serial.print("Umidade do solo: ");
  Serial.print(porcentagemUmidade);
  Serial.println("%");
  Serial.println("============================================");
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  lerTemperatura();
  lerLuminosidade();
  lerUmidade();
}