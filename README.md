# Zephyr IO-Link Master Module

This repository integrates a [forked](https://github.com/vChavezB/i-link/tree/zephyr) IO-Link Master stack from RT Labs that enables its integration with Zephyr.

## Supported PHYS

The RT-Labs stack only supports the MAX14189 PHY. This requires only from the Zephyr board side an SPI interface and an IRQ pin.

## License

The IO-Link stack (i.e., backend) is provided by RT-Labs as GPLv3.0. Hence This Zephyr port
is also licensed as GPLv3.0.


## Current Status

This repository is under development and will not provide a stable API/Documentation until the first release version.

### TODOS

- Device Tree bindings for Drivers
- Friendly API to initialize stack 
- Test with different boards
- Stability
- Samples
- Documentation (Usage, Caveats)



