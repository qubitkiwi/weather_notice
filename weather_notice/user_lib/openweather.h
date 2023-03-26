/*
 * openweather.h
 *
 *  Created on: 2023. 3. 23.
 *      Author: asong
 */

#ifndef OPENWEATHER_H_
#define OPENWEATHER_H_

#include "esp8266client.h"
#include "weather_iocn.h"
#include "time.h"

struct weater {
	weather_id weather_ID;
	int temp;
	int humidity;
	char city[10];
	char time_str[12];
};

struct particulate_matter {
	int pm10;
	int pm2_5;
};

typedef struct {
	char *lon;
	char *lat;
	char *api_key;
}weather_cfg;

int weather_init(char *lon, char *lat, char *api_key);

bool weather_get(uint8_t *req, struct weater *W);
bool air_pollution_get(uint8_t *req, struct particulate_matter *PM);


#endif /* OPENWEATHER_H_ */
