/*
 * openweather.c
 *
 *  Created on: 2023. 3. 23.
 *      Author: asong
 */

#include "openweather.h"

weather_cfg w_cig;

char *openweathermap_rul = "api.openweathermap.org";
char *weather_rul 		 = "/data/2.5/weather?";
char *PM_rul 			 = "/data/2.5/air_pollution?";

int weather_init(char *lon, char *lat, char *api_key) {
	w_cig.lon = lon;
	w_cig.lat = lat;
	w_cig.api_key = api_key;

	return 1;
}



bool weather_get(uint8_t *req, struct weater *W) {

	char path[256];
	sprintf(path, "%slat=%s&lon=%s&appid=%s", weather_rul, w_cig.lat, w_cig.lon, w_cig.api_key);

	if (!HTTP_GET(req, openweathermap_rul, path))
			return false;

	char *ptr;
	ptr = strstr(req, "id");
	if (ptr == NULL)
		return false;
	while(*ptr++ != ':');
	W->weather_ID = atoi(ptr)/100;

	ptr = strstr(ptr, "temp");
	while(*ptr++ != ':');
	W->temp = atoi(ptr) - 273;

	ptr = strstr(ptr, "humidity");
	while(*ptr++ != ':');
	W->humidity = atoi(ptr);

	//time
	ptr = strstr(ptr, "dt");
	while(*ptr++ != ':');
	time_t unix_time = atol(ptr);

	ptr = strstr(ptr, "timezone");
	while(*ptr++ != ':');
	unix_time += atol(ptr);

	struct tm *time_info;
	char time_str[20];

	time_info = localtime(&unix_time);
	strftime(W->time_str, sizeof(W->time_str), "%m.%d %H:%M", time_info);


	//city
	ptr = strstr(ptr, "name");
	while(*ptr++ != ':');
	ptr++;
	uint32_t idx = 0;
	while (*ptr != '"' && idx < 9 ) {
		W->city[idx] = *ptr;
		ptr++;
		idx++;
	}
	W->city[idx] = 0;

	return true;
}

bool air_pollution_get(uint8_t *req, struct particulate_matter *PM) {

	char path[256];
	sprintf(path, "%slat=%s&lon=%s&appid=%s", PM_rul, w_cig.lat, w_cig.lon, w_cig.api_key);

	if (!HTTP_GET(req, openweathermap_rul, path))
				return false;

	char *ptr;
	ptr = strstr(req, "pm2_5");
	if (ptr == NULL) return false;
	while(*ptr++ != ':');
	PM->pm2_5 = atoi(ptr);

	ptr = strstr(ptr, "pm10");
	while(*ptr++ != ':');
	PM->pm10 = atoi(ptr);

	return true;
}
