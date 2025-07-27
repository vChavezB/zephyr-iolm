/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <iolink_max14819_pl.h>
#include <iolink_max14819.h>
#include <maxim14819.h>
#include <osal.h>
#include <osal_irq.h>
#include <stdlib.h>
#include <iolm/transciever.h>

LOG_MODULE_REGISTER(maxim14819, LOG_LEVEL_DBG);
#define DT_DRV_COMPAT iolm_maxim14819

#define DEFAULT_CHIP_ADDRESS 0x00
#define DEFAULT_INT_E 0x00
#define DEFAULT_LED_CTRL 0x00
#define DEFAULT_CQC_CFG (CQCFG_DRVDIS | CQCFG_SINKSEL(0x2))
#define DEFAULT_CQ_CTRL 0x00
#define DEFAULT_LPCNFG LPCNFG_LPEN
#define DEFAULT_IOST_CFG (IOSTCFG_DICSINK | IOSTCFG_DIEC3TH)
#define DEFAULT_DRV_CURR_LIMIT (CL_CONF(CURR_300MA))
#define DEFAULT_CLOCK (CLOCK_XTALEN | CLOCK_TXTXENDIS)

struct isr_data {
    struct gpio_callback cb;
    void * arg;
    isr_func_t max14819_isr;
};

struct maxim_14819_cfg {
    struct spi_dt_spec spi;
    iolink_14819_cfg_t drv;
    uint8_t id;
};

struct maxim_14819_data {
    struct isr_data isr;
    iolink_hw_drv_t * hw;
    struct gpio_dt_spec irq_dt;
};


void irq_cb(const struct device *dev, struct gpio_callback *gpio_cb, uint32_t pins){
    const struct isr_data * isr = CONTAINER_OF(gpio_cb, struct isr_data, cb);
    isr->max14819_isr(isr->arg);
}


int _iolink_setup_int (int gpio_pin, isr_func_t isr_func, void* irq_arg)
{
    struct maxim_14819_data *data = (struct maxim_14819_data *)(uintptr_t)(uint32_t)gpio_pin;
    int rc = gpio_pin_configure_dt(&data->irq_dt, GPIO_INPUT);
    if (rc != 0) {
        LOG_ERR("Failed to configure IRQ pin %d", data->irq_dt.pin);
        return rc;
    }
    data->isr.arg = irq_arg;
    data->isr.max14819_isr = isr_func;
    gpio_init_callback(&data->isr.cb, irq_cb, BIT(data->irq_dt.pin));
    rc = gpio_add_callback(data->irq_dt.port, &data->isr.cb);
    if (rc != 0) {
        LOG_ERR("Failed to add IRQ callback %d", data->irq_dt.pin);
        return rc;
    }
    rc =  gpio_pin_interrupt_configure(data->irq_dt.port, data->irq_dt.pin, GPIO_INT_EDGE_TO_ACTIVE);
    if (rc != 0) {
        LOG_ERR("Failed to configure IRQ interrupt %d", data->irq_dt.pin);
    }
    return 0;
}

static int maxim_init(const struct device *dev){
    const struct maxim_14819_cfg *cfg = dev->config;
    struct maxim_14819_data *data = dev->data;
    iolink_hw_drv_t * iol_drv = iolink_14819_init(&cfg->drv);
    int rc = 0;
    do {
        if (iol_drv == NULL) {
            LOG_ERR("Failed to initialize Maxim 14819 %d", cfg->id);
            rc = -ENODEV;
            break;
        }
        data->hw = iol_drv;
    }while(0);
    return rc;
}



#define MAXIM14819_DEFINE(inst)                                        \
    static struct maxim_14819_data maxim_14819_data_##inst ={          \
    .hw = NULL,                                                     \
    .isr = {.cb = {{0}}, .arg = NULL},                                \
    .irq_dt = GPIO_DT_SPEC_GET(DT_DRV_INST(inst), irq_gpios)        \
    };                                                                \
    static const struct maxim_14819_cfg maxim_14819_cfg_##inst = {    \
        .id = inst,                                                 \
        .spi = SPI_DT_SPEC_GET(DT_DRV_INST(inst),                     \
                SPI_WORD_SET(8) | SPI_TRANSFER_MSB, 0),             \
        .drv={                                                        \
            .chip_address = DT_INST_PROP(inst, chip_address),         \
            .chip_irq = (uint32_t)&maxim_14819_data_##inst,         \
            .spi_slave_name = (const char *)&maxim_14819_cfg_##inst,\
            .IntE = DT_INST_PROP_OR(inst, IntE,DEFAULT_INT_E),            \
            .CQCtrlA = DT_INST_PROP_OR(inst, CQCtrlA,DEFAULT_CQ_CTRL),  \
            .CQCtrlB = DT_INST_PROP_OR(inst, CQCtrlB,DEFAULT_CQ_CTRL),  \
            .LEDCtrl = DT_INST_PROP_OR(inst, LEDCtrl,DEFAULT_LED_CTRL), \
            .CQCfgA = DT_INST_PROP_OR(inst, CQCfgA,DEFAULT_CQC_CFG),    \
            .CQCfgB = DT_INST_PROP_OR(inst, CQCfgB,DEFAULT_CQC_CFG),    \
            .LPCnfgA = DT_INST_PROP_OR(inst, LPCnfgA,DEFAULT_LPCNFG),   \
            .LPCnfgB = DT_INST_PROP_OR(inst, LPCnfgB,DEFAULT_LPCNFG),   \
            .IOStCfgA = DT_INST_PROP_OR(inst, IOStCfgA,DEFAULT_IOST_CFG),    \
            .IOStCfgB = DT_INST_PROP_OR(inst, IOStCfgB,DEFAULT_IOST_CFG),   \
            .DrvCurrLim = DT_INST_PROP_OR(inst,                         \
                        DrvCurrLim,DEFAULT_DRV_CURR_LIMIT),                \
            .Clock = DT_INST_PROP_OR(inst, Clock,DEFAULT_CLOCK)         \
            }                                                            \
    };                                                                  \
    DEVICE_DT_INST_DEFINE(inst,                                         \
                          maxim_init,                                   \
                          NULL,                                         \
                          &maxim_14819_data_##inst,                     \
                          &maxim_14819_cfg_##inst,                      \
                          POST_KERNEL,                                  \
                          CONFIG_TRANSCIEVER_INIT_PRIORITY,             \
                          NULL);

DT_INST_FOREACH_STATUS_OKAY(MAXIM14819_DEFINE)


static const struct device *maxim14819_devices[] = {
    DT_INST_FOREACH_STATUS_OKAY(DEVICE_DT_INST_GET)
};

iolink_hw_drv_t * get_drv(uint8_t port){
    if (port >= CONFIG_IOLINK_NUM_PORTS ) {
        LOG_ERR ("port >= CONFIG_IOLINK_NUM_PORTS");
        return NULL;
    }
    const uint8_t drv_idx = port/ TRANSCIEVER_MAX_PORTS;
    if (drv_idx > DT_NUM_INST_STATUS_OKAY(DT_DRV_COMPAT)) {
        LOG_ERR ("Driver instance %d > static instances", drv_idx);
        return NULL;
    }
    if (!device_is_ready(maxim14819_devices[port])) {
        LOG_ERR ("Driver %d status != ok", drv_idx);
        return NULL;
    }
    struct maxim_14819_data *data = maxim14819_devices[drv_idx]->data;
    return data->hw;
}

/*-----------------------------------------
 RT Labs Required PL implementation methods
------------------------------------------ */
void _iolink_pl_hw_spi_transfer (
   void * device,
   void * data_read,
   const void * data_written,
   size_t n_bytes_to_transfer)
{
    const struct spi_buf tx_buf = {.buf=(uint8_t*)(data_written), .len=n_bytes_to_transfer};
    const struct spi_buf rx_buf = {.buf=data_read, .len=n_bytes_to_transfer};
    struct spi_buf_set tx_set = {.buffers=&tx_buf, .count=1};
    struct spi_buf_set rx_set = {.buffers=&rx_buf, .count=1};
    const int spi_res = spi_transceive_dt(device,
                                        &tx_set,
                                        &rx_set);
}

void * _iolink_pl_hw_spi_init (const char * device)
{
    struct maxim_14819_cfg* cfg = (struct maxim_14819_cfg*) device;
    if (!device_is_ready(cfg->spi.bus)) {
        return NULL;
    }
    return &cfg->spi;
}

void _iolink_pl_hw_spi_close (void * fd)
{
    // Nothing to do
}
