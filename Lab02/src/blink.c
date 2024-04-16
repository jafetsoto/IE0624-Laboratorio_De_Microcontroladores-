// ========================================================
// --------------------------------------------------------
// HEADERS:
#include <avr/io.h>
#include <avr/interrupt.h>
// --------------------------------------------------------
// Estados de la FSM:
typedef enum{
  IDL,
  SETUP,
  STOP_V,
  SYSTEM_ON,
  DEACTIVATE,
  SYSTEM_OFF
} Signal_State;

Signal_State state;
int Time = 0;
int BUTTON_INTERRUPT = 0;
int TIMER_COUNTER = 0;

// --------------------------------------------------------
// Prototipos:
void configureInterrupts();
void FSM();
void BLINK_LEDs();

// Interrupciones:
ISR(INT0_vect){
  BUTTON_INTERRUPT = 1;
}

ISR(TIMER0_OVF_vect){
    BLINK_LEDs();
}
// ========================================================
// ------------------- Main Loop --------------------------
int main(void)
{
  configureInterrupts();
  sei();


  while (1)
  {
    FSM();
  }
  
}
// --------------------------------------------------------
// ---------------- Conf Interrupts -----------------------
void configureInterrupts(){
  sei();
  state = IDL;

  // Pines de salida
  DDRB |= (1 << PB1)|(1 << PB2)|(1 << PB3)|(1 << PB4);
  PORTB = (0 << PB1)|(1 << PB2)|(1 << PB3)|(0 << PB4);  // Estadaos iniciales.

  // Interrupciones por INT0.
  GIMSK |= (1 << INT0);
  MCUCR |= (1 << ISC00) | (1 << ISC01);                 // Posedge de INT0.

  // TIMER:
  TCCR0A = 0x00;                                        // Modo normal de operacion.
  TCCR0B = (1 << CS00) | (1 << CS02);                   // Divir la frecuencia de reloj.
  TCNT0  = 0;                                           // Inicializar contador.
  TIMSK |= (1 << TOIE0);                                // Interrupcion de control de rabase.
  
  Time = 0;

  sei();
}
// --------------------------------------------------------
// ---------------------- FSM -----------------------------
void FSM(){
  switch (state)
  {
  case IDL:
    /* Estado de espera, se puede iniciar el sistema por la señal
    de botón después de 10 segundos en estado de paso de vehículos.*/

    PORTB = (0 << PB1)|(1 << PB2)|(1 << PB3)|(0 << PB4);

    if((BUTTON_INTERRUPT == 1) ) // && (Time >= 10)
    {
      Time = 0;
      TIMER_COUNTER = 0;

      state = SETUP;
    }
    break;

  case SETUP:
    /* Señal de cambio del modo, paso de vehículo -> paso de
    peatones. */
    if ((Time >= 3) && (TIMER_COUNTER == 30 || TIMER_COUNTER == 60))
    {
      Time = 0;
      TIMER_COUNTER = 0;

      state = STOP_V;
    }
  case STOP_V:
    /* Se detienen los vehiculos y se espera un segundo para 
    iniciar el paso de peatones. */
    PORTB = (1 << PB1)|(0 << PB2);

    if (Time >= 1)
    {
      PORTB = (0 << PB3)|(1 << PB4);

      Time = 0;
      TIMER_COUNTER = 0;

      state = SYSTEM_ON;
    }
    break;
  
  case SYSTEM_ON:
    /* Se mantiene el paso activo por 10 segundos. */
    if (Time >= 10)
    {
      Time = 0;
      TIMER_COUNTER = 0;

      state = DEACTIVATE;
    }
  break;

  case DEACTIVATE:
    /* Señal del cambio de modo, paso de peatones -> paso de
    vehículos. */
    if ((Time >= 3) && (TIMER_COUNTER == 30 || TIMER_COUNTER == 60))
    {
      Time = 0;
      TIMER_COUNTER = 0;

      state = SYSTEM_OFF;
    }
    break;
  case SYSTEM_OFF:
    PORTB = (1 << PB3)|(0 << PB4);

    if (Time >= 1)
    {
      Time = 0;
      TIMER_COUNTER = 0;

      state = IDL;
      BUTTON_INTERRUPT = 0;
    }
    break;

  default:
    state = IDL;
    break;
  }
}
// --------------------------------------------------------
// ------------------ Blink LEDs --------------------------
void BLINK_LEDs(){
  switch (state)
  {
  case SETUP:
    if(TIMER_COUNTER == 30 || TIMER_COUNTER == 60)
    {
    PORTB ^= (1 << PB2);
    }
    break;
  
  case DEACTIVATE:
    if(TIMER_COUNTER == 30 || TIMER_COUNTER == 60)
    {
      PORTB ^= (1 << PB4);
    }
    break;

  default:
    break;
  }

  if (TIMER_COUNTER == 63)
  {
    Time ++;
    TIMER_COUNTER = 0;
  } else
  {
    TIMER_COUNTER ++;
  }
}
