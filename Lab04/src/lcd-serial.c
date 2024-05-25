#include <stdint.h>
#include <math.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"

#define IDLE 0
#define ACTIVE 1

int state;

void ALERT(void);

int main(void)
{   
    // ---------------- Inicialisar el Display: --------------------
	clock_setup();
	console_setup(115200);

	sdram_init();
	lcd_spi_init();

	gfx_init(lcd_draw_pixel, 240, 320);

	while (1) 
    {
        //---------------------- UI --------------------------------
        gfx_fillScreen(LCD_BLACK);
		gfx_fillRoundRect(5, 5, 230, 300, 5, LCD_GREY);
        gfx_fillRoundRect(10, 10, 220, 70, 10, LCD_WHITE);
        gfx_fillRoundRect(10, 90, 220, 210, 10, LCD_WHITE);
        gfx_drawRoundRect(10, 10, 220, 290, 10, LCD_GREY);
        
        // Display:
        gfx_setTextSize(2);
        gfx_setCursor(35, 25);
        gfx_puts("MONITOR DE");
        gfx_setCursor(35, 50);
        gfx_puts("PENDIENTES");


        gfx_setTextSize(2);
        gfx_setCursor(70, 110);
        gfx_puts("X=000");
        gfx_setTextSize(2);
        gfx_setCursor(70, 145);
        gfx_puts("Y=000");
        gfx_setTextSize(2);
        gfx_setCursor(70, 180);
        gfx_puts("Z=000");
        


        state = IDLE;
        ALERT();

        gfx_setTextSize(1);
        gfx_setCursor(15, 285);
        gfx_puts("COMM SERIAL: OFF");
		
        lcd_show_frame();

	}
}


//----------------------------------------------------------
// Boton de destado:
void ALERT(void){
    // FSM pora mostrar el estado del sitema. "OK" para estdo IDLE  y "ALERTA"
    // para desviaciones matores a 5°.
    switch (state)
    {
    case IDLE:
        // OK:
        gfx_fillRoundRect(40, 220, 150, 50, 5, LCD_GREEN);
        gfx_drawRoundRect(40, 220, 150, 50, 5, LCD_BLACK);
        gfx_setTextSize(3);
        gfx_setTextColor(LCD_BLACK, LCD_GREEN);
        gfx_setCursor(90, 230);
        gfx_puts("Ok");

        gfx_setTextColor(LCD_BLACK, LCD_WHITE);
        break;
    
    case ACTIVE:
        // Alerta:
        gfx_fillRoundRect(40, 220, 150, 50, 5, LCD_RED);
        gfx_drawRoundRect(40, 220, 150, 50, 5, LCD_BLACK);
        gfx_setTextSize(3);
        gfx_setTextColor(LCD_WHITE, LCD_RED);
        gfx_setCursor(43, 230);
        gfx_puts("Alerta");

        gfx_setTextColor(LCD_BLACK, LCD_WHITE);
        break;
    
    default:
        break;
    }
}
//----------------------------------------------------------