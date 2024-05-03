/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef OPTIONS_H
#define OPTIONS_H

/* #undef WITH_MALLOC */

//#cmakedefine LOG_ENABLE
//#cmakedefine WITH_MALLOC

/*
 * Supported IO-Link HW
 */
#define IOLINK_HW_NONE     0
#define IOLINK_HW_MAX14819 1

#ifndef IOLINK_NUM_PORTS
#define IOLINK_NUM_PORTS      (CONFIG_IOLINK_NUM_PORTS)
#endif

#ifndef IOLINK_NUM_DIAG_ENTRIES
#define IOLINK_NUM_DIAG_ENTRIES (CONFIG_IOLINK_DIAG_ENTRIES)
#endif

#ifndef IOLINK_MAX_EVENTS
#define IOLINK_MAX_EVENTS (CONFIG_IOLINK_MAX_EVENTS)
#endif

/*
 * IO-Link HW
 */
#ifndef IOLINK_HW
#define IOLINK_HW IOLINK_HW_MAX14819
#endif

#if IOLINK_HW == IOLINK_HW_MAX14819
#define IOLINK_MAX14819_IRQ_GPIO
//#define IOLINK_MAX14819_IRQ_SPI

//#define IOLINK_MAX14819_RXRDY_GPIO
#define IOLINK_MAX14819_RXRDY_SPI

//#define IOLINK_MAX14819_RXERR_GPIO
#define IOLINK_MAX14819_RXERR_SPI
#endif

#endif  /* OPTIONS_H */
