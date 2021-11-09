EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "gbaHD Shield"
Date "2021-04-06"
Rev "v1"
Comp ""
Comment1 "Idea by: sanchezand"
Comment2 "made by: ManCloud"
Comment3 "Version: v20210406"
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x40 J5
U 1 1 5FF9439E
P 6950 2600
F 0 "J5" H 6950 4600 50  0000 C CNN
F 1 "FFC_OGBA_40" H 6750 500 50  0000 C CNN
F 2 "Connector_FFC-FPC:TE_4-1734839-0_1x40-1MP_P0.5mm_Horizontal" H 6950 2600 50  0001 C CNN
F 3 "~" H 6950 2600 50  0001 C CNN
	1    6950 2600
	1    0    0    -1  
$EndComp
Text GLabel 5400 2900 0    50   Input ~ 0
GB_B1
Text GLabel 5400 2800 0    50   Input ~ 0
GB_B2
Text GLabel 5400 2700 0    50   Input ~ 0
GB_B3
Text GLabel 5400 2600 0    50   Input ~ 0
GB_B4
Text GLabel 5400 2500 0    50   Input ~ 0
GB_B5
Text GLabel 5400 2300 0    50   Input ~ 0
GB_G1
Text GLabel 5400 2200 0    50   Input ~ 0
GB_G2
Text GLabel 5400 2100 0    50   Input ~ 0
GB_G3
Text GLabel 5400 2000 0    50   Input ~ 0
GB_G4
Text GLabel 5400 1900 0    50   Input ~ 0
GB_G5
Text GLabel 5400 1800 0    50   Input ~ 0
GB_R1
Text GLabel 5400 1700 0    50   Input ~ 0
GB_R2
Text GLabel 5400 1600 0    50   Input ~ 0
GB_R3
Text GLabel 5400 3200 0    50   Input ~ 0
GB_SPS
Text GLabel 7700 850  0    50   Input ~ 0
GB_SPS
Text GLabel 7750 850  2    50   Input ~ 0
SP_IO0
Text GLabel 7700 1000 0    50   Input ~ 0
GB_B1
Text GLabel 7700 1100 0    50   Input ~ 0
GB_B2
Text GLabel 7700 1200 0    50   Input ~ 0
GB_B3
Text GLabel 7700 1300 0    50   Input ~ 0
GB_B4
Text GLabel 7700 1400 0    50   Input ~ 0
GB_B5
Text GLabel 7700 1550 0    50   Input ~ 0
GB_G1
Text GLabel 7700 1750 0    50   Input ~ 0
GB_G3
Text GLabel 7700 1850 0    50   Input ~ 0
GB_G4
Text GLabel 7700 2100 0    50   Input ~ 0
GB_R1
Text GLabel 7700 2200 0    50   Input ~ 0
GB_R2
Text GLabel 7700 2300 0    50   Input ~ 0
GB_R3
Text GLabel 7700 2400 0    50   Input ~ 0
GB_R4
Text GLabel 7700 2500 0    50   Input ~ 0
GB_R5
Text GLabel 7700 750  0    50   Input ~ 0
GB_DCLK
Text GLabel 7750 750  2    50   Input ~ 0
SP_IO9
Text GLabel 7750 1000 2    50   Input ~ 0
SP_IO1
Text GLabel 7750 1100 2    50   Input ~ 0
SP_ARD5
Text GLabel 7750 1200 2    50   Input ~ 0
SP_IO2
Text GLabel 7750 1300 2    50   Input ~ 0
SP_ARD6
Text GLabel 7750 1400 2    50   Input ~ 0
SP_IO3
Text GLabel 7700 1950 0    50   Input ~ 0
GB_G5
Text GLabel 7750 1550 2    50   Input ~ 0
SP_ARD7
Text GLabel 7750 1650 2    50   Input ~ 0
SP_IO4
Text GLabel 7750 1750 2    50   Input ~ 0
SP_ARD8
Text GLabel 7750 1850 2    50   Input ~ 0
SP_IO5
Text GLabel 7750 1950 2    50   Input ~ 0
SP_ARD9
Text GLabel 7750 2100 2    50   Input ~ 0
SP_IO6
Text GLabel 7750 2200 2    50   Input ~ 0
SP_ARD10
Text GLabel 7750 2300 2    50   Input ~ 0
SP_IO7
Text GLabel 7750 2400 2    50   Input ~ 0
SP_ARD11
Text GLabel 7750 2500 2    50   Input ~ 0
SP_ARD13
Wire Wire Line
	7700 750  7750 750 
Wire Wire Line
	7750 850  7700 850 
Wire Wire Line
	7700 1000 7750 1000
Wire Wire Line
	7750 1100 7700 1100
Wire Wire Line
	7700 1200 7750 1200
Wire Wire Line
	7750 1300 7700 1300
Wire Wire Line
	7700 1400 7750 1400
Wire Wire Line
	7700 1550 7750 1550
Wire Wire Line
	7750 1650 7700 1650
Wire Wire Line
	7700 1750 7750 1750
Wire Wire Line
	7750 1850 7700 1850
Wire Wire Line
	7700 1950 7750 1950
Wire Wire Line
	7700 2100 7750 2100
Wire Wire Line
	7750 2200 7700 2200
Wire Wire Line
	7700 2300 7750 2300
Wire Wire Line
	7750 2400 7700 2400
Wire Wire Line
	7700 2500 7750 2500
$Comp
L GBAHD_Shield-rescue:SNES_7PIN-Raizu U4
U 1 1 5FFBD0A5
P 4100 1050
F 0 "U4" V 4550 1000 50  0000 L CNN
F 1 "SNES_7PIN" V 4650 850 50  0000 L CNN
F 2 "OFFSET:SNES_7PIN" H 4100 1250 50  0001 C CNN
F 3 "" H 3950 900 50  0001 C CNN
	1    4100 1050
	0    1    1    0   
$EndComp
$Comp
L MCU_Microchip_ATmega:ATmega328P-AU U5
U 1 1 5FFBF3E6
P 9700 2400
F 0 "U5" H 9700 2400 50  0000 C CNN
F 1 "ATmega328P-AU" V 9100 2400 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 9700 2400 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 9700 2400 50  0001 C CNN
	1    9700 2400
	1    0    0    -1  
$EndComp
Text GLabel 7700 2800 0    50   Input ~ 0
GBA_CLK
Text GLabel 7700 2900 0    50   Input ~ 0
GBA_AUD1
Text GLabel 7700 3000 0    50   Input ~ 0
GBA_AUD2
Text GLabel 7750 2800 2    50   Input ~ 0
SP_IO8
Text GLabel 7750 2900 2    50   Input ~ 0
SP_ARD2
Text GLabel 7750 3000 2    50   Input ~ 0
SP_ARD3
Wire Wire Line
	7700 2800 7750 2800
Wire Wire Line
	7750 2900 7700 2900
Wire Wire Line
	7700 3000 7750 3000
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J11
U 1 1 5FFC6AC4
P 10250 6050
F 0 "J11" H 10300 6367 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 10300 6276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 10250 6050 50  0001 C CNN
F 3 "~" H 10250 6050 50  0001 C CNN
	1    10250 6050
	1    0    0    -1  
$EndComp
Text GLabel 10000 6050 0    50   Input ~ 0
ICSP_SCK
Text GLabel 10000 6150 0    50   Input ~ 0
AT_RST
$Comp
L power:GND #PWR040
U 1 1 5FFCCACC
P 10600 6150
F 0 "#PWR040" H 10600 5900 50  0001 C CNN
F 1 "GND" V 10605 6022 50  0000 R CNN
F 2 "" H 10600 6150 50  0001 C CNN
F 3 "" H 10600 6150 50  0001 C CNN
	1    10600 6150
	0    -1   -1   0   
$EndComp
Text GLabel 10350 1600 2    50   Input ~ 0
ICSP_MISO
Text GLabel 10350 1700 2    50   Input ~ 0
ICSP_SCK
Text GLabel 10350 1500 2    50   Input ~ 0
ICSP_MOSI
$Comp
L power:GND #PWR041
U 1 1 5FFD7A59
P 10700 5100
F 0 "#PWR041" H 10700 4850 50  0001 C CNN
F 1 "GND" V 10705 4972 50  0000 R CNN
F 2 "" H 10700 5100 50  0001 C CNN
F 3 "" H 10700 5100 50  0001 C CNN
	1    10700 5100
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR036
U 1 1 5FFD839C
P 9700 3950
F 0 "#PWR036" H 9700 3700 50  0001 C CNN
F 1 "GND" H 9705 3777 50  0000 C CNN
F 2 "" H 9700 3950 50  0001 C CNN
F 3 "" H 9700 3950 50  0001 C CNN
	1    9700 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5FFEE439
P 8800 3300
F 0 "C3" V 8548 3300 50  0000 C CNN
F 1 "100nF" V 8639 3300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 8838 3150 50  0001 C CNN
F 3 "~" H 8800 3300 50  0001 C CNN
	1    8800 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	10200 5050 10200 5000
$Comp
L Device:R R4
U 1 1 5FFD4753
P 10200 4850
F 0 "R4" H 10270 4896 50  0000 L CNN
F 1 "10K" H 10270 4805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 10130 4850 50  0001 C CNN
F 3 "~" H 10200 4850 50  0001 C CNN
	1    10200 4850
	-1   0    0    -1  
$EndComp
Text GLabel 10100 5050 0    50   Input ~ 0
AT_RST
Wire Wire Line
	10200 4700 10200 4650
Text GLabel 10350 2700 2    50   Input ~ 0
AT_RST
Text GLabel 3800 900  0    50   Input ~ 0
SNS_CLK
Text GLabel 3800 1050 0    50   Input ~ 0
SNS_LATCH
Text GLabel 10350 1400 2    50   Input ~ 0
SNS_LATCH
Text GLabel 7750 3800 2    50   Input ~ 0
SNS_CLK
Text GLabel 7750 3700 2    50   Input ~ 0
SNS_DATA
Text GLabel 3800 1200 0    50   Input ~ 0
SNS_DATA
Text GLabel 10350 3100 2    50   Input ~ 0
GBA_POWER_SEL
Text GLabel 7700 1650 0    50   Input ~ 0
GB_G2
Text GLabel 5400 1500 0    50   Input ~ 0
GB_R4
Text GLabel 5400 1400 0    50   Input ~ 0
GB_R5
Text GLabel 5400 900  0    50   Input ~ 0
GB_DCLK
Wire Wire Line
	5400 900  5450 900 
Wire Wire Line
	5400 1400 5450 1400
Wire Wire Line
	5450 1500 5400 1500
Wire Wire Line
	5400 1600 5450 1600
Wire Wire Line
	5450 1700 5400 1700
Wire Wire Line
	5400 1800 5450 1800
Wire Wire Line
	5450 1900 5400 1900
Wire Wire Line
	5400 2000 5450 2000
Wire Wire Line
	5450 2100 5400 2100
Wire Wire Line
	5400 2200 5450 2200
Wire Wire Line
	5450 2300 5400 2300
Wire Wire Line
	5400 2500 5450 2500
Wire Wire Line
	5450 2600 5400 2600
Wire Wire Line
	5400 2700 5450 2700
Wire Wire Line
	5450 2800 5400 2800
Wire Wire Line
	5400 2900 5450 2900
Wire Wire Line
	5450 3200 5400 3200
$Comp
L Connector_Generic:Conn_01x34 J3
U 1 1 60521025
P 6300 2400
F 0 "J3" H 6300 4100 50  0000 C CNN
F 1 "FFC_GBA-SP" H 6050 600 50  0000 C CNN
F 2 "Connector_FFC-FPC:TE_3-1734839-4_1x34-1MP_P0.5mm_Horizontal" H 6300 2400 50  0001 C CNN
F 3 "~" H 6300 2400 50  0001 C CNN
	1    6300 2400
	1    0    0    -1  
$EndComp
Connection ~ 10200 5050
Wire Wire Line
	10200 5050 10250 5050
Wire Wire Line
	10700 5100 10700 5050
Wire Wire Line
	10700 5050 10650 5050
Wire Wire Line
	10100 5050 10200 5050
Wire Wire Line
	3850 750  3800 750 
Wire Wire Line
	10550 5950 10600 5950
Text GLabel 10600 6050 2    50   Input ~ 0
ICSP_MOSI
Wire Wire Line
	10600 6050 10550 6050
Wire Wire Line
	10600 6150 10550 6150
Wire Wire Line
	10000 5950 10050 5950
Wire Wire Line
	10050 6050 10000 6050
Wire Wire Line
	10000 6150 10050 6150
Wire Wire Line
	8800 3100 8800 3150
Wire Wire Line
	8800 3500 8800 3450
Wire Wire Line
	9100 1200 9050 1200
Wire Wire Line
	9700 850  9700 900 
Wire Wire Line
	9800 850  9800 900 
Wire Wire Line
	10350 2700 10300 2700
Wire Wire Line
	10350 1700 10300 1700
Wire Wire Line
	10300 1600 10350 1600
Wire Wire Line
	10350 1500 10300 1500
Wire Wire Line
	9700 3950 9700 3900
Wire Wire Line
	3800 900  3850 900 
Wire Wire Line
	3850 1050 3800 1050
Wire Wire Line
	3800 1200 3850 1200
Wire Wire Line
	10350 1400 10300 1400
Text GLabel 7700 3700 0    50   Input ~ 0
ICSP_MISO
Text GLabel 7700 3800 0    50   Input ~ 0
ICSP_SCK
Wire Wire Line
	7700 3700 7750 3700
Wire Wire Line
	7750 3800 7700 3800
$Comp
L power:GND #PWR016
U 1 1 6001269A
P 3800 1350
F 0 "#PWR016" H 3800 1100 50  0001 C CNN
F 1 "GND" V 3805 1222 50  0000 R CNN
F 2 "" H 3800 1350 50  0001 C CNN
F 3 "" H 3800 1350 50  0001 C CNN
	1    3800 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 1350 3850 1350
Text GLabel 10000 5950 0    50   Input ~ 0
ICSP_MISO
Wire Wire Line
	10300 2900 10350 2900
Wire Wire Line
	10300 3000 10350 3000
Text GLabel 10350 2900 2    50   Input ~ 0
TX
Text GLabel 10350 3000 2    50   Input ~ 0
RX
Text GLabel 7700 3400 0    50   Input ~ 0
TX
Text GLabel 7700 3300 0    50   Input ~ 0
RX
Text GLabel 7750 3300 2    50   Input ~ 0
SP_ARD0
Text GLabel 7750 3400 2    50   Input ~ 0
SP_ARD1
Wire Wire Line
	7750 3400 7700 3400
Wire Wire Line
	7700 3300 7750 3300
Wire Notes Line
	9450 5550 11150 5550
Text Notes 11100 6450 2    50   ~ 0
ICSP
Wire Notes Line
	8600 550  8600 4200
Wire Notes Line
	11150 550  11150 6450
Wire Notes Line
	9450 4200 9450 6450
Text Notes 11100 5550 2    50   ~ 0
Reset Button
Text Notes 11100 4200 2    50   ~ 0
Microcontroller
Text Notes 9400 6450 2    50   ~ 0
Shield Connector
Text Notes 8550 4200 2    50   ~ 0
Signal Connections
Wire Notes Line
	8550 4100 7150 4100
Wire Notes Line
	7150 4100 7150 600 
Wire Notes Line
	7150 600  8550 600 
Wire Notes Line
	8550 600  8550 4100
Wire Notes Line
	8550 3550 7150 3550
Wire Notes Line
	8550 3150 7150 3150
Text Notes 8500 3950 2    50   ~ 0
SNES Data
Text Notes 8500 3550 2    50   ~ 0
UART
Text Notes 8500 2650 2    50   ~ 0
GBA Display
Wire Notes Line
	8550 2650 7150 2650
Text Notes 8500 3150 2    50   ~ 0
GBA Audio
Text Notes 7050 4950 2    50   ~ 0
GBA Display Connectors
Wire Notes Line
	550  7750 6900 7750
Wire Notes Line
	6900 7750 6900 6450
Wire Notes Line
	6900 6450 11150 6450
Wire Notes Line
	550  5000 7100 5000
$Comp
L Connector_Generic:Conn_01x16 J4
U 1 1 5FFAB979
P 6300 6100
F 0 "J4" H 6380 6092 50  0000 L CNN
F 1 "Conn_01x16" H 6380 6001 50  0000 L CNN
F 2 "Connector_FFC-FPC:TE_1-1734839-6_1x16-1MP_P0.5mm_Horizontal" H 6300 6100 50  0001 C CNN
F 3 "~" H 6300 6100 50  0001 C CNN
	1    6300 6100
	1    0    0    -1  
$EndComp
Text GLabel 6050 5500 0    50   Input ~ 0
GBA_UP
$Comp
L power:GND #PWR024
U 1 1 5FFB0831
P 6050 6900
F 0 "#PWR024" H 6050 6650 50  0001 C CNN
F 1 "GND" V 6055 6772 50  0000 R CNN
F 2 "" H 6050 6900 50  0001 C CNN
F 3 "" H 6050 6900 50  0001 C CNN
	1    6050 6900
	0    1    1    0   
$EndComp
Text GLabel 6050 5600 0    50   Input ~ 0
GBA_DOWN
Text GLabel 6050 5700 0    50   Input ~ 0
GBA_LEFT
Text GLabel 6050 5800 0    50   Input ~ 0
GBA_RGHT
Text GLabel 6050 5900 0    50   Input ~ 0
GBA_A
Text GLabel 6050 6000 0    50   Input ~ 0
GBA_B
Text GLabel 6050 6100 0    50   Input ~ 0
GBA_L
Text GLabel 6050 6200 0    50   Input ~ 0
GBA_R
Text GLabel 6050 6300 0    50   Input ~ 0
GBA_START
Text GLabel 6050 6400 0    50   Input ~ 0
GBA_SELECT
Text GLabel 6050 6600 0    50   Input ~ 0
GBA_AUD1
Text GLabel 6050 6500 0    50   Input ~ 0
GBA_AUD2
Text GLabel 6050 6700 0    50   Input ~ 0
GBA_CLK'
Text GLabel 2200 6200 2    50   Input ~ 0
GBA_UP
Text GLabel 2200 6300 2    50   Input ~ 0
GBA_DOWN
Text GLabel 2200 6400 2    50   Input ~ 0
GBA_LEFT
Text GLabel 2200 6500 2    50   Input ~ 0
GBA_RGHT
Text GLabel 2200 6600 2    50   Input ~ 0
GBA_A
Text GLabel 2200 6700 2    50   Input ~ 0
GBA_B
Text GLabel 2200 6800 2    50   Input ~ 0
GBA_L
Text GLabel 2200 6900 2    50   Input ~ 0
GBA_R
Text GLabel 4250 6200 2    50   Input ~ 0
GBA_START
Text GLabel 4250 6300 2    50   Input ~ 0
GBA_SELECT
Text GLabel 6050 5400 0    50   Input ~ 0
GBA_VCC
$Comp
L 74xx:74HC595 U2
U 1 1 60616F11
P 1750 6600
F 0 "U2" H 1500 7150 50  0000 C CNN
F 1 "74HC595" H 1950 5950 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 1750 6600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 1750 6600 50  0001 C CNN
	1    1750 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6200 2200 6200
Wire Wire Line
	2150 6300 2200 6300
Wire Wire Line
	2150 6400 2200 6400
Wire Wire Line
	2150 6500 2200 6500
Wire Wire Line
	2150 6600 2200 6600
Wire Wire Line
	2150 6700 2200 6700
Wire Wire Line
	2150 6800 2200 6800
Wire Wire Line
	2150 6900 2200 6900
Wire Wire Line
	4200 6200 4250 6200
Wire Wire Line
	4200 6300 4250 6300
Text GLabel 2200 7100 2    50   Input ~ 0
QH_OUT-SER_IN
Text GLabel 3350 6200 0    50   Input ~ 0
QH_OUT-SER_IN
Wire Wire Line
	3350 6200 3400 6200
Wire Wire Line
	2200 7100 2150 7100
Wire Wire Line
	6050 6900 6100 6900
Wire Wire Line
	6050 6700 6100 6700
Wire Wire Line
	6100 6600 6050 6600
Wire Wire Line
	6050 6500 6100 6500
Wire Wire Line
	6100 6400 6050 6400
Wire Wire Line
	6050 6300 6100 6300
Wire Wire Line
	6100 6200 6050 6200
Wire Wire Line
	6050 6100 6100 6100
Wire Wire Line
	6100 6000 6050 6000
Wire Wire Line
	6050 5900 6100 5900
Wire Wire Line
	6100 5800 6050 5800
Wire Wire Line
	6050 5700 6100 5700
Wire Wire Line
	6100 5600 6050 5600
Wire Wire Line
	6050 5500 6100 5500
Text GLabel 1300 6200 0    50   Input ~ 0
ICSP_MOSI
Wire Wire Line
	1300 6200 1350 6200
Text GLabel 1300 6700 0    50   Input ~ 0
SNS_LATCH
Wire Wire Line
	1300 6700 1350 6700
Text GLabel 3300 6700 0    50   Input ~ 0
SNS_LATCH
Wire Wire Line
	3300 6700 3400 6700
Text GLabel 1300 6400 0    50   Input ~ 0
ICSP_SCK
Wire Wire Line
	1300 6400 1350 6400
Text GLabel 3300 6400 0    50   Input ~ 0
ICSP_SCK
Wire Wire Line
	3300 6400 3400 6400
Wire Wire Line
	3800 5950 3800 6000
Wire Wire Line
	1750 5950 1750 6000
$Comp
L power:GND #PWR07
U 1 1 6099BE80
P 1750 7350
F 0 "#PWR07" H 1750 7100 50  0001 C CNN
F 1 "GND" V 1755 7222 50  0000 R CNN
F 2 "" H 1750 7350 50  0001 C CNN
F 3 "" H 1750 7350 50  0001 C CNN
	1    1750 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 7350 1750 7300
$Comp
L power:GND #PWR018
U 1 1 609A4B1B
P 3800 7350
F 0 "#PWR018" H 3800 7100 50  0001 C CNN
F 1 "GND" V 3805 7222 50  0000 R CNN
F 2 "" H 3800 7350 50  0001 C CNN
F 3 "" H 3800 7350 50  0001 C CNN
	1    3800 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 7350 3800 7300
Wire Wire Line
	1300 6500 1350 6500
Wire Wire Line
	1300 6800 1350 6800
Wire Wire Line
	3350 6500 3400 6500
Wire Wire Line
	3350 6800 3400 6800
Wire Notes Line
	4800 7750 4800 5000
Text Notes 6850 7700 2    50   ~ 0
GBA FFC-Connector
Text Notes 4750 7700 2    50   ~ 0
GBA Buttons Serial-to-Parallel
Wire Notes Line
	3050 5000 3050 550 
Wire Notes Line
	3050 550  11150 550 
Text Notes 4450 1800 2    50   ~ 0
SNES Controller Connector
Text Notes 4850 4950 2    50   ~ 0
GBA ISR/Power Supply
Text Notes 3000 2200 2    512  ~ 102
gbaHD\nShield
Wire Wire Line
	10350 3100 10300 3100
Wire Wire Line
	10300 3200 10350 3200
Text GLabel 10350 3200 2    50   Input ~ 0
FPGA_COM
Text GLabel 7700 4050 0    50   Input ~ 0
FPGA_COM
Text GLabel 7750 4050 2    50   Input ~ 0
SP_ARD4
Wire Wire Line
	7700 4050 7750 4050
Wire Notes Line
	7100 4200 11150 4200
Wire Notes Line
	8550 3950 7150 3950
$Comp
L power:GND #PWR022
U 1 1 60837A36
P 6050 1300
F 0 "#PWR022" H 6050 1050 50  0001 C CNN
F 1 "GND" V 6055 1172 50  0000 R CNN
F 2 "" H 6050 1300 50  0001 C CNN
F 3 "" H 6050 1300 50  0001 C CNN
	1    6050 1300
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR021
U 1 1 60847128
P 5400 2400
F 0 "#PWR021" H 5400 2150 50  0001 C CNN
F 1 "GND" V 5405 2272 50  0000 R CNN
F 2 "" H 5400 2400 50  0001 C CNN
F 3 "" H 5400 2400 50  0001 C CNN
	1    5400 2400
	0    1    -1   0   
$EndComp
Wire Wire Line
	5400 2400 5450 2400
$Comp
L power:GND #PWR023
U 1 1 6085655B
P 6050 3900
F 0 "#PWR023" H 6050 3650 50  0001 C CNN
F 1 "GND" V 6055 3772 50  0000 R CNN
F 2 "" H 6050 3900 50  0001 C CNN
F 3 "" H 6050 3900 50  0001 C CNN
	1    6050 3900
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR026
U 1 1 60895719
P 6700 4100
F 0 "#PWR026" H 6700 3850 50  0001 C CNN
F 1 "GND" V 6705 3972 50  0000 R CNN
F 2 "" H 6700 4100 50  0001 C CNN
F 3 "" H 6700 4100 50  0001 C CNN
	1    6700 4100
	0    1    -1   0   
$EndComp
Wire Wire Line
	6700 4100 6750 4100
Wire Wire Line
	6700 1200 6750 1200
$Comp
L power:GND #PWR025
U 1 1 60885541
P 6700 1200
F 0 "#PWR025" H 6700 950 50  0001 C CNN
F 1 "GND" V 6705 1072 50  0000 R CNN
F 2 "" H 6700 1200 50  0001 C CNN
F 3 "" H 6700 1200 50  0001 C CNN
	1    6700 1200
	0    1    -1   0   
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 608BAED6
P 800 3850
F 0 "#PWR01" H 800 3700 50  0001 C CNN
F 1 "+5V" H 815 4023 50  0000 C CNN
F 2 "" H 800 3850 50  0001 C CNN
F 3 "" H 800 3850 50  0001 C CNN
	1    800  3850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 608BC7C6
P 800 4100
F 0 "C1" H 850 4000 50  0000 C CNN
F 1 "100nF" H 650 4200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 838 3950 50  0001 C CNN
F 3 "~" H 800 4100 50  0001 C CNN
	1    800  4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	800  3850 800  3900
Wire Wire Line
	850  3900 800  3900
Connection ~ 800  3900
Wire Wire Line
	800  3900 800  3950
Wire Wire Line
	800  4300 800  4250
$Comp
L Device:C C2
U 1 1 609BF4FF
P 1700 4100
F 0 "C2" H 1800 4000 50  0000 C CNN
F 1 "22u" H 1600 4200 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 1738 3950 50  0001 C CNN
F 3 "~" H 1700 4100 50  0001 C CNN
	1    1700 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 3850 1700 3900
Connection ~ 1700 3900
Wire Wire Line
	1700 3900 1700 3950
Wire Wire Line
	1700 4250 1700 4300
Wire Notes Line
	550  3500 550  7750
Text Notes 3000 4950 2    50   ~ 0
Power-Supply
$Comp
L 74xx:74HC595 U3
U 1 1 6061969C
P 3800 6600
F 0 "U3" H 3550 7150 50  0000 C CNN
F 1 "74HC595" H 4000 5950 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 3800 6600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3800 6600 50  0001 C CNN
	1    3800 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 2100 10500 2100
$Comp
L Connector_Generic:Conn_01x06 J6
U 1 1 6057EEAF
P 7650 5950
F 0 "J6" H 7600 6250 50  0000 L CNN
F 1 "Conn_01x06" V 7750 5600 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7650 5950 50  0001 C CNN
F 3 "~" H 7650 5950 50  0001 C CNN
	1    7650 5950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J7
U 1 1 6057F4B4
P 8400 4700
F 0 "J7" H 8350 5100 50  0000 L CNN
F 1 "Conn_01x08" V 8500 4250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 8400 4700 50  0001 C CNN
F 3 "~" H 8400 4700 50  0001 C CNN
	1    8400 4700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J9
U 1 1 6058616D
P 9250 4700
F 0 "J9" H 9200 5100 50  0000 L CNN
F 1 "Conn_01x08" V 9350 4250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 9250 4700 50  0001 C CNN
F 3 "~" H 9250 4700 50  0001 C CNN
	1    9250 4700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J8
U 1 1 60586BBC
P 8400 5750
F 0 "J8" H 8350 6250 50  0000 L CNN
F 1 "Conn_01x10" V 8500 5200 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 8400 5750 50  0001 C CNN
F 3 "~" H 8400 5750 50  0001 C CNN
	1    8400 5750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J10
U 1 1 60587763
P 9250 5750
F 0 "J10" H 9200 6250 50  0000 L CNN
F 1 "Conn_01x10" V 9350 5200 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 9250 5750 50  0001 C CNN
F 3 "~" H 9250 5750 50  0001 C CNN
	1    9250 5750
	1    0    0    -1  
$EndComp
Text GLabel 8150 6250 0    50   Input ~ 0
SP_IO9
Text GLabel 8150 6150 0    50   Input ~ 0
SP_IO8
Text GLabel 8150 6050 0    50   Input ~ 0
SP_IO7
Text GLabel 8150 5950 0    50   Input ~ 0
SP_IO6
Text GLabel 8150 5850 0    50   Input ~ 0
SP_IO5
Text GLabel 8150 5650 0    50   Input ~ 0
SP_IO3
Text GLabel 8150 5550 0    50   Input ~ 0
SP_IO2
Text GLabel 8150 5450 0    50   Input ~ 0
SP_IO1
Text GLabel 8150 5350 0    50   Input ~ 0
SP_IO0
Text GLabel 8150 5750 0    50   Input ~ 0
SP_IO4
Text GLabel 9000 6250 0    50   Input ~ 0
SP_ARSCL
Text GLabel 9000 6150 0    50   Input ~ 0
SP_ARSDA
Text GLabel 9000 5850 0    50   Input ~ 0
SP_ARD13
Text GLabel 9000 5750 0    50   Input ~ 0
SP_ARD12
Text GLabel 9000 5650 0    50   Input ~ 0
SP_ARD11
Text GLabel 9000 5550 0    50   Input ~ 0
SP_ARD10
Text GLabel 9000 5450 0    50   Input ~ 0
SP_ARD9
Text GLabel 9000 5350 0    50   Input ~ 0
SP_ARD8
Wire Wire Line
	9050 5350 9000 5350
Wire Wire Line
	9000 5450 9050 5450
Wire Wire Line
	9050 5650 9000 5650
Wire Wire Line
	9000 5750 9050 5750
Wire Wire Line
	9050 5850 9000 5850
Wire Wire Line
	9000 5950 9050 5950
Wire Wire Line
	9000 6150 9050 6150
Wire Wire Line
	9050 6250 9000 6250
Wire Wire Line
	8150 5350 8200 5350
Wire Wire Line
	8200 5450 8150 5450
Wire Wire Line
	8150 5550 8200 5550
Wire Wire Line
	8200 5650 8150 5650
Wire Wire Line
	8150 5750 8200 5750
Wire Wire Line
	8200 5850 8150 5850
Wire Wire Line
	8150 5950 8200 5950
Wire Wire Line
	8200 6050 8150 6050
Wire Wire Line
	8150 6150 8200 6150
Wire Wire Line
	8200 6250 8150 6250
Text GLabel 9000 5100 0    50   Input ~ 0
SP_ARD7
Text GLabel 9000 5000 0    50   Input ~ 0
SP_ARD6
Text GLabel 9000 4900 0    50   Input ~ 0
SP_ARD5
Text GLabel 9000 4800 0    50   Input ~ 0
SP_ARD4
Text GLabel 9000 4700 0    50   Input ~ 0
SP_ARD3
Text GLabel 9000 4600 0    50   Input ~ 0
SP_ARD2
Text GLabel 9000 4500 0    50   Input ~ 0
SP_ARD1
Text GLabel 9000 4400 0    50   Input ~ 0
SP_ARD0
Wire Wire Line
	9000 4400 9050 4400
Wire Wire Line
	9050 4500 9000 4500
Wire Wire Line
	9000 4600 9050 4600
Wire Wire Line
	9050 4700 9000 4700
Wire Wire Line
	9000 4800 9050 4800
Wire Wire Line
	9050 4900 9000 4900
Wire Wire Line
	9000 5000 9050 5000
Wire Wire Line
	9050 5100 9000 5100
Text GLabel 7400 6250 0    50   Input ~ 0
AR_A5
Text GLabel 7400 6150 0    50   Input ~ 0
AR_A4
Text GLabel 7400 6050 0    50   Input ~ 0
AR_A3
Text GLabel 7400 5950 0    50   Input ~ 0
AR_A2
Text GLabel 7400 5850 0    50   Input ~ 0
AR_A1
Text GLabel 7400 5750 0    50   Input ~ 0
AR_A0
Wire Wire Line
	7400 5750 7450 5750
Wire Wire Line
	7450 5850 7400 5850
Wire Wire Line
	7400 5950 7450 5950
Wire Wire Line
	7450 6050 7400 6050
Wire Wire Line
	7400 6150 7450 6150
Wire Wire Line
	7450 6250 7400 6250
Wire Wire Line
	8200 4400 8150 4400
Wire Wire Line
	8150 4500 8200 4500
Wire Wire Line
	8200 4600 8150 4600
Wire Wire Line
	8150 4700 8200 4700
Wire Wire Line
	8150 4900 8200 4900
Text GLabel 8150 4400 0    50   Input ~ 0
SP_VIN
$Comp
L power:+5V #PWR029
U 1 1 609ED1F7
P 8150 4700
F 0 "#PWR029" H 8150 4550 50  0001 C CNN
F 1 "+5V" V 8150 4900 50  0000 C CNN
F 2 "" H 8150 4700 50  0001 C CNN
F 3 "" H 8150 4700 50  0001 C CNN
	1    8150 4700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR027
U 1 1 609EE280
P 8150 4500
F 0 "#PWR027" H 8150 4250 50  0001 C CNN
F 1 "GND" V 8155 4372 50  0000 R CNN
F 2 "" H 8150 4500 50  0001 C CNN
F 3 "" H 8150 4500 50  0001 C CNN
	1    8150 4500
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR028
U 1 1 609EE9FF
P 8150 4600
F 0 "#PWR028" H 8150 4350 50  0001 C CNN
F 1 "GND" V 8155 4472 50  0000 R CNN
F 2 "" H 8150 4600 50  0001 C CNN
F 3 "" H 8150 4600 50  0001 C CNN
	1    8150 4600
	0    1    1    0   
$EndComp
Text GLabel 8150 4900 0    50   Input ~ 0
SP_ARRES
Wire Notes Line
	7100 550  7100 6450
$Comp
L power:GND #PWR033
U 1 1 60B318CE
P 9000 5950
F 0 "#PWR033" H 9000 5700 50  0001 C CNN
F 1 "GND" V 9005 5822 50  0000 R CNN
F 2 "" H 9000 5950 50  0001 C CNN
F 3 "" H 9000 5950 50  0001 C CNN
	1    9000 5950
	0    1    1    0   
$EndComp
Wire Notes Line
	3050 1850 4900 1850
Wire Notes Line
	3050 3500 550  3500
$Comp
L Connector_Generic:Conn_01x32 J2
U 1 1 60C1E768
P 5650 2300
F 0 "J2" H 5600 3900 50  0000 L CNN
F 1 "FFC_OGBA_32" H 5400 600 50  0000 L CNN
F 2 "Connector_FFC-FPC:TE_3-1734839-2_1x32-1MP_P0.5mm_Horizontal" H 5650 2300 50  0001 C CNN
F 3 "~" H 5650 2300 50  0001 C CNN
	1    5650 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 1300 6100 1300
Wire Wire Line
	6050 3900 6100 3900
Wire Wire Line
	6100 900  6750 900 
Connection ~ 6100 900 
Wire Wire Line
	6100 1400 6750 1400
Connection ~ 6100 1400
Wire Wire Line
	5450 900  6100 900 
Connection ~ 5450 900 
Wire Wire Line
	5450 1400 6100 1400
Connection ~ 5450 1400
Wire Wire Line
	5450 1500 6100 1500
Connection ~ 5450 1500
Wire Wire Line
	5450 1600 6100 1600
Connection ~ 5450 1600
Wire Wire Line
	5450 1700 6100 1700
Connection ~ 5450 1700
Wire Wire Line
	5450 1800 6100 1800
Connection ~ 5450 1800
Wire Wire Line
	5450 1900 6100 1900
Connection ~ 5450 1900
Wire Wire Line
	5450 2000 6100 2000
Connection ~ 5450 2000
Wire Wire Line
	5450 2100 6100 2100
Connection ~ 5450 2100
Wire Wire Line
	5450 2200 6100 2200
Connection ~ 5450 2200
Wire Wire Line
	5450 2300 6100 2300
Connection ~ 5450 2300
Wire Wire Line
	5450 2400 6100 2400
Connection ~ 5450 2400
Wire Wire Line
	5450 2500 6100 2500
Connection ~ 5450 2500
Wire Wire Line
	5450 2600 6100 2600
Connection ~ 5450 2600
Wire Wire Line
	5450 2700 6100 2700
Connection ~ 5450 2700
Wire Wire Line
	5450 2800 6100 2800
Connection ~ 5450 2800
Wire Wire Line
	5450 2900 6100 2900
Connection ~ 5450 2900
Wire Wire Line
	5450 3200 6100 3200
Connection ~ 5450 3200
Wire Wire Line
	6100 1500 6750 1500
Connection ~ 6100 1500
Wire Wire Line
	6750 1600 6100 1600
Connection ~ 6100 1600
Wire Wire Line
	6100 1700 6750 1700
Connection ~ 6100 1700
Wire Wire Line
	6750 1800 6100 1800
Connection ~ 6100 1800
Wire Wire Line
	6100 1900 6750 1900
Connection ~ 6100 1900
Wire Wire Line
	6100 2000 6750 2000
Connection ~ 6100 2000
Wire Wire Line
	6100 2100 6750 2100
Connection ~ 6100 2100
Wire Wire Line
	6750 2200 6100 2200
Connection ~ 6100 2200
Wire Wire Line
	6100 2300 6750 2300
Connection ~ 6100 2300
Wire Wire Line
	6750 2400 6100 2400
Connection ~ 6100 2400
Wire Wire Line
	6100 2500 6750 2500
Connection ~ 6100 2500
Wire Wire Line
	6750 2600 6100 2600
Connection ~ 6100 2600
Wire Wire Line
	6100 2700 6750 2700
Connection ~ 6100 2700
Wire Wire Line
	6750 2800 6100 2800
Connection ~ 6100 2800
Wire Wire Line
	6100 2900 6750 2900
Connection ~ 6100 2900
Wire Wire Line
	6750 3200 6100 3200
Connection ~ 6100 3200
$Comp
L power:GND #PWR032
U 1 1 610E1905
P 8800 3500
F 0 "#PWR032" H 8800 3250 50  0001 C CNN
F 1 "GND" H 8805 3327 50  0000 C CNN
F 2 "" H 8800 3500 50  0001 C CNN
F 3 "" H 8800 3500 50  0001 C CNN
	1    8800 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 610E1DA2
P 1700 4300
F 0 "#PWR09" H 1700 4050 50  0001 C CNN
F 1 "GND" H 1705 4127 50  0000 C CNN
F 2 "" H 1700 4300 50  0001 C CNN
F 3 "" H 1700 4300 50  0001 C CNN
	1    1700 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 610E25E2
P 800 4300
F 0 "#PWR02" H 800 4050 50  0001 C CNN
F 1 "GND" H 805 4127 50  0000 C CNN
F 2 "" H 800 4300 50  0001 C CNN
F 3 "" H 800 4300 50  0001 C CNN
	1    800  4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5550 9050 5550
$Comp
L Regulator_Linear:AZ1117R-ADJ U1
U 1 1 605CD92F
P 1150 3900
F 0 "U1" H 1150 4142 50  0000 C CNN
F 1 "AZ1117R-ADJ" H 1150 4051 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-89-3" H 1150 4150 50  0001 C CIN
F 3 "https://www.diodes.com/assets/Datasheets/products_inactive_data/AZ1117.pdf" H 1150 3900 50  0001 C CNN
	1    1150 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 605FB724
P 1500 4100
F 0 "R2" H 1570 4146 50  0000 L CNN
F 1 "1k" H 1570 4055 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1430 4100 50  0001 C CNN
F 3 "~" H 1500 4100 50  0001 C CNN
	1    1500 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1500 3950 1500 3900
Wire Wire Line
	1500 3900 1450 3900
Wire Wire Line
	1500 3900 1700 3900
Connection ~ 1500 3900
Wire Wire Line
	1500 4250 1500 4300
Wire Wire Line
	1500 4300 1150 4300
Wire Wire Line
	1150 4300 1150 4200
$Comp
L Device:R R1
U 1 1 60640679
P 1150 4500
F 0 "R1" H 1220 4546 50  0000 L CNN
F 1 "2.2k" H 1220 4455 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1080 4500 50  0001 C CNN
F 3 "~" H 1150 4500 50  0001 C CNN
	1    1150 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	1150 4350 1150 4300
Connection ~ 1150 4300
$Comp
L power:GND #PWR03
U 1 1 60657D1E
P 1150 4700
F 0 "#PWR03" H 1150 4450 50  0001 C CNN
F 1 "GND" H 1155 4527 50  0000 C CNN
F 2 "" H 1150 4700 50  0001 C CNN
F 3 "" H 1150 4700 50  0001 C CNN
	1    1150 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 4700 1150 4650
$Comp
L power:+4V #PWR08
U 1 1 6066F638
P 1700 3850
F 0 "#PWR08" H 1700 3700 50  0001 C CNN
F 1 "+4V" H 1715 4023 50  0000 C CNN
F 2 "" H 1700 3850 50  0001 C CNN
F 3 "" H 1700 3850 50  0001 C CNN
	1    1700 3850
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Open JP1
U 1 1 60670A7B
P 2450 4050
F 0 "JP1" H 2450 4163 50  0000 C CNN
F 1 "SolderJumper_3_Open" H 2450 4254 50  0000 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_Pad1.0x1.5mm_NumberLabels" H 2450 4050 50  0001 C CNN
F 3 "~" H 2450 4050 50  0001 C CNN
	1    2450 4050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2450 3900 2450 3850
Wire Wire Line
	2250 4050 2150 4050
Wire Wire Line
	2150 4050 2150 3900
$Comp
L power:+4V #PWR010
U 1 1 606D39B1
P 2150 3900
F 0 "#PWR010" H 2150 3750 50  0001 C CNN
F 1 "+4V" H 2165 4073 50  0000 C CNN
F 2 "" H 2150 3900 50  0001 C CNN
F 3 "" H 2150 3900 50  0001 C CNN
	1    2150 3900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR030
U 1 1 606D462B
P 8150 4800
F 0 "#PWR030" H 8150 4650 50  0001 C CNN
F 1 "+3V3" V 8165 4928 50  0000 L CNN
F 2 "" H 8150 4800 50  0001 C CNN
F 3 "" H 8150 4800 50  0001 C CNN
	1    8150 4800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8150 4800 8200 4800
$Comp
L power:+3V3 #PWR012
U 1 1 606ECC1D
P 2750 3900
F 0 "#PWR012" H 2750 3750 50  0001 C CNN
F 1 "+3V3" H 2765 4073 50  0000 C CNN
F 2 "" H 2750 3900 50  0001 C CNN
F 3 "" H 2750 3900 50  0001 C CNN
	1    2750 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 4050 2750 4050
Wire Wire Line
	2750 4050 2750 3900
$Comp
L Switch:SW_Push SW1
U 1 1 5FFC3468
P 10450 5050
F 0 "SW1" H 10450 4850 50  0000 C CNN
F 1 "SW_Push" H 10450 4950 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_CK_KMR2" H 10450 5250 50  0001 C CNN
F 3 "~" H 10450 5250 50  0001 C CNN
	1    10450 5050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 VCC1
U 1 1 608BF56E
P 5550 7350
F 0 "VCC1" H 5750 7350 50  0000 C CNN
F 1 "Conn_01x01" H 5400 7250 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 5550 7350 50  0001 C CNN
F 3 "~" H 5550 7350 50  0001 C CNN
	1    5550 7350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 GND1
U 1 1 608C02ED
P 5550 7500
F 0 "GND1" H 5750 7500 50  0000 C CNN
F 1 "Conn_01x01" H 5400 7350 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 5550 7500 50  0001 C CNN
F 3 "~" H 5550 7500 50  0001 C CNN
	1    5550 7500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 608C1A96
P 5300 7500
F 0 "#PWR0102" H 5300 7250 50  0001 C CNN
F 1 "GND" H 5305 7327 50  0000 C CNN
F 2 "" H 5300 7500 50  0001 C CNN
F 3 "" H 5300 7500 50  0001 C CNN
	1    5300 7500
	0    1    1    0   
$EndComp
Wire Wire Line
	5300 7500 5350 7500
Wire Wire Line
	5350 7350 5300 7350
Wire Wire Line
	5700 6800 6100 6800
Text GLabel 5300 7350 0    50   Input ~ 0
GBA_VCC
Wire Notes Line
	4850 7700 4850 7250
Wire Notes Line
	4850 7250 6000 7250
Wire Notes Line
	6000 7250 6000 7700
Wire Notes Line
	4850 7700 6000 7700
Text Notes 5950 7650 2    50   ~ 0
optional battery connector
$Comp
L Device:Q_PMOS_GSD Q1
U 1 1 6051233F
P 4700 4500
F 0 "Q1" H 4550 4450 50  0000 L CNN
F 1 "Q_PMOS_GSD" H 4300 4700 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4900 4600 50  0001 C CNN
F 3 "~" H 4700 4500 50  0001 C CNN
	1    4700 4500
	1    0    0    1   
$EndComp
Wire Wire Line
	4800 4300 4800 4100
$Comp
L Device:R R3
U 1 1 60519F8D
P 4450 4300
F 0 "R3" H 4520 4346 50  0000 L CNN
F 1 "10K" H 4520 4255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 4380 4300 50  0001 C CNN
F 3 "~" H 4450 4300 50  0001 C CNN
	1    4450 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	4450 4150 4450 4100
Wire Wire Line
	4450 4450 4450 4500
Wire Wire Line
	4800 4700 4800 4750
Wire Wire Line
	4800 4750 4850 4750
Text GLabel 4850 4750 2    50   Input ~ 0
GBA_VCC
Text GLabel 4000 4100 0    50   Input ~ 0
GBA_POWER_SEL
Connection ~ 4450 4500
Wire Wire Line
	4450 4500 4500 4500
Wire Wire Line
	4450 4050 4450 4100
Connection ~ 4450 4100
Text Notes 3250 3750 0    50   ~ 0
There might be input clamping here \ndue to 3.3V supply volgate of µC vs. \n4V pullup Voltage.\n\nThis would result in approx. 400µA \ncurrent over the clamping current.
$Comp
L power:VCC #PWR0103
U 1 1 606F2F2F
P 4450 4050
F 0 "#PWR0103" H 4450 3900 50  0001 C CNN
F 1 "VCC" H 4465 4223 50  0000 C CNN
F 2 "" H 4450 4050 50  0001 C CNN
F 3 "" H 4450 4050 50  0001 C CNN
	1    4450 4050
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0104
U 1 1 606F36E5
P 2450 3850
F 0 "#PWR0104" H 2450 3700 50  0001 C CNN
F 1 "VCC" H 2465 4023 50  0000 C CNN
F 2 "" H 2450 3850 50  0001 C CNN
F 3 "" H 2450 3850 50  0001 C CNN
	1    2450 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0105
U 1 1 606F3B3C
P 1750 5950
F 0 "#PWR0105" H 1750 5800 50  0001 C CNN
F 1 "+3V3" H 1765 6123 50  0000 C CNN
F 2 "" H 1750 5950 50  0001 C CNN
F 3 "" H 1750 5950 50  0001 C CNN
	1    1750 5950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0106
U 1 1 606F42C9
P 3800 5950
F 0 "#PWR0106" H 3800 5800 50  0001 C CNN
F 1 "+3V3" H 3815 6123 50  0000 C CNN
F 2 "" H 3800 5950 50  0001 C CNN
F 3 "" H 3800 5950 50  0001 C CNN
	1    3800 5950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0107
U 1 1 606F491E
P 3350 6500
F 0 "#PWR0107" H 3350 6350 50  0001 C CNN
F 1 "+3V3" V 3350 6750 50  0000 C CNN
F 2 "" H 3350 6500 50  0001 C CNN
F 3 "" H 3350 6500 50  0001 C CNN
	1    3350 6500
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0108
U 1 1 606F5295
P 1300 6500
F 0 "#PWR0108" H 1300 6350 50  0001 C CNN
F 1 "+3V3" V 1300 6750 50  0000 C CNN
F 2 "" H 1300 6500 50  0001 C CNN
F 3 "" H 1300 6500 50  0001 C CNN
	1    1300 6500
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0109
U 1 1 606F593B
P 9700 850
F 0 "#PWR0109" H 9700 700 50  0001 C CNN
F 1 "+3V3" H 9715 1023 50  0000 C CNN
F 2 "" H 9700 850 50  0001 C CNN
F 3 "" H 9700 850 50  0001 C CNN
	1    9700 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0110
U 1 1 606F69EE
P 9800 850
F 0 "#PWR0110" H 9800 700 50  0001 C CNN
F 1 "+3V3" H 9815 1023 50  0000 C CNN
F 2 "" H 9800 850 50  0001 C CNN
F 3 "" H 9800 850 50  0001 C CNN
	1    9800 850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0111
U 1 1 606F6B5F
P 9050 1200
F 0 "#PWR0111" H 9050 1050 50  0001 C CNN
F 1 "+3V3" V 9050 1400 50  0000 C CNN
F 2 "" H 9050 1200 50  0001 C CNN
F 3 "" H 9050 1200 50  0001 C CNN
	1    9050 1200
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0112
U 1 1 606F74C3
P 8800 3100
F 0 "#PWR0112" H 8800 2950 50  0001 C CNN
F 1 "+3V3" H 8815 3273 50  0000 C CNN
F 2 "" H 8800 3100 50  0001 C CNN
F 3 "" H 8800 3100 50  0001 C CNN
	1    8800 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0113
U 1 1 606F7A9E
P 10200 4650
F 0 "#PWR0113" H 10200 4500 50  0001 C CNN
F 1 "+3V3" H 10215 4823 50  0000 C CNN
F 2 "" H 10200 4650 50  0001 C CNN
F 3 "" H 10200 4650 50  0001 C CNN
	1    10200 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0114
U 1 1 606F8102
P 10600 5950
F 0 "#PWR0114" H 10600 5800 50  0001 C CNN
F 1 "+3V3" V 10600 6150 50  0000 C CNN
F 2 "" H 10600 5950 50  0001 C CNN
F 3 "" H 10600 5950 50  0001 C CNN
	1    10600 5950
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR0115
U 1 1 606F8ADF
P 3800 750
F 0 "#PWR0115" H 3800 600 50  0001 C CNN
F 1 "+3V3" V 3800 950 50  0000 C CNN
F 2 "" H 3800 750 50  0001 C CNN
F 3 "" H 3800 750 50  0001 C CNN
	1    3800 750 
	0    -1   -1   0   
$EndComp
Text GLabel 10350 1300 2    50   Input ~ 0
~GBA_BUTTONS_EN~
Wire Wire Line
	10350 1300 10300 1300
Text GLabel 1300 6800 0    50   Input ~ 0
~GBA_BUTTONS_EN~
Text GLabel 3350 6800 0    50   Input ~ 0
~GBA_BUTTONS_EN~
Wire Wire Line
	6100 5400 6050 5400
Text GLabel 10350 2600 2    50   Input ~ 0
SP_ARSCL
Text GLabel 10350 2500 2    50   Input ~ 0
SP_ARSDA
Wire Wire Line
	10350 2500 10300 2500
Wire Wire Line
	10300 2600 10350 2600
Wire Wire Line
	10300 2300 10500 2300
Wire Wire Line
	10350 2200 10300 2200
$Comp
L Connector_Generic:Conn_01x01 T3
U 1 1 60D9FC47
P 10700 2300
F 0 "T3" H 10850 2300 50  0000 C CNN
F 1 "Conn_01x01" H 11050 2300 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10700 2300 50  0001 C CNN
F 3 "~" H 10700 2300 50  0001 C CNN
	1    10700 2300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 T2
U 1 1 60D9F9D9
P 10550 2200
F 0 "T2" H 10700 2200 50  0000 C CNN
F 1 "Conn_01x01" H 10900 2200 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10550 2200 50  0001 C CNN
F 3 "~" H 10550 2200 50  0001 C CNN
	1    10550 2200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 T1
U 1 1 60D9F60C
P 10700 2100
F 0 "T1" H 10850 2100 50  0000 C CNN
F 1 "Conn_01x01" H 11050 2100 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10700 2100 50  0001 C CNN
F 3 "~" H 10700 2100 50  0001 C CNN
	1    10700 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 2400 10300 2400
$Comp
L Connector_Generic:Conn_01x01 T4
U 1 1 615C5AF4
P 10550 2400
F 0 "T4" H 10700 2400 50  0000 C CNN
F 1 "Conn_01x01" H 10900 2400 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10550 2400 50  0001 C CNN
F 3 "~" H 10550 2400 50  0001 C CNN
	1    10550 2400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 T7
U 1 1 60D3CACB
P 10550 1200
F 0 "T7" H 10700 1200 50  0000 C CNN
F 1 "Conn_01x01" H 10900 1200 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10550 1200 50  0001 C CNN
F 3 "~" H 10550 1200 50  0001 C CNN
	1    10550 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 3400 10300 3400
Wire Wire Line
	10350 3300 10300 3300
$Comp
L Connector_Generic:Conn_01x01 T5
U 1 1 60D3EA71
P 10550 3300
F 0 "T5" H 10700 3300 50  0000 C CNN
F 1 "Conn_01x01" H 10900 3300 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10550 3300 50  0001 C CNN
F 3 "~" H 10550 3300 50  0001 C CNN
	1    10550 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 T6
U 1 1 60D3E627
P 10700 3400
F 0 "T6" H 10850 3400 50  0000 C CNN
F 1 "Conn_01x01" H 11050 3400 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10700 3400 50  0001 C CNN
F 3 "~" H 10700 3400 50  0001 C CNN
	1    10700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 1200 10350 1200
Wire Wire Line
	10300 3500 10400 3500
Wire Wire Line
	10300 3600 10400 3600
Text GLabel 10400 3500 2    50   Input ~ 0
LED_RED
Text GLabel 10400 3600 2    50   Input ~ 0
LED_GREEN
$Comp
L Device:Q_NMOS_GSD Q3
U 1 1 616F8501
P 3950 2450
F 0 "Q3" H 4154 2496 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 4154 2405 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4150 2550 50  0001 C CNN
F 3 "~" H 3950 2450 50  0001 C CNN
	1    3950 2450
	1    0    0    -1  
$EndComp
Text GLabel 4050 2100 0    50   Input ~ 0
GBA_CLK
Text GLabel 4050 2800 2    50   Input ~ 0
GBA_CLK'
Wire Wire Line
	4050 2100 4050 2250
Wire Wire Line
	4050 2650 4050 2800
Wire Wire Line
	3750 2450 3700 2450
Text GLabel 3650 2450 0    50   Input ~ 0
GBA_VCC
Wire Notes Line
	3050 3100 4900 3100
Text Notes 4650 2000 2    50   ~ 0
Disable clock if GBA is not supplied!
$Comp
L Device:R R8
U 1 1 617D8C43
P 3700 2650
F 0 "R8" H 3770 2696 50  0000 L CNN
F 1 "10K" H 3770 2605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3630 2650 50  0001 C CNN
F 3 "~" H 3700 2650 50  0001 C CNN
	1    3700 2650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR05
U 1 1 617D937D
P 3700 2850
F 0 "#PWR05" H 3700 2600 50  0001 C CNN
F 1 "GND" H 3705 2677 50  0000 C CNN
F 2 "" H 3700 2850 50  0001 C CNN
F 3 "" H 3700 2850 50  0001 C CNN
	1    3700 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2850 3700 2800
Wire Wire Line
	3700 2500 3700 2450
Connection ~ 3700 2450
Wire Wire Line
	3700 2450 3650 2450
Wire Notes Line
	4800 5700 550  5700
$Comp
L Device:R R6
U 1 1 61841643
P 3600 5200
F 0 "R6" V 3650 5300 50  0000 L CNN
F 1 "10K" V 3600 5150 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3530 5200 50  0001 C CNN
F 3 "~" H 3600 5200 50  0001 C CNN
	1    3600 5200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 61841EDE
P 3600 5400
F 0 "R7" V 3650 5500 50  0000 L CNN
F 1 "10K" V 3600 5350 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3530 5400 50  0001 C CNN
F 3 "~" H 3600 5400 50  0001 C CNN
	1    3600 5400
	0    -1   -1   0   
$EndComp
Text GLabel 3350 5200 0    50   Input ~ 0
LED_RED
Text GLabel 3350 5400 0    50   Input ~ 0
LED_GREEN
$Comp
L Device:LED D1
U 1 1 61842569
P 4000 5200
F 0 "D1" H 4100 5150 50  0000 C CNN
F 1 "RED" H 3850 5250 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4000 5200 50  0001 C CNN
F 3 "~" H 4000 5200 50  0001 C CNN
	1    4000 5200
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D2
U 1 1 61843873
P 4000 5400
F 0 "D2" H 4100 5350 50  0000 C CNN
F 1 "GREEN" H 3850 5500 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4000 5400 50  0001 C CNN
F 3 "~" H 4000 5400 50  0001 C CNN
	1    4000 5400
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 5200 3750 5200
Wire Wire Line
	3850 5400 3750 5400
Wire Wire Line
	3450 5400 3350 5400
Wire Wire Line
	3350 5200 3450 5200
Wire Wire Line
	4150 5400 4400 5400
Wire Wire Line
	4400 5200 4400 5400
Wire Wire Line
	4150 5200 4400 5200
$Comp
L power:GND #PWR06
U 1 1 6191A61B
P 4400 5450
F 0 "#PWR06" H 4400 5200 50  0001 C CNN
F 1 "GND" H 4405 5277 50  0000 C CNN
F 2 "" H 4400 5450 50  0001 C CNN
F 3 "" H 4400 5450 50  0001 C CNN
	1    4400 5450
	1    0    0    -1  
$EndComp
Connection ~ 4400 5400
Wire Wire Line
	4400 5400 4400 5450
Text Notes 4750 5650 2    50   ~ 0
LEDs
Wire Notes Line
	2800 5000 2800 5700
Wire Notes Line
	4900 3300 5300 3300
Wire Notes Line
	4900 550  4900 3300
$Comp
L Device:Q_NMOS_GSD Q2
U 1 1 61A4CBB4
P 3650 4500
F 0 "Q2" H 3450 4450 50  0000 L CNN
F 1 "Q_NMOS_GSD" H 3200 4700 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3850 4600 50  0001 C CNN
F 3 "~" H 3650 4500 50  0001 C CNN
	1    3650 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 4500 4050 4500
Connection ~ 4050 4500
Wire Wire Line
	4050 4500 4450 4500
Wire Wire Line
	4800 4750 3550 4750
Wire Wire Line
	3550 4750 3550 4700
Connection ~ 4800 4750
Wire Wire Line
	3550 4300 3550 4200
Wire Wire Line
	3550 4200 3350 4200
$Comp
L power:GND #PWR04
U 1 1 61B7FD79
P 3350 4650
F 0 "#PWR04" H 3350 4400 50  0001 C CNN
F 1 "GND" V 3355 4522 50  0000 R CNN
F 2 "" H 3350 4650 50  0001 C CNN
F 3 "" H 3350 4650 50  0001 C CNN
	1    3350 4650
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 61B80C6C
P 3350 4450
F 0 "R5" H 3420 4496 50  0000 L CNN
F 1 "500" H 3420 4405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 3280 4450 50  0001 C CNN
F 3 "~" H 3350 4450 50  0001 C CNN
	1    3350 4450
	-1   0    0    1   
$EndComp
Wire Wire Line
	3350 4200 3350 4300
Wire Wire Line
	3350 4600 3350 4650
Wire Wire Line
	4050 4100 4000 4100
Wire Wire Line
	4050 4100 4050 4500
Wire Notes Line
	5300 3300 5300 5000
Wire Wire Line
	4800 4100 4450 4100
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 600D67A4
P 5500 6800
F 0 "J1" H 5650 6800 50  0000 C CNN
F 1 "Conn_01x01" H 5800 6800 50  0001 C CNN
F 2 "TestPoint:TestPoint_Pad_1.5x1.5mm" H 5500 6800 50  0001 C CNN
F 3 "~" H 5500 6800 50  0001 C CNN
	1    5500 6800
	-1   0    0    1   
$EndComp
$EndSCHEMATC
