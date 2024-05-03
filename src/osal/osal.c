#include <osal.h>

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <sys/osal_sys.h>
#include <stdbool.h>

void * os_malloc (size_t size)
{
   return malloc (size);
}

void os_free (void * ptr)
{
   free (ptr);
}

static inline k_timeout_t get_timeout(uint32_t osal_time) {
   if (osal_time == OS_WAIT_FOREVER) {
      return K_FOREVER;
   } else {
      return K_MSEC(time);
   }
}

void os_thread_entry(void *entry, void *arg, void *name) {
   printk("Thread entry %s\n",name);
   void (*entry_func)(void * arg) = (void (*)(void * arg))entry;
   entry_func(arg);
   //free(thread);  // TODO do we need to free the thread or iol-stack thread does not exit?
}

os_thread_t * os_thread_create (
   const char * name,
   uint32_t priority,
   void * const stkSto,
   const uint16_t stkSize,
   void (*entry) (void * arg),
   void * arg)
{
   struct k_thread * thread = (struct k_thread *)malloc (sizeof(struct k_thread));
   if (thread == NULL) {
      return NULL;
   }
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
   printk("thread create %s, prio %d\n",name,priority);
   return thread;
}

os_mutex_t * os_mutex_create (void)
{
   struct k_mutex * mutex = (struct k_mutex *)malloc (sizeof(struct k_mutex));
   CC_ASSERT (mutex != NULL);
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
   free(mutex);
}

void os_usleep (uint32_t us)
{
   k_sleep(K_USEC(us));
}

uint32_t os_get_current_time_us (void)
{
   return k_uptime_get_32()*1000;
}

os_sem_t * os_sem_create (size_t count)
{
   struct k_sem * sem = (struct k_sem *)malloc (sizeof(struct k_sem));
   CC_ASSERT (sem != NULL);
   int rc = k_sem_init(sem, count, count);
   if (rc!=0) {
      free(sem);
      return NULL;
   }
   return sem;
}

bool os_sem_wait (os_sem_t * sem, uint32_t time)
{
   return k_sem_take(sem, get_timeout(time)) == 0;
}

void os_sem_signal (os_sem_t * sem)
{
   k_sem_give(sem);
}

void os_sem_destroy (os_sem_t * sem)
{
   k_sem_reset(sem);
   free(sem);
}

os_event_t * os_event_create (void)
{
   struct k_event * event = (struct k_event *)malloc (sizeof(struct k_event));
   CC_ASSERT (event != NULL);
   k_event_init(event);
   printk("os_event_create addr %x\n",event);
   return event;
}

bool os_event_wait (os_event_t * event, uint32_t mask, uint32_t * value, uint32_t time)
{
  printk("[%d] os_event_wait addr %x, t %d\n",k_uptime_get_32(),event, time);
  int evts =  k_event_wait(event, mask,false, get_timeout(time));
  printk("rc: %d\n",evts);
  if (evts <= 0 ) {
     printk("timeout\n");
     return true;
  }
  printk("[%d] os_event_wait_end %x\n",k_uptime_get_32(),event);
  *value = evts;
  return false;
}

void os_event_set (os_event_t * event, uint32_t value)
{
   k_event_set(event, value);
}

void os_event_clr (os_event_t * event, uint32_t value)
{
   k_event_clear(event, value);
}

void os_event_destroy (os_event_t * event)
{
   free(event);
}

os_mbox_t * os_mbox_create (size_t size)
{
  
   os_mbox_t * mbox;
   mbox = (os_mbox_t *)malloc (sizeof (*mbox) + size * sizeof (void *));
   if (mbox == NULL)
   {
      return NULL;
   }
   k_mutex_init(&mbox->mutex);

   mbox->r     = 0;
   mbox->w     = 0;
   mbox->count = 0;
   mbox->size  = size;
   return mbox;

}

bool os_mbox_fetch (os_mbox_t * mbox, void ** msg, uint32_t time)
{
   int error     = 0;
   //printk("os_mbox_fetch %x\n",mbox);
   while (mbox->count == 0)
   {
      error = k_mutex_lock (&mbox->mutex, get_timeout(time));
      if (error) {
         //printk("nok\n");
         return true;
      }
   }
   *msg = mbox->msg[mbox->r++];
   if (mbox->r == mbox->size)
      mbox->r = 0;

   mbox->count--;
   //printk("ok\n");
   return false;
}

bool os_mbox_post (os_mbox_t * mbox, void * msg, uint32_t time)
{
   //printk("os_mbox_post %x\n",mbox);
   int error = 0;
   while (mbox->count == mbox->size)
   {
      error = k_mutex_lock (&mbox->mutex, get_timeout(time));
      if (error) {
         //printk("nok\n");
         return true;
      }
   }


   mbox->msg[mbox->w++] = msg;
   if (mbox->w == mbox->size)
      mbox->w = 0;

   mbox->count++;
   //printk("ok\n");
   return false;
}

void os_mbox_destroy (os_mbox_t * mbox)
{
   free (mbox);
}

static void timer_internal_cb(struct k_timer * timer)
{
   os_timer_t * os_timer = (os_timer_t *)timer;
   os_timer->fn(os_timer, os_timer->arg);
}

os_timer_t * os_timer_create (
   uint32_t us,
   void (*fn) (os_timer_t *, void * arg),
   void * arg,
   bool oneshot)
{
   struct k_timer * handle = (struct k_timer *)malloc (sizeof(struct k_timer));
   if (handle == NULL) {
      return NULL;
   }
   os_timer_t * os_timer = (os_timer_t *)malloc (sizeof(os_timer_t));
   if (os_timer == NULL) {
      free(handle);
      return NULL;
   }
   k_timer_init(handle, timer_internal_cb, NULL);
   os_timer->one_shot = oneshot;
   os_timer->handle = handle;
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
   if (timer->one_shot) {
      k_timer_start(timer->handle, K_USEC(timer->us), K_NO_WAIT);
   } else {
      k_timer_start(timer->handle, K_USEC(timer->us), K_USEC(timer->us));
   }
}

void os_timer_stop (os_timer_t * timer)
{
   k_timer_stop(timer->handle);
}

void os_timer_destroy (os_timer_t * timer)
{
   free(timer->handle);
   free(timer);
}