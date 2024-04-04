;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"blink.c"
	list	p=12f675
	radix dec
	include "p12f675.inc"
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_delay
	global	_RandomNum

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0020
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_blink_0	udata
r0x100C	res	1
r0x1005	res	1
r0x1004	res	1
r0x1006	res	1
r0x1007	res	1
r0x1008	res	1
r0x1009	res	1
r0x100A	res	1
r0x100B	res	1
r0x1001	res	1
r0x1002	res	1
r0x1003	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

IDD_blink_0	idata
_RandomNum_lfsr_65536_13
	db	0x07	; 7

;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_blink	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _RandomNum
;   _delay
;   _RandomNum
;   _delay
;2 compiler assigned registers:
;   r0x100C
;   STK00
;; Starting pCode block
S_blink__main	code
_main:
; 2 exit points
;	.line	11; "blink.c"	TRISIO = 0b00100000;                    // COnfigurar como entra el PIN5.
	MOVLW	0x20
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	12; "blink.c"	GPIO = 0x00;                            // Poner en bajo las GPIO.
	BANKSEL	_GPIO
	CLRF	_GPIO
_00115_DS_:
;	.line	17; "blink.c"	if (GP5 == 0) {                     // Condición de inicio.
	BANKSEL	_GPIObits
	BTFSC	_GPIObits,5
	GOTO	_00115_DS_
;	.line	18; "blink.c"	valor = RandomNum();
	PAGESEL	_RandomNum
	CALL	_RandomNum
	PAGESEL	$
	MOVWF	r0x100C
;	.line	19; "blink.c"	GPIO = 0x00;                    // Estado inicial del GPIO.
	BANKSEL	_GPIO
	CLRF	_GPIO
;;unsigned compare: left < lit(0x1=1), size=1
;	.line	21; "blink.c"	switch (valor) {
	MOVLW	0x01
	SUBWF	r0x100C,W
	BTFSS	STATUS,0
	GOTO	_00111_DS_
;;genSkipc:3307: created from rifx:0x7ffce9179560
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit(0x7=7), size=1
	MOVLW	0x07
	SUBWF	r0x100C,W
	BTFSC	STATUS,0
	GOTO	_00111_DS_
;;genSkipc:3307: created from rifx:0x7ffce9179560
	DECF	r0x100C,F
	MOVLW	HIGH(_00137_DS_)
	MOVWF	PCLATH
	MOVLW	_00137_DS_
	ADDWF	r0x100C,W
	BTFSC	STATUS,0
	INCF	PCLATH,F
	MOVWF	PCL
_00137_DS_:
	GOTO	_00105_DS_
	GOTO	_00106_DS_
	GOTO	_00107_DS_
	GOTO	_00108_DS_
	GOTO	_00109_DS_
	GOTO	_00110_DS_
_00105_DS_:
;	.line	23; "blink.c"	GP0 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,0
;	.line	24; "blink.c"	break;
	GOTO	_00111_DS_
_00106_DS_:
;	.line	27; "blink.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	28; "blink.c"	break;
	GOTO	_00111_DS_
_00107_DS_:
;	.line	31; "blink.c"	GP2 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,2
;	.line	32; "blink.c"	break;
	GOTO	_00111_DS_
_00108_DS_:
;	.line	35; "blink.c"	GP0 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,0
;	.line	36; "blink.c"	GP2 = 1;
	BSF	_GPIObits,2
;	.line	37; "blink.c"	break;
	GOTO	_00111_DS_
_00109_DS_:
;	.line	40; "blink.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	41; "blink.c"	GP2 = 1;
	BSF	_GPIObits,2
;	.line	42; "blink.c"	break;
	GOTO	_00111_DS_
_00110_DS_:
;	.line	45; "blink.c"	GP0 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,0
;	.line	46; "blink.c"	GP1 = 1;
	BSF	_GPIObits,1
;	.line	47; "blink.c"	GP2 = 1;
	BSF	_GPIObits,2
_00111_DS_:
;	.line	52; "blink.c"	delay(300);
	MOVLW	0x2c
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	53; "blink.c"	GPIO = 0x00;
	BANKSEL	_GPIO
	CLRF	_GPIO
	GOTO	_00115_DS_
;	.line	56; "blink.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;3 compiler assigned registers:
;   r0x1001
;   r0x1002
;   r0x1003
;; Starting pCode block
S_blink__RandomNum	code
_RandomNum:
; 2 exit points
_00174_DS_:
;	.line	80; "blink.c"	random_b = ((lfsr >> 0) ^ (lfsr >> 1)) & 1; 
	MOVF	_RandomNum_lfsr_65536_13,W
	MOVWF	r0x1001
;;shiftRight_Left2ResultLit:5474: shCount=1, size=1, sign=0, same=0, offr=0
	BCF	STATUS,0
	RRF	_RandomNum_lfsr_65536_13,W
	MOVWF	r0x1002
	XORWF	r0x1001,F
	MOVLW	0x01
	ANDWF	r0x1001,F
;	.line	81; "blink.c"	lfsr = (lfsr >> 1) | (random_b << 2);
	MOVF	r0x1001,W
	MOVWF	r0x1003
	BCF	STATUS,0
	RLF	r0x1003,W
	MOVWF	r0x1001
	BCF	STATUS,0
	RLF	r0x1001,F
	MOVF	r0x1001,W
	MOVWF	r0x1003
	IORWF	r0x1002,W
	MOVWF	_RandomNum_lfsr_65536_13
;;swapping arguments (AOP_TYPEs 1/3)
;;unsigned compare: left >= lit(0x7=7), size=1
;	.line	82; "blink.c"	} while (lfsr > 6);  // Nos aseguramos de solo obtener valores entre 1 y 6
	MOVLW	0x07
	SUBWF	_RandomNum_lfsr_65536_13,W
	BTFSC	STATUS,0
	GOTO	_00174_DS_
;;genSkipc:3307: created from rifx:0x7ffce9179560
;	.line	84; "blink.c"	return lfsr;  
	MOVF	_RandomNum_lfsr_65536_13,W
;	.line	85; "blink.c"	}
	RETURN	
; exit point of _RandomNum

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x1004
;   STK00
;   r0x1005
;   r0x1006
;   r0x1007
;   r0x1008
;   r0x1009
;   r0x100A
;   r0x100B
;; Starting pCode block
S_blink__delay	code
_delay:
; 2 exit points
;	.line	61; "blink.c"	void delay(unsigned int time) {
	MOVWF	r0x1004
	MOVF	STK00,W
	MOVWF	r0x1005
;	.line	66; "blink.c"	for (i = 0; i < time; i++) {
	CLRF	r0x1006
	CLRF	r0x1007
_00148_DS_:
	MOVF	r0x1004,W
	SUBWF	r0x1007,W
	BTFSS	STATUS,2
	GOTO	_00169_DS_
	MOVF	r0x1005,W
	SUBWF	r0x1006,W
_00169_DS_:
	BTFSC	STATUS,0
	GOTO	_00150_DS_
;;genSkipc:3307: created from rifx:0x7ffce9179560
;	.line	68; "blink.c"	for (j = 0; j < 1200; j++);
	MOVLW	0xb0
	MOVWF	r0x1008
	MOVLW	0x04
	MOVWF	r0x1009
_00146_DS_:
	MOVLW	0xff
	ADDWF	r0x1008,W
	MOVWF	r0x100A
	MOVLW	0xff
	MOVWF	r0x100B
	MOVF	r0x1009,W
	BTFSC	STATUS,0
	INCFSZ	r0x1009,W
	ADDWF	r0x100B,F
	MOVF	r0x100A,W
	MOVWF	r0x1008
	MOVF	r0x100B,W
	MOVWF	r0x1009
	MOVF	r0x100B,W
	IORWF	r0x100A,W
	BTFSS	STATUS,2
	GOTO	_00146_DS_
;	.line	66; "blink.c"	for (i = 0; i < time; i++) {
	INCF	r0x1006,F
	BTFSC	STATUS,2
	INCF	r0x1007,F
	GOTO	_00148_DS_
_00150_DS_:
;	.line	70; "blink.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  116+   15 =   131 instructions (  292 byte)

	end
