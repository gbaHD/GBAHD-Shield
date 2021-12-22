EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x16 J3
U 1 1 618CE4CC
P 3800 2700
F 0 "J3" H 3718 3617 50  0000 C CNN
F 1 "Conn_01x16" H 3718 3526 50  0000 C CNN
F 2 "Connector_FFC-FPC:TE_1-1734839-6_1x16-1MP_P0.5mm_Horizontal" H 3800 2700 50  0001 C CNN
F 3 "~" H 3800 2700 50  0001 C CNN
	1    3800 2700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4000 2100 4600 2100
Wire Wire Line
	4000 2200 4600 2200
Wire Wire Line
	4000 2300 4600 2300
Wire Wire Line
	4000 2400 4600 2400
Wire Wire Line
	4000 2500 4600 2500
Wire Wire Line
	4000 2600 4600 2600
Wire Wire Line
	4000 2700 4600 2700
Wire Wire Line
	4000 2800 4600 2800
Wire Wire Line
	4000 2900 4600 2900
Wire Wire Line
	4000 3000 4600 3000
Wire Wire Line
	4000 3100 4600 3100
Wire Wire Line
	4000 3200 4600 3200
Wire Wire Line
	4000 3300 4600 3300
Wire Wire Line
	4000 3400 4600 3400
Text Label 4500 2100 2    50   ~ 0
GBA_UP
Text Label 4500 2200 2    50   ~ 0
GBA_DOWN
Text Label 4500 2300 2    50   ~ 0
GBA_LEFT
Text Label 4500 2400 2    50   ~ 0
GBA_RIGHT
Text Label 4500 2500 2    50   ~ 0
GBA_A
Text Label 4500 2600 2    50   ~ 0
GBA_B
Text Label 4500 2700 2    50   ~ 0
GBA_L
Text Label 4500 2800 2    50   ~ 0
GBA_R
Text Label 4500 2900 2    50   ~ 0
GBA_START
Text Label 4500 3000 2    50   ~ 0
GBA_SELECT
Text Label 4500 3100 2    50   ~ 0
GBA_AUD_L
Text Label 4500 3200 2    50   ~ 0
GBA_AUD_R
Text Label 4500 3300 2    50   ~ 0
GBA_CLK
Text Label 4500 3400 2    50   ~ 0
GBA_RESET
$Comp
L Connector_Generic:Conn_01x01 BAT+1
U 1 1 618D2A1D
P 3000 2050
F 0 "BAT+1" V 2872 2130 50  0000 L CNN
F 1 "Conn_01x01" V 2963 2130 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3000 2050 50  0001 C CNN
F 3 "~" H 3000 2050 50  0001 C CNN
	1    3000 2050
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 GND1
U 1 1 618D3B8B
P 3000 2200
F 0 "GND1" V 2964 2112 50  0000 R CNN
F 1 "Conn_01x01" V 2873 2112 50  0000 R CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 3000 2200 50  0001 C CNN
F 3 "~" H 3000 2200 50  0001 C CNN
	1    3000 2200
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 618D4507
P 4100 3600
F 0 "#PWR0101" H 4100 3350 50  0001 C CNN
F 1 "GND" H 4105 3427 50  0000 C CNN
F 2 "" H 4100 3600 50  0001 C CNN
F 3 "" H 4100 3600 50  0001 C CNN
	1    4100 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3600 4100 3500
Wire Wire Line
	4100 3500 4000 3500
Wire Wire Line
	4000 2000 4100 2000
Wire Wire Line
	4100 2000 4100 1900
$Comp
L power:VCC #PWR0102
U 1 1 618D4F38
P 4100 1900
F 0 "#PWR0102" H 4100 1750 50  0001 C CNN
F 1 "VCC" H 4115 2073 50  0000 C CNN
F 2 "" H 4100 1900 50  0001 C CNN
F 3 "" H 4100 1900 50  0001 C CNN
	1    4100 1900
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0103
U 1 1 618D5479
P 3000 1800
F 0 "#PWR0103" H 3000 1650 50  0001 C CNN
F 1 "VCC" H 3015 1973 50  0000 C CNN
F 2 "" H 3000 1800 50  0001 C CNN
F 3 "" H 3000 1800 50  0001 C CNN
	1    3000 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1800 3000 1850
$Comp
L power:GND #PWR0104
U 1 1 618D599D
P 3000 2450
F 0 "#PWR0104" H 3000 2200 50  0001 C CNN
F 1 "GND" H 3005 2277 50  0000 C CNN
F 2 "" H 3000 2450 50  0001 C CNN
F 3 "" H 3000 2450 50  0001 C CNN
	1    3000 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2450 3000 2400
Text Notes 700  2000 0    50   ~ 0
Pin             Function   CPU AGB A    CPU AGB B\nTP0            A           4              3\nTP1            B           5              4\nTP2            SELECT     126            154\nTP3            START      127            153\nTP4            RIGHT      123            149\nTP5            LEFT       121            147\nTP6            UP         120            146\nTP7            DOWN      122            148\nTP8            R          3               2\nTP9            L          119             145\nTP10           CK1       113             138\nS01            R-Out     6               5\nS02            L-Out     7               6\n
Wire Notes Line
	2800 800  2800 2050
Wire Notes Line
	2800 2050 650  2050
Wire Notes Line
	650  2050 650  800 
Wire Notes Line
	650  800  2800 800 
$Comp
L Nintendo:CPU_AGB_B_Interface J4
U 1 1 618D78F3
P 4800 2650
F 0 "J4" H 4700 3350 50  0000 L CNN
F 1 "CPU_AGB_B_Interface" V 5050 1800 50  0000 L CNN
F 2 "Nintendo:CPU_AGB_B_Interface" H 5050 2150 50  0001 C CNN
F 3 "" H 5050 2150 50  0001 C CNN
	1    4800 2650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
