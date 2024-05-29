/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <iolink.h>
#include <iolink_types.h>

struct iolm_port_cfg {
    uint16_t vid;        // Required if validation = IOLINK_VALIDATION_CHECK_V11_RESTORE or IOLINK_VALIDATION_CHECK_V11_BAK_RESTORE
    uint32_t did;    // Required if validation = IOLINK_VALIDATION_CHECK_V11_RESTORE or IOLINK_VALIDATION_CHECK_V11_BAK_RESTORE
    uint32_t cycle_time_us; // Desired cycle time in microseconds, initialize to 0 if you want to set As fast as Possible
    iolink_portmode_t portmode;
    iolink_validation_check_t validation;
};


enum iolm_port_evt{
    IOLM_PORT_PD= 0,    // New PD cycle data available
    IOLM_PORT_STATUS,   // Port status changed
    IOLM_PORT_DEV_EVT     // Device event
};

struct iolm_pd_data {
    const uint8_t * data;
    size_t len;
};

/**
 * @brief IO-Link Device event entry
 * 
 */
struct iolm_evt_entry {
    iolink_event_mode_t mode;
    iolink_event_type_t type;
    uint16_t event_code;
};

/**
 * @brief IO-Link Master device event
 * 
 */
struct iolm_dev_evt {
    struct iolm_evt_entry * entry;
    size_t len;
};

/**
 * @brief  IO-Link Master port event callback
 * 
 */
typedef void (*iolm_port_evt_cb)(uint8_t port, enum iolm_port_evt evt, void *data, void *arg);

/**
 * @brief Initialize the IO-Link Master
 * @param init_cfg Initial configuration for the IO-Link ports. If NULL is passed
 *                  all the ports will be set to IOLINK_PORTMODE_IOL_AUTO and the cycle time as fast as possible
 * @return 0 on success
 *          -ENODEV When a driver fails to initialize or Master struct failed to initialize
 *          -EIO When port configuration failed
 */
int iolm_init(const struct iolm_port_cfg * init_cfg);

/**
 * @brief  Set the IO-Link Master port event callback
 * 
 * @param cb   Callback function
 * @param arg  Argument to be passed to the callback
 */
void iolm_set_port_evt_cb(iolm_port_evt_cb cb, void * arg);

