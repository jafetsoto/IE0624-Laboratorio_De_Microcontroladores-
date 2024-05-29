OBJS = sdram.o clock.o console.o lcd-spi.o gfx.o

BINARY = lcd-serial

# we use sin/cos from the library
LDLIBS += -lm

LDSCRIPT = ../stm32f429i-discovery.ld

include ../../Makefile.include

# Rule to flash the binary to the microcontroller
run: all
	@arm-none-eabi-objcopy -O binary lcd-serial.elf lcd-serial.bin
	@st-flash --reset write $(BINARY).bin 0x8000000
