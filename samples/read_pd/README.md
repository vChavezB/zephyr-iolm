# Zephyr IOLM Read Process Data Input

A sample application that demonstrates how to read PDIn from an IO-Link device


## Build and Flash

```
git clone --recursive https://github.com/vChavezB/zephyr-iolm/
cd YOUR_ZEPHYR_WEST_WORKSPACE
west build zephyr-iolm/samples/read_pd -b YOUR_BOARD
west flash
```

Where:

- YOUR_ZEPHYR_WEST_WORKSPACE: Is the installation of your Zephyr SDK (i.e. `west init ..`). 
  Note: This has only been tested with Zephyr revision v3.6.99 commit `34c84eccec0508b16f5001b20fa369f387d856df`
- `YOUR_BOARD`: `esp32`, `nrf52833dk_nrf52833`

## Expected Output
