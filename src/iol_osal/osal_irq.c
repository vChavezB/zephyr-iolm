/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <osal.h>
#include <osal_irq.h>
#include <iolm/zephyr.h>
#include <stdlib.h>

struct gpio_cb_data {
    struct gpio_callback zephyr_data;
    isr_func_t isr_func;
    void * arg;
};

void gpio_cb(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
    const struct gpio_cb_data * cb_data = CONTAINER_OF(cb, struct gpio_cb_data, zephyr_data);
    if (cb_data->isr_func) {
        cb_data->isr_func(cb_data->arg);
    }  
}

int _iolink_setup_int_zephyr (const struct device * port, gpio_pin_t pin, isr_func_t isr_func, void* irq_arg)
{
    if (!device_is_ready(port)) {
        return -ENODEV;
    }
    int rc = gpio_pin_configure(port, pin, GPIO_ACTIVE_LOW | GPIO_INPUT | GPIO_PULL_UP );
    struct gpio_cb_data * cb_data = (struct gpio_cb_data *)malloc(sizeof(struct gpio_cb_data));
    cb_data->arg = irq_arg;
    cb_data->isr_func = isr_func;
    gpio_init_callback(&cb_data->zephyr_data, gpio_cb, BIT(pin));
    gpio_add_callback(port, &cb_data->zephyr_data);
    rc =  gpio_pin_interrupt_configure(port,pin,GPIO_INT_EDGE_TO_ACTIVE);
    return rc == 0;
}