/*
 * esp8266.h
 *
 *  Created on: 2023. 2. 24.
 *      Author: asong
 */

#ifndef ESP8266CLIENT_H_
#define ESP8266CLIENT_H_

//#include "stm32f1xx.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_usart.h"

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"


#define DMA_BUF_SIZE 1024


typedef struct {
	USART_TypeDef *uart;
	DMA_TypeDef *DMA;
	uint32_t DMA_Channel;

	char *SSID;
	char *password;
	uint16_t port;

	bool uart_idle;

	char DMA_buf[DMA_BUF_SIZE];
} ESP_CONFIG;


int esp8266_init(USART_TypeDef *uart, DMA_TypeDef *DMA, uint32_t Channel);
int client_init(char *ssid, char* password, uint16_t port);

void esp_send(uint8_t *pdata);
int response(uint8_t *req, uint32_t m_time);
bool wait_for(uint8_t *str, uint32_t m_time);

int get_IPD(char *pdata);
char* get_path_ptr(char *pdata);
char* get_body_ptr(char *pdata);

bool tcp_Send(uint8_t *req, uint8_t *pAddress ,uint8_t *pdata);
bool HTTP_GET(uint8_t *req, uint8_t *url , uint8_t *path);

void uart_it();


#endif /* ESP8266CLIENT_H_ */
