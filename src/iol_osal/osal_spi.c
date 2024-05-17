/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <zephyr/drivers/spi.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <osal_spi.h>

static const struct gpio_dt_spec cs_pin = GPIO_DT_SPEC_GET(DT_ALIAS(iolm_cs), gpios);

static const struct spi_cs_control iolm_cs_pin={
    cs_pin,0
}; 

// TODO check SPI cfg required for MAX14819
static struct spi_config spi_cfg = {
    .frequency = 20000000U,
    .operation = {{SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER}},
    .slave = 0U,
    .cs = NULL
};

void * _iolink_pl_hw_spi_init (const char * device)
{
    gpio_pin_configure_dt(&cs_pin,GPIO_OUTPUT);
    gpio_pin_set_dt(&cs_pin,0);
    const struct device * spi_dev = (struct device *) device;
    if(device_is_ready(spi_dev)!=true) {
        return NULL;
    }
    return (void *)spi_dev;
}

void _iolink_pl_hw_spi_close (void * fd)
{
    // Nothing to do, driver is initialized by zephyr in kernel
}

void _iolink_pl_hw_spi_transfer (
   void * fd,
   void * data_read,
   const void * data_written,
   size_t n_bytes_to_transfer)
{
    const struct device * spi_dev = (struct device *) fd;
	const struct spi_buf tx_buf = {.buf=(uint8_t*)(data_written), .len=n_bytes_to_transfer};
	const struct spi_buf rx_buf = {.buf=data_read, .len=n_bytes_to_transfer};
	struct spi_buf_set tx_set = {.buffers=&tx_buf, .count=1};
	struct spi_buf_set rx_set = {.buffers=&rx_buf, .count=1};
    gpio_pin_set_dt(&cs_pin,1);
	const int spi_res = spi_transceive(spi_dev,
										&spi_cfg,
										&tx_set,
										&rx_set);
    gpio_pin_set_dt(&cs_pin,0);
    // TODO log error, check how to deal with LOG_DBG
}