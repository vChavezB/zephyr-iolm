/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <osal.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <sys/osal_sys.h>
#include <stdbool.h>
#include <osal_log.h>

#define TIMERS_PER_PORT 3
#define MUTEX_PER_PORT 6
#define EVT_PER_PORT 2
#define THREAD_PER_PORT 1
#define MBOX_PER_PORT 2
#define TOTAL_TIMERS (TIMERS_PER_PORT * CONFIG_IOLINK_NUM_PORTS)
#define TOTAL_MUTEX (MUTEX_PER_PORT*CONFIG_IOLINK_NUM_PORTS)
#define TOTAL_EVT (EVT_PER_PORT*CONFIG_IOLINK_NUM_PORTS+20)
#define TOTAL_THREADS (CONFIG_IOLINK_NUM_PORTS*THREAD_PER_PORT+5)
#define TOTAL_MBOX (MBOX_PER_PORT*CONFIG_IOLINK_NUM_PORTS)

static uint8_t timer_cnt;
static uint8_t mutex_cnt;
static uint8_t evt_cnt;
static uint8_t thread_cnt;
static uint8_t mbox_cnt;
os_timer_t timer_inst[TOTAL_TIMERS];
struct k_mutex mutex_inst[TOTAL_MUTEX];
struct k_event evt_inst[TOTAL_EVT];
struct k_thread thread_inst[TOTAL_THREADS];
os_mbox_t mbox_inst[TOTAL_MBOX];


static k_timeout_t get_timeout(uint32_t osal_time) {
   if (osal_time == OS_WAIT_FOREVER) {
      return K_FOREVER;
   } else {
      return K_MSEC(osal_time);
   }
}

void os_thread_entry(void *entry, void *arg, void *name) {
   void (*entry_func)(void * arg) = (void (*)(void * arg))entry;
   entry_func(arg);
}

os_thread_t * os_thread_create (
   const char * name,
   uint32_t priority,
   void * const stkSto,
   const uint16_t stkSize,
   void (*entry) (void * arg),
   void * arg)
{
   if (thread_cnt >= TOTAL_THREADS) {
      return NULL;
   }
   struct k_thread * thread = &thread_inst[thread_cnt++];
   const k_tid_t tid = k_thread_create(thread,
                                      (k_thread_stack_t *)(stkSto),
                                       (size_t)stkSize,
                                       &os_thread_entry,
                                       entry, // p1
                                       arg,    // p2
                                       name,    // p3
                                       priority,// Zephyr priority
                                       0,        // thread options
                                       K_NO_WAIT); // start immediately
   if (name != NULL) {
      k_thread_name_set(thread, name);
   }
   return thread;
}

os_mutex_t * os_mutex_create (void)
{
   if(mutex_cnt >= TOTAL_MUTEX) {
      return NULL;
   }
   struct k_mutex * mutex = &mutex_inst[mutex_cnt++];
   k_mutex_init(mutex);
   return (os_mutex_t *)mutex;
}

void os_mutex_lock (os_mutex_t * mutex)
{
   k_mutex_lock(mutex, K_FOREVER);
}

void os_mutex_unlock (os_mutex_t * mutex)
{
   k_mutex_unlock(mutex);
}

void os_mutex_destroy (os_mutex_t * mutex)
{
   mutex_cnt--;
}

void os_usleep (uint32_t us)
{
   k_sleep(K_USEC(us));
}

uint32_t os_get_current_time_us (void)
{
   return k_uptime_get_32()*1000;
}


os_event_t * os_event_create (void)
{
   if (evt_cnt >= TOTAL_EVT) {
      return NULL;
   }
   struct k_event * event = &evt_inst[evt_cnt++];
   CC_ASSERT (event != NULL);
   k_event_init(event);
   LOG_INF("os_event_create addr %x\n",event);
   return event;
}

bool os_event_wait (os_event_t * event, uint32_t mask, uint32_t * value, uint32_t time)
{
  int evts =  k_event_wait(event, mask,false, get_timeout(time));
  if (evts <= 0 ) {
     return true;
  }
  *value = evts;
  return false;
}

void os_event_set (os_event_t * event, uint32_t value)
{
   k_event_post(event, value);
}

void os_event_clr (os_event_t * event, uint32_t value)
{
   k_event_clear(event, value);
}

void os_event_destroy (os_event_t * event)
{
   evt_cnt--;
}


os_mbox_t * os_mbox_create (size_t size)
{
  
   os_mbox_t * mbox;

   if (mbox_cnt >= TOTAL_MBOX || size > MAX_MAILBOX_MSGS) {
      return NULL;
   }
   mbox = &mbox_inst[mbox_cnt++];
   k_mutex_init(&mbox->mutex);
   k_event_init(&mbox->evt);

   mbox->r     = 0;
   mbox->w     = 0;
   mbox->count = 0;
   mbox->size  = size;
   return mbox;
}

size_t get_mbox_cnt(os_mbox_t * mbox){
   uint32_t mbox_cnt;
   k_mutex_lock (&mbox->mutex, K_FOREVER);
   mbox_cnt = mbox->count;
   k_mutex_unlock (&mbox->mutex);
   return mbox_cnt;
}

bool os_mbox_fetch (os_mbox_t * mbox, void ** msg, uint32_t time)
{
   int error     = 0;
   size_t mbox_cnt = get_mbox_cnt(mbox);

   if (mbox_cnt == 0) {
      int evts =  k_event_wait(&mbox->evt, 0x01, false, get_timeout(time));
      if (evts <= 0 ) {
         return true;
      } else {
         k_event_clear(&mbox->evt, 0x01);
      }
   }
   k_mutex_lock (&mbox->mutex, K_FOREVER);
   *msg = mbox->msg[mbox->r++];
   if (mbox->r == mbox->size)
      mbox->r = 0;

   mbox->count--;
   k_event_set(&mbox->evt, 0x02);
   k_mutex_unlock (&mbox->mutex);
   return false;
}

bool os_mbox_post (os_mbox_t * mbox, void * msg, uint32_t time)
{
   int error = 0;
   size_t mbox_cnt = get_mbox_cnt(mbox);

   if (mbox_cnt == mbox->size) {
      int evts =  k_event_wait(&mbox->evt, 0x02, false, get_timeout(time));
      if (evts <= 0 ) {
         return true;
      } else {
         k_event_clear(&mbox->evt, 0x02);
      }
   }
   k_mutex_lock (&mbox->mutex, K_FOREVER);
   mbox->msg[mbox->w++] = msg;
   if (mbox->w == mbox->size)
      mbox->w = 0;

   mbox->count++;
   k_event_set(&mbox->evt, 0x01);
   k_mutex_unlock (&mbox->mutex);
   return false;
}

void os_mbox_destroy (os_mbox_t * mbox)
{
   mbox_cnt--;
}

static void timer_internal_cb(struct k_timer * timer)
{
   os_timer_t * os_timer = (os_timer_t *)timer->user_data;
   if(os_timer->fn) {
      os_timer->fn(os_timer, os_timer->arg);
   }
}

os_timer_t * os_timer_create (
   uint32_t us,
   void (*fn) (os_timer_t *, void * arg),
   void * arg,
   bool oneshot)
{

   if (timer_cnt > TOTAL_TIMERS) {
      return NULL;
   }

   os_timer_t * os_timer = &timer_inst[timer_cnt++];
   k_timer_init(&os_timer->handle, timer_internal_cb, NULL);
   os_timer->handle.user_data = os_timer;
   os_timer->one_shot = oneshot;
   os_timer->fn = fn;
   os_timer->arg = arg;
   return os_timer;
}

void os_timer_set (os_timer_t * timer, uint32_t us)
{
   timer->us = us;
}

void os_timer_start (os_timer_t * timer)
{
   k_timeout_t period;
   if (timer->one_shot) {
      period = K_NO_WAIT;

   } else {
      period = K_USEC(timer->us);
   }
   k_timer_start(&timer->handle, K_USEC(timer->us), period);
}

void os_timer_stop (os_timer_t * timer)
{
   k_timer_stop(&timer->handle);
}
