#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configurar la dirección de la pantalla I2C y el tamaño (20 columnas y 4 filas)
LiquidCrystal_I2C lcd(0x27, 20, 4);

byte N[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

enum Estado {
  Cruce_H,
  Cruce_V,
  Cambio
};

class Semaforo {
public:
  int rojoNorteSur, verdeNorteSur, rojoOesteEste, verdeOesteEste;

  Semaforo(int rNS, int vNS, int rOE, int vOE) {
    rojoNorteSur = rNS;
    verdeNorteSur = vNS;
    rojoOesteEste = rOE;
    verdeOesteEste = vOE;

    pinMode(rojoNorteSur, OUTPUT);
    pinMode(verdeNorteSur, OUTPUT);
    pinMode(rojoOesteEste, OUTPUT);
    pinMode(verdeOesteEste, OUTPUT);
  }

  void FSM_Semaforo(int S_State) {
    switch (S_State) {
      case Cruce_H:
        digitalWrite(verdeNorteSur, LOW);
        digitalWrite(rojoNorteSur, HIGH);
        digitalWrite(verdeOesteEste, HIGH);
        digitalWrite(rojoOesteEste, LOW);
        break;

      case Cruce_V:
        digitalWrite(verdeNorteSur, HIGH);
        digitalWrite(rojoNorteSur, LOW);
        digitalWrite(verdeOesteEste, LOW);
        digitalWrite(rojoOesteEste, HIGH);
        break;

      case Cambio:
        for (int i = 0; i < 3; i++) {
          digitalWrite(verdeNorteSur, LOW);
          digitalWrite(verdeOesteEste, LOW);
          
          delay(100);
          
          digitalWrite(rojoNorteSur, HIGH);
          digitalWrite(rojoOesteEste, HIGH);
          
        }
        break;

      default:
        break;
    }
  }
};

Semaforo semaforoA(5, 3, 4, 6);
Semaforo semaforoB(8, 10, 7, 9);
Semaforo semaforoC(12, A0, 11, 13);
Semaforo semaforoD(A2, 1, A1, A3);

Estado estadoA = Cruce_H;
Estado estadoB = Cruce_H;
Estado estadoC = Cruce_H;
Estado estadoD = Cruce_H;

void setup() {
  lcd.createChar(0, N);

  // Configurar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema de");
  lcd.setCursor(0, 1);
  lcd.print("Semaforos.");

  lcd.setCursor(15, 2);
  lcd.write(byte(0));
}

void loop() {
  // Primera fase: Cruce_H
  Estado estadoA = Cruce_H;
  Estado estadoB = Cruce_H;
  Estado estadoC = Cruce_H;
  Estado estadoD = Cruce_H;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);
  
  delay(1000);

  // Segunda fase: Cruce_V
  estadoA = Cruce_V;
  estadoB = Cruce_V;
  estadoC = Cruce_V;
  estadoD = Cruce_V;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);
  delay(1000);

  // Tercera fase: Cambio
  estadoA = Cambio;
  estadoB = Cambio;
  estadoC = Cambio;
  estadoD = Cambio;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);
  delay(1000);
}

