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


// Definición de contadores para simular las calles y conexiones entre los cruces
int Entrada_Norte_A = 0;
int Salida_Norte_A = 0;
int Entrada_Oeste_A = 0;
int Salida_Oeste_A = 0;

int Entrada_Norte_B = 0;
int Salida_Norte_B = 0;
int Entrada_Este_B = 0;
int Salida_Este_B = 0;

int Entrada_Oeste_C = 0;
int Salida_Oeste_C = 0;
int Entrada_Sur_C = 0;
int Salida_Sur_C = 0;

int Entrada_Sur_D = 0;
int Salida_Sur_D = 0;
int Entrada_Este_D = 0;
int Salida_Este_D = 0;

// Vías compartidas entre los cruces
int Via_AE_BO = 0;  // Calle que va de A este a B oeste
int Via_BO_AE = 0;  // Calle que va de B oeste a A este

int Via_AS_CN = 0;  // Calle que va de A sur a C norte
int Via_CN_AS = 0;  // Calle que va de C norte a A sur

int Via_BS_DN = 0;  // Calle que va de B sur a D norte
int Via_DN_BS = 0;  // Calle que va de D norte a B sur

int Via_DO_CE = 0;  // Calle que va de D oeste a C este
int Via_CE_DO = 0;  // Calle que va de C este a D oeste


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
  Cruces_FSM();
}

enum S_CONGEST{
  IDLE,
  WARNING,
  ALERT,
  DESCON_H1,
  DESCON_H2,
  DESCON_V1,
  DESCON_V2
};

S_CONGEST STATE_CONGEST = IDLE;

void Cruces_FSM(){
  switch (STATE_CONGEST)
  {
  case IDLE:
    // Modo normal de operación:
    controladorIdle();

    if (Entrada_Norte_A > 12 || Entrada_Oeste_A > 12||
        Entrada_Norte_B > 12 || Entrada_Este_B > 12 ||
        Entrada_Oeste_C > 12 || Entrada_Sur_C  > 12 ||
        Entrada_Sur_D   > 12 || Entrada_Este_D > 12 ||
        Via_AE_BO > 12 || Via_BO_AE > 12 ||
        Via_AS_CN > 12 || Via_CN_AS > 12 ||
        Via_BS_DN > 12 || Via_DN_BS > 12 ||
        Via_DO_CE > 12 || Via_CE_DO > 12) 
        {
        STATE_CONGEST = WARNING;
        
        } else {
      STATE_CONGEST = IDLE;
      }
    break;


  case WARNING:
    if (Entrada_Oeste_A > 12  || Via_AE_BO > 12 || Entrada_Este_B > 12  || Via_BO_AE > 12)
    {
      STATE_CONGEST = DESCON_H1;
    }
    else if (Entrada_Oeste_C > 12 || Via_CE_DO > 12 || Via_DO_CE > 12 || Entrada_Este_D > 12)
    {
      STATE_CONGEST = DESCON_H2;
    }
    else if (Entrada_Norte_A > 12 || Via_AS_CN > 12 || Entrada_Sur_C > 12 || Via_CN_AS > 12)
    {
      STATE_CONGEST = DESCON_V1;
    }
    else if (Entrada_Norte_B > 12 || Via_BS_DN > 12 || Entrada_Sur_D   > 12 || Via_DN_BS > 12)
    {
      STATE_CONGEST = DESCON_V2;
    }
  break;

  case DESCON_H1:
    semaforoA.FSM_Semaforo(Cruce_H);
    semaforoB.FSM_Semaforo(Cruce_H);

  break;

  case DESCON_H2:
    semaforoC.FSM_Semaforo(Cruce_H);
    semaforoD.FSM_Semaforo(Cruce_H);

  break;

  case DESCON_V1:
    semaforoA.FSM_Semaforo(Cruce_V);
    semaforoC.FSM_Semaforo(Cruce_V);

  break;

  case DESCON_V2:
    semaforoB.FSM_Semaforo(Cruce_H);
    semaforoD.FSM_Semaforo(Cruce_H);

  break;

  default:
    break;
  }
}

void controladorIdle() {
  // Primero fase: Cruces Horizontales (Cruce_H)
  Estado estadoA = Cruce_H;
  Estado estadoB = Cruce_H;
  Estado estadoC = Cruce_H;
  Estado estadoD = Cruce_H;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);
  
  delay(1000);

  // Segunda fase: Cruces Verticales (Cruce_V)
  estadoA = Cruce_V;
  estadoB = Cruce_V;
  estadoC = Cruce_V;
  estadoD = Cruce_V;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);

  delay(1000);

  // Tercera fase: Señal de Cambio
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
