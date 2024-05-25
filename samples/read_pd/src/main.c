/* Copyright (c) 2024 Victor Chavez
   SPDX-License-Identifier: GPL-3.0-or-later
*/
#include <iolm/iolm.h>
#include <zephyr/kernel.h>

int main(void)
{
   int rc = iolm_init();
   if (rc != 0) {
      printk("Failed to initialize IO-Link Master: %d\n",rc);
   }
   return rc;
}