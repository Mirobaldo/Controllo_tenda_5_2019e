EESchema Schematic File Version 4
LIBS:Bus Posteriore-cache
EELAYER 26 0
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
L Bus-Posteriore-rescue:ARDUINO_as_UNO SH1
U 1 1 58D77C53
P 5650 3650
F 0 "SH1" H 4950 3050 60  0000 C CNN
F 1 "ARDUINO_as_UNO" H 5675 5000 60  0000 C CNN
F 2 "SHIELD_ARDUINO:ARDUINO_as_UNO_R3_CONTOUR" H 5450 3425 60  0001 C CNN
F 3 "" H 5450 3425 60  0000 C CNN
	1    5650 3650
	1    0    0    -1  
$EndComp
$Comp
L Bus-Posteriore-rescue:DB9 J1
U 1 1 58D77E0F
P 9350 4000
F 0 "J1" H 9350 4550 50  0000 C CNN
F 1 "DB9" H 9350 3450 50  0000 C CNN
F 2 "Connect:DB9MC" H 9350 4000 50  0001 C CNN
F 3 "" H 9350 4000 50  0000 C CNN
	1    9350 4000
	1    0    0    -1  
$EndComp
$Comp
L Bus-Posteriore-rescue:DB9 J2
U 1 1 58D77F7A
P 2700 2750
F 0 "J2" H 2700 3300 50  0000 C CNN
F 1 "DB9" H 2700 2200 50  0000 C CNN
F 2 "SUNBlind:DB9FCsmall" H 2700 2750 50  0001 C CNN
F 3 "" H 2700 2750 50  0000 C CNN
	1    2700 2750
	-1   0    0    -1  
$EndComp
$Comp
L Bus-Posteriore-rescue:DB9 J3
U 1 1 58D77F80
P 2700 4000
F 0 "J3" H 2700 4550 50  0000 C CNN
F 1 "DB9" H 2700 3450 50  0000 C CNN
F 2 "SUNBlind:DB9FCsmall" H 2700 4000 50  0001 C CNN
F 3 "" H 2700 4000 50  0000 C CNN
	1    2700 4000
	-1   0    0    -1  
$EndComp
$Comp
L Bus-Posteriore-rescue:DB9 J4
U 1 1 58D77F86
P 2700 5200
F 0 "J4" H 2700 5750 50  0000 C CNN
F 1 "DB9" H 2700 4650 50  0000 C CNN
F 2 "SUNBlind:DB9FCsmall" H 2700 5200 50  0001 C CNN
F 3 "" H 2700 5200 50  0000 C CNN
	1    2700 5200
	-1   0    0    -1  
$EndComp
Text Notes 8850 4750 0    60   ~ 0
Pulsanti + Comunicazione
Text Label 6550 3950 0    60   ~ 0
WindSwitch
Text Label 6550 3350 0    60   ~ 0
cmdSBOpen
Text Label 6550 3250 0    60   ~ 0
cmdSBclose
Text Label 6550 3650 0    60   ~ 0
psbSBOpen
Text Label 6550 3550 0    60   ~ 0
psbSBClose
Text Label 6550 3850 0    60   ~ 0
PosReed
Text Label 6550 3750 0    60   ~ 0
DHTdata
Text Label 6550 3150 0    60   ~ 0
DispLoad
Text Label 6550 3050 0    60   ~ 0
DispClock
Text Label 6550 2950 0    60   ~ 0
DispData
$Comp
L power:+5V #PWR01
U 1 1 58D80640
P 3200 1950
F 0 "#PWR01" H 3200 1800 50  0001 C CNN
F 1 "+5V" H 3200 2090 50  0000 C CNN
F 2 "" H 3200 1950 50  0000 C CNN
F 3 "" H 3200 1950 50  0000 C CNN
	1    3200 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 58D80921
P 3300 5750
F 0 "#PWR02" H 3300 5500 50  0001 C CNN
F 1 "GND" H 3300 5600 50  0000 C CNN
F 2 "" H 3300 5750 50  0000 C CNN
F 3 "" H 3300 5750 50  0000 C CNN
	1    3300 5750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 58D81137
P 8600 4400
F 0 "#PWR03" H 8600 4150 50  0001 C CNN
F 1 "GND" H 8600 4250 50  0000 C CNN
F 2 "" H 8600 4400 50  0000 C CNN
F 3 "" H 8600 4400 50  0000 C CNN
	1    8600 4400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR04
U 1 1 58D81182
P 8700 2200
F 0 "#PWR04" H 8700 2050 50  0001 C CNN
F 1 "+5V" H 8700 2340 50  0000 C CNN
F 2 "" H 8700 2200 50  0000 C CNN
F 3 "" H 8700 2200 50  0000 C CNN
	1    8700 2200
	1    0    0    -1  
$EndComp
Text Notes 1750 4050 0    60   ~ 0
Comandi + wind\n
Text Notes 2000 5200 0    60   ~ 0
Expansion
Text Notes 1900 2800 0    60   ~ 0
Temperature\nSensore Hall
Text Label 3400 3900 0    60   ~ 0
WindSwitch
Text Label 3400 4200 0    60   ~ 0
cmdSBOpen
Text Label 3400 4000 0    60   ~ 0
cmdSBclose
Text Label 7850 4200 0    60   ~ 0
psbSBOpen
Text Label 8350 4000 2    60   ~ 0
psbSBClose
Text Label 3400 2650 0    60   ~ 0
PosReed
Text Label 3400 2350 0    60   ~ 0
DHTdata
Text Label 8800 4100 0    60   ~ 0
SDA
Text Label 8800 3600 0    60   ~ 0
SCL
$Comp
L Bus-Posteriore-rescue:PIC12F675-I_P U1
U 1 1 58D82896
P 5700 5200
F 0 "U1" H 5050 5750 50  0000 L CNN
F 1 "PIC12F675-I/P" H 5050 5650 50  0000 L CNN
F 2 "Housings_DIP:DIP-8_W7.62mm" H 5700 5200 50  0001 C CNN
F 3 "" H 5700 5200 50  0001 C CNN
	1    5700 5200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR05
U 1 1 58D82980
P 4700 4850
F 0 "#PWR05" H 4700 4700 50  0001 C CNN
F 1 "+5V" H 4700 4990 50  0000 C CNN
F 2 "" H 4700 4850 50  0000 C CNN
F 3 "" H 4700 4850 50  0000 C CNN
	1    4700 4850
	1    0    0    -1  
$EndComp
Text Label 3800 5200 2    60   ~ 0
TX
Text Label 6750 4150 0    60   ~ 0
TX
Text Label 6750 4250 0    60   ~ 0
RX
Text Label 3650 5100 0    60   ~ 0
SP1
Text Label 3650 5300 0    60   ~ 0
SP2
$Comp
L device:C C4
U 1 1 58D83C5D
P 4700 5200
F 0 "C4" H 4725 5300 50  0000 L CNN
F 1 "100nF" H 4725 5100 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 4738 5050 50  0001 C CNN
F 3 "" H 4700 5200 50  0000 C CNN
	1    4700 5200
	1    0    0    -1  
$EndComp
$Comp
L device:C C1
U 1 1 58D83D11
P 1600 2750
F 0 "C1" H 1625 2850 50  0000 L CNN
F 1 "100nF" H 1625 2650 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 1638 2600 50  0001 C CNN
F 3 "" H 1600 2750 50  0000 C CNN
	1    1600 2750
	1    0    0    -1  
$EndComp
$Comp
L device:C C2
U 1 1 58D83E55
P 1600 4000
F 0 "C2" H 1625 4100 50  0000 L CNN
F 1 "100nF" H 1625 3900 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 1638 3850 50  0001 C CNN
F 3 "" H 1600 4000 50  0000 C CNN
	1    1600 4000
	1    0    0    -1  
$EndComp
$Comp
L device:C C3
U 1 1 58D83F13
P 1600 5200
F 0 "C3" H 1625 5300 50  0000 L CNN
F 1 "100nF" H 1625 5100 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 1638 5050 50  0001 C CNN
F 3 "" H 1600 5200 50  0000 C CNN
	1    1600 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 58D83FE1
P 1600 5500
F 0 "#PWR06" H 1600 5250 50  0001 C CNN
F 1 "GND" H 1600 5350 50  0000 C CNN
F 2 "" H 1600 5500 50  0000 C CNN
F 3 "" H 1600 5500 50  0000 C CNN
	1    1600 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 58D8402C
P 1600 4300
F 0 "#PWR07" H 1600 4050 50  0001 C CNN
F 1 "GND" H 1600 4150 50  0000 C CNN
F 2 "" H 1600 4300 50  0000 C CNN
F 3 "" H 1600 4300 50  0000 C CNN
	1    1600 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 58D84055
P 1600 3050
F 0 "#PWR08" H 1600 2800 50  0001 C CNN
F 1 "GND" H 1600 2900 50  0000 C CNN
F 2 "" H 1600 3050 50  0000 C CNN
F 3 "" H 1600 3050 50  0000 C CNN
	1    1600 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR09
U 1 1 58D840A6
P 1600 2400
F 0 "#PWR09" H 1600 2250 50  0001 C CNN
F 1 "+5V" H 1600 2540 50  0000 C CNN
F 2 "" H 1600 2400 50  0000 C CNN
F 3 "" H 1600 2400 50  0000 C CNN
	1    1600 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR010
U 1 1 58D840CF
P 1600 3700
F 0 "#PWR010" H 1600 3550 50  0001 C CNN
F 1 "+5V" H 1600 3840 50  0000 C CNN
F 2 "" H 1600 3700 50  0000 C CNN
F 3 "" H 1600 3700 50  0000 C CNN
	1    1600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 58D840E9
P 1600 4900
F 0 "#PWR011" H 1600 4750 50  0001 C CNN
F 1 "+5V" H 1600 5040 50  0000 C CNN
F 2 "" H 1600 4900 50  0000 C CNN
F 3 "" H 1600 4900 50  0000 C CNN
	1    1600 4900
	1    0    0    -1  
$EndComp
$Comp
L device:R R7
U 1 1 58D84DCB
P 8500 3200
F 0 "R7" V 8550 3400 50  0000 C CNN
F 1 "10kR" V 8500 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8430 3200 50  0001 C CNN
F 3 "" H 8500 3200 50  0000 C CNN
	1    8500 3200
	1    0    0    -1  
$EndComp
$Comp
L device:R R6
U 1 1 58D84FD2
P 8400 3200
F 0 "R6" V 8450 3400 50  0000 C CNN
F 1 "10kR" V 8400 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8330 3200 50  0001 C CNN
F 3 "" H 8400 3200 50  0000 C CNN
	1    8400 3200
	1    0    0    -1  
$EndComp
$Comp
L device:R R5
U 1 1 58D85022
P 8300 3200
F 0 "R5" V 8350 3400 50  0000 C CNN
F 1 "10kR" V 8300 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8230 3200 50  0001 C CNN
F 3 "" H 8300 3200 50  0000 C CNN
	1    8300 3200
	1    0    0    -1  
$EndComp
$Comp
L device:R R4
U 1 1 58D85028
P 8200 3200
F 0 "R4" V 8250 3400 50  0000 C CNN
F 1 "10kR" V 8200 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8130 3200 50  0001 C CNN
F 3 "" H 8200 3200 50  0000 C CNN
	1    8200 3200
	1    0    0    -1  
$EndComp
$Comp
L device:R R2
U 1 1 58D856FF
P 7600 3200
F 0 "R2" V 7650 3400 50  0000 C CNN
F 1 "10kR" V 7600 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7530 3200 50  0001 C CNN
F 3 "" H 7600 3200 50  0000 C CNN
	1    7600 3200
	1    0    0    -1  
$EndComp
$Comp
L device:R R1
U 1 1 58D85705
P 7500 3200
F 0 "R1" V 7550 3400 50  0000 C CNN
F 1 "10kR" V 7500 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7430 3200 50  0001 C CNN
F 3 "" H 7500 3200 50  0000 C CNN
	1    7500 3200
	1    0    0    -1  
$EndComp
$Comp
L device:R R3
U 1 1 58D8638C
P 7700 3200
F 0 "R3" V 7750 3400 50  0000 C CNN
F 1 "10kR" V 7700 3200 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7630 3200 50  0001 C CNN
F 3 "" H 7700 3200 50  0000 C CNN
	1    7700 3200
	1    0    0    -1  
$EndComp
Text Label 4800 4600 0    60   ~ 0
Pin2
Text Label 9000 2550 0    60   ~ 0
DispLoad
Text Label 9000 2450 0    60   ~ 0
DispClock
Text Label 9000 2650 0    60   ~ 0
DispData
Text Label 6900 2550 2    60   ~ 0
SDA
Text Label 3800 5400 2    60   ~ 0
RX
Text Label 6900 2450 2    60   ~ 0
SCL
$Comp
L Bus-Posteriore-rescue:CONN_01X05 P1
U 1 1 58DF67B1
P 9700 2650
F 0 "P1" H 9700 2950 50  0000 C CNN
F 1 "CONN_01X05" V 9800 2650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 9700 2650 50  0001 C CNN
F 3 "" H 9700 2650 50  0000 C CNN
	1    9700 2650
	1    0    0    1   
$EndComp
Text Notes 2400 1400 0    60   ~ 0
Modifiche:\nPin J2-7 collegato a GND
Text Label 6750 4900 0    60   ~ 0
SP1
Text Label 6750 5400 0    60   ~ 0
SP2
Connection ~ 3200 2850
Wire Wire Line
	3150 2850 3200 2850
Connection ~ 3300 2950
Wire Wire Line
	3150 2950 3300 2950
Wire Wire Line
	8200 3600 8900 3600
Connection ~ 8700 2650
Wire Wire Line
	6650 5800 4300 5800
Wire Wire Line
	6650 5000 6650 5800
Wire Wire Line
	6500 5000 6650 5000
Wire Wire Line
	7300 2850 6450 2850
Wire Wire Line
	7500 2850 7600 2850
Connection ~ 8600 2750
Wire Wire Line
	6450 2550 6950 2550
Wire Wire Line
	6450 2450 6950 2450
Wire Wire Line
	9500 2650 8900 2650
Wire Wire Line
	9500 2450 8900 2450
Wire Wire Line
	9500 2550 8900 2550
Connection ~ 4700 5500
Wire Wire Line
	4700 5500 4700 5350
Connection ~ 4700 4900
Connection ~ 7700 2850
Wire Wire Line
	7700 2850 7700 3050
Connection ~ 6550 4050
Wire Wire Line
	7700 4050 7700 3350
Wire Wire Line
	7600 4250 7600 3350
Wire Wire Line
	7500 3350 7500 4150
Connection ~ 7600 2850
Connection ~ 8200 2850
Wire Wire Line
	7600 3050 7600 2850
Wire Wire Line
	7500 2850 7500 3050
Wire Wire Line
	8200 3350 8200 3600
Wire Wire Line
	8300 3350 8300 4100
Connection ~ 8400 4200
Wire Wire Line
	8400 3350 8400 4200
Connection ~ 8500 4000
Connection ~ 8300 2850
Wire Wire Line
	8300 2850 8300 3050
Connection ~ 8400 2850
Wire Wire Line
	8400 2850 8400 3050
Connection ~ 8500 2850
Wire Wire Line
	8200 2850 8200 3050
Connection ~ 8700 2850
Wire Wire Line
	8500 2850 8500 3050
Wire Wire Line
	1600 5350 1600 5500
Wire Wire Line
	1600 4900 1600 5050
Wire Wire Line
	1600 4150 1600 4300
Wire Wire Line
	1600 3700 1600 3850
Wire Wire Line
	1600 2900 1600 3050
Wire Wire Line
	1600 2400 1600 2600
Wire Wire Line
	3150 5100 3800 5100
Wire Wire Line
	6450 4250 7600 4250
Wire Wire Line
	3150 5300 3800 5300
Wire Wire Line
	6450 4050 6550 4050
Wire Wire Line
	7500 4150 6450 4150
Wire Wire Line
	3150 5400 3800 5400
Wire Wire Line
	3150 5200 3800 5200
Connection ~ 8600 4300
Wire Wire Line
	6450 2750 8600 2750
Wire Wire Line
	4700 4850 4700 4900
Wire Wire Line
	4900 4900 4700 4900
Connection ~ 3300 3050
Wire Wire Line
	3150 3050 3300 3050
Wire Wire Line
	4300 2650 4900 2650
Wire Wire Line
	4300 5800 4300 2650
Wire Wire Line
	7300 5300 6500 5300
Wire Wire Line
	7300 2850 7300 5300
Wire Wire Line
	3900 2650 3150 2650
Wire Wire Line
	3900 2350 3150 2350
Wire Wire Line
	3900 4000 3150 4000
Wire Wire Line
	3900 4200 3150 4200
Connection ~ 3300 3700
Wire Wire Line
	3300 3700 3150 3700
Wire Wire Line
	3900 3900 3150 3900
Wire Wire Line
	8600 2750 8600 3700
Wire Wire Line
	8600 4300 8900 4300
Wire Wire Line
	8700 4400 8900 4400
Wire Wire Line
	8700 2200 8700 2650
Connection ~ 3300 5500
Connection ~ 3300 4300
Wire Wire Line
	3150 5500 3300 5500
Wire Wire Line
	3300 4300 3150 4300
Wire Wire Line
	3300 2450 3300 2950
Connection ~ 3200 3150
Connection ~ 3200 4400
Wire Wire Line
	3200 3150 3150 3150
Wire Wire Line
	3200 4400 3150 4400
Wire Wire Line
	3200 1950 3200 2850
Wire Wire Line
	3200 5600 3150 5600
Wire Wire Line
	7050 2950 6450 2950
Wire Wire Line
	7050 3050 6450 3050
Wire Wire Line
	7050 3650 6450 3650
Wire Wire Line
	7050 3250 6450 3250
Wire Wire Line
	7050 3350 6450 3350
Wire Wire Line
	7050 3950 6450 3950
Wire Wire Line
	7050 3150 6450 3150
Wire Wire Line
	7050 3750 6450 3750
Wire Wire Line
	7050 3850 6450 3850
Wire Wire Line
	7050 3550 6450 3550
Wire Wire Line
	8300 4100 8900 4100
Wire Wire Line
	7850 4200 8400 4200
Wire Wire Line
	7850 4000 8500 4000
Wire Wire Line
	8900 3700 8600 3700
Connection ~ 8600 3700
Wire Wire Line
	4100 4600 6550 4600
Wire Wire Line
	8500 3350 8500 4000
Wire Wire Line
	6550 4600 6550 4050
Wire Wire Line
	8400 5200 6500 5200
Wire Wire Line
	8500 5100 6500 5100
Wire Wire Line
	7000 4900 6500 4900
Wire Wire Line
	6500 5400 7000 5400
Wire Wire Line
	3150 2450 3300 2450
Wire Wire Line
	3150 4900 3300 4900
Connection ~ 3300 4900
Wire Wire Line
	4100 2550 3150 2550
Wire Wire Line
	4100 5000 3150 5000
Connection ~ 4100 4600
Wire Wire Line
	4900 3350 4200 3350
Wire Wire Line
	4200 3350 4200 2750
Wire Wire Line
	4200 2750 3150 2750
Text Label 3400 2750 0    60   ~ 0
Zero
Wire Wire Line
	3150 3800 3900 3800
Text Label 3450 3800 0    60   ~ 0
WindSwIn
Wire Notes Line
	9650 3800 9800 3800
Wire Notes Line
	9800 3800 9800 3900
Wire Notes Line
	9800 3900 9650 3900
Text Notes 9650 4000 0    60   ~ 0
Collegare insieme nel connettore\n
Wire Wire Line
	3150 3600 4000 3600
Wire Wire Line
	4000 3600 4000 3450
Wire Wire Line
	4000 3450 4900 3450
Wire Wire Line
	4900 3550 4200 3550
Wire Wire Line
	4200 3550 4200 4800
Wire Wire Line
	4200 4800 3150 4800
$Comp
L device:R R8
U 1 1 59E1AD64
P 7350 2650
F 0 "R8" V 7400 2850 50  0000 C CNN
F 1 "100R" V 7350 2650 50  0000 C CNN
F 2 "Resistors_ThroughHole:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7280 2650 50  0001 C CNN
F 3 "" H 7350 2650 50  0000 C CNN
	1    7350 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 2550 4100 4600
$Comp
L device:C C5
U 1 1 59E3857A
P 9400 5350
F 0 "C5" H 9425 5450 50  0000 L CNN
F 1 "100nF" H 9425 5250 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 9438 5200 50  0001 C CNN
F 3 "" H 9400 5350 50  0000 C CNN
	1    9400 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 59E38580
P 9400 5650
F 0 "#PWR012" H 9400 5400 50  0001 C CNN
F 1 "GND" H 9400 5500 50  0000 C CNN
F 2 "" H 9400 5650 50  0000 C CNN
F 3 "" H 9400 5650 50  0000 C CNN
	1    9400 5650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 59E38586
P 9400 5050
F 0 "#PWR013" H 9400 4900 50  0001 C CNN
F 1 "+5V" H 9400 5190 50  0000 C CNN
F 2 "" H 9400 5050 50  0000 C CNN
F 3 "" H 9400 5050 50  0000 C CNN
	1    9400 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5500 9400 5550
Wire Wire Line
	9400 5050 9400 5150
$Comp
L device:CP C6
U 1 1 59E385CC
P 9100 5350
F 0 "C6" H 9125 5450 50  0000 L CNN
F 1 "100uF" H 9125 5250 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Axial_L11.0mm_D5.0mm_P18.00mm_Horizontal" H 9138 5200 50  0001 C CNN
F 3 "" H 9100 5350 50  0000 C CNN
	1    9100 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 5200 9100 5150
Wire Wire Line
	9100 5150 9400 5150
Connection ~ 9400 5150
Wire Wire Line
	9100 5500 9100 5550
Wire Wire Line
	9100 5550 9400 5550
Connection ~ 9400 5550
Wire Wire Line
	6450 2650 7200 2650
Wire Wire Line
	7500 2650 8700 2650
NoConn ~ 4900 2450
NoConn ~ 4900 2550
NoConn ~ 4900 2750
NoConn ~ 4900 3150
NoConn ~ 4900 3750
NoConn ~ 4900 3850
NoConn ~ 8900 3800
NoConn ~ 8900 3900
Wire Wire Line
	3200 2850 4900 2850
Wire Wire Line
	3200 2850 3200 3150
Wire Wire Line
	3300 2950 4900 2950
Wire Wire Line
	3300 2950 3300 3050
Wire Wire Line
	8700 2650 8700 2850
Wire Wire Line
	8600 2750 9500 2750
Wire Wire Line
	4700 5500 4900 5500
Wire Wire Line
	4700 4900 4700 5050
Wire Wire Line
	7700 2850 8200 2850
Wire Wire Line
	6550 4050 7700 4050
Wire Wire Line
	7600 2850 7700 2850
Wire Wire Line
	8200 2850 8300 2850
Wire Wire Line
	8400 4200 8400 5200
Wire Wire Line
	8400 4200 8900 4200
Wire Wire Line
	8500 4000 8900 4000
Wire Wire Line
	8500 4000 8500 5100
Wire Wire Line
	8300 2850 8400 2850
Wire Wire Line
	8400 2850 8500 2850
Wire Wire Line
	8500 2850 8700 2850
Wire Wire Line
	8700 2850 8700 4400
Wire Wire Line
	8700 2850 9500 2850
Wire Wire Line
	8600 4300 8600 4400
Wire Wire Line
	3300 3050 4900 3050
Wire Wire Line
	3300 3050 3300 3700
Wire Wire Line
	3300 3700 3300 4300
Wire Wire Line
	3300 5500 4700 5500
Wire Wire Line
	3300 5500 3300 5750
Wire Wire Line
	3300 4300 3300 4900
Wire Wire Line
	3200 3150 3200 4400
Wire Wire Line
	3200 4400 3200 5600
Wire Wire Line
	8600 3700 8600 4300
Wire Wire Line
	3300 4900 3300 5500
Wire Wire Line
	4100 4600 4100 5000
Wire Wire Line
	9400 5150 9400 5200
Wire Wire Line
	9400 5550 9400 5650
Text Notes 9950 2850 0    60   ~ 0
Su display:\nclk\ncs\ndin\ngnd\nvcc\n
Text Notes 4400 6150 0    60   ~ 0
(*) su PCB V1 va aggiunta con un filo
Wire Wire Line
	3150 4100 4400 4100
Wire Wire Line
	4400 4100 4400 3650
Wire Wire Line
	4400 3650 4900 3650
Text Label 3400 4100 0    60   ~ 0
swEndRun
Text Label 4350 2650 0    60   ~ 0
WDReset
Text Label 6550 2850 0    60   ~ 0
WDPulse
Text Notes 5100 5800 0    60   ~ 0
(*)
Text Notes 3900 4100 0    60   ~ 0
(*)
Text Notes 7000 2850 0    60   ~ 0
(*)
$EndSCHEMATC
