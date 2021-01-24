# gbaHD Shield
This is a shield for the [gbaHD project](https://github.com/zwenergy/gbaHD).  

Has the following features.
- Connects via female headers to the Edge board (shield)
- Has a FFC 40 pin connector for GBA LCD connector.
- Has a FFC 16 pin connector for controls with a wire up board.
- Has a ATMEGA328p for using a SNES connector to control the GBA.
- Has a analog switch for GBA power controls (IGR).
- **Both FFC cables need to be opposite side**

## Issues
- Some footprints on shield need to be made bigger for it to be easier to hand solder (X1, U3).
- Breakout board needs to be made bigger.
- FFC connectors need more space.
- Grabs power from the 3.3V rail of the Edge board, ~~this might be an issue~~ so far no issues.

## Contributing
If you wish to contribute, see something wrong or want to add a feature please make a pull request or leave an issue!

![PCB](./static/pcb.png "PCB")
![Breakout](./static/breakout.png "Breakout PCB")
