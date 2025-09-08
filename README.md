# SmartHouse

This repository contains the SmartHouse project, an IoT-enabled smart home controller built on Arduino. It monitors environmental conditions and provides remote control of connected devices.

## Features

- Reads temperature and humidity using a DHT sensor.
- Controls home appliances via relay modules.
- Provides a simple serial interface for monitoring and commands.
- Modular design allows adding more sensors and actuators.

## Software / Stack

- Language: C++
- Platform: Arduino
- Libraries: DHT sensor library (included under `library/` folder).

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
