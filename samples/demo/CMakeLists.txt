# Copyright (c) 2024 Victor Chavez
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.20.0)
set(IOLINK_MODULE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(ZEPHYR_EXTRA_MODULES ${IOLINK_MODULE})
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(iolm_demo)

target_sources(app PRIVATE src/iolink_smi.c
                            src/iolink_handler.c
                            src/demo.c
)
