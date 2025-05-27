/**
 * @file device.h
 * @brief Header file for device management.
 * This file provides functions to create, destroy, and manage a device,
 * including adding observers and processing sensor data.
 */
#pragma once
#include <stdint.h>
#include "observer.h"

/**
 * @brief Opaque type representing a device.
 * This type is used to encapsulate the details of the device implementation.
 */
typedef struct device *device_t;

/**
 * @brief Creates a device instance.
 *
 * @param id Unique identifier for the device.
 * @param version Version of the device.
 * @param processing_interval Interval in milliseconds for processing sensor data.
 * @return device_t A pointer to the created device instance, or NULL if memory allocation fails.
 */
device_t device_create(const char *id, const char *version, uint32_t processing_interval);

/**
 * @brief Destroys a device instance.
 *
 * @param self Pointer to the device instance to be destroyed.
 */
void device_destroy(device_t self);

/**
 * @brief Adds an observer to the device.
 * This function allows an observer to be added to the device's sensor manager,
 * enabling it to receive updates when sensor data changes.
 *
 * @param self Pointer to the device instance.
 * @param observer Pointer to the observer instance to be added.
 */
void device_add_observer(device_t self, observer_t observer);

/**
 * @brief Processes the device, reading sensor data at specified intervals.
 * This function initializes an interrupt task to read sensor data at the specified processing interval.
 *
 * @param self Pointer to the device instance to be processed.
 */
void device_process(device_t self);
