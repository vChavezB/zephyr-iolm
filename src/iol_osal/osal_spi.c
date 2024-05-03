/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <zephyr/drivers/spi.h>
#include <zephyr/device.h>
#include <osal_spi.h>

// TODO check SPI cfg required for MAX14819
static struct spi_config spi_cfg = {
    .frequency = 0U,
    .operation = {{SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER}},
    .slave = 0U,
    .cs = {NULL}
};

void * _iolink_pl_hw_spi_init (const char * device)
{
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
	const int spi_res = spi_transceive(spi_dev,
										&spi_cfg,
										&tx_set,
										&rx_set);
    // TODO log error, check how to deal with LOG_DBG

}