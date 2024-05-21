       
user_iram_end = (0x20000 - ((0x3ffe8000) - 0x3ffe0000) + 0x400a0000);
user_iram_seg_org = (0x40070000 + 0x10000);
user_iram_seg_len = user_iram_end - user_iram_seg_org;
user_dram_seg_org = 0x3ffb0000;
user_dram_seg_len = (0x3ffe0000 - 0x3ffb0000);
user_dram_2_seg_org = 0x3ffe8000;
user_dram_2_seg_len = (0x40000000 - 0x3ffe8000);
MEMORY
{
  FLASH (R): org = 0x0, len = 4194304 - 0x100
  iram0_0_seg(RX): org = user_iram_seg_org, len = user_iram_seg_len
  dram0_0_seg(RW): org = user_dram_seg_org + 0x0,
                   len = user_dram_seg_len - 0x0
  dram0_shm0_seg(RW): org = 0x3ffe5230, len = 2K
  dram0_sem0_seg(RW): org = 0x3ffe5a30, len = 8
  dram0_1_seg(RW): org = 0x3ffe5a38, len = 0K
  irom0_0_seg(RX): org = 0x400d0000, len = (4194304 - 0x1000)
  drom0_0_seg(R): org = 0x3f400000, len = (4194304 - 0x1000)
  rtc_iram_seg(RWX): org = 0x400c0000, len = 0x2000
  rtc_slow_seg(RW): org = 0x50000000, len = 0x1000
  IDT_LIST(RW): org = 0x3ebfe010, len = 0x2000
}
ENTRY("__start")
_rom_store_table = 0;
PROVIDE(_memmap_vecbase_reset = 0x40000450);
PROVIDE(_memmap_reset_vector = 0x40000400);
_heap_sentry = 0x3ffe4350;
SECTIONS
{
 .rel.plt :
 {
 *(.rel.plt)
 PROVIDE_HIDDEN (__rel_iplt_start = .);
 *(.rel.iplt)
 PROVIDE_HIDDEN (__rel_iplt_end = .);
 }
 .rela.plt :
 {
 *(.rela.plt)
 PROVIDE_HIDDEN (__rela_iplt_start = .);
 *(.rela.iplt)
 PROVIDE_HIDDEN (__rela_iplt_end = .);
 }
 .rel.dyn :
 {
 *(.rel.*)
 }
 .rela.dyn :
 {
 *(.rela.*)
 }
  .rtc.text :
  {
    . = ALIGN(4);
    *(.rtc.literal .rtc.text)
    *rtc_wake_stub*.o(.literal .text .literal.* .text.*)
  } > rtc_iram_seg AT > FLASH
  .rtc.data :
  {
    _rtc_data_start = ABSOLUTE(.);
    *(.rtc.data)
    *(.rtc.rodata)
    *rtc_wake_stub*.o(.data .rodata .data.* .rodata.* .bss .bss.*)
    _rtc_data_end = ABSOLUTE(.);
  } > rtc_slow_seg AT > FLASH
  .rtc.bss (NOLOAD) :
  {
    _rtc_bss_start = ABSOLUTE(.);
    *rtc_wake_stub*.o(.bss .bss.*)
    *rtc_wake_stub*.o(COMMON)
    _rtc_bss_end = ABSOLUTE(.);
  } > rtc_slow_seg
  .rtc.force_slow :
  {
    . = ALIGN(4);
    _rtc_force_slow_start = ABSOLUTE(.);
    *(.rtc.force_slow .rtc.force_slow.*)
    . = ALIGN(4) ;
    _rtc_force_slow_end = ABSOLUTE(.);
  } > rtc_slow_seg
  _rtc_slow_length = (_rtc_force_slow_end - _rtc_data_start);
  .iram0.vectors : ALIGN(4)
  {
    _iram_start = ABSOLUTE(.);
    _init_start = ABSOLUTE(.);
    . = 0x0;
    KEEP(*(.WindowVectors.text));
    . = 0x180;
    KEEP(*(.Level2InterruptVector.text));
    . = 0x1c0;
    KEEP(*(.Level3InterruptVector.text));
    . = 0x200;
    KEEP(*(.Level4InterruptVector.text));
    . = 0x240;
    KEEP(*(.Level5InterruptVector.text));
    . = 0x280;
    KEEP(*(.DebugExceptionVector.text));
    . = 0x2c0;
    KEEP(*(.NMIExceptionVector.text));
    . = 0x300;
    KEEP(*(.KernelExceptionVector.text));
    . = 0x340;
    KEEP(*(.UserExceptionVector.text));
    . = 0x3C0;
    KEEP(*(.DoubleExceptionVector.text));
    . = 0x400;
    *(.*Vector.literal)
    *(.UserEnter.literal);
    *(.UserEnter.text);
    . = ALIGN (16);
    *(.entry.text)
    *(.init.literal)
    *(.init)
    _init_end = ABSOLUTE(.);
  } > iram0_0_seg AT > FLASH
  .iram0.text : ALIGN(4)
  {
    _iram_text_start = ABSOLUTE(.);
    *(.iram1 .iram1.*)
    *(.iram0.literal .iram.literal .iram.text.literal .iram0.text .iram.text)
    *libarch__xtensa__core.a:(.literal .text .literal.* .text.*)
    *libkernel.a:(.literal .text .literal.* .text.*)
    *libgcc.a:lib2funcs.*(.literal .text .literal.* .text.*)
    *libzephyr.a:windowspill_asm.*(.literal .text .literal.* .text.*)
    *libzephyr.a:cbprintf_complete.*(.literal .text .literal.* .text.*)
    *libzephyr.a:printk.*(.literal.printk .literal.vprintk .literal.char_out .text.printk .text.vprintk .text.char_out)
    *libzephyr.a:log_noos.*(.literal .text .literal.* .text.*)
    *libzephyr.a:log_core.*(.literal .text .literal.* .text.*)
    *libzephyr.a:log_msg.*(.literal .text .literal.* .text.*)
    *libzephyr.a:log_list.*(.literal .text .literal.* .text.*)
    *libzephyr.a:log_output.*(.literal .text .literal.* .text.*)
    *libzephyr.a:log_backend_uart.*(.literal .text .literal.* .text.*)
    *libzephyr.a:loader.*(.literal .text .literal.* .text.*)
    *libzephyr.a:rtc_*.*(.literal .text .literal.* .text.*)
    *libzephyr.a:cpu_util.*(.literal .text .literal.* .text.*)
    *libdrivers__flash.a:flash_esp32.*(.literal .text .literal.* .text.*)
    *libdrivers__timer.a:xtensa_sys_timer.*(.literal .text .literal.* .text.*)
    *libdrivers__console.a:uart_console.*(.literal.console_out .text.console_out)
    *liblib__libc__picolib.a:abort.*(.literal .text .literal.* .text.*)
    *liblib__libc__minimal.a:string.*(.literal .text .literal.* .text.*)
    *liblib__libc__newlib.a:string.*(.literal .text .literal.* .text.*)
    *libc.a:*(.literal .text .literal.* .text.*)
    *libphy.a:( .phyiram .phyiram.*)
    *libgcov.a:(.literal .text .literal.* .text.*)
    *libzephyr.a:spi_flash_rom_patch.*(.literal .text .literal.* .text.*)
    *libzephyr.a:mmu_psram_flash.*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_psram_impl_quad.*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_psram_impl_octal.*(.literal .literal.* .text .text.*)
    *libzephyr.a:mmu_hal.*(.literal .literal.* .text .text.*)
    *libzephyr.a:cache_hal.*(.literal .text .literal.* .text.*)
    *libzephyr.a:ledc_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:i2c_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:wdt_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_encrypt_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:lldesc.*(.literal .literal.* .text .text.*)
    *(.literal.esp_log_write .text.esp_log_write)
    *(.literal.esp_log_timestamp .text.esp_log_timestamp)
    *(.literal.esp_log_early_timestamp .text.esp_log_early_timestamp)
    *(.literal.esp_log_impl_lock .text.esp_log_impl_lock)
    *(.literal.esp_log_impl_lock_timeout .text.esp_log_impl_lock_timeout)
    *(.literal.esp_log_impl_unlock .text.esp_log_impl_unlock)
    *libzephyr.a:spi_flash_chip_boya.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_gd.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_generic.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_issi.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_mxic.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_mxic_opi.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_th.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_chip_winbond.*(.literal .literal.* .text .text.*)
    *libzephyr.a:memspi_host_driver.*(.literal .literal.* .text .text.*)
    *libzephyr.a:flash_brownout_hook.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_wrap.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_hpm_enable.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_oct_flash_init*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_err.*(.literal .literal.* .text .text.*)
    *(.literal.esp_system_abort .text.esp_system_abort)
    *(.literal.esp_cpu_stall .text.esp_cpu_stall)
    *(.literal.esp_cpu_unstall .text.esp_cpu_unstall)
    *(.literal.esp_cpu_reset .text.esp_cpu_reset)
    *(.literal.esp_cpu_wait_for_intr .text.esp_cpu_wait_for_intr)
    *(.literal.esp_cpu_compare_and_set .text.esp_cpu_compare_and_set)
    *(.literal.esp_gpio_reserve_pins .text.esp_gpio_reserve_pins)
    *(.literal.esp_gpio_is_pin_reserved .text.esp_gpio_is_pin_reserved)
    *(.literal.rtc_vddsdio_get_config .text.rtc_vddsdio_get_config)
    *(.literal.rtc_vddsdio_set_config .text.rtc_vddsdio_set_config)
    *libzephyr.a:esp_memory_utils.*(.literal .literal.* .text .text.*)
    *libzephyr.a:rtc_init.*(.literal .literal.* .text .text.*)
    *libzephyr.a:rtc_clk.*(.literal .literal.* .text .text.*)
    *libzephyr.a:rtc_clk_init.*(.literal .literal.* .text .text.*)
    *libzephyr.a:rtc_sleep.*(.literal .literal.* .text .text.*)
    *libzephyr.a:rtc_time.*(.literal .literal.* .text .text.*)
    *libzephyr.a:systimer.*(.literal .literal.* .text .text.*)
    *libzephyr.a:mspi_timing_config.*(.literal .literal.* .text .text.*)
    *libzephyr.a:mspi_timing_tuning.*(.literal .literal.* .text .text.*)
    *libzephyr.a:periph_ctrl.*(.literal .text .literal.* .text.*)
    *(.literal.sar_periph_ctrl_power_enable .text.sar_periph_ctrl_power_enable)
    *libzephyr.a:esp_system_chip.*(.literal.esp_system_abort .text.esp_system_abort)
    *libzephyr.a:spi_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_slave_hal_iram.*(.literal .literal.* .text .text.*)
    *libzephyr.a:flash_brownout_hook.*(.literal .literal.* .text .text.*)
    *libzephyr.a:heap_caps_zephyr.*(.literal .literal.* .text .text.*)
    *(.literal.GPIO_HOLD_MASK .text.GPIO_HOLD_MASK)
    *libzephyr.a:esp_rom_spiflash.*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_rom_systimer.*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_rom_wdt.*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_cache.*(.literal .literal.* .text .text.*)
    *libzephyr.a:cache_esp32.*(.literal .literal.* .text .text.*)
    . = ALIGN(4);
  } > iram0_0_seg AT > FLASH
  .loader.text :
  {
    . = ALIGN(4);
    *libzephyr.a:bootloader_soc.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_init.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_esp32.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_clock_init.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_wdt.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_flash.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_flash_config_esp32.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_clock_loader.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_common_loader.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_common.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_mem.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_random.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_random*.*(.literal.bootloader_random_disable .text.bootloader_random_disable)
    *libzephyr.a:bootloader_random*.*(.literal.bootloader_random_enable .text.bootloader_random_enable)
    *libzephyr.a:bootloader_efuse.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_utility.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_sha.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_console.*(.literal .text .literal.* .text.*)
    *libzephyr.a:bootloader_panic.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_image_format.*(.literal .text .literal.* .text.*)
    *libzephyr.a:flash_encrypt.*(.literal .text .literal.* .text.*)
    *libzephyr.a:flash_encryption_secure_features.*(.literal .text .literal.* .text.*)
    *libzephyr.a:flash_partitions.*(.literal .text .literal.* .text.*)
    *libzephyr.a:spi_flash_hal.*(.literal .literal.* .text .text.*)
    *libzephyr.a:spi_flash_hal_common.*(.literal .literal.* .text .text.*)
    *libzephyr.a:esp_flash_api.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_flash_spi_init.*(.literal .text .literal.* .text.*)
    *libzephyr.a:secure_boot.*(.literal .text .literal.* .text.*)
    *libzephyr.a:secure_boot_secure_features.*(.literal .text .literal.* .text.*)
    *libzephyr.a:secure_boot_signatures_bootloader.*(.literal .text .literal.* .text.*)
    *libzephyr.a:efuse_hal.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_efuse_table.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_efuse_fields.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_efuse_api.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_efuse_utility.*(.literal .text .literal.* .text.*)
    *libzephyr.a:esp_efuse_api_key_esp32.*(.literal .text .literal.* .text.*)
    *libzephyr.a:app_cpu_start.*(.literal .text .literal.* .text.*)
    *libzephyr.a:mpu_hal.*(.literal .text .literal.* .text.*)
    *libzephyr.a:cpu_region_protect.*(.literal .text .literal.* .text.*)
    . += 16;
    . = ALIGN(16);
  } > iram0_0_seg AT > FLASH
  .iram0.text_end (NOLOAD) :
  {
    . += 16;
    . = ALIGN(0x100);
    _iram_text_end = ABSOLUTE(.);
  } > iram0_0_seg
  .iram0.data :
  {
    . = ALIGN(16);
    *(.iram.data)
    *(.iram.data*)
  } > iram0_0_seg AT > FLASH
  .iram0.bss (NOLOAD) :
  {
    . = ALIGN(16);
    _iram_bss_start = ABSOLUTE(.);
    *(.iram.bss)
    *(.iram.bss.*)
    _iram_bss_end = ABSOLUTE(.);
    . = ALIGN(4);
    _iram_end = ABSOLUTE(.);
  } > iram0_0_seg
  .dram0.data :
  {
    _dram_data_start = ABSOLUTE(.);
    _data_start = ABSOLUTE(.);
    _btdm_data_start = ABSOLUTE(.);
    *libbtdm_app.a:(.data .data.*)
    . = ALIGN (4);
    _btdm_data_end = ABSOLUTE(.);
    *(.data)
    *(.data.*)
    *(.gnu.linkonce.d.*)
    *(.data1)
    *(.sdata)
    *(.sdata.*)
    *(.gnu.linkonce.s.*)
    *(.sdata2)
    *(.sdata2.*)
    *(.gnu.linkonce.s2.*)
    *libarch__xtensa__core.a:(.rodata .rodata.*)
    *libkernel.a:fatal.*(.rodata .rodata.*)
    *libkernel.a:init.*(.rodata .rodata.*)
    *libzephyr.a:cbprintf_complete*(.rodata .rodata.*)
    *libzephyr.a:log_core.*(.rodata .rodata.*)
    *libzephyr.a:log_backend_uart.*(.rodata .rodata.*)
    *libzephyr.a:log_output.*(.rodata .rodata.*)
    *libzephyr.a:loader.*(.rodata .rodata.*)
    *libdrivers__flash.a:flash_esp32.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_rom_patch.*(.rodata .rodata.*)
    *libdrivers__serial.a:uart_esp32.*(.rodata .rodata.*)
    *libzephyr.a:esp_memory_utils.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:mmu_psram_flash.*(.rodata .rodata.*)
    *libzephyr.a:esp_psram_impl_octal.*(.rodata .rodata.*)
    *libzephyr.a:esp_psram_impl_quad.*(.rodata .rodata.*)
    *libzephyr.a:mmu_hal.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_hal_iram.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_encrypt_hal_iram.*(.rodata .rodata.*)
    *libzephyr.a:cache_hal.*(.rodata .rodata.*)
    *libzephyr.a:ledc_hal_iram.*(.rodata .rodata.*)
    *libzephyr.a:i2c_hal_iram.*(.rodata .rodata.*)
    *libzephyr.a:wdt_hal_iram.*(.rodata .rodata.*)
    *libzephyr.a:systimer_hal.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_hal_gpspi.*(.rodata .rodata.*)
    *libzephyr.a:lldesc.*(.rodata .rodata.*)
    *(.rodata.esp_log_write)
    *(.rodata.esp_log_timestamp)
    *(.rodata.esp_log_early_timestamp)
    *(.rodata.esp_log_impl_lock)
    *(.rodata.esp_log_impl_lock_timeout)
    *(.rodata.esp_log_impl_unlock)
    *libzephyr.a:spi_flash_chip_boya.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_gd.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_generic.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_issi.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_mxic.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_mxic_opi.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_th.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_chip_winbond.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:memspi_host_driver.*(.rodata .rodata.*)
    *libzephyr.a:flash_brownout_hook.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_wrap.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_hpm_enable.*(.rodata .rodata.*)
    *libzephyr.a:spi_flash_oct_flash_init.*(.rodata .rodata.*)
    *libzephyr.a:esp_cache.*(.rodata .rodata.*)
    *(.rodata.esp_cpu_stall)
    *(.rodata.esp_cpu_unstall)
    *(.rodata.esp_cpu_reset)
    *(.rodata.esp_cpu_wait_for_intr)
    *(.rodata.esp_cpu_compare_and_set)
    *(.rodata.esp_gpio_reserve_pins)
    *(.rodata.esp_gpio_is_pin_reserved)
    *(.rodata.rtc_vddsdio_get_config)
    *(.rodata.rtc_vddsdio_set_config)
    *libzephyr.a:esp_memory_utils.*(.rodata .rodata.*)
    *libzephyr.a:rtc_clk.*(.rodata .rodata.*)
    *libzephyr.a:rtc_clk_init.*(.rodata .rodata.*)
    *libzephyr.a:systimer.*(.rodata .rodata.*)
    *libzephyr.a:mspi_timing_config.*(.rodata .rodata.*)
    *libzephyr.a:mspi_timing_tuning.*(.rodata .rodata.*)
    *(.rodata.sar_periph_ctrl_power_enable)
    *libzephyr.a:cache_esp32.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_cache.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_err.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:i2c_hal_iram.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:ledc_hal_iram.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:mmu_hal.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_encrypt_hal_iram.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_hal.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_hal_iram.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_hal_common.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_hal_iram.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_slave_hal_iram.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:flash_brownout_hook.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:memspi_host_driver.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:spi_flash_wrap.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:heap_caps_zephyr.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_rom_spiflash.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_rom_systimer.*(.rodata .rodata.*)
    *libzephyr.a:esp_rom_wdt.*(.rodata .rodata.*)
    KEEP(*(.jcr))
    *(.dram1 .dram1.*)
    . = ALIGN(4);
    . = ALIGN(4);
  } > dram0_0_seg AT > FLASH
  .loader.data :
  {
    . = ALIGN(4);
    _loader_data_start = ABSOLUTE(.);
    *libzephyr.a:bootloader_init.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_esp32.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_clock_init.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_wdt.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_flash.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_flash_config_esp32.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_common.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_common_loader.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:bootloader_efuse.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:cpu_util.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:clk.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_clk.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:rtc_clk_init.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:rtc_time.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:efuse_hal.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:cpu_region_protect.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:periph_ctrl.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_flash_api.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    *libzephyr.a:esp_flash_spi_init.*(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.*)
    . = ALIGN(4);
    _loader_data_end = ABSOLUTE(.);
  } > dram0_0_seg AT > FLASH
 .gcc_except_table : ONLY_IF_RO
 {
 *(.gcc_except_table .gcc_except_table.*)
 } > drom0_0_seg AT > FLASH
 sw_isr_table : ALIGN_WITH_INPUT
 {
  . = ALIGN(4);
  *(.gnu.linkonce.sw_isr_table*)
 } > dram0_0_seg AT > FLASH
        device_states : ALIGN_WITH_INPUT
        {
                __device_states_start = .;
  KEEP(*(".z_devstate"));
  KEEP(*(".z_devstate.*"));
                __device_states_end = .;
        } > dram0_0_seg AT > FLASH
 log_mpsc_pbuf_area : ALIGN_WITH_INPUT SUBALIGN(4) { _log_mpsc_pbuf_list_start = .; *(SORT_BY_NAME(._log_mpsc_pbuf.static.*)); _log_mpsc_pbuf_list_end = .; } > dram0_0_seg AT > FLASH
 log_msg_ptr_area : ALIGN_WITH_INPUT SUBALIGN(4) { _log_msg_ptr_list_start = .; KEEP(*(SORT_BY_NAME(._log_msg_ptr.static.*))); _log_msg_ptr_list_end = .; } > dram0_0_seg AT > FLASH
 log_dynamic_area : ALIGN_WITH_INPUT SUBALIGN(4) { _log_dynamic_list_start = .; KEEP(*(SORT_BY_NAME(._log_dynamic.static.*))); _log_dynamic_list_end = .; } > dram0_0_seg AT > FLASH
 k_timer_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_timer_list_start = .; *(SORT_BY_NAME(._k_timer.static.*)); _k_timer_list_end = .; } > dram0_0_seg AT > FLASH
 k_mem_slab_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_mem_slab_list_start = .; *(SORT_BY_NAME(._k_mem_slab.static.*)); _k_mem_slab_list_end = .; } > dram0_0_seg AT > FLASH
 k_heap_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_heap_list_start = .; *(SORT_BY_NAME(._k_heap.static.*)); _k_heap_list_end = .; } > dram0_0_seg AT > FLASH
 k_mutex_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_mutex_list_start = .; *(SORT_BY_NAME(._k_mutex.static.*)); _k_mutex_list_end = .; } > dram0_0_seg AT > FLASH
 k_stack_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_stack_list_start = .; *(SORT_BY_NAME(._k_stack.static.*)); _k_stack_list_end = .; } > dram0_0_seg AT > FLASH
 k_msgq_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_msgq_list_start = .; *(SORT_BY_NAME(._k_msgq.static.*)); _k_msgq_list_end = .; } > dram0_0_seg AT > FLASH
 k_mbox_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_mbox_list_start = .; *(SORT_BY_NAME(._k_mbox.static.*)); _k_mbox_list_end = .; } > dram0_0_seg AT > FLASH
 k_pipe_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_pipe_list_start = .; *(SORT_BY_NAME(._k_pipe.static.*)); _k_pipe_list_end = .; } > dram0_0_seg AT > FLASH
 k_sem_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_sem_list_start = .; *(SORT_BY_NAME(._k_sem.static.*)); _k_sem_list_end = .; } > dram0_0_seg AT > FLASH
 k_event_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_event_list_start = .; *(SORT_BY_NAME(._k_event.static.*)); _k_event_list_end = .; } > dram0_0_seg AT > FLASH
 k_queue_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_queue_list_start = .; *(SORT_BY_NAME(._k_queue.static.*)); _k_queue_list_end = .; } > dram0_0_seg AT > FLASH
 k_fifo_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_fifo_list_start = .; *(SORT_BY_NAME(._k_fifo.static.*)); _k_fifo_list_end = .; } > dram0_0_seg AT > FLASH
 k_lifo_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_lifo_list_start = .; *(SORT_BY_NAME(._k_lifo.static.*)); _k_lifo_list_end = .; } > dram0_0_seg AT > FLASH
 k_condvar_area : ALIGN_WITH_INPUT SUBALIGN(4) { _k_condvar_list_start = .; *(SORT_BY_NAME(._k_condvar.static.*)); _k_condvar_list_end = .; } > dram0_0_seg AT > FLASH
 sys_mem_blocks_ptr_area : ALIGN_WITH_INPUT SUBALIGN(4) { _sys_mem_blocks_ptr_list_start = .; *(SORT_BY_NAME(._sys_mem_blocks_ptr.static.*)); _sys_mem_blocks_ptr_list_end = .; } > dram0_0_seg AT > FLASH
 net_buf_pool_area : ALIGN_WITH_INPUT SUBALIGN(4) { _net_buf_pool_list_start = .; KEEP(*(SORT_BY_NAME(._net_buf_pool.static.*))); _net_buf_pool_list_end = .; } > dram0_0_seg AT > FLASH
 .gcc_except_table : ALIGN_WITH_INPUT ONLY_IF_RW
 {
 *(.gcc_except_table .gcc_except_table.*)
 } > dram0_0_seg AT > FLASH
         
 log_strings_area : SUBALIGN(4) { _log_strings_list_start = .; KEEP(*(SORT_BY_NAME(._log_strings.static.*))); _log_strings_list_end = .; } > dram0_0_seg AT > FLASH
 log_const_area : SUBALIGN(4) { _log_const_list_start = .; KEEP(*(SORT_BY_NAME(._log_const.static.*))); _log_const_list_end = .; } > dram0_0_seg AT > FLASH
 log_backend_area : SUBALIGN(4) { _log_backend_list_start = .; KEEP(*(SORT_BY_NAME(._log_backend.static.*))); _log_backend_list_end = .; } > dram0_0_seg AT > FLASH
 log_link_area : SUBALIGN(4) { _log_link_list_start = .; KEEP(*(SORT_BY_NAME(._log_link.static.*))); _log_link_list_end = .; } > dram0_0_seg AT > FLASH
         
  .dram0.end :
  {
    __data_end = ABSOLUTE(.);
    _data_end = ABSOLUTE(.);
  } > dram0_0_seg AT > FLASH
  .dram0.bss (NOLOAD) :
  {
    . = ALIGN (8);
    _bss_start = ABSOLUTE(.);
    __bss_start = ABSOLUTE(.);
    _btdm_bss_start = ABSOLUTE(.);
    *libbtdm_app.a:(.bss .bss.* COMMON)
    . = ALIGN (4);
    _btdm_bss_end = ABSOLUTE(.);
    *libkernel.a:mempool.*(.noinit.kheap_buf__system_heap .noinit.*.kheap_buf__system_heap)
    *(.dynsbss)
    *(.sbss)
    *(.sbss.*)
    *(.gnu.linkonce.sb.*)
    *(.scommon)
    *(.sbss2)
    *(.sbss2.*)
    *(.gnu.linkonce.sb2.*)
    *(.dynbss)
    *(.bss)
    *(.bss.*)
    *(.share.mem)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN (8);
    __bss_end = ABSOLUTE(.);
    _bss_end = ABSOLUTE(.);
  } > dram0_0_seg
  ASSERT(((__bss_end - ORIGIN(dram0_0_seg)) <= LENGTH(dram0_0_seg)),
          "DRAM segment data does not fit.")
  .dram0.noinit (NOLOAD) :
  {
    . = ALIGN (8);
    *(.noinit)
    *(.noinit.*)
    . = ALIGN (8);
  } > dram0_0_seg
  .dram0.end (NOLOAD) :
  {
    . = ALIGN(4);
    _end = ABSOLUTE(.);
  } > dram0_0_seg AT > FLASH
  .flash.rodata_dummy (NOLOAD) :
  {
    . = ALIGN(0x10000);
  } > FLASH
  _image_drom_start = LOADADDR(.flash.rodata);
  _image_drom_size = LOADADDR(.flash.rodata_end) - LOADADDR(.flash.rodata);
  _image_drom_vaddr = ADDR(.flash.rodata);
  .flash.rodata : ALIGN(0x10000)
  {
    _rodata_start = ABSOLUTE(.);
    _rodata_reserved_start = ABSOLUTE(.);
    . = ALIGN(4);
    . = ALIGN(4);
    *(.rodata .rodata.* .sdata2 .sdata2.* .srodata .srodata.* )
    _flash_rodata_start = ABSOLUTE(.);
    __rodata_region_start = ABSOLUTE(.);
    *(.irom1.text)
    *(.gnu.linkonce.r.*)
    *(.rodata1)
    __XT_EXCEPTION_TABLE_ = ABSOLUTE(.);
    *(.xt_except_table)
    *(.gcc_except_table .gcc_except_table.*)
    *(.gnu.linkonce.e.*)
    *(.gnu.version_r)
    . = (. + 3) & ~ 3;
    __eh_frame = ABSOLUTE(.);
    KEEP(*(.eh_frame))
    . = (. + 7) & ~ 3;
    __XT_EXCEPTION_DESCS_ = ABSOLUTE(.);
    *(.xt_except_desc)
    *(.gnu.linkonce.h.*)
    __XT_EXCEPTION_DESCS_END__ = ABSOLUTE(.);
    *(.xt_except_desc_end)
    *(.dynamic)
    *(.gnu.version_d)
    . = ALIGN(4);
    __rodata_region_end = ABSOLUTE(.);
    _lit4_start = ABSOLUTE(.);
    *(*.lit4)
    *(.lit4.*)
    *(.gnu.linkonce.lit4.*)
    _lit4_end = ABSOLUTE(.);
    . = ALIGN(4);
    *(.rodata_wlog)
    *(.rodata_wlog*)
    . = ALIGN(4);
  } > drom0_0_seg AT > FLASH
 ctors :
 {
  . = ALIGN(4);
  __ZEPHYR_CTOR_LIST__ = .;
  LONG((__ZEPHYR_CTOR_END__ - __ZEPHYR_CTOR_LIST__) / 4 - 2)
  KEEP(*(SORT_BY_NAME(".ctors*")))
  __CTOR_LIST__ = .;
  LONG(0)
  __ZEPHYR_CTOR_END__ = .;
  LONG(0)
  __CTOR_END__ = .;
 } > drom0_0_seg AT > FLASH
 init_array :
 {
  . = ALIGN(4);
  __init_array_start = .;
  __init_array_end = .;
  __zephyr_init_array_start = .;
  KEEP(*(SORT_BY_NAME(".init_array*")))
  __zephyr_init_array_end = .;
 } > drom0_0_seg AT > FLASH
 initlevel :
 {
  __init_start = .;
  __init_EARLY_start = .; KEEP(*(SORT(.z_init_EARLY?_*))); KEEP(*(SORT(.z_init_EARLY??_*)));
  __init_PRE_KERNEL_1_start = .; KEEP(*(SORT(.z_init_PRE_KERNEL_1?_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_1??_*)));
  __init_PRE_KERNEL_2_start = .; KEEP(*(SORT(.z_init_PRE_KERNEL_2?_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_2??_*)));
  __init_POST_KERNEL_start = .; KEEP(*(SORT(.z_init_POST_KERNEL?_*))); KEEP(*(SORT(.z_init_POST_KERNEL??_*)));
  __init_APPLICATION_start = .; KEEP(*(SORT(.z_init_APPLICATION?_*))); KEEP(*(SORT(.z_init_APPLICATION??_*)));
  __init_SMP_start = .; KEEP(*(SORT(.z_init_SMP?_*))); KEEP(*(SORT(.z_init_SMP??_*)));
  __init_end = .;
  __deferred_init_list_start = .;
  KEEP(*(.z_deferred_init))
  __deferred_init_list_end = .;
 } > drom0_0_seg AT > FLASH
 device_area : SUBALIGN(4) { _device_list_start = .; KEEP(*(SORT(._device.static.*_?_*))); KEEP(*(SORT(._device.static.*_??_*))); _device_list_end = .; } > drom0_0_seg AT > FLASH
 initlevel_error :
 {
  KEEP(*(SORT(.z_init_[_A-Z0-9]*)))
 }
 ASSERT(SIZEOF(initlevel_error) == 0, "Undefined initialization levels used.")
 app_shmem_regions : ALIGN_WITH_INPUT
 {
  __app_shmem_regions_start = .;
  KEEP(*(SORT(.app_regions.*)));
  __app_shmem_regions_end = .;
 } > drom0_0_seg AT > FLASH
 k_p4wq_initparam_area : SUBALIGN(4) { _k_p4wq_initparam_list_start = .; KEEP(*(SORT_BY_NAME(._k_p4wq_initparam.static.*))); _k_p4wq_initparam_list_end = .; } > drom0_0_seg AT > FLASH
 _static_thread_data_area : SUBALIGN(4) { __static_thread_data_list_start = .; KEEP(*(SORT_BY_NAME(.__static_thread_data.static.*))); __static_thread_data_list_end = .; } > drom0_0_seg AT > FLASH
 device_deps : ALIGN_WITH_INPUT
 {
__device_deps_start = .;
KEEP(*(SORT(.__device_deps_pass2*)));
__device_deps_end = .;
 } > drom0_0_seg AT > FLASH
ztest :
{
 _ztest_expected_result_entry_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_expected_result_entry.static.*))); _ztest_expected_result_entry_list_end = .;
 _ztest_suite_node_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_suite_node.static.*))); _ztest_suite_node_list_end = .;
 _ztest_unit_test_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_unit_test.static.*))); _ztest_unit_test_list_end = .;
 _ztest_test_rule_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_test_rule.static.*))); _ztest_test_rule_list_end = .;
} > drom0_0_seg AT > FLASH
 bt_l2cap_fixed_chan_area : SUBALIGN(4) { _bt_l2cap_fixed_chan_list_start = .; KEEP(*(SORT_BY_NAME(._bt_l2cap_fixed_chan.static.*))); _bt_l2cap_fixed_chan_list_end = .; } > drom0_0_seg AT > FLASH
 bt_gatt_service_static_area : SUBALIGN(4) { _bt_gatt_service_static_list_start = .; KEEP(*(SORT_BY_NAME(._bt_gatt_service_static.static.*))); _bt_gatt_service_static_list_end = .; } > drom0_0_seg AT > FLASH
 tracing_backend_area : SUBALIGN(4) { _tracing_backend_list_start = .; KEEP(*(SORT_BY_NAME(._tracing_backend.static.*))); _tracing_backend_list_end = .; } > drom0_0_seg AT > FLASH
 zephyr_dbg_info : ALIGN_WITH_INPUT
 {
  KEEP(*(".dbg_thread_info"));
 } > drom0_0_seg AT > FLASH
 symbol_to_keep : ALIGN_WITH_INPUT
 {
  __symbol_to_keep_start = .;
  KEEP(*(SORT(.symbol_to_keep*)));
  __symbol_to_keep_end = .;
 } > drom0_0_seg AT > FLASH
 shell_area : SUBALIGN(4) { _shell_list_start = .; KEEP(*(SORT_BY_NAME(._shell.static.*))); _shell_list_end = .; } > drom0_0_seg AT > FLASH
 shell_root_cmds_area : SUBALIGN(4) { _shell_root_cmds_list_start = .; KEEP(*(SORT_BY_NAME(._shell_root_cmds.static.*))); _shell_root_cmds_list_end = .; } > drom0_0_seg AT > FLASH
 shell_subcmds_area : SUBALIGN(4) { _shell_subcmds_list_start = .; KEEP(*(SORT_BY_NAME(._shell_subcmds.static.*))); _shell_subcmds_list_end = .; } > drom0_0_seg AT > FLASH
 shell_dynamic_subcmds_area : SUBALIGN(4) { _shell_dynamic_subcmds_list_start = .; KEEP(*(SORT_BY_NAME(._shell_dynamic_subcmds.static.*))); _shell_dynamic_subcmds_list_end = .; } > drom0_0_seg AT > FLASH
 cfb_font_area : SUBALIGN(4) { _cfb_font_list_start = .; KEEP(*(SORT_BY_NAME(._cfb_font.static.*))); _cfb_font_list_end = .; } > drom0_0_seg AT > FLASH
/DISCARD/ :
{
 KEEP(*(.irq_info*))
 KEEP(*(.intList*))
}
  .flash.rodata_end :
  {
    . = ALIGN(0x10000);
    _rodata_end = ABSOLUTE(.);
    _rodata_reserved_end = ABSOLUTE(.);
  } > drom0_0_seg AT > FLASH
  .flash.text_dummy (NOLOAD):
  {
    . = ALIGN(0x10000);
  } > FLASH
  _image_irom_start = LOADADDR(.flash.text);
  _image_irom_size = LOADADDR(.flash.text) + SIZEOF(.flash.text) - _image_irom_start;
  _image_irom_vaddr = ADDR(.flash.text);
  .flash.text : ALIGN(0x10000)
  {
    _stext = .;
    _instruction_reserved_start = ABSOLUTE(.);
    _text_start = ABSOLUTE(.);
    __text_region_start = ABSOLUTE(.);
    *libnet80211.a:( .wifi0iram .wifi0iram.* .wifislpiram .wifislpiram.*)
    *libpp.a:( .wifi0iram .wifi0iram.* .wifislpiram .wifislpiram.* .wifiorslpiram .wifiorslpiram.*)
    *libnet80211.a:( .wifirxiram .wifirxiram.* .wifislprxiram .wifislprxiram.*)
    *libpp.a:( .wifirxiram .wifirxiram.* .wifislprxiram .wifislprxiram.*)
    *(.fini.literal)
    *(.fini)
    *(.literal .text .literal.* .text.*)
    . = ALIGN(4);
    _text_end = ABSOLUTE(.);
    _instruction_reserved_end = ABSOLUTE(.);
    __text_region_end = ABSOLUTE(.);
    _etext = .;
  } > irom0_0_seg AT > FLASH
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .gnu.build.attributes 0 : { *(.gnu.build.attributes .gnu.build.attributes.*) }
 .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end ) }
 .debug_frame 0 : { *(.debug_frame) }
 .debug_str 0 : { *(.debug_str) }
 .debug_loc 0 : { *(.debug_loc) }
 .debug_macinfo 0 : { *(.debug_macinfo) }
 .debug_weaknames 0 : { *(.debug_weaknames) }
 .debug_funcnames 0 : { *(.debug_funcnames) }
 .debug_typenames 0 : { *(.debug_typenames) }
 .debug_varnames 0 : { *(.debug_varnames) }
 .debug_pubtypes 0 : { *(.debug_pubtypes) }
 .debug_ranges 0 : { *(.debug_ranges) }
 .debug_addr 0 : { *(.debug_addr) }
 .debug_line_str 0 : { *(.debug_line_str) }
 .debug_loclists 0 : { *(.debug_loclists) }
 .debug_macro 0 : { *(.debug_macro) }
 .debug_names 0 : { *(.debug_names) }
 .debug_rnglists 0 : { *(.debug_rnglists) }
 .debug_str_offsets 0 : { *(.debug_str_offsets) }
 .debug_sup 0 : { *(.debug_sup) }
  .xtensa.info 0 : { *(.xtensa.info) }
  .xt.insn 0 :
  {
    KEEP (*(.xt.insn))
    KEEP (*(.gnu.linkonce.x.*))
  }
  .xt.prop 0 :
  {
    KEEP (*(.xt.prop))
    KEEP (*(.xt.prop.*))
    KEEP (*(.gnu.linkonce.prop.*))
  }
  .xt.lit 0 :
  {
    KEEP (*(.xt.lit))
    KEEP (*(.xt.lit.*))
    KEEP (*(.gnu.linkonce.p.*))
  }
  .xt.profile_range 0 :
  {
    KEEP (*(.xt.profile_range))
    KEEP (*(.gnu.linkonce.profile_range.*))
  }
  .xt.profile_ranges 0 :
  {
    KEEP (*(.xt.profile_ranges))
    KEEP (*(.gnu.linkonce.xt.profile_ranges.*))
  }
  .xt.profile_files 0 :
  {
    KEEP (*(.xt.profile_files))
    KEEP (*(.gnu.linkonce.xt.profile_files.*))
  }
/DISCARD/ :
{
 KEEP(*(.irq_info*))
 KEEP(*(.intList*))
}
}
ASSERT(((_iram_end - ORIGIN(iram0_0_seg)) <= LENGTH(iram0_0_seg)),
          "IRAM0 segment data does not fit.")
