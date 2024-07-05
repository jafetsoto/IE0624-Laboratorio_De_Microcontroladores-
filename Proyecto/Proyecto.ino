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
int Salida_Norte_A  = 0;
int Entrada_Oeste_A = 0;
int Salida_Oeste_A  = 0;

int Entrada_Norte_B = 0;
int Salida_Norte_B  = 0;
int Entrada_Este_B  = 0;
int Salida_Este_B   = 0;

int Entrada_Oeste_C = 0;
int Salida_Oeste_C  = 0;
int Entrada_Sur_C  = 0;
int Salida_Sur_C   = 0;

int Entrada_Sur_D  = 0;
int Salida_Sur_D   = 0;
int Entrada_Este_D = 0;
int Salida_Este_D  = 0;

// Vías compartidas entre los cruces
int Via_AE_BO = 0;                      // Calle que va de A este a B oeste
int Via_BO_AE = 0;                      // Calle que va de B oeste a A este

int Via_AS_CN = 0;                      // Calle que va de A sur a C norte
int Via_CN_AS = 0;                      // Calle que va de C norte a A sur

int Via_BS_DN = 0;                      // Calle que va de B sur a D norte
int Via_DN_BS = 0;                      // Calle que va de D norte a B sur

int Via_DO_CE = 0;                      // Calle que va de D oeste a C este
int Via_CE_DO = 0;                      // Calle que va de C este a D oeste


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
        digitalWrite(verdeNorteSur, LOW);
        digitalWrite(verdeOesteEste, LOW);
        delay(250);
        digitalWrite(rojoNorteSur, HIGH);
        digitalWrite(rojoOesteEste, HIGH);
        delay(250);
        digitalWrite(verdeNorteSur, LOW);
        digitalWrite(verdeOesteEste, LOW);
        delay(250);
        digitalWrite(rojoNorteSur, HIGH);
        digitalWrite(rojoOesteEste, HIGH);
        delay(250);
        digitalWrite(rojoNorteSur, HIGH);
        digitalWrite(rojoOesteEste, HIGH);
        delay(250);
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

  set_condiciones_iniciales(): //Inicializar estados aleatoreos de carriles, entradas y salidas
}

void loop() {
  // Ejecutamos 2 veces sin la prueba:
  Cruces_FSM();
  Cruces_FSM();

  // Inicio de la prueba de descongestión:
  lcd.setCursor(0, 3);
  lcd.print("01:EAN");
  
  Cruces_FSM();

  Entrada_Norte_A = 13;
  Cruces_FSM();

  Entrada_Norte_A = 12;
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
        lcd.setCursor(0, 3);
        lcd.print("WARNING");
        //delay(100);
        
        } else {
      STATE_CONGEST = IDLE;
      // Modo normal de operación:
      controladorIdle();                  // Es mejor correr el modo normal aquí, para evitar la espera de los delays().
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
    } else {
      STATE_CONGEST = IDLE;
    }
  break;

  case DESCON_H1:
    lcd.setCursor(0, 3);
    lcd.print("DESCON_H1");
    semaforoA.FSM_Semaforo(Cruce_H);
    semaforoB.FSM_Semaforo(Cruce_H);
    semaforoC.FSM_Semaforo(estadoC);
    semaforoD.FSM_Semaforo(estadoD);
    STATE_CONGEST = WARNING;

  break;

  case DESCON_H2:
    lcd.setCursor(0, 3);  
    lcd.print("DESCON_H2");
    semaforoA.FSM_Semaforo(estadoA);
    semaforoB.FSM_Semaforo(estadoB);
    semaforoC.FSM_Semaforo(Cruce_H);
    semaforoD.FSM_Semaforo(Cruce_H);
    STATE_CONGEST = WARNING;

  break;

  case DESCON_V1:
    lcd.setCursor(0, 3);
    lcd.print("DESCON_V1");
    semaforoA.FSM_Semaforo(Cruce_V);
    semaforoB.FSM_Semaforo(estadoB);
    semaforoC.FSM_Semaforo(Cruce_V);
    semaforoD.FSM_Semaforo(estadoD);
    if (Entrada_Norte_A < 13 || Via_AS_CN < 13 || Entrada_Sur_C < 13 || Via_CN_AS < 13)
    {
      STATE_CONGEST = IDLE;
    }

  break;

  case DESCON_V2:
    lcd.setCursor(0, 3);
    lcd.print("DESCON_V2");
    semaforoA.FSM_Semaforo(estadoA);
    semaforoB.FSM_Semaforo(Cruce_V);
    semaforoC.FSM_Semaforo(estadoC);
    semaforoD.FSM_Semaforo(Cruce_V);
    STATE_CONGEST = WARNING;

  break;

  default:
    break;
  }
}

void controladorIdle() {
  // Primero fase: Cruces Horizontales (Cruce_H)
  estadoA = Cruce_H;
  estadoB = Cruce_H;
  estadoC = Cruce_H;
  estadoD = Cruce_H;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);
  
  delay(5000);

  // Señal de cambio:
  /*
  estadoA = Cambio;
  estadoB = Cambio;
  estadoC = Cambio;
  estadoD = Cambio;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);

  delay(5000);*/

  // Cruce vertical:
  estadoA = Cruce_V;
  estadoB = Cruce_V;
  estadoC = Cruce_V;
  estadoD = Cruce_V;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);

  delay(5000);
  /*
  // Señal de cambio:
  estadoA = Cambio;
  estadoB = Cambio;
  estadoC = Cambio;
  estadoD = Cambio;

  semaforoA.FSM_Semaforo(estadoA);
  semaforoB.FSM_Semaforo(estadoB);
  semaforoC.FSM_Semaforo(estadoC);
  semaforoD.FSM_Semaforo(estadoD);

  delay(5000);*/
}

void set_condiciones_iniciales(){
  // Definición de contadores para simular las calles y conexiones entre los cruces
  Entrada_Norte_A = 13;
  Salida_Norte_A  = 2;
  Entrada_Oeste_A = 8;
  Salida_Oeste_A  = 0;

  Entrada_Norte_B = 4;
  Salida_Norte_B  = 2;
  Entrada_Este_B  = 1;
  Salida_Este_B   = 0;

  Entrada_Oeste_C = 7;
  Salida_Oeste_C  = 4;
  Entrada_Sur_C   = 10;
  Salida_Sur_C    = 0;

  Entrada_Sur_D   = 0;
  Salida_Sur_D    = 1;
  Entrada_Este_D  = 0;
  Salida_Este_D   = 0;

  // Vías compartidas entre los cruces
  Via_AE_BO = 5;
  Via_BO_AE = 8;

  Via_AS_CN = 6;
  Via_CN_AS = 10;

  Via_BS_DN = 2;
  Via_DN_BS = 6;

  Via_DO_CE = 5;
  Via_CE_DO = 0;
}
void move_E_to_S(int Entrada, int Carril, int Salida){
  if (Entrada)
  {
    Entrada--;
    if (Carril)
    {
      Carril++;
    }
    if (Salida)
    {
      Salida--;
      Carril
    }
    
    
  }
  
}


void salidas_update(){
  Salida_Norte_A = (Salida_Norte_A == 0) ? Salida_Norte_A : Salida_Norte_A--;
  Salida_Oeste_A = (Salida_Oeste_A == 0) ? Salida_Oeste_A : Salida_Oeste_A--;

  Salida_Norte_B = (Salida_Norte_B == 0) ? Salida_Norte_B : Salida_Norte_B--;
  Salida_Este_B  = (Salida_Norte_A == 0) ? Salida_Norte_A : Salida_Norte_A--;

  Salida_Este_D  = (Salida_Este_D == 0)  ? Salida_Este_D  : Salida_Este_D--;
  Salida_Sur_D   = (Salida_Sur_D == 0)   ? Salida_Sur_D   : Salida_Sur_D--;

  Salida_Sur_C   = (Salida_Sur_C == 0)   ? Salida_Sur_C   : Salida_Sur_C--;
  Salida_Oeste_C = (Salida_Oeste_C == 0) ? Salida_Oeste_C : Salida_Oeste_C--;
}

void car_move(STATE_TO_MOVE){
  if (STATE_TO_MOVE == Cruce_H)
  {
    Entrada_Norte_A = (Entrada_Norte_A == 0) ? Entrada_Norte_A : move_E_to_S(Entrada_Norte_A, Via_AS_CN, Salida_Sur_C);
    Entrada_Norte_B = (Entrada_Norte_B == 0) ? Entrada_Norte_B : move_E_to_S(Entrada_Norte_B, Via_AE_BO, Salida_Este_B);

    Entrada_Oeste_A = (Entrada_Oeste_A == 0) ? Entrada_Oeste_A : move_E_to_S(Entrada_Oeste_A, Via_AE_BO, Salida_Este_B);
    Entrada_Oeste_C = (Entrada_Oeste_C == 0) ? Entrada_Oeste_C : move_E_to_S(Entrada_Oeste_C, Via_BS_DN, Salida_SUR_D);

    
    Entrada_Oeste_A = (Entrada_Oeste_A == 0) ? Entrada_Oeste_A : move_E_to_S(Entrada_Oeste_A, Via_AE_BO, Salida_Este_B);

    Entrada_Oeste_A = (Entrada_Oeste_A == 0) ? Entrada_Oeste_A : move_E_to_S(Entrada_Oeste_A, Via_AE_BO, Salida_Este_B);
    Entrada_Oeste_A = (Entrada_Oeste_A == 0) ? Entrada_Oeste_A : move_E_to_S(Entrada_Oeste_A, Via_AE_BO, Salida_Este_B);

    Entrada_Oeste_A = (Entrada_Oeste_A == 0) ? Entrada_Oeste_A : move_E_to_S(Entrada_Oeste_A, Via_AE_BO, Salida_Este_B);
    

  } else if (STATE_TO_MOVE == Cruce_v)
  {
    
  }
  salidas_update();
}
  
