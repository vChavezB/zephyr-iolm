#pragma once

#include <stdint.h>

/**
 * @brief Decode IOL-Device cycle time 
 * @details See IO-Link v1.1.3 Table B.15
 * 
 * @param encoded_time Encoded Cycle Time
 * @return uint32_t Cycle time in microseconds
 */
uint32_t cyctime_decode_us(uint8_t encoded_time);

/**
 * @brief Encode IOL-Device cycle time
 * @details See IO-Link v1.1.3 Table B.15
 * @param CycleTimeUs Cycle time in microseconds
 * @return uint8_t  Encoded Cycle Time according to spec
 */
uint8_t cyctime_encode(uint32_t cycletime_us);