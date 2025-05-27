# VIA Semester 4 Project

This repository contains an Arduino PlatformIO project with Python tooling and devcontainer support for streamlined local development.

Authors: [Alex Tofan](https://github.com/savaryna), [Lucia Andronic](https://github.com/PawsitiveCoder)

Links: [Code docs](https://pawsitivecoder.github.io/VIA-SEP4/index.html), [Test Coverage](https://pawsitivecoder.github.io/VIA-SEP4/coverage/index.html)

## Project Overview

This project focuses on greenhouse automation, utilizing sensors to monitor environmental conditions and controlling a pump to optimize plant growth. The system is designed to automate tasks such as irrigation and climate management, making greenhouse operation more efficient and reliable.

## Hardware

The hardware for this project, including the custom Arduino HAT, PCB layout diagrams, pin layout diagrams, and related documentation, is provided by the college. Refer to the `arduino/doc` directory for schematics, pinout diagrams, and assembly instructions. So are most drivers `arduino/lib/drivers` for the sensors used in this project, as well as their respective tests.

## Features

- **PlatformIO** for Arduino firmware development and testing
- **Python virtual environment** for dependency management
- **Pre-commit hooks** for code quality
- **Devcontainers** for consistent development environments

## Getting Started

1. Make sure you have [Docker](https://www.docker.com) installed and running.
2. Clone the repository and cd into it.
3. Open the project in [VS Code](https://code.visualstudio.com/) and click "Reopen in Container".
4. Open the arduino workspace and reload once PlatformIO is done installing.
5. Open a local terminal and run `make setup`. This will:
    - Remove any existing Python venv
    - Create a new venv in `arduino/.venv`
    - Install Python requirements
    - Set up pre-commit hooks

## Useful Make targets

- `make setup` - Recreate venv, install requirements, set up pre-commit hooks
- `make upload-monitor` - Upload firmware and start serial monitor
- `make test-native` - Run native (host) tests

This targets are meant to be run from a local terminal (not devcontainer).
You can learn more about the available make targets by running `make` in the root dir.
