#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>

#define sensorLuz 36
#define sensorUmidade1 39
#define sensorUmidade2 34
#define sensorUmidade3 35
#define botao 23
#define sensorTemperatura 4
#define DHTTYPE DHT22

const float resistorReferencia = 10000.0;
int estadoAnterior1 = HIGH;
int contador = 1;

unsigned long ultimoCiclo = 0;
const unsigned long INTERVALO = 2000;

float converterParaLux(int leituraLuminosidade);
void gravarPlaca(float lux, float temperatura, uint8_t umidade, int numeroSerialUmidade);

LiquidCrystal_I2C lcd(0x27,16,2);
DHT dht(sensorTemperatura, DHTTYPE);

void gravarPlaca(float lux, float temperatura, uint8_t umidade, int numeroSerialUmidade) {
  int TAMANHO_PACOTE = 64;
  uint8_t pacote[TAMANHO_PACOTE] = {0};

  pacote[0] = 0xAA;
  pacote[1] = 0xBB;
  pacote[2] = 0xCC;
  pacote[3] = numeroSerialUmidade;

  memcpy(&pacote[10], &lux, sizeof(float));
  memcpy(&pacote[14], &temperatura, sizeof(float));
  pacote[18] = umidade;

  uint16_t checksum = 0;

  for (int i = 0; i < TAMANHO_PACOTE - 2; i++) {
      checksum += pacote[i];
  }

  pacote[62] = checksum & 0xFF;
  pacote[63] = (checksum >> 8) & 0xFF;

  Serial.write(pacote, TAMANHO_PACOTE);
}

float lerTemperatura(){
  float temperatura = dht.readTemperature();

  lcd.setCursor(0, 0); // coluna 0, linha 0
  lcd.print("T:");
  lcd.print(temperatura);
  lcd.print("C ");

  Serial.print("Temperatura: ");
  Serial.println(temperatura);

  return temperatura;
}

float lerLuminosidade(){
  int valorluminosidade = analogRead(sensorLuz);
  long porcentagemLuminosidade = (long)valorluminosidade*100/4096;
   
  float lux = converterParaLux(valorluminosidade);

  lcd.setCursor(0, 1); // coluna 0, linha 0
  lcd.print("L:");
  lcd.print(lux);
  lcd.print("lux ");

  Serial.print("Lux: ");
  Serial.println(lux);

  return lux;
}

uint8_t lerUmidade(int sensor, int contador){
  int valorUmidade = analogRead(sensor);
  uint8_t porcentagemUmidade = (4096 - valorUmidade) * 100.0 / 4096;

  lcd.setCursor(9, 0); // coluna 0, linha 0
  lcd.print("U");
  lcd.print(contador);
  lcd.print(":");
  lcd.print(porcentagemUmidade);
  
  lcd.print("% ");

  Serial.print("Umidade: ");
  Serial.println(porcentagemUmidade);

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

    return lux;
  }
  return 0.0;
}

uint8_t mudarSensorUmidade(int contador) {
  uint8_t umidade;
  switch (contador)
  {
  case 1:
    umidade = lerUmidade(sensorUmidade1, contador);
    break;
    
  case 2:
    umidade = lerUmidade(sensorUmidade2, contador);
    break;
    
  case 3:
    umidade = lerUmidade(sensorUmidade3, contador);
    break;

  default:
    contador = 1;
    break;
  }
  return umidade;
}

void setup() {
  pinMode(botao, INPUT_PULLUP);
  Serial.begin(115200);
  Wire.begin(21, 22);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  int sensorAtual = sensorUmidade1;
  int estadoAtual1 = digitalRead(botao);

  if (estadoAtual1 == LOW && estadoAnterior1 == HIGH) {
    contador++;
    if (contador > 3) contador = 1;
    delay(30); // debounce simples
  }
  estadoAnterior1 = estadoAtual1;

  if (millis() - ultimoCiclo >= INTERVALO) {
    ultimoCiclo = millis();

    float temperatura = lerTemperatura();
    float luminosidade = lerLuminosidade();

    Serial.print("Sensor atual: ");
    Serial.println(contador);

    uint8_t umidade = mudarSensorUmidade(contador);

    if (contador == 1) gravarPlaca(luminosidade, temperatura, umidade, 0xDD);
    else if (contador == 2) gravarPlaca(luminosidade, temperatura, umidade, 0xEE);
    else if (contador == 3) gravarPlaca(luminosidade, temperatura, umidade, 0xFF);
  }
}