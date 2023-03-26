/*
 * weather_icon.c
 *
 *  Created on: 2023. 3. 23.
 *      Author: asong
 */

#include "weather_iocn.h"

const uint32_t weater_icon[] = {
	/*Thunderstorm*/0x0,0x400080,0xc00180,0x1c00180,0x1400280,0x34002c0,0x2600440,0x6200440,0x4200c40,0xc200860,0x820083b,0x8300805,0x81f8801,0x180089c1,0x10008622,0x13c10024,0x1c410044,0x42004c,0x820048,0x840098,0x8400a0,0x10800a0,0x11000c0,0x1200180,0x1c00100,0x1000000,0x0,0x0
	/*Drizzle*/,0x29e04a0,0xd713b58,0x1000e006,0x20000001,0x20000001,0x30000001,0x1c5f1806,0x3e0e56c,0x80000,0x4,0x8004000,0x8040,0x880800,0x800,0x10,0x10000,0x2000282,0x8,0x10841010,0x0,0x100,0x102000,0x100400,0x4002002,0x800020,0x20000,0x0,0x200
	/*Rain*/,0x29e04a0,0xd713b58,0x1000e006,0x20000001,0x20000001,0x30000001,0x1c5f1986,0x3e6e77c,0x9120820,0x9120824,0x9124831,0xa928811,0x9320811,0x9220811,0x9220811,0x9220811,0xb220a91,0x1320819,0x1961811,0x9120811,0x9120910,0x9122811,0x9120c11,0x9272813,0x9a10831,0x8b10810,0x8910810,0x910a00
	/*snow*/	,0x108400,0x10888884,0x8849088,0x482a090,0x281c0a0,0x18080c0,0x1f81c0fc,0x42a100,0x249200,0x80188c01,0x40188c02,0x20249204,0x1042a108,0x841c090,0xffff7fff,0x841c090,0x1042a108,0x20249204,0x40188c02,0x80188c01,0x249200,0x43e100,0x3f8080fc,0x181c0c0,0x282a0a0,0x4849090,0x8888888,0x10908484
	/*Atmosphere*/	,0x0,0x0,0x3f81f00,0x7fc3f80,0x0,0x0,0x3ffffc00,0x7ffffc00,0x0,0x0,0x3fffff8,0x3fffffc,0x0,0x0,0x3ffff800,0x7ffff800,0x0,0x0,0x3ffffff8,0x1ffffffc,0x0,0x0,0x0,0x7ffe00,0x3ffc00,0x0,0x0,0x0
	/*Clear*/	,0x10000,0x10000,0x8010020,0x4010040,0x2010080,0x11ff100,0xb01a00,0x600c00,0xc00600,0x1800300,0x1000100,0x7f0001fe,0x1000100,0x1000100,0x1000100,0x1800300,0xc00600,0x600c00,0xb01a00,0x11ff100,0x2010080,0x4010040,0x8010020,0x10010010,0x10000,0x10000,0x0,0x0
	/*cloud*/	,0x0,0x0,0x0,0x1c000,0x27000,0x41800,0xc04c0,0x1cc0520,0x3680610,0x2380210,0x6100210,0xc000008,0x18000004,0x10000002,0x30000003,0x30000001,0x60000001,0x60000001,0x60000003,0x30000003,0x1c00e003,0xfbfb7a6,0x3ef1c7e,0x0,0x0,0x0,0x0,0x0
};


weather_icon_t weather_icon = {
		32,
		28,
		weater_icon
};
