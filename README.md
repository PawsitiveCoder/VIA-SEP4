# VIA Semester 4 Project

This repository contains an Arduino PlatformIO project with Python tooling and devcontainer support for streamlined local development.

## Authors

- [Alex Tofan](https://github.com/savaryna)
- [Lucia Andronic](https://github.com/PawsitiveCoder)

## Project Overview

This project focuses on greenhouse automation, utilizing sensors to monitor environmental conditions and controlling a pump to optimize plant growth. The system is designed to automate tasks such as irrigation and climate management, making greenhouse operation more efficient and reliable.

## Features

- **PlatformIO** for Arduino firmware development and testing
- **Python virtual environment** for dependency management
- **Pre-commit hooks** for code quality
- **Devcontainers** for consistent development environments

## Getting Started

### 1. Clone the repository

```sh
git clone <repo-url>
cd <into-the-cloned-repository>
```

### 2. Open in Devcontainer (Recommended)

Open the project in [VS Code](https://code.visualstudio.com/) and use the "Reopen in Container" feature.

### 3. Open the arduino workspace

Open the arduino workspace so that PlatformIO recognizes the sub-folder and reload once installed.

### 4. Open a local terminal and run

```sh
make setup
```

This will:

- Remove any existing Python venv
- Create a new venv in `arduino/.venv`
- Install Python requirements
- Set up pre-commit hooks

## Common Make Targets

- `make build` - Build Arduino firmware
- `make upload` - Upload firmware to Arduino
- `make test` - Run all PlatformIO tests
- `make test-native` - Run native tests

You can see all targets with:

```sh
make help
```
