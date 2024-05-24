# Copyright (c) 2024 Victor Chavez
# SPDX-License-Identifier: GPL-3.0-or-later

if(CONFIG_IOLINK)
    set(lib_name iolink_master)
    set(OSAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/osal)
    set(IOL_OSAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/iol_osal)
    set(STACK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/i-link)
    set(STACK_SRC ${STACK_DIR}/src)
    zephyr_library_named(${lib_name})
    # todo separate private and public 
    zephyr_include_directories( ${STACK_DIR}/include
                                ${STACK_DIR}/include/src
                                ${STACK_DIR}/src
                                ${STACK_DIR}/iol_osal/include
                                ${CMAKE_CURRENT_SOURCE_DIR}/src/osal
                                ${CMAKE_CURRENT_SOURCE_DIR}/src
                                include
        )
    
    zephyr_library_sources(${STACK_SRC}/iolink_al.c
                            ${STACK_SRC}/iolink_cm.c
                            ${STACK_SRC}/iolink_dl.c
                            ${STACK_SRC}/iolink_ds.c
                            ${STACK_SRC}/iolink_main.c
                            ${STACK_SRC}/iolink_max14819_pl.c
                            ${STACK_SRC}/iolink_ode.c
                            ${STACK_SRC}/iolink_pde.c
                            ${STACK_SRC}/iolink_pl.c
                            ${STACK_SRC}/iolink_sm.c
                            src/osal/osal.c
                            src/osal/osal_log.c
                            src/transciever.c)
    
    #zephyr_library_sources_ifdef(CONFIG_XX xx)
    #target_compile_definitions(${lib_name} PUBLIC MY_DEF="123")
    target_include_directories(${lib_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dts/bindings/iolm)
    target_compile_options(${lib_name} PRIVATE -Wfatal-errors)
endif()
    
