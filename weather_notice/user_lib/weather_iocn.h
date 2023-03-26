/*
 * weather_iocn.h
 *
 *  Created on: 2023. 3. 23.
 *      Author: asong
 */

#ifndef WEATHER_IOCN_H_
#define WEATHER_IOCN_H_

#include "stdint.h"

typedef struct {
	uint16_t Width;
	uint16_t Height;
	const uint32_t *data;
} weather_icon_t;

extern weather_icon_t weather_icon;

typedef enum {
	Thunderstorm = 2,
	Drizzle,
	Rain,
	snow,
	Atmosphere,
	Clear,
	Clouds
} weather_id;

#endif /* WEATHER_IOCN_H_ */
