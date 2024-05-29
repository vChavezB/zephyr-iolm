/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef OSAL_LOG_H
#define OSAL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "osal.h"
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(iol_master);

/** Log debug messages */
#define LOG_DEBUG(type, ...) LOG_DBG( __VA_ARGS__)

/** Log informational messages */
#define LOG_INFO(type, ...) LOG_INF( __VA_ARGS__)

/** Log warning messages */
#define LOG_WARNING(type, ...) LOG_WRN( __VA_ARGS__)

/** Log error messages */
#define LOG_ERROR(type, ...) LOG_ERR(__VA_ARGS__)

/** Log fatal messages */
#define LOG_FATAL(type, ...) LOG_ERR( __VA_ARGS__)



#ifdef __cplusplus
}
#endif

#endif /* OSAL_LOG_H */
