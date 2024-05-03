/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef OSAL_SYS_H
#define OSAL_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/kernel.h>
#include <time.h>

#define OS_THREAD
#define OS_MUTEX
#define OS_SEM
#define OS_EVENT
#define OS_MBOX
#define OS_TIMER
#define OS_TICK

typedef struct k_thread os_thread_t;
typedef struct k_mutex os_mutex_t;
typedef struct k_sem os_sem_t;
typedef struct k_event os_event_t;
typedef uint32_t os_tick_t;

typedef struct os_timer
{
   struct k_timer * handle;
   void(*fn) (struct os_timer *, void * arg);
   void * arg;
   uint32_t us;
   bool one_shot;
} os_timer_t;


typedef struct os_mbox
{
   struct k_mutex mutex;
   size_t r;
   size_t w;
   size_t count;
   size_t size;
   void * msg[];
} os_mbox_t;

#ifdef __cplusplus
}
#endif

#endif /* OSAL_SYS_H */