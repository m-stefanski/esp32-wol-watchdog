# esp32-wol-watchdog

Tiny ESP32 watchdog that once turned on:

1. Continuously tries to connect to defined WiFi
2. When connected to WiFi, continously pings defined IP addresses 
3. In case IP adress does not respond in defined amount of attempts it sends Wake-on-LAN package

It is intended use case it to wake up devices after power shortage, in case they are missing option for waking up when power is available - or they shut down while on UPS but the power state never changed because UPS maintained it until grid came back online.

## Prerequisites

* Any esp32 board
* VScode 
* PlatformIO

## How to use

1. Copy `src/config.sample.h` to `config.h`
2. Fill in all the variables, make sure you put IP and MAC adresses in the same order!
3. Build and upload to ESP32 

## License

This project is licensed under GPLv3.