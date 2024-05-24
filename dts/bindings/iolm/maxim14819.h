/*  Copyright (c) 2024 Victor Chavez
    SPDX-License-Identifier: GPL-3.0-or-later
*/
#define CQCFG_CQFILTEREN    BIT(0)
#define CQCFG_DRVDIS        BIT(1)
#define CQCFG_PUSHPUL       BIT(2)
#define CQCFG_NPN           BIT(3)
#define CQCFG_SINKSEL(x)    (((x) & 0x03) << 4)
#define CQCFG_SINKSEL_MASK  CQCFG_SINKSEL (0x3)
#define CQCFG_SOURCESINK    BIT(6)
#define CQCFG_IEC3TH        BIT(7)

#define CLOCK_XTALEN        BIT (0)
#define CLOCK_EXTCLKEN      BIT (1)
#define CLOCK_CLKDIV(x)     (((x) & 0x03) << 2)
#define CLOCK_CLKDIV_MASK   CLOCK_CLKDIV (0x03)
#define CLOCK_CLKOEN        BIT (4)
#define CLOCK_EXTCLKMIS     BIT (5)
#define CLOCK_TXTXENDIS     BIT (6)
#define CLOCK_VCCWARNEN     BIT (7)

#define IOSTCFG_DICSINK     BIT (0)
#define IOSTCFG_DICSOURCE   BIT (1)
#define IOSTCFG_DIEC3TH     BIT (2)
#define IOSTCFG_DIFILTEREN  BIT (3)
#define IOSTCFG_TX          BIT (4)
#define IOSTCFG_TXEN        BIT (5)
#define IOSTCFG_CQLEVEL     BIT (6)
#define IOSTCFG_DILEVEL     BIT (7)

#define LPCNFG_LPEN         BIT (0)
#define LPCNFG_LPCLIMDIS    BIT (1)
#define LPCNFG_LPCL2X       BIT (2)
#define LPCNFG_LPBL(x)      (((x) & 0x03) << 3)
#define LPCNFG_LPBL_MASK    LPCNFG_BLA (0x03)
#define LPCNFG_LPDYNBL      BIT (5)
#define LPCNFG_LPRT(x)      (((x) & 0x03) << 6)
#define LPCNFG_LPRT_MASK    LPCNFG_BLA (0x03)

#define CURR_100MA 0x00
#define CURR_200MA 0x01
#define CURR_300MA 0x02
#define CURR_500MA 0x03
#define DRVRCURRLIM_CLDIS BIT(5)

#define CURR_OFF 0x06

#define CL_CONF(x)  ((x) << CURR_OFF)