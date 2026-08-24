#include <Arduino.h>

#define sensorLuz A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int valor = analogRead(sensorLuz);
  Serial.print("valor lido luminosidade: ");
  Serial.println(valor);
  delay(1000);
}