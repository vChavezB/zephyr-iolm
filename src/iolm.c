
/* Copyright (c) 2024 Victor Chavez
   SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <iolm/transciever.h>
#include <osal.h>
#include <iolink_main.h>
#include <iolink.h>
#include <iolink_dl.h>
#include <zephyr/sys/__assert.h>


LOG_MODULE_DECLARE(iol_master, CONFIG_IOLINK_LOG_LEVEL);
/*
#include <stdio.h>

#include "osal_irq.h"

#include "iolink.h"
#include "iolink_max14819.h"
#include "iolink_handler.h"

*/

#define IOLINK_HANDLER_THREAD_STACK_SIZE (1024)
#define IOLINK_HANDLER_THREAD_PRIO       CONFIG_IOLINK_MASTER_PRIO+1
static K_THREAD_STACK_DEFINE(iolm_handler_stack, IOLINK_HANDLER_THREAD_STACK_SIZE); 

static iolink_port_cfg_t port_cfgs[CONFIG_IOLINK_NUM_PORTS];
static iolink_pl_mode_t init_mode[] =
{
   iolink_mode_SDCI,
   iolink_mode_INACTIVE,
};

static iolink_m_cfg_t iolink_cfg = {
   .cb_arg                   = NULL,
   .cb_smi                   = NULL,
   .cb_pd                    = NULL,
};

static const char * port_status_msg [] = 
{
   "No Device",
   "Deactivated",
   "Port Diagnostics",
   "Preoperate",
   "Operate",
   "DI",
   "DO",
   "Power Off",
   "Not Available"
};

static iolink_m_t * master;
// Retry comm timer, see figure 33 spec v1.1.3
static struct k_timer tsd_timr[IOLINK_NUM_PORTS];

static const uint32_t SMI_PORT_CFG_TIMEOUT = 1000; 


static arg_block_portstatuslist_t port_status[CONFIG_IOLINK_NUM_PORTS];

enum evt_type {
   COMLost,
   RetryCOM,
   PortStatusChange,
   PortStatusList,
   PortCnf
};

typedef struct iolm_msgq_evt {
   enum evt_type type;
   uint8_t port_no;
}iolm_msgq_evt_t;

#define MAX_MSGS 50
#define MSG_TIMEOUT_MS 100
K_MSGQ_DEFINE(iol_msgq, sizeof(iolm_msgq_evt_t), MAX_MSGS, 1);

static void handle_smi_portevent (uint8_t port_no,diag_entry_t * event)
{
   iolm_msgq_evt_t msg_evt;
   int rc;
   
   msg_evt.port_no = port_no ;
   //uint8_t portnumber                   = app_port->portnumber;
   //iolink_app_master_ctx_t * app_master = app_port->app_master;
   //uint8_t port_index                   = portnumber - 1;


   /*
   if (
      ((app_port->type == GOLDEN) || (app_port->type == UNKNOWN)) &&
      event->event_code != IOLINK_EVENTCODE_NO_DEV)
   */
   {
      uint8_t event_index;
      
      //os_mutex_lock (app_port->event_mtx);
     // event_index = app_port->events.count++;
      //CC_ASSERT (event_index < PORT_EVENT_COUNT);
      //app_port->events.diag_entry[event_index].event_qualifier =
      //   event->event_qualifier;
      //app_port->events.diag_entry[event_index].event_code = event->event_code;
      //os_mutex_unlock (app_port->event_mtx);
   }

   /*
   if (event->event_code != IOLINK_EVENTCODE_NO_DEV)
   {
      LOG_INFO (
         LOG_STATE_ON,
         "%s (%d): type = %d, event_code = 0x%04X, count = %d\n",
         __func__,
         portnumber,
         app_port->type,
         event->event_code,
         app_port->events.count);
   }
   */

   switch (event->event_code)
   {
   case IOLINK_EVENTCODE_PORT_STATUS_CHANGE:
      LOG_INF("Port [%u] status changed", port_no);
      msg_evt.type = PortStatusChange;
      rc = k_msgq_put(&iol_msgq, &msg_evt, K_MSEC(MSG_TIMEOUT_MS));
      if (rc != 0) {
         LOG_ERR("Failed to queue evt %d", rc);
      }
      break;
   case IOLINK_EVENTCODE_BAD_DID:
   case IOLINK_EVENTCODE_BAD_VID:
      //LOG_WARNING (
      //   LOG_STATE_ON,
      //   "%s: Port %u: portevent bad vid/did.\n",
      //   __func__,
      //   portnumber);
      /* Port status changed - Use SMI_PortStatus() for details */
      //os_event_set (app_master->app_event, (EVENT_PORTE_0 << port_index));
      break;
   case IOLINK_EVENTCODE_NO_DEV: /* COMLOST */
      LOG_WRN("Port[%u]: COM Lost", port_no);
      msg_evt.type = COMLost;
      rc = k_msgq_put(&iol_msgq, &msg_evt, K_MSEC(MSG_TIMEOUT_MS));
      if (rc != 0) {
         LOG_ERR("Failed to queue evt %d", rc);
      }
      break;
   case IOLINK_EVENTCODE_BACKUP_INCON:
   case IOLINK_EVENTCODE_BACKUP_INCON_SIZE:
   case IOLINK_EVENTCODE_BACKUP_INCON_ID:
   case IOLINK_EVENTCODE_BACKUP_INCON_UL:
   case IOLINK_EVENTCODE_BACKUP_INCON_DL:
      // TODO add evt
      break;
   default:
      LOG_WRN ("Port[%u]: Unknown SMI_PortEvent code 0x%04x\n", port_no, event->event_code);
      break;
   }
}


static void SMI_cnf_cb (void * arg,
    uint8_t portnumber,
    iolink_arg_block_id_t ref_arg_block_id,
    uint16_t arg_block_len,
    arg_block_t * arg_block)
{
/*
   iolink_app_master_ctx_t * app_m  = arg;
   iolink_app_port_ctx_t * app_port = &app_m->app_port[portnumber - 1];

   

   CC_ASSERT (arg_block != NULL);
*/

   iolm_msgq_evt_t msg_evt;
   msg_evt.port_no = portnumber;
   int rc;
   __ASSERT_NO_MSG(arg_block != NULL);
   bool match_found = true;
   switch (arg_block->id) {
   case IOLINK_ARG_BLOCK_ID_JOB_ERROR:
      //handle_smi_joberror (app_port, ref_arg_block_id, (arg_block_joberror_t *)arg_block);
      //LOG_DBG ("%s: IOLINK_ARG_BLOCK_ID_JOB_ERROR\n", __func__);
      break;
   case IOLINK_ARG_BLOCK_ID_PORT_STATUS_LIST:
   {
      arg_block_portstatuslist_t * port_status_list =
         (arg_block_portstatuslist_t *)arg_block;
      uint8_t port_idx = portnumber - 1;
      port_status[port_idx].port_status_info  = port_status_list->port_status_info;
      port_status[port_idx].port_quality_info = port_status_list->port_quality_info;
      port_status[port_idx].revision_id       = port_status_list->revision_id;
      port_status[port_idx].transmission_rate = port_status_list->transmission_rate;
      port_status[port_idx].master_cycle_time = port_status_list->master_cycle_time;
      port_status[port_idx].vendorid          = port_status_list->vendorid;
      port_status[port_idx].deviceid          = port_status_list->deviceid;

      msg_evt.type = PortStatusList;
      rc = k_msgq_put(&iol_msgq, &msg_evt, K_MSEC(MSG_TIMEOUT_MS));
      if (rc != 0) {
         LOG_ERR("Failed to queue evt %d", rc);
      }
      break;
   }
   case IOLINK_ARG_BLOCK_ID_PORT_EVENT:;
     /* SMI_PortEvent_ind */
      arg_block_portevent_t * arg_block_portevent = (arg_block_portevent_t *)arg_block;
      handle_smi_portevent (portnumber, &arg_block_portevent->event);
      break;
   case IOLINK_ARG_BLOCK_ID_DEV_EVENT:
      // SMI_DeviceEvent_ind 
      //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_DEV_EVENT\n", __func__);
      //handle_smi_deviceevent (app_port, (arg_block_devevent_t *)arg_block);
      break;
   case IOLINK_ARG_BLOCK_ID_OD_RD:
      // SMI_DeviceRead_cnf 
      //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_OD_RD\n", __func__);
      //app_port->param_read.data_len = arg_block_len - sizeof (arg_block_od_t);
      //os_event_set (app_port->event, SMI_READ_CNF);
      break;
   case IOLINK_ARG_BLOCK_ID_PD_IN:
     // SMI_PDIn_cnf 
      //arg_block_pdin_t * arg_block_pdin = (arg_block_pdin_t *)arg_block;
      //memcpy (app_port->pdin.data, arg_block_pdin->data, arg_block_pdin->h.len);
      //app_port->pdin.data_len = arg_block_pdin->h.len;
      //app_port->pdin.pqi      = arg_block_pdin->h.port_qualifier_info;
      //os_event_set (iolink_app_master.app_event, (SMI_PDIN_CNF << portnumber-1));
      break;
   case IOLINK_ARG_BLOCK_ID_PD_IN_OUT:
      // SMI_PDInOut_cnf
      //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_PD_IN_OUT\n", __func__);
      break;
   case IOLINK_ARG_BLOCK_ID_MASTERIDENT:;
      // SMI_MasterIdentification_cnf()
      //arg_block_masterident_t * arg_block_masterident = (arg_block_masterident_t *)arg_block;
      //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_MASTERIDENT\n", __func__);
      //app_m->vendorid = arg_block_masterident->h.vendorid;
      //app_m->masterid = arg_block_masterident->h.masterid;
      //os_event_set (app_port->event, SMI_MASTERIDENT_CNF);
      break;
   default:
      match_found = false;
      break;
   }

   if (!match_found)
   {
      switch (ref_arg_block_id) {
      case IOLINK_ARG_BLOCK_ID_PD_OUT:
         // SMI_DeviceWrite_cnf 
         //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_PD_OUT\n", __func__);
         break;
      case IOLINK_ARG_BLOCK_ID_DS_DATA:
         // SMI_ParServToDS_cnf
         //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_DS_DATA\n", __func__);
         //os_event_set (app_port->event, SMI_PARSERV_TO_DS_CNF);
         break;
      case IOLINK_ARG_BLOCK_ID_OD_WR:
         // SMI_DeviceWrite_cnf 
         //LOG_DEBUG (LOG_STATE_ON, "%s: IOLINK_ARG_BLOCK_ID_OD_WR\n", __func__);
         //os_event_set (app_port->event, SMI_WRITE_CNF);
         break;
      case IOLINK_ARG_BLOCK_ID_PORT_CFG_LIST:
         // SMI_PortConfiguration_cnf 
         //LOG_DEBUG (
         //   LOG_STATE_ON,
         //   "%s: IOLINK_ARG_BLOCK_ID_PORT_CFG_LIST\n",
         //   __func__);
         msg_evt.type = PortCnf;
         rc = k_msgq_put(&iol_msgq, &msg_evt, K_MSEC(MSG_TIMEOUT_MS));
         if (rc != 0) {
            LOG_ERR("Failed to queue evt %d", rc);
         }
         break;
      default:
         /*LOG_WARNING (
            LOG_STATE_ON,
            "%s: Port %u: Unexpected ref_arg_block_id 0x%04x and arg_block_id "
            "0x%04x\n",
            __func__,
            portnumber,
            ref_arg_block_id,
            arg_block->id);
         */
         break;
      }
   }
}


static void PD_cb (
   uint8_t portnumber,
   void * arg,
   uint8_t data_len,
   const uint8_t * inputdata)
{
    /*
   uint8_t port_index                   = portnumber - 1;
   iolink_app_master_ctx_t * app_master = (iolink_app_master_ctx_t *)arg;
   iolink_app_port_ctx_t * app_port = &iolink_app_master.app_port[port_index];

   os_event_set (app_master->app_event, EVENT_PD_0 << port_index);
   memcpy (app_port->pdin.data, inputdata, data_len);
   app_port->pdin.data_len = data_len;
   */
}


static void retry_com_cb (struct k_timer * timer)
{
   iolm_msgq_evt_t msg_evt;
   msg_evt.type = RetryCOM;
   msg_evt.port_no = (uint8_t)timer->user_data;
   k_msgq_put(&iol_msgq, &msg_evt, K_NO_WAIT);
}


struct k_thread hdl_thread; 


static void iolm_hdl_thread(void * p1, void * p2, void * p3);


static void iolink_common_config (
   arg_block_portconfiglist_t * port_cfg,
   uint16_t vid,
   uint32_t did,
   uint8_t cycle_time,
   iolink_portmode_t portmode,
   iolink_validation_check_t validation)
{
   port_cfg->arg_block.id                 = IOLINK_ARG_BLOCK_ID_PORT_CFG_LIST;
   port_cfg->configlist.port_cycle_time   = cycle_time;
   port_cfg->configlist.vendorid          = vid;
   port_cfg->configlist.deviceid          = did;
   port_cfg->configlist.portmode          = portmode;
   port_cfg->configlist.validation_backup = validation;
   port_cfg->configlist.iq_behavior       = IOLINK_IQ_BEHAVIOR_NO_SUPPORT;
}

/**
 * @brief Blocking wait for smi cnf
 * 
 * @param evt_mask 
 * @param ms 
 * @return iolink_smi_errortypes_t 
 */
iolink_smi_errortypes_t wait_smi_cnf (enum evt_type wait_evt, uint32_t ms)
{
   iolm_msgq_evt_t msg_evt;
   iolink_smi_errortypes_t errortype = IOLINK_SMI_ERRORTYPE_NONE;

   /* Wait for SMI_xxx_cnf() */
   if(k_msgq_get(&iol_msgq, &msg_evt, K_MSEC(ms)) < 0) {
      return IOLINK_SMI_ERRORTYPE_APP_DEV;
   }
   if (msg_evt.type != wait_evt) {
      return IOLINK_SMI_ERRORTYPE_APP_DEV;
   }

   /*
   if ((evt_mask == SMI_WRITE_CNF) || (evt_mask == SMI_PARSERV_TO_DS_CNF))
   {
      errortype = app_port->errortype;

      if (errortype != IOLINK_SMI_ERRORTYPE_NONE)
      {
         app_port->errortype = IOLINK_SMI_ERRORTYPE_NONE;
      }
   }
   */
   return errortype;
}


static uint8_t iolink_config_port_sdci_auto(uint8_t port_no)
{
   arg_block_portconfiglist_t port_cfg;

   iolink_common_config (
      &port_cfg,
      0,
      0,
      0 /* AFAP (As fast as possible) */, // TODO ADD API to set Cycle time and encode it
      IOLINK_PORTMODE_IOL_AUTO,
      IOLINK_VALIDATION_CHECK_NO);

   iolink_error_t err = SMI_PortConfiguration_req (
      port_no+1,
      IOLINK_ARG_BLOCK_ID_VOID_BLOCK,
      sizeof (arg_block_portconfiglist_t),
      (arg_block_t *)&port_cfg);


   if ( (err != IOLINK_ERROR_NONE) ||
         (wait_smi_cnf ( PortCnf, SMI_PORT_CFG_TIMEOUT) != IOLINK_SMI_ERRORTYPE_NONE))
   {
      return 1;
   }

   return 0;
}

static uint8_t iolink_config_port_dido (uint8_t portno, bool di)
{
   arg_block_portconfiglist_t port_cfg;

   memset (&port_cfg, 0, sizeof (arg_block_portconfiglist_t));

   iolink_common_config (
      &port_cfg,
      0,
      0,
      0 /* AFAP (As fast as possible) */,
      (di) ? IOLINK_PORTMODE_DI_CQ : IOLINK_PORTMODE_DO_CQ,
      (di) ? IOLINK_IQ_BEHAVIOR_DI : IOLINK_IQ_BEHAVIOR_DO);

   iolink_error_t err = SMI_PortConfiguration_req (
      portno+1,
      IOLINK_ARG_BLOCK_ID_VOID_BLOCK,
      sizeof (arg_block_portconfiglist_t),
      (arg_block_t *)&port_cfg);
      
   if ( (err != IOLINK_ERROR_NONE) ||
         (wait_smi_cnf (PortCnf, SMI_PORT_CFG_TIMEOUT) != IOLINK_SMI_ERRORTYPE_NONE))
   {
      return 1;
   }

   return 0;
}

static uint8_t iolink_config_port (uint8_t port_no,
   iolink_pl_mode_t port_mode)
{
   uint8_t res = 0;
   switch (port_mode)
   {
   case iolink_mode_SDCI:
      res = iolink_config_port_sdci_auto (port_no);
      break;
   case iolink_mode_DO:
      res = iolink_config_port_dido (port_no, false);
      break;
   case iolink_mode_DI:
      res = iolink_config_port_dido (port_no, true);
      break;
   case iolink_mode_INACTIVE:
      break;
   }
   return res;
}

int iolm_init(){
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

   iolink_m_cfg_t m_cfg = {
      .port_cnt = CONFIG_IOLINK_NUM_PORTS,
      .port_cfgs = port_cfgs,
      .cb_arg = NULL,
      .cb_smi = SMI_cnf_cb,
      .cb_pd = PD_cb
   };

   master = iolink_m_init (&m_cfg);
   if (master == NULL) {
      LOG_ERR("iolink_m_init() failed!");
      return -ENODEV;
   }

   for (uint8_t port_idx = 0; port_idx < CONFIG_IOLINK_NUM_PORTS; port_idx++) {
      k_timer_init(&tsd_timr[port_idx], retry_com_cb, NULL);
      tsd_timr[port_idx].user_data = (void*) (port_idx +1);
   }

   for (uint8_t port_no = 0; port_no < CONFIG_IOLINK_NUM_PORTS; port_no++) {
      iolink_port_t * port = iolink_get_port (master, port_no);
      // Note priority and stack are statically defined in Kconfig
      // see CONFIG_IOLINK_DL_STACK_SIZE, 
      iolink_dl_instantiate (port,0,0);

      // TODO Add API for initial config of port
      uint8_t port_res = iolink_config_port (port_no, iolink_mode_SDCI);
      if (port_res != 0) {
         LOG_ERR("Port %d config failed", port_no);
         return -EIO;
      }
   }

   k_thread_create(&hdl_thread,
                     iolm_handler_stack,
                     K_THREAD_STACK_SIZEOF(iolm_handler_stack),
                     iolm_hdl_thread,
                     NULL,NULL,NULL,
                     CONFIG_IOLINK_MASTER_PRIO+1,
                     0,        // thread options
                     K_NO_WAIT); // start immediately
   return 0;
}

void iolm_hdl_thread(void * p1, void * p2, void * p3) {
   while(true) {
      iolm_msgq_evt_t evt;

      k_msgq_get(&iol_msgq, &evt, K_FOREVER);
      uint8_t port_idx = evt.port_no-1;

      switch(evt.type) {
         case COMLost:
            k_timer_stop(&tsd_timr[port_idx]);
            // Wait 500ms before sending new WURQ
            k_timer_start(&tsd_timr[port_idx], K_MSEC(500), K_NO_WAIT);
            LOG_WRN("Port[%u]: COM Lost", evt.port_no);
            break;
         case RetryCOM:
         {
            iolink_port_t * port = iolink_get_port(master, evt.port_no);
            iolink_dl_reset (port);
            const uint8_t portcfg_ok = iolink_config_port (port_idx, iolink_mode_SDCI);
            if (portcfg_ok != 1) {
               LOG_ERR("RetryCOM Port[%d] failed", evt.port_no);
            }
            break;
         }
         case PortStatusChange:
         {
            arg_block_void_t arg_block_void;

            memset (&arg_block_void, 0, sizeof (arg_block_void_t));
            arg_block_void.arg_block.id = IOLINK_ARG_BLOCK_ID_VOID_BLOCK;

            iolink_error_t err = SMI_PortStatus_req (
               evt.port_no,
               IOLINK_ARG_BLOCK_ID_PORT_STATUS_LIST,
               sizeof (arg_block_void_t),
               (arg_block_t *)&arg_block_void);   
            if (err != IOLINK_ERROR_NONE) {
               LOG_ERR("SMI_PortStatus_req failed: %d", err);
            }
            break;
         }
         case PortCnf:
            // notify user of port configuration success
            break;
         case PortStatusList:
            // handle port status list
            // Inform user of change in port status
            const iolink_port_status_info_t status = port_status[port_idx].port_status_info;
            uint8_t msg_idx = status;
            if (msg_idx >= IOLINK_PORT_STATUS_INFO_POWER_OFF) {
               // Last two status are at the end
               msg_idx -= IOLINK_PORT_STATUS_INFO_POWER_OFF;
               msg_idx += (IOLINK_PORT_STATUS_INFO_DO + 1);
            }
            LOG_INF("Port [%u] status changed to %s",
                     evt.port_no,  port_status_msg[status]);
            break;
      }
   }
}