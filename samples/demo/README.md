# Zephyr IO-Link Master Demo

A sample application running the Zephyr IO-Link Master module with the i-link rt-lab stack.

## Tested Boards

- NRF52833DK
- ESP32

## Build and Flash

```
git clone --recursive https://github.com/vChavezB/zephyr-iolm/
cd YOUR_ZEPHYR_WEST_WORKSPACE
west build zephyr-iolm/samples/demo -b YOUR_BOARD
west flash
```

Where:

- YOUR_ZEPHYR_WEST_WORKSPACE: Is the installation of your Zephyr SDK (i.e. `west init ..`). 
  Note: This has only been tested with Zephyr revision v3.6.99 commit `34c84eccec0508b16f5001b20fa369f387d856df`
- `YOUR_BOARD`: `esp32`, `nrf52833dk_nrf52833`

## Expected Output

```
*** Booting Zephyr OS build ***
[00:00:00.252,868] <inf> iol_master: IOLM Demo
[00:00:00.252,868] <inf> iol_master: Total port cfg 2
[00:00:00.252,929] <inf> iol_master: thread created iolink_handler_thread, prio 4
[00:00:00.252,960] <inf> iol_master: Thread entry iolink_handler_thread
[00:00:00.252,990] <inf> iol_master: os_event_create addr 2000a550
[00:00:00.255,493] <inf> iol_master: Thread entry iolink_m_thread
[00:00:00.255,584] <inf> iol_master: thread created iolink_m_thread, prio 3
[00:00:00.255,737] <inf> iol_master: os_event_create addr 2000cd40
[00:00:00.255,859] <inf> iol_master: Thread entry iolport1
[00:00:00.255,920] <inf> iol_master: thread created iolport1, prio 2
[00:00:00.255,981] <inf> iol_master: os_event_create addr 2000ced0
[00:00:00.257,843] <inf> iol_master: os_event_create addr 2000cf68
[00:00:00.257,965] <inf> iol_master: Thread entry iolport2
[00:00:00.258,026] <inf> iol_master: thread created iolport2, prio 2
[00:00:00.258,056] <inf> iol_master: os_event_create addr 2000d0f8
[00:00:00.259,765] <inf> iol_master: iolink_handler started
[00:00:00.386,138] <inf> iol_master: handle_smi_portevent (1): type = 0, event_code = 0xFF26, count = 0
[00:00:00.386,169] <inf> iol_master: Port status changed
[00:00:00.386,505] <inf> iol_master: iolink_start_port: Port 0: iolink device 0x0002b1 for VID 0x0136
[00:00:00.386,535] <inf> iol_master: iolink_start_port: Port 0: Start done!
[00:00:00.950,927] <inf> iol_master: handle_smi_portevent (2): type = 0, event_code = 0xFF26, count = 0
[00:00:00.950,927] <inf> iol_master: Port status changed
[00:00:00.951,263] <inf> iol_master: iolink_start_port: Port 1: iolink device 0x000243 for VID 0x0136
[00:00:00.951,293] <inf> iol_master: iolink_start_port: Port 1: Start done!
[00:00:01.006,591] <inf> iol_master: Port [2] PDIN Cnf 0b61
[00:00:01.017,425] <inf> iol_master: Port [1] PDIN Cnf 00aa
```