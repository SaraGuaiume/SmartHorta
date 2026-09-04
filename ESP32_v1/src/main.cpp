#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <iostream>
#include <fstream>

using namespace std;

#define sensorLuz 36
#define sensorUmidade 39
#define sensorTemperatura 4
#define DHTTYPE DHT22

const float resistorReferencia = 10000.0;

float converterParaLux(int leituraLuminosidade);
void exportarCSV(float lux, double temperatura, long umidade);

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(sensorTemperatura, DHTTYPE);

void exportarCSV(float lux, double temperatura, long umidade) {
  ofstream arquivo("dados.csv", ios::app);
  
  arquivo << "Luminosidade,Temperatura,Umidade\n";
  arquivo << lux, temperatura, umidade;

  arquivo.close();  
}

double lerTemperatura(){
  double temperatura = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  lcd.setCursor(0, 0); // coluna 0, linha 0
  lcd.print("T:");
  lcd.print(temperatura);
  lcd.print("C ");

  delay(200);

  return temperatura;
}

float lerLuminosidade(){
  int valorluminosidade = analogRead(sensorLuz);
  long porcentagemLuminosidade = (long)valorluminosidade*100/4096;
   
  Serial.print("Luminosidade: ");
  Serial.print(porcentagemLuminosidade);
  Serial.println('%');

  float lux = converterParaLux(valorluminosidade);

  lcd.setCursor(0, 1); // coluna 0, linha 0
  lcd.print("L:");
  lcd.print(lux);
  lcd.print("lux ");

  delay(200);

  return lux;
}

long lerUmidade(){
  int valorUmidade = analogRead(sensorUmidade);
  long porcentagemUmidade = (4096 - valorUmidade) * 100.0 / 4096;

  Serial.print("Umidade do solo: ");
  Serial.print(porcentagemUmidade);
  Serial.println("%");

  Serial.print("valor umidade: ");
  Serial.println(valorUmidade);
  Serial.println("============================================");

  lcd.setCursor(10, 0); // coluna 0, linha 0
  lcd.print("U:");
  lcd.print(porcentagemUmidade);
  lcd.print("% ");

  delay(200);

  return porcentagemUmidade;
}

float converterParaLux(int leituraLuminosidade) {  
  if (leituraLuminosidade > 0) {
    // 1. Calcula a tensão lida no pino A0
    float tensao = leituraLuminosidade * (5.0 / 4096.0);
    
    // 2. Calcula a resistência do LDR em ohms
    float resistenciaLDR = resistorReferencia * (5 / tensao - 1.0);
    
    // 3. Aproximação do valor em Lux (lm/m²) para o modelo do Tinkercad
    float lux = 500.0 / (resistenciaLDR / 1000.0);

    // Exibe no Monitor Serial
    Serial.print("Intensidade: ");
    Serial.print(lux, 1);
    Serial.println(" lm/m² (lux)");
    delay(200);

    return lux;
  }
  return 0.0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  double temperatura = lerTemperatura();
  float luminosidade = lerLuminosidade();
  long umidade = lerUmidade();

  exportarCSV(luminosidade, temperatura, umidade);
}