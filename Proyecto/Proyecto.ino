#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configurar la dirección de la pantalla I2C y el tamaño (20 columnas y 4 filas)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Definición de pines para LEDs
const int ledSerie1 = 4;
const int ledSerie2 = 5;
const int ledSerie3 = 6;
const int ledSerie4 = 7;
const int ledSerie5 = 8;
const int ledSerie6 = 9;
const int ledSerie7 = 10;
const int ledSerie8 = 11;
const int ledSerie9 = 12;
const int ledSerie10 = 13;
const int ledSerie11 = A0;
const int ledSerie12 = A1;
const int ledSerie13 = A2;
const int ledSerie14 = A3;

// Definición de pines para módulo RGB
const int pinR = 2;
const int pinG = 3;

const int T_ON = 250;

void setup() {
  // Configurar pines como salida
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(ledSerie1, OUTPUT);
  pinMode(ledSerie2, OUTPUT);
  pinMode(ledSerie3, OUTPUT);
  pinMode(ledSerie4, OUTPUT);
  pinMode(ledSerie5, OUTPUT);
  pinMode(ledSerie6, OUTPUT);
  pinMode(ledSerie7, OUTPUT);
  pinMode(ledSerie8, OUTPUT);
  pinMode(ledSerie9, OUTPUT);
  pinMode(ledSerie10, OUTPUT);
  pinMode(ledSerie11, OUTPUT);
  pinMode(ledSerie12, OUTPUT);
  pinMode(ledSerie13, OUTPUT);
  pinMode(ledSerie14, OUTPUT);

  // Configurar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema de");
  lcd.setCursor(0, 1);
  lcd.print("Semaforos");
}

void loop() {
  Red_RGB();  // Encender el módulo RGB en rojo
  delay(T_ON); // Mantener por 0.5 segundos

  Green_RGB(); // Encender el módulo RGB en verde
  delay(T_ON);  // Mantener por 0.5 segundos
  OFF_RGB();

  // Secuencia para LEDs en serie (pin 4-13, A0-A3)
  for (int i = 4; i <= 13; i++) {
    digitalWrite(i, HIGH); // Encender los LEDs
    delay(T_ON);           // Mantener encendidos por 0.5 segundos
    digitalWrite(i, LOW);  // Apagar los LEDs
    delay(T_ON);           // Mantener apagados por 0.5 segundos
  }

  // Secuencia para LEDs en serie (pines A0-A3)
  for (int i = A0; i <= A3; i++) {
    digitalWrite(i, HIGH); // Encender los LEDs
    delay(T_ON);           // Mantener encendidos por 0.5 segundos
    digitalWrite(i, LOW);  // Apagar los LEDs
    delay(T_ON);           // Mantener apagados por 0.5 segundos
  }
}

// Función para el módulo RGB (Rojo)
void Red_RGB() {
  digitalWrite(pinR, HIGH);
  digitalWrite(pinG, LOW);
}

// Función para el módulo RGB (Verde)
void Green_RGB() {
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, HIGH);
}

// Función para el módulo RGB (OFF)
void OFF_RGB() {
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, LOW);
}

