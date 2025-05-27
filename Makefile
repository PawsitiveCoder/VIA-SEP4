# Makefile for Arduino PlatformIO + Python venv management
ARDUINO_DIR := arduino
VENV_NAME := .venv
VENV_DIR := $(ARDUINO_DIR)/$(VENV_NAME)
REQ_FILE := $(ARDUINO_DIR)/requirements.txt
PIO := $(VENV_NAME)/bin/platformio

.PHONY: help venv venv-clean install setup build upload monitor upload-monitor test test-target test-native test-coverage pre-commit-install pre-commit-run commit docs

help:
	@echo "Available targets:"
	@echo "  help               - Show this help message"
	@echo "  venv               - Create Python venv in $(VENV_DIR)"
	@echo "  venv-clean         - Remove Python venv"
	@echo "  install            - Install Python requirements into venv"
	@echo "  setup              - Setup Python venv and install requirements"
	@echo "  build              - Build Arduino firmware (target_run)"
	@echo "  upload             - Upload firmware to Arduino (target_run)"
	@echo "  monitor            - Start serial monitor for (target_run)"
	@echo "  upload-monitor     - Upload firmware and start serial monitor"
	@echo "  test               - Run all PlatformIO tests"
	@echo "  test-target        - Run target (on-device) tests"
	@echo "  test-native        - Run native (host) tests"
	@echo "  test-coverage      - Generate code coverage report"
	@echo "  pre-commit-install - Install pre-commit hooks"
	@echo "  pre-commit-run     - Run pre-commit hooks on all files"
	@echo "  commit             - Run Commitizen to create a conventional commit"
	@echo "  docs               - Generate code documentation"

venv:
	python3 -m venv $(VENV_DIR)
	@echo "Python venv created at $(VENV_DIR)"

venv-clean:
	rm -rf $(VENV_DIR)
	@echo "Python venv removed from $(VENV_DIR)"

install:
	$(VENV_DIR)/bin/pip install --upgrade pip
	$(VENV_DIR)/bin/pip install -r $(REQ_FILE)
	@echo "Requirements installed in venv"

setup: venv-clean venv install pre-commit-install pre-commit-run
	@echo "Setup complete."

build:
	cd $(ARDUINO_DIR) && $(PIO) run -e target_run

upload:
	cd $(ARDUINO_DIR) && $(PIO) run -e target_run --target upload

monitor:
	cd $(ARDUINO_DIR) && $(PIO) device monitor -e target_run

upload-monitor: upload monitor

test:
	cd $(ARDUINO_DIR) && $(PIO) test

test-target:
	cd $(ARDUINO_DIR) && $(PIO) test -e target_test

test-native:
	cd $(ARDUINO_DIR) && $(PIO) test -e native_test

test-coverage:
	lcov --capture --include "*/lib/*" \
		--directory $(ARDUINO_DIR)/.pio/build \
		--base-directory $(ARDUINO_DIR) \
		--output-file $(ARDUINO_DIR)/coverage.info
	genhtml $(ARDUINO_DIR)/coverage.info \
		--output-directory $(ARDUINO_DIR)/docs/coverage \
		--legend --highlight --function-coverage \
		--branch-coverage --show-details --sort
	@echo "Coverage report generated in $(ARDUINO_DIR)/docs/coverage"

pre-commit-install:
	$(VENV_DIR)/bin/pre-commit install
	@echo "pre-commit hooks installed"

pre-commit-run: venv
	$(VENV_DIR)/bin/pre-commit run --all-files
	@echo "pre-commit hooks run on all files"

commit:
	$(VENV_DIR)/bin/cz commit

docs:
	cd $(ARDUINO_DIR) && doxygen Doxyfile
	@echo "Documentation generated in $(ARDUINO_DIR)/docs"
	$(MAKE) test-coverage
