/*
 * esp8266.c
 *
 *  Created on: 2023. 2. 24.
 *      Author: asong
 */

#include "esp8266client.h"
#include "html.h"


static ESP_CONFIG esp_config;

/**
 * USART3_IRQHandler안에 있어야 하는 함수
 * uart가 idle 상태인지 확인한다.
 */
void uart_it() {
	if(LL_USART_IsActiveFlag_IDLE(esp_config.uart)) {
		esp_config.uart_idle = true;
		LL_USART_ClearFlag_IDLE(esp_config.uart);
	}
}


int esp8266_init(USART_TypeDef *uart, DMA_TypeDef *DMA, uint32_t Channel) {

	esp_config.uart = uart;
	esp_config.DMA = DMA;
	esp_config.DMA_Channel = Channel;
	esp_config.uart_idle = false;

	// configure dma source & destination
	LL_DMA_ConfigAddresses(esp_config.DMA, esp_config.DMA_Channel, LL_USART_DMA_GetRegAddr(esp_config.uart), (uint32_t)esp_config.DMA_buf, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	// configure data length
	LL_DMA_SetDataLength(esp_config.DMA, esp_config.DMA_Channel, DMA_BUF_SIZE);

	// enable interrupts
	LL_USART_EnableIT_IDLE(esp_config.uart);

	// enable DMA stream
	LL_USART_EnableDMAReq_RX(esp_config.uart);
	LL_DMA_EnableChannel(esp_config.DMA, esp_config.DMA_Channel);


	return 1;
}

void esp_send(uint8_t *pdata) {

	printf("tx: %s\r\n==========\r\n", pdata);

	while (*pdata) {
		while(!LL_USART_IsActiveFlag_TXE(esp_config.uart));
		LL_USART_TransmitData8(esp_config.uart, *pdata);
		pdata++;
	}
}

int response(uint8_t *req, uint32_t m_time) {

	memset(req, 0, DMA_BUF_SIZE);

	uint32_t tickstart = HAL_GetTick();

	while (!esp_config.uart_idle) {
		//시간 초과시 0을 리턴
		if ((HAL_GetTick() - tickstart) > m_time)
			return 0;
	}

	esp_config.uart_idle = false;

	//disble DMA
	LL_DMA_DisableChannel(esp_config.DMA, esp_config.DMA_Channel);

	//total received size = total buffer size - left size for DMA
	uint32_t buff_len = DMA_BUF_SIZE - LL_DMA_GetDataLength(esp_config.DMA, esp_config.DMA_Channel);
	memcpy(req, esp_config.DMA_buf, buff_len+1);
	req[buff_len] = 0;

	//clear DMA flags
	esp_config.DMA->IFCR &= 0b1111 << ((esp_config.DMA_Channel -1) * 4);

	// configure data length
	LL_DMA_SetDataLength(esp_config.DMA, esp_config.DMA_Channel, DMA_BUF_SIZE);

	// enable DMA stream
	LL_DMA_EnableChannel(esp_config.DMA, esp_config.DMA_Channel);


	printf("rx len: %lu\r\n%s\r\n==========\r\n", buff_len, req);

	return buff_len;
}

bool wait_for(uint8_t *str, uint32_t m_time) {

	uint8_t buf[DMA_BUF_SIZE];

	uint32_t tickstart = HAL_GetTick();

	char *str_ptr;

	while ((HAL_GetTick() - tickstart) < m_time) {
		response(buf, m_time);
		str_ptr = strstr((char*)buf, (char*)str);
		if (str_ptr != NULL)
			return true;
	}
	return false;
}

int client_init(char *ssid, char* password, uint16_t port) {

	esp_config.SSID = ssid;
	esp_config.password = password;
	esp_config.port = port;

	char CMD[40];

	CLIENT_RESET:
	LL_mDelay(6000);

	printf("0-----------\r\n");
	esp_send("AT\r\n");
	if(!wait_for("OK", 4000))
		goto CLIENT_RESET;

	//AT reset
	printf("1-----------\r\n");
	esp_send("AT+RST\r\n");
	if(!wait_for("OK", 4000))
		goto CLIENT_RESET;

	//station mode
	printf("2-----------\r\n");
	esp_send("AT+CWMODE=1\r\n");
	if(!wait_for("OK", 4000))
		goto CLIENT_RESET;

	//Connect to an AP
	printf("3-----------\r\n");
	sprintf(CMD, "AT+CWJAP=\"%s\",\"%s\"\r\n", esp_config.SSID, esp_config.password);
	esp_send(CMD);
	if(!wait_for("ready", 4000))
		goto CLIENT_RESET;

	// get IP info
	printf("4-----------\r\n");
	sprintf(CMD, "AT+CIFSR\r\n");
	esp_send(CMD);
	if(!wait_for("OK", 4000))
		goto CLIENT_RESET;

	//Query the connection type. : single connection.
	printf("5-----------\r\n");
	esp_send("AT+CIPMUX=0\r\n");
	if(!wait_for("OK", 4000))
		goto CLIENT_RESET;


	return 1;
}




char* get_path_ptr(char *pdata) {

	char *path_ptr;

	path_ptr = strstr(pdata, "HTTP");

	if (path_ptr == NULL)
		return NULL;

	while (!(*(path_ptr-1) == ' ' && *path_ptr == '/')) {
		path_ptr--;
	}

	return path_ptr;
}

char* get_body_ptr(char *pdata) {

	char *body_ptr;
	body_ptr = strstr(pdata, "\r\n\r\n");

	while (body_ptr != NULL) {
		body_ptr += 4;
		if (strncmp(body_ptr, "+IPD", 4)) {
			return body_ptr;
		}
		body_ptr = strstr(body_ptr, "\r\n\r\n");
	}
	return NULL;
}

bool tcp_Send(uint8_t *req, uint8_t *pAddress ,uint8_t *pdata) {

	//tcp start
	uint8_t CMD[130];
	sprintf (CMD, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", pAddress, esp_config.port);
	esp_send(CMD);
	if (!wait_for("OK", 1000))
		return false;


	//Send Data
	uint32_t data_len = strlen(pdata);
	sprintf (CMD, "AT+CIPSEND=%d\r\n", data_len);
	esp_send(CMD);
	if (!wait_for("OK", 2000)){
		esp_send("AT+CIPCLOSE\r\n");
		return false;
	}


	esp_send(pdata);
	uint32_t cnt=0;

	do{
		response(req, 1000);
		cnt++;
	} while ( (strstr(req, "HTTP/1.1") == NULL) && cnt < 4);

//	response(req, 1000);

	esp_send("AT+CIPCLOSE\r\n");
	wait_for("OK", 100);

	return true;
}



bool HTTP_GET(uint8_t *req, uint8_t *url , uint8_t *path) {

	uint8_t data[160];
	sprintf(data, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, url);
	return tcp_Send(req, url, data);

}


