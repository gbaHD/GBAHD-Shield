# gbaHD Shield
This is a shield for the [gbaHD project](https://github.com/zwenergy/gbaHD).  

Has the following features.
- Connects via female headers to the Edge board (shield)
- Has a FFC 40 pin connector for OGBA LCD connector.
- Has a FFC 34 pin connector for GBA-SP LCD connector.
- Has a FFC 16 pin connector for controls with a wire up board.
- Has a ATMEGA328p for using a SNES connector to control the GBA.
- Has a high-side switch for GBA power controls (IGR).
- Has eight (8) testpoints on the bottom side for future features.
- **Both FFC cables need to be opposite side**

## Issues
- Breakout board needs to be made bigger.
- FFC connectors need more space.

## Contributing
If you wish to contribute, see something wrong or want to add a feature please make a pull request or leave an issue!

## BOM
|Reference	|Value							|
|---------	|--------						|
|C1, C3			|100nF 0805							|
|C4			|22uF 0805							|
|R1, R2, R3			|10K 0805								|
|R4			|16.2K 0805								|
|R5			|7.5K 0805								|
|U1			|SNES 7Pin						|
|U2			|ATMEGA328p-AU					|
|U4,U3		|74HC595 TSSOP16			|
|U5			|AMS1117-adj SOT223-3						|
|SW1			|XKB-TS-1185EC-C-D-B			|
|J1			|JUSHUO_AFC07-S40FCC-00		|
|J2			|PinHeader 2x08 2.54mm		|
|J3			|JUSHUO_AFC07-S16FCC-00		|
|J4			|PinHeader 2x10 2.54mm		|
|J6			|PinHeader 2x3 2.54mm		|
|J9			|JUSHUO_AFC07-S34FCC-00		|
|Q1			|SI2301DS P-Chan MOSFET		|

## Programming the MCU
In order to program the MCU, you need an arduino to use as ISP.  
Use the following image to wire up an arduino to the board to program it.  
**I would also recomend not programming it while seated in the FPGA board.**
![PCB](./static/icsp.png "Wireup")  
After wiring the board, follow this tutorial to flash the bootloader and the sketch to the board.  
[ICSP Tutorial](https://www.arduino.cc/en/pmwiki.php?n=Tutorial/ArduinoISP)

## Wiring up the +5V
To have no cables laying around i decided to connect +5V supply to the unsused Pin on the Spartan 7 board.
Therefore you need to solder a little wire from pin ARD15 (pin between GND and AD_SDA) to any +5V supply.

## Choosing the supply voltage
As the OGBA and the GBA-SP use different battery technologies, they also require different voltages.
The OGBA needs approx. 3V (2x 1.5V from AA cells - 3.3 will also fit) and the GBA-SP needs 4.2V (fully loaded lithium rechargeable battery).
Therefore we have a solder-jumper (SJ2) which we can close to get 3.3V or open to get 4.2V.

## Images
![PCB](./static/pcb.png "PCB")
![Breakout](./static/breakout.png "Breakout PCB")
