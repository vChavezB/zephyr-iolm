/* Copyright (c) 2024 Victor Chavez
   SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <stdio.h>
#include "osal.h"
#include "osal_irq.h"
#include "osal_log.h"
#include "iolink.h"
#include "iolink_max14819.h"
#include "iolink_handler.h"
#include <iolm/transciever.h>

#define IOLINK_HANDLER_THREAD_STACK_SIZE (1024)
#define IOLINK_HANDLER_THREAD_PRIO       CONFIG_IOLINK_MASTER_PRIO+1
static K_THREAD_STACK_DEFINE(iolm_handler_stack, IOLINK_HANDLER_THREAD_STACK_SIZE); 

void main_handler_thread (void * ctx)
{
   const iolink_m_cfg_t * cfg = (const iolink_m_cfg_t *)ctx;
   iolink_handler (*cfg);
}

static iolink_m_cfg_t iolink_cfg = {
   .cb_arg                   = NULL,
   .cb_smi                   = NULL,
   .cb_pd                    = NULL,
};

os_thread_t * iolink_handler_thread;


iolink_port_cfg_t port_cfgs[CONFIG_IOLINK_NUM_PORTS];
static iolink_pl_mode_t init_mode[] =
{
   iolink_mode_SDCI,
   iolink_mode_INACTIVE,
};


int main(void)
{
   LOG_INF("IOLM Demo");
   uint8_t transciever_cnt = 0;
   for (int i = 0; i < CONFIG_IOLINK_NUM_PORTS; i++) {
      iolink_hw_drv_t * drv =  get_drv(i);
      if (drv == NULL) {
         LOG_ERR("No driver found for port %d", i);
         return -ENODEV;
      }
      port_cfgs[i].drv = drv;
      port_cfgs[i].name = "dummy"; // not used in stack at all
      port_cfgs[i].mode = &init_mode[0];
      const uint8_t ch_no = i%TRANSCIEVER_MAX_PORTS;
      port_cfgs[i].arg = (void*)(ch_no);
   }

   iolink_cfg.port_cnt  = CONFIG_IOLINK_NUM_PORTS;
   iolink_cfg.port_cfgs = port_cfgs;
   LOG_INF("Total port cfg %d", iolink_cfg.port_cnt);

   iolink_handler_thread = os_thread_create (
      "iolink_handler_thread",
      IOLINK_HANDLER_THREAD_PRIO,
      iolm_handler_stack,
      K_THREAD_STACK_SIZEOF(iolm_handler_stack),
      main_handler_thread,
      (void*)&iolink_cfg);
   CC_ASSERT (iolink_handler_thread != NULL);
   return 0;
}
