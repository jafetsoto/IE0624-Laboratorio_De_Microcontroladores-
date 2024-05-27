// =======================================================
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"
#include <stdio.h>
// =======================================================
#define IDLE 0
#define ACTIVE 1
// =======================================================
int state;
// =======================================================
// Prototipos:
void ALERT(void);
void adc_setup(void);
// =======================================================
int main(void) {   
    // Inicializar el Display:
    clock_setup();
    console_setup(115200);

    sdram_init();
    lcd_spi_init();

    gfx_init(lcd_draw_pixel, 240, 320);

    adc_setup();                                        // Configurar el ADC

    // Constantes para la conversión
    const uint16_t* const ADC_TEMP_3V3_30C = (uint16_t*)0x1FFF7A2C;
    const uint16_t* const ADC_TEMP_3V3_110C = (uint16_t*)0x1FFF7A2E;
    const float CALIBRATION_REFERENCE_VOLTAGE = 3.3F;
    const float REFERENCE_VOLTAGE = 3.3F; // o el valor de tu Vref+

    // Calibrar las constantes según el voltaje de referencia actual
    float adcCalTemp30C = (float)(*ADC_TEMP_3V3_30C) * (REFERENCE_VOLTAGE / CALIBRATION_REFERENCE_VOLTAGE);
    float adcCalTemp110C = (float)(*ADC_TEMP_3V3_110C) * (REFERENCE_VOLTAGE / CALIBRATION_REFERENCE_VOLTAGE);

    while (1) {
        // UI
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

        // Lectura de datos del ADC
        while (!adc_eoc(ADC1));
        uint16_t adc_value = adc_read_regular(ADC1);

        // Calcular la temperatura
        float temperature = (((float)adc_value - adcCalTemp30C) / (adcCalTemp110C - adcCalTemp30C) * (110.0F - 30.0F) + 30.0F)-51;

        char temp_str[10];                              // Buffer para almacenar la temperatura como cadena de caracteres
        sprintf(temp_str, "%.2f", temperature);         // Convertimos la temperatura a cadena de caracteres


        // Temperatura:
        gfx_setTextSize(2);
        gfx_setCursor(55, 100);
        gfx_puts("Temp=");
        gfx_puts(temp_str); 
        // Ejes (X,Y,Z):
        gfx_setTextSize(2);
        gfx_setCursor(70, 130);
        gfx_puts("X=000");
        gfx_setTextSize(2);
        gfx_setCursor(70, 155);
        gfx_puts("Y=000");
        gfx_setTextSize(2);
        gfx_setCursor(70, 180);
        gfx_puts("Z=000");

        state = IDLE;
        ALERT();
        // COMM:
        gfx_setTextSize(1);
        gfx_setCursor(15, 285);
        gfx_puts("COMM SERIAL: OFF");

        lcd_show_frame();
    }
}
// =======================================================
// ================== Boton de estado: ===================
void ALERT(void) {
    // FSM para mostrar el estado del sistema. "OK" para estado IDLE y "ALERTA"
    // para desviaciones mayores a 5°.
    switch (state) {
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
// =======================================================
// ==================== ADC-SETUP: =======================
void adc_setup(void) {
    // Habilitar el reloj para el ADC1
    rcc_periph_clock_enable(RCC_ADC1);

    // Configurar el ADC1
    adc_power_off(ADC1);
    adc_disable_scan_mode(ADC1);
    adc_set_continuous_conversion_mode(ADC1);
    adc_disable_discontinuous_mode_regular(ADC1);
    adc_disable_analog_watchdog_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_480CYC);

    // Habilitar el sensor de temperatura y el Vrefint
    adc_enable_temperature_sensor();

    // Canal del sensor de temperatura, canal 16
    uint8_t channel_array[16];
    channel_array[0] = 16; // El canal del sensor de temperatura es generalmente el 16 en STM32F4
    adc_set_regular_sequence(ADC1, 1, channel_array);
    adc_power_on(ADC1);
    adc_start_conversion_regular(ADC1);
}
// =======================================================
