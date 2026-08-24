#include <Arduino.h>

#define sensorLuz A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int valor = analogRead(sensorLuz);
  long valorPorcentagem = (long)valor*100/1023;
  Serial.print("valor lido luminosidade: ");
  Serial.print(valorPorcentagem);
  Serial.println('%');
  delay(1000);
}