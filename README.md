# SmartHouse

This repository contains the SmartHouse project, an IoT-enabled smart home controller built on ESP32.

## Features

- Reads temperature and humidity using a DHT sensor.
- Monitors air quality using MQ sensors.
- Checks for leaks in kitchen and basement using water level sensors.
- Controls home appliances via IR remote.
- Triggers sound and LED alarm when movement is noticed inside of the house while Guard Mode is active.
- Sends data to a real-time database and serial for monitoring.
- Modular design allows adding more sensors and actuators.

## Software / Stack

- Language: C++
- Platform: ESP32
- Libraries: Included under `library/` folder

## Build & Run

Compile and upload `SmartHouse.ino` to your Arduino board using the Arduino IDE:

```bash
# Using Arduino IDE:
# 1. Install required libraries (see library folder).
# 2. Open SmartHouse.ino.
# 3. Select the correct board and port.
# 4. Upload to the board.
```

Alternatively, you can build from the command line with `arduino-cli`:

```bash
arduino-cli compile --fqbn <your-board> SmartHouse
arduino-cli upload -p <port> --fqbn <your-board> SmartHouse
```

## Library

Required libraries are stored in the `library/` directory. Extract them into your Arduino libraries folder before building.

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.
