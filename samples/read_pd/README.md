# Zephyr IOLM Read Process Data Input

A sample application that demonstrates reading PDIN from two IO-Link Devices each second.

## Configuration

Use `CONFIG_PDIN_RATE_MS` and `CONFIG_CYCLETIME_US` to change PDIn parameters for the IO-Link ports.

Note: If the Cycle time is less than the minimum cycle time of the device the stack will use the minimum cycle instead.

## Pinout

Check the overlay configuration to set the correct SPI Pins and IRQ/CS for the MAXIM14819 transciever.


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

```python
*** Booting Zephyr OS build v3.6.0-3220-g69fa7dc2686e ***
[00:00:00.366,668] <inf> read_pd: Zephyr IO-Link Master PDIn Sample Application
[00:00:00.474,731] <inf> read_pd: Port [2]: Status changed
[00:00:00.503,967] <inf> iol_master: Port [1] status changed to Operate
[00:00:00.503,967] <inf> read_pd: Port [1]: Status changed
[00:00:01.026,672] <inf> read_pd: Port [1] PDIn 0000
[00:00:01.062,683] <inf> iol_master: Port [2] status changed to Operate
[00:00:01.062,713] <inf> read_pd: Port [2]: Status changed
[00:00:01.067,932] <inf> read_pd: Port [2] PDIn 0000
[00:00:02.059,936] <inf> read_pd: Port [1] PDIn 0000
```
