#include <stdint.h>

#define CYC_TIME_ENCODINGS 3

static uint8_t time_base_offset = 6;
static uint8_t RANGE_MIN_IDX = 0;
static uint8_t RANGE_MAX_IDX = 1;
static uint8_t MULT_MASKING = 0x3F; /* Multiplier masking */

static uint8_t MAX_CYC_MULT=63; /* Max Cycletime multiplier factor*/

/* Cycle time ranges according to IOLink Spec */
static const uint32_t RangeUs[CYC_TIME_ENCODINGS][2]=
{ 
    {400,6300},
    {6400,31600},
    {32000,132800}
};

const uint16_t TimeBaseUs[CYC_TIME_ENCODINGS]={100,400,1600};
const uint16_t TimeOffsetUs[CYC_TIME_ENCODINGS]={0,6400,32000};

static inline uint32_t calculate_cyctime(uint8_t mult,uint8_t encode_idx)
{
    return (TimeOffsetUs[encode_idx] + (TimeBaseUs[encode_idx]*mult));
}

uint32_t cyctime_decode_us(uint8_t encoded_time)
{
    const uint8_t time_base_idx = encoded_time >> time_base_offset;
    const uint8_t multiplier =  encoded_time & 0x3F;
    return calculate_cyctime(multiplier,time_base_idx);
}


uint8_t cyctime_encode(uint32_t CycleTimeUs)
{
    uint8_t encoded_cycle_time=0;
    do {
        if(CycleTimeUs <= RangeUs[0][RANGE_MIN_IDX]) {
            encoded_cycle_time= 0x04; //Minimum of 0.4 ms cycle time
            break;
        }    
        if(CycleTimeUs >= RangeUs[2][RANGE_MAX_IDX]) {
            //Maximum 132.8ms cycle time
            encoded_cycle_time= 0xBF;
            break;
        }  

        uint8_t encoding_idx=0xFF;
        for(uint8_t i=0;i<CYC_TIME_ENCODINGS;i++) {
            if(CycleTimeUs>=RangeUs[i][RANGE_MIN_IDX] 
                && CycleTimeUs<=RangeUs[i][RANGE_MAX_IDX]) {
                encoding_idx=i;
                break;
            }
            if(i != (CYC_TIME_ENCODINGS-1)) {
                /* if cycle time between two ranges */
               if(CycleTimeUs<RangeUs[i+1][RANGE_MIN_IDX])
                {
                    /* Change cycle time to minimum value for
                        next range */
                    CycleTimeUs = RangeUs[i+1][RANGE_MIN_IDX];
                    encoding_idx = i+1;
                    break;
                }
            }
        }
        uint8_t required_mult=0xFF;

        for(uint8_t mult=0; mult<MAX_CYC_MULT; mult++) {
            uint32_t cyc1 = calculate_cyctime(mult,encoding_idx);
            uint32_t cyc2 = calculate_cyctime(mult+1,encoding_idx);
            if(CycleTimeUs == cyc1) {
                required_mult = mult;
                break;
            }
            if((CycleTimeUs> cyc1 && CycleTimeUs< cyc2) /* If not within the range */
                ||CycleTimeUs == cyc2) { /* or equals next cycle time */
                required_mult = mult+1;
                break;
            }
        }
        
    encoded_cycle_time= encoding_idx<<time_base_offset
                         | (required_mult & MULT_MASKING);
    } while(0);
    return encoded_cycle_time;
}
