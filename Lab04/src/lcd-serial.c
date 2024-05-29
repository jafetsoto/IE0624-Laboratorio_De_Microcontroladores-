// =======================================================
# include "stm32/stm32_all.h"
// =======================================================
#define IDLE 0
#define ACTIVE 1

#define PW_OK 0
#define PW_LOW 1

#define L3GD20_SENSITIVITY_500DPS  (0.0175F)
// =======================================================
typedef struct GYRO{
  int16_t Eje_X;
  int16_t Eje_Y;
  int16_t Eje_Z;
  int16_t Temp;
} GYRO;

int state;
int pw_state;

// ================= Prototipos: =========================
void ALERT(void);
void PW_State(void);

static void adc_setup(void);

static void spi_setup(void);

void GPIO_CONFIG(void);

GYRO GYRO_DATA(void);

static void usart_setup(void);
static uint16_t read_adc_naiive(uint8_t channel);

// =======================================================
int main(void){
    
    console_setup(115200);                  // 115200 baudios.
    clock_setup();

    GPIO_CONFIG();
    usart_setup();
    sdram_init();

    spi_setup();
    adc_setup();

	gfx_init(lcd_draw_pixel, 240, 320);     // Cargar pantalla despues de Set-Ups.
    lcd_spi_init();

    GYRO GYRO_01;
    char DATA_X[10], DATA_Y[10], DATA_Z[10], TEMP[10];
    char mensaje[35], comma[] = ",";

    char Batery_Level[10];

    int TX_EN = 0;                          // Comunicaciones por USART/USB.

    while (1){
		// Giroscopio:
		sprintf(DATA_X, "%d", GYRO_01.Eje_X);
		sprintf(DATA_Y, "%d", GYRO_01.Eje_Y);
		sprintf(DATA_Z, "%d", GYRO_01.Eje_Z);
        sprintf(TEMP, "%d", GYRO_01.Temp - 21);

        // Batería:
        float Batery_Data = (float)((read_adc_naiive(3)*9)/4095.0)*100;
        sprintf(Batery_Level, "%.2f", Batery_Data);

        // UI:
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

        // Temperatura:
        gfx_setTextSize(2);
        gfx_setCursor(60, 100);
        gfx_puts("Temp=");
        gfx_puts(TEMP);

        // Ejes (X,Y,Z):
        gfx_setTextSize(2);
        gfx_setCursor(90, 130);
        gfx_puts("X=");
        gfx_puts(DATA_X);

        gfx_setTextSize(2);
        gfx_setCursor(90, 155);
        gfx_puts("Y=");
        gfx_puts(DATA_Y);

        gfx_setTextSize(2);
        gfx_setCursor(90, 180);
        gfx_puts("Z=");
        gfx_puts(DATA_Z);

        float Delta_X = strtof(DATA_X, NULL);
        float Delta_Y = strtof(DATA_Y, NULL);
        float Delta_Z = strtof(DATA_Z, NULL);

        if ((Delta_X >= 5.0 || Delta_X <= -5.0) || 
            (Delta_Y >= 5.0 || Delta_Y <= -5.0) || 
            (Delta_Z >= 5.0 || Delta_Z <= -5.0)) {
            state = ACTIVE;
        }
        else 
        {
            state = IDLE;
        }
        
        ALERT();

        // Batería:
        pw_state = PW_OK;
        
        gfx_setTextSize(1);
        gfx_setCursor(32, 275);
        gfx_puts(Batery_Level);
        gfx_puts("%");

        if (Batery_Data < 6.5)
        {
            pw_state = PW_LOW;
        } 
        else
        {
            pw_state = PW_OK;
        }
        PW_State();

        gfx_drawFastVLine(115, 270, 20, LCD_GREY);

        // COMM:
        gfx_setTextSize(1);
        gfx_setCursor(125, 270);
        gfx_puts("COMM SERIAL:");
        gfx_setCursor(125, 280);

		if (TX_EN)
        {
			gfx_puts("ON");
		}
		else
        {
			gfx_puts("OFF");
		}

		lcd_show_frame();

		GYRO_01 = GYRO_DATA();
		gpio_set(GPIOC, GPIO1);

		if (TX_EN)
        {
			gpio_toggle(GPIOG, GPIO13);

			strcat(mensaje, DATA_X);
			strcat(mensaje, comma);
		
			strcat(mensaje, DATA_Y);
			strcat(mensaje, comma);
	
			strcat(mensaje, DATA_Z);
			strcat(mensaje, comma);
			
			console_puts(mensaje);
			console_puts("\n");
			memset(mensaje, 0, 35);
		}

		if (gpio_get(GPIOA, GPIO0))
        {
			if(TX_EN)
            {
				TX_EN = 0;	
				gpio_clear(GPIOG, GPIO13); //TX LED OFF
			} 
            else
            {
				TX_EN = ~TX_EN;
			}
		}
  }

  return 0;
}
// =======================================================
// ================= Alerta de estado: ===================
void ALERT(void) {
    // FSM para mostrar el estado del sistema. "OK" para estado IDLE y "ALERTA"
    // para desviaciones mayores a 5°.
    switch (state) {
    case IDLE:
        // OK:
        gpio_clear(GPIOG, GPIO14);

        gfx_fillRoundRect(45, 210, 150, 50, 5, LCD_CYAN);
        gfx_drawRoundRect(45, 210, 150, 50, 5, LCD_BLACK);
        gfx_setTextSize(3);
        gfx_setTextColor(LCD_BLACK, LCD_CYAN);
        gfx_setCursor(95, 220);
        gfx_puts("Ok");

        gfx_setTextColor(LCD_BLACK, LCD_WHITE);
        break;
    
    case ACTIVE:
        // Alerta:
        gpio_toggle(GPIOG, GPIO13); // Blink LED.

        gfx_fillRoundRect(45, 210, 150, 50, 5, LCD_RED);
        gfx_drawRoundRect(45, 210, 150, 50, 5, LCD_BLACK);
        gfx_setTextSize(3);
        gfx_setTextColor(LCD_WHITE, LCD_RED);
        gfx_setCursor(45, 220);
        gfx_puts("Alerta");

        gfx_setTextColor(LCD_BLACK, LCD_WHITE);
        break;
    
    default:
        break;
    }
}
// =======================================================
static void spi_setup(void){
    rcc_periph_clock_enable(RCC_SPI5);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOF); 
    
    // GPIO:
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
    gpio_set(GPIOC, GPIO1);

    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7 | GPIO8 | GPIO9);
    gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);

    // Giroscopio:
    spi_set_master_mode(SPI5);                  // Se define SPI5 como maestro.
    spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);

    spi_set_clock_polarity_0(SPI5);
    spi_set_clock_phase_0(SPI5);
    spi_set_full_duplex_mode(SPI5);             //  Comunicacion bilateral, envio y llamado. 
    spi_set_unidirectional_mode(SPI5);

    spi_enable_software_slave_management(SPI5);
    spi_send_msb_first(SPI5);                   // MSB
    spi_set_nss_high(SPI5);                     // Señal High en el modo de espera.

    SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD;   // Deshabilitar modo I2S

    spi_enable(SPI5);
    gpio_clear(GPIOC, GPIO1);

    spi_send(SPI5, 0x20);
    spi_read(SPI5);
    
    spi_send(SPI5, (1 << 3) | (1 << 1) | (1 << 0) | (1 << 2) | (3 << 4)); // Giroscopio pullup.
    spi_read(SPI5);
    gpio_set(GPIOC, GPIO1);
    gpio_clear(GPIOC, GPIO1);

    spi_send(SPI5, 0x23);
    spi_read(SPI5);

    spi_send(SPI5, (1 << 4));
    spi_read(SPI5);

    gpio_set(GPIOC, GPIO1);
}
// =======================================================
void GPIO_CONFIG(void){
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_USART1);  
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOG);

    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);

    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);

}
// =======================================================
static void adc_setup(void){
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);

    // Configurar el ADC_01
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);

    adc_power_off(ADC1);
    adc_disable_scan_mode(ADC1);

    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

    adc_power_on(ADC1);

}
// =======================================================
static uint16_t read_adc_naiive(uint8_t channel){
    uint8_t channel_array[1];
    channel_array[0] = channel;
    adc_set_regular_sequence(ADC1, 1, channel_array);

    // ADC Conv:
    adc_start_conversion_regular(ADC1);
    while (!adc_eoc(ADC1));

    uint16_t reg16 = adc_read_regular(ADC1);
    return reg16;
}
// =======================================================
GYRO GYRO_DATA(void){
	GYRO get;

	gpio_clear(GPIOC, GPIO1);

    // L3GD20:
	spi_send(SPI5, 0x0F | (1 << 7));
	spi_read(SPI5);

	spi_send(SPI5, 0);
	spi_read(SPI5);

	gpio_set(GPIOC, GPIO1);

    // Giroscopio:
	gpio_clear(GPIOC, GPIO1);               // Begin

	spi_send(SPI5, 0x27 | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	spi_read(SPI5);

	gpio_set(GPIOC, GPIO1);                 // End
    // ---------------------------------------------------
    // Temperatura:
	gpio_clear(GPIOC, GPIO1);               // Inicio de transmisión 
	spi_send(SPI5, 0x26 | (1 << 7));        //  Señal de control para leer temperatura
	spi_read(SPI5);                         //  Se lee dato
    
	spi_send(SPI5, 0);                      //  Se envía bit de control 
	get.Temp =spi_read(SPI5);
    
	gpio_set(GPIOC, GPIO1);                 //  Se termina transmisión

	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, 0x26 | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Temp |=spi_read(SPI5) << 8;
	gpio_set(GPIOC, GPIO1);
    // ---------------------------------------------------
    // Eje X:
	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, 0x28 | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Eje_X = spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, 0x29 | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Eje_X |=spi_read(SPI5) << 8;
	gpio_set(GPIOC, GPIO1);
    // ---------------------------------------------------
    // Eje Y:
	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, 0x2A | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Eje_Y =spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, 0x2B | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Eje_Y|=spi_read(SPI5) << 8;
	gpio_set(GPIOC, GPIO1);
    // ---------------------------------------------------
    // Eje Z:
	gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, 0x2C | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Eje_Z=spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);

	gpio_clear(GPIOC, GPIO1);          // Lee el valor convertido
	spi_send(SPI5, 0x2D | (1 << 7));
	spi_read(SPI5);
	spi_send(SPI5, 0);
	get.Eje_Z|=spi_read(SPI5) << 8;
	gpio_set(GPIOC, GPIO1);
    // ---------------------------------------------------
	get.Eje_X = get.Eje_X*L3GD20_SENSITIVITY_500DPS;
	get.Eje_Y = get.Eje_Y*L3GD20_SENSITIVITY_500DPS;
	get.Eje_Z = get.Eje_Z*L3GD20_SENSITIVITY_500DPS;

    get.Temp = (get.Temp*L3GD20_SENSITIVITY_500DPS);

	return get;
}
// =======================================================
// USART:
static void usart_setup(void){
    usart_set_baudrate(USART1, 115200);               // bps
    usart_set_databits(USART1, 8);                    // Tramas de 8 bits 
    usart_set_stopbits(USART1, USART_STOPBITS_1);     // Bit de parada
    usart_set_mode(USART1, USART_MODE_TX);            
    usart_set_parity(USART1, USART_PARITY_NONE);      // Parity bits off

    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    usart_enable(USART1);

    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);

    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);

}
// =======================================================
void PW_State(void){
    switch (pw_state)
    {
    case PW_OK:
        // Draw  Bateria:
        gfx_fillRoundRect(75, 275, 30, 10, 1, LCD_YELLOW);
        gfx_drawRoundRect(75, 275, 30, 10, 1, LCD_BLACK);
        gfx_fillRoundRect(105, 277, 2, 5, 0, LCD_YELLOW);
        gfx_drawRoundRect(105, 277, 2, 5, 0, LCD_BLACK);
        break;
    case PW_LOW:
        // Draw Alerta-Bateria:
        gfx_fillCircle(90,280, 10, LCD_RED);
        gfx_fillCircle(90,280, 8, LCD_WHITE);
        gfx_setTextSize(1);
        gfx_setCursor(85, 275);
        gfx_puts("!");
    
    default:
        break;
    }
}
// =======================================================
