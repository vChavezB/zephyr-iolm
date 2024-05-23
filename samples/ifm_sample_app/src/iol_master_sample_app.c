/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include <stdio.h>
#include "osal.h"
#include "osal_irq.h"
#include "osal_log.h"
#include "iolink.h"
#include "iolink_max14819.h"
#include "iolink_handler.h"
#include <iolm/zephyr.h>

static const struct device *const spi_dev = DEVICE_DT_GET(DT_ALIAS(iolm_spi));
static const struct gpio_dt_spec irq_dev = GPIO_DT_SPEC_GET(DT_ALIAS(iolm_irq), gpios);

#define IOLINK_HANDLER_THREAD_STACK_SIZE (8192)
#define IOLINK_HANDLER_THREAD_PRIO       5
static K_THREAD_STACK_DEFINE(iolm_handler_stack, IOLINK_HANDLER_THREAD_STACK_SIZE); 


#define IOLINK_APP_CHIP0_IRQ      25

#define IOLINK_APP_CHIP0_ADDRESS 0x0

#ifndef IOLINK_APP_CHIP1_ADDRESS
#define IOLINK_APP_CHIP1_ADDRESS 0x0
#endif


static iolink_pl_mode_t mode_ch[] =
{
   iolink_mode_SDCI,
   iolink_mode_INACTIVE,
#ifdef IOLINK_APP_CHIP1_SPI
   iolink_mode_SDCI,
   iolink_mode_INACTIVE,
#endif
};

void main_handler_thread (void * ctx)
{
   const iolink_m_cfg_t * cfg = (const iolink_m_cfg_t *)ctx;
   iolink_handler (*cfg);
}

static iolink_hw_drv_t * main_14819_init(const char* name, const iolink_14819_cfg_t * cfg,
                                          const struct device * gpio_port,int irq_pin)
{
   iolink_hw_drv_t  * drv;
   drv = iolink_14819_init (cfg);
   if (drv == NULL)
   {
      LOG_ERR ("APP: Failed to open SPI %s\n", name);
      return NULL;
   }
   if (_iolink_setup_int_zephyr (gpio_port, irq_pin, iolink_14819_isr, drv) < 0)
   {
      LOG_ERR ("APP: Failed to setup interrupt %s\n", name);
      return NULL;
   }
   return drv;
}

static iolink_m_cfg_t iolink_cfg = {
   .cb_arg                   = NULL,
   .cb_smi                   = NULL,
   .cb_pd                    = NULL,
   /* Not needed, statically set with Kconfig*/
   .master_thread_prio       = 0,
   .master_thread_stack_size = 0,
   .dl_thread_prio           = 0,
   .dl_thread_stack_size     = 0,
};

os_thread_t * iolink_handler_thread;
iolink_hw_drv_t * hw[2];
#define CURR_100MA 0x00
#define CURR_200MA 0x01
#define CURR_300MA 0x02
#define CURR_500MA 0x03
#define DRVRCURRLIM_CLDIS BIT(5)

#define CURR_OFF 0x06

#define CL_CONF(x)  ((x) << CURR_OFF)
static const iolink_14819_cfg_t iol_14819_0_cfg = {
      .chip_address   = IOLINK_APP_CHIP0_ADDRESS,
      .spi_slave_name = spi_dev,
      .CQCfgA         = MAX14819_CQCFG_DRVDIS | MAX14819_CQCFG_SINKSEL(0x2),
      .CQCfgB         = MAX14819_CQCFG_DRVDIS | MAX14819_CQCFG_SINKSEL(0x2),
      .LPCnfgA        = MAX14819_LPCNFG_LPEN,
      .LPCnfgB        = MAX14819_LPCNFG_LPEN,
      .IOStCfgA       = MAX14819_IOSTCFG_DICSINK | MAX14819_IOSTCFG_DIEC3TH,
      .IOStCfgB       = MAX14819_IOSTCFG_DICSINK | MAX14819_IOSTCFG_DIEC3TH,
      .DrvCurrLim     = CL_CONF(CURR_300MA),
      .Clock          = MAX14819_CLOCK_XTALEN | MAX14819_CLOCK_TXTXENDIS,
};

#ifdef IOLINK_APP_CHIP1_SPI

static const iolink_14819_cfg_t iol_14819_1_cfg = {
      .chip_address   = IOLINK_APP_CHIP1_ADDRESS,
      .spi_slave_name = IOLINK_APP_CHIP1_SPI,
      .CQCfgA         = MAX14819_CQCFG_DRVDIS | MAX14819_CQCFG_SINKSEL(0x2),
      .LPCnfgA        = MAX14819_LPCNFG_LPEN,
      .IOStCfgA       = MAX14819_IOSTCFG_DICSINK | MAX14819_IOSTCFG_DIEC3TH,
      .DrvCurrLim     = 0x00,
      .Clock          = MAX14819_CLOCK_XTALEN | MAX14819_CLOCK_TXTXENDIS,
};
#endif

iolink_port_cfg_t port_cfgs[] = {
      {
         .name = "/iolink0/0",
         .mode = &mode_ch[0],
         .arg  = (void*)0,
      },
      {
         .name = "/iolink0/1",
         .mode = &mode_ch[0],
         .arg  = (void*)1,
      },
#ifdef IOLINK_APP_CHIP1_SPI
      {
         .name = "/iolink1/0",
         .mode = &mode_ch[2],
         .drv  = hw[1],
         .arg  = (void*)0,
      },
      {
         .name = "/iolink1/1",
         .mode = &mode_ch[3],
         .drv  = hw[1],
         .arg  = (void*)1,
      },
#endif
   };

int main(void)
{


   LOG_INF("Starting app");
   hw[0] = main_14819_init("/iolink0", &iol_14819_0_cfg, irq_dev.port, irq_dev.pin);
   if (hw[0] == NULL) {
      return 0;
   }
#ifdef IOLINK_APP_CHIP1_SPI
   hw[1] = main_14819_init("/iolink1", &iol_14819_1_cfg, irq_dev.port, irq_dev.pin);
#endif
   port_cfgs[0].drv = hw[0];
   port_cfgs[1].drv = hw[0];


   iolink_cfg.port_cnt  = NELEMENTS (port_cfgs);
   iolink_cfg.port_cfgs = port_cfgs;
   LOG_INF("Total port cfg %d",iolink_cfg.port_cnt);

   k_sleep(K_MSEC(200));


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
