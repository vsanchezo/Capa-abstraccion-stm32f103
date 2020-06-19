#include "stm32f10x.h"                  // Device header
#include "capa_de_abstraccion_gpio.h"

//***************para debug************
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

#define DEBUG_UART USART1
#define delay for(int i = 0; i < 5000000; i++)

static void imprimirMsj(char *msj, ...);

//****************fin debug ************

//CRH - Control Register High

int main(void){
	/*	
	//inicializa el reloj para GPIOC
	RCC->APB2ENR |= (1<<4);
	
	//Configura el pin13 de GPIOC
	GPIOC->CRH |= ( (1<<20) | (1<<21) );    //output mode a 50 MHz
	
	GPIOC->CRH &= ~( (1>>22) | (1<<23) );    //Configurado como SALIDA de propósito general
	*/
	
	//***********debug uart********
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;    //activar el reloj en usart1
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;    //activar el reloj en el puerto A
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;    //activar el reloj en la función alterna
		//todo junto
		//RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
		
		GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_CNF9_0 | GPIO_CRH_MODE9_1;
		GPIOA->CRH &= ~(GPIO_CRH_CNF9_0);
		
		//ajustes de uart
		//frecReloj/(bitRate * 16) = 72MHz / (9600 * 16)
		//468.75
		//int brr = (24000000/16)/9600;
		//USART1->BRR = (unsigned)(brr*16);    //0x341;
		USART1->BRR = 0x1d4c;
		USART1->CR1 |= USART_CR1_TE;    //activar el transmisor
		USART1->CR1 |= USART_CR1_UE;    //activar uart
		
		//uint32_t brr = (72000000 / 16) / 9600;  // what you're actually computing, ie the reverse of pulling the baud rate by knowing the brr and input clock source
    //brr = 468.75      // the rate in a fractional sense in OVER16 mode
    //USART->BRR = (unsigned)(468.75 * 16.0);  // fixed point 12.4 representation of fractional rate
    //USART->BRR = 7500
	//***********fin debug uart****
		
	/*	
	TIPO_GPIO miGPIO;
	miGPIO.puerto = PUERTOC;
	miGPIO.pin = 13;
	miGPIO.modo = MODO_SALIDA;
	miGPIO.tipo_modo = SALIDA_PROP_GEN;
	miGPIO.velocidad = VEL_2MHZ;
	
	gpio_iniciar(miGPIO);
	*/
	
	//uso de las interrupciones
	configurar_interrupcion_gpio(PUERTOB, 0, FLANCO_DE_SUBIDA);
	
	activar_interrupcion_gpio(0, EXTI0_IRQn);
	
	while(1){
		
		imprimirMsj("Hola, tengo %d anios\n", 28);
		delay;
		
		/*
		GPIOC->BSRR = (1<<13);    //set pin 13 is high
		for(int i = 0; i < 5000000; i++);
		GPIOC->BSRR = 1<<(13+16);
		for(int i = 0; i < 5000000; i++);
		*/
		
		//gpio_intercambiar(PUERTOC, 13);
		//for(int i = 0; i < 1000000; i++);
	}
	
}

void EXTI0_IRQ_IRQHandler(){    //administradora de la interrupcion
	
	limpiar_interrupcion_gpio(0);
}

static void imprimirMsj(char *msj, ...){
	
	char buff[80];
	#ifdef DEBUG_UART
	
	va_list args;    //lista de argumentos
	va_start(args, msj);    //extrae los argumentos del mensaje
	vsprintf(buff, msj, args);    //reformarea el mensaje y lo pone en buff
	
	for(int i = 0; i < strlen(buff); i++){
		
		USART1->DR = buff[i];   //data register (8 bits)
		while( !(USART1->SR & USART_SR_TXE) );
	}
	
	#endif
}

