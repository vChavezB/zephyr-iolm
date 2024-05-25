# Zephyr IO-Link Master Module

This repository integrates a [forked](https://github.com/vChavezB/i-link/tree/zephyr) IO-Link Master stack from RT Labs that enables its integration with Zephyr.

## Supported PHYS

The RT-Labs stack only supports the MAX14189 PHY. This requires a Zephyr board that has an SPI interface and GPIOs for the Chip select and IRQ.

## License

The IO-Link stack (i.e., backend) is provided by RT-Labs as GPLv3.0. Hence This Zephyr port
is also licensed as GPLv3.0.

## Tested boards

- ESP32 Devkit C
- NRF52833 DK

Zephyr version v3.6.99 commit `34c84eccec0508b16f5001b20fa369f387d856df`

## Testing

Build and flash the sample located in `samples/demo` and follow the Readme.md. This 
can be considered as a Proof-of-concept and will be deprecated in the future.

In addition a sample to read PD In is in progress (`samples/read_pd`). This shall
demonstrate the use of the API once the first alpha version is released.

## Current Status

This project is in alpha version and will not provide a stable API/Documentation until the first release.

Currently building API to use easily the IOLM Stack.

## Issues

- Legacy devices (V.1.0) do not go to operate. Some tests have been done and it seems
that the RT-Labs stack cannot process events for legacy devices. The device goes to operate
but the System Managemen (SM) and SMI Configuration Management (CM) do not report back this change.
This happens as it seems that the OD Handler for the event state machine gets stuck.


### TODOS

- API to use IOLM
- Documentation (Usage, Caveats)




