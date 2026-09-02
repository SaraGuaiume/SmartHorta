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

void lerTemperatura(){
  double temperatura = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  lcd.setCursor(0, 0); // coluna 0, linha 0
  lcd.print("T: ");
  lcd.print(temperatura);
  lcd.print("C");

  delay(2000);
}

void lerLuminosidade(){
  int valorluminosidade = analogRead(sensorLuz);
  long porcentagemLuminosidade = (long)valorluminosidade*100/1023;
  
  Serial.print("Luminosidade: ");
  Serial.print(porcentagemLuminosidade);
  Serial.println('%');

  Serial.print("Luminosidade: ");
  Serial.println(valorluminosidade);

  lcd.setCursor(0, 1); // coluna 0, linha 0
  lcd.print("L: ");
  lcd.print(porcentagemLuminosidade);
  lcd.print("%");

  delay(2000);
}

void lerUmidade(){
  int valorUmidade = analogRead(sensorUmidade);
  long porcentagemUmidade = (1023 - valorUmidade) * 100.0 / 923.0;

  Serial.print("Umidade do solo: ");
  Serial.print(porcentagemUmidade);
  Serial.println("%");

  Serial.print("valor umidade: ");
  Serial.println(valorUmidade);
  Serial.println("============================================");

  lcd.setCursor(8, 1); // coluna 0, linha 0
  lcd.print("U: ");
  lcd.print(porcentagemUmidade);
  lcd.print("%");

  delay(2000);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  lerTemperatura();
  lerLuminosidade();
  lerUmidade();
}