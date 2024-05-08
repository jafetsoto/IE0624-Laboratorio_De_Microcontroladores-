// ________________________________________________________
// ------------------------Headers:------------------------ 
#include <PCD8544.h>
#include <PID_v1.h>

// -------------------Definiciones:------------------------
// Definiciones de pines:
#define PIN_BLUE 8
#define PIN_GREEN 12
#define PIN_RED 13

// -----------------------Objetos:-------------------------
// Variables para control PID:
double Input, Output, Setpoint;
PID myPID(&Input, &Output, &Setpoint, 15, 0.5, 1, DIRECT);

// Objeto para la pantalla LCD:
PCD8544 lcd;

// ________________________________________________________
// -----------------------Setup:---------------------------
void setup() {
    Serial.begin(9600);
    
    // Configuración de pines para LEDs:
    pinMode(PIN_BLUE, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_RED, OUTPUT);

    // Inicialización del LCD:
    lcd.begin(); // Resolución por defecto: 84x48

    // Configuración del PID:
    myPID.SetOutputLimits(-125, 125);
    myPID.SetMode(AUTOMATIC);
}

// ________________________________________________________
// ------------------------Loop:---------------------------
void loop() {
    LCD_state();

    // Lectura y ajuste del Setpoint:
    int value = analogRead(A0);
    float temp = map(value, 0, 1022, 20, 80); // Temperatura de 20 a 80 °C
    Setpoint = temp;

    // Cálculo del Input:
    float tempWatts = (int)Output * 25.0 / 255.0;
    Input = simPlant(tempWatts);

    // Computar PID:
    myPID.Compute();

    // Mostrar datos en LCD:
    lcd.setCursor(0, 1);
    lcd.print("SetPoint: "); lcd.print(temp);

    lcd.setCursor(0, 2);
    lcd.print("PID: "); lcd.print(Output);

    lcd.setCursor(0, 3);
    lcd.print("T: "); lcd.print(Input);

    // Lectura del interruptor de comunicaciones:
    int commSwitch = analogRead(A2);
    if (commSwitch > 700) {
        Serial.print(Setpoint);
        Serial.print(",");
        Serial.print(Output);
        Serial.print(",");
        Serial.println(Input); 
        delay(500);
    }
    delay(500);

    // Control de LEDs indicadores de temperatura:
    if (Input < 30) {
        digitalWrite(PIN_BLUE, HIGH); 
        digitalWrite(PIN_RED, LOW); 
        digitalWrite(PIN_GREEN, LOW);
    } else if (Input > 42) {
        digitalWrite(PIN_RED, HIGH); 
        digitalWrite(PIN_GREEN, LOW); 
        digitalWrite(PIN_BLUE, LOW);
    } else {
        digitalWrite(PIN_GREEN, HIGH); 
        digitalWrite(PIN_RED, LOW); 
        digitalWrite(PIN_BLUE, LOW);
    }
}

// ________________________________________________________
// ----------------Funciones Adicionales:------------------
void LCD_state() {
    int switchState = analogRead(A1);  
    if (switchState > 700) {
        lcd.setPower(1);
    } else {
        lcd.setPower(0);
    }
}

float simPlant(float Q) {
    float h = 5;              // W/m2K
    float Cps = 0.89;         // J/g°C
    float area = 1e-4;        // m2
    float mass = 10;          // g
    float Tamb = 25;          // °C
    static float T = Tamb;
    static uint32_t last = 0;
    uint32_t interval = 100;  // ms

    if (millis() - last >= interval) {
        last += interval;
        T += Q * interval / 1000 / mass / Cps - (T - Tamb) * area * h;
    }
    return T;
}
