//--------------------------------------------
//Headers:
#include <pic14/pic12f675.h>

// Declaración de funciones:
void delay(unsigned int time);
unsigned char RandomNum();
//--------------------------------------------
//================== MAIN ====================
void main(void) {
    TRISIO = 0b00100000;                    // COnfigurar como entra el PIN5.
    GPIO = 0x00;                            // Poner en bajo las GPIO.

    unsigned char valor;

    while (1) {
        if (GP5 == 0) {                     // Condición de inicio.
            valor = RandomNum();
            GPIO = 0x00;                    // Estado inicial del GPIO.

            switch (valor) {
                case 1:
                    GP0 = 1;
                    break;

                case 2:
                    GP1 = 1;
                    break;

                case 3:
                    GP2 = 1;
                    break;

                case 4:
                    GP0 = 1;
                    GP2 = 1;
                    break;

                case 5:
                    GP1 = 1;
                    GP2 = 1;
                    break;

                case 6:
                    GP0 = 1;
                    GP1 = 1;
                    GP2 = 1;
                    break;
            }

            // Mantener el valor para que sea legible y limpiar registros.
            delay(300);
            GPIO = 0x00;
        }
    }
}
//--------------------------------------------
// Esta función introduce un retraso en milisegundos aproximadamente igual al valor de 'time'.
// Parámetros:
// - time: el tiempo en milisegundos para el cual se desea retrasar la ejecución del programa.
void delay(unsigned int time) {
    unsigned int i;
    unsigned int j;

    // Bucle externo para controlar el tiempo de retardo
    for (i = 0; i < time; i++) {
        // Bucle interno para introducir el retraso
        for (j = 0; j < 1200; j++);
    }
}

//--------------------------------------------
// Utiliza un generador de números pseudoaleatorios de desplazamiento de registro de retroalimentación lineal (LFSR).
unsigned char RandomNum() {
    static unsigned char lfsr = 0x07;
    unsigned char random_b;

    do {
        // Generación del random_b pseudoaleatorio utilizando el algoritmo LFSR
        random_b = ((lfsr >> 0) ^ (lfsr >> 1)) & 1; 
        lfsr = (lfsr >> 1) | (random_b << 2);
    } while (lfsr > 6);  // Nos aseguramos de solo obtener valores entre 1 y 6

    return lfsr;  
}
//--------------------------------------------
