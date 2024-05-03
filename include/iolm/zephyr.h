/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once
#include <zephyr/drivers/gpio.h>
#include <osal_irq.h>

/**
 * A function that creates an interrupt handler thread, which will call the
 * isr_func when a falling edge is detected on the GPIO pin.
 *
 * @note In USB mode, the GPIO pin value does not matter, instead the interrupt
 * handler is setup to poll a pre defined pin on the FT2232H chip.
 * @note In USB mode, state of the GPIO pin is polled and isr_func is called
 * when GPIO pin is detected as low.
 * @param port      In: The GPIO port that is connected to the IRQ pin
 * @param gpio_pin  In: The GPIO pin that is connected to the IRQ pin
 * @param isr_func  In: The function that will be called when the interrupt gets
 * triggered
 * @param irq_arg   In: Pointer to the iolink_hw_drv_t structure containing
 * thread parameter
 */
int _iolink_setup_int_zephyr (const struct device * port, gpio_pin_t pin, isr_func_t isr_func, void * irq_arg);