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
  double temperature = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println("°C");

  delay(1000);
}

void lerLuminosidade(){
  int valor = analogRead(sensorLuz);
  long valorPorcentagem = (long)valor*100/1023;
  
  Serial.print("valor lido luminosidade: ");
  Serial.print(valorPorcentagem);
  Serial.println('%');

  delay(1000);
}

void lerUmidade(){
  int leituraADC = analogRead(sensorUmidade);
  float leituraADCPorcentagem = (1023 - leituraADC) * 100.0 / 723.0;

  Serial.print("Umidade do solo: ");
  Serial.print(leituraADCPorcentagem);
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