#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define sensorLuz A0
#define sensorUmidade A1
#define sensorTemperatura 2
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(sensorTemperatura, DHTTYPE);

void lerTemperatura(LiquidCrystal_I2C lcd2){
  double temperatura = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  lcd2.setCursor(0, 0); // coluna 0, linha 0
  lcd2.print("T: ");
  lcd2.print(temperatura);
  lcd2.print("C");

  delay(2000);
}

void lerLuminosidade(LiquidCrystal_I2C lcd2){
  int valorluminosidade = analogRead(sensorLuz);
  long porcentagemLuminosidade = (long)valorluminosidade*100/1023;
  
  Serial.print("Luminosidade: ");
  Serial.print(porcentagemLuminosidade);
  Serial.println('%');

  Serial.print("Luminosidade: ");
  Serial.println(valorluminosidade);

  lcd2.setCursor(0, 1); // coluna 0, linha 0
  lcd2.print("L: ");
  lcd2.print(porcentagemLuminosidade);
  lcd2.print("%");

  delay(2000);
}

void lerUmidade(LiquidCrystal_I2C lcd2){
  int valorUmidade = analogRead(sensorUmidade);
  long porcentagemUmidade = (1023 - valorUmidade) * 100.0 / 923.0;

  Serial.print("Umidade do solo: ");
  Serial.print(porcentagemUmidade);
  Serial.println("%");

  Serial.print("valor umidade: ");
  Serial.println(valorUmidade);
  Serial.println("============================================");

  lcd2.setCursor(8, 1); // coluna 0, linha 0
  lcd2.print("U: ");
  lcd2.print(porcentagemUmidade);
  lcd2.print("%");

  delay(2000);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  lerTemperatura(lcd);
  lerLuminosidade(lcd);
  lerUmidade(lcd);
}