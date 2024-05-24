/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/
#pragma once
#include <iolink_pl_hw_drv.h>
#include <stdint.h>

#define TRANSCIEVER_MAX_PORTS 2

/**
 * @brief Get the Maxim14819 Driver object initialized in src/transciever.c
 * 
 * @param port The IO-Link port number associated to the maxim driver.
 *              The value should be 0 to CONFIG_IOLINK_NUM_PORTS-1
 * @return driver object if found, NULL otherwise
 */
iolink_hw_drv_t * get_drv(uint8_t port);