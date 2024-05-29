/* Copyright (c) 2024 Victor Chavez
   SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <iolm/iolm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(read_pd,CONFIG_LOG_DEFAULT_LEVEL);

static const char * event_types_str [] = 
{
   "Reserved",
   "notification",
   "warning",
   "error"
};


static const char * event_modes_str [] = 
{
   "Reserved",
   "single shot",
   "dissappears",
   "appears"
};


struct iolm_port_cfg port_cfgs[CONFIG_IOLINK_NUM_PORTS] = 
{
   {
      .vid = 0,
      .did = 0,
      .cycle_time_us = CONFIG_CYCLE_TIME_US,
      .portmode = IOLINK_PORTMODE_IOL_AUTO,
      .validation = IOLINK_VALIDATION_CHECK_NO
   },
   {
      .vid = 0,
      .did = 0,
      .cycle_time_us = CONFIG_CYCLE_TIME_US,
      .portmode = IOLINK_PORTMODE_IOL_AUTO,
      .validation = IOLINK_VALIDATION_CHECK_NO
   }
};

uint32_t last_print[CONFIG_IOLINK_NUM_PORTS] = {0};
const uint32_t PDIN_PRINT_TIME_MS = 1000;

void iolm_port_event_cb(uint8_t port, enum iolm_port_evt evt, void *data, void *arg)
{
   const uint8_t port_idx = port - 1;
   switch (evt) {
      case IOLM_PORT_PD:
      {
         const uint32_t current_time = k_uptime_get_32();
         if ( current_time - last_print[port_idx] < CONFIG_PDIN_RATE_MS) {
            return;
         }
         uint8_t pdin_data[64];
         struct iolm_pd_data *pd_data = (struct iolm_pd_data *)data;
         for (size_t i = 0; i < pd_data->len; i++) {
            sprintf(pdin_data + (i * 2), "%02x", pd_data->data[i]);
         }
         LOG_INF("Port [%d] PDIn %s", port, pdin_data);
         last_print[port_idx] =  current_time;
         break;
      }
      case IOLM_PORT_STATUS:
      {
         const arg_block_portstatuslist_t * status_list = (arg_block_portstatuslist_t *)data;
         if (status_list->port_status_info ==  IOLINK_PORT_STATUS_INFO_OP) {
            LOG_INF("Port [%d] Operate: VID %x DID %x", port,status_list->vendorid,status_list->deviceid);
         }
         if (status_list->port_status_info ==  IOLINK_PORT_STATUS_INFO_NO_DEV) {
            LOG_WRN("Port [%d] COM Lost", port);
         }
         break;
      }
      case IOLM_PORT_DEV_EVT:
      {
         struct iolm_dev_evt *dev_evt = (struct iolm_dev_evt *)data;
         for(int i = 0; i < dev_evt->len; i++) {
            LOG_INF("Port [%d] Event type: %s mode: %s code %x", port,
                                                               event_types_str[dev_evt->entry[i].type],
                                                               event_modes_str[dev_evt->entry[i].mode],
                                                               dev_evt->entry[i].event_code);
         }
         break;
      }
      default:
         break;
   }
}

int main(void)
{
   LOG_INF("Zephyr IO-Link Master PDIn Sample Application");
   iolm_set_port_evt_cb(iolm_port_event_cb, NULL);
   int rc = iolm_init(port_cfgs);
   if (rc != 0) {
      LOG_ERR("Failed to initialize IO-Link Master: %d",rc);
   }
   return rc;
}