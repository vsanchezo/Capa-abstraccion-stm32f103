#include "stm32f10x.h"                  // Device header
#include "capa_de_abstraccion_gpio.h"

//CRH - Control Register High

int main(void){
	/*	
	//inicializa el reloj para GPIOC
	RCC->APB2ENR |= (1<<4);
	
	//Configura el pin13 de GPIOC
	GPIOC->CRH |= ( (1<<20) | (1<<21) );    //output mode a 50 MHz
	
	GPIOC->CRH &= ~( (1>>22) | (1<<23) );    //Configurado como SALIDA de propósito general
	*/
	TIPO_GPIO miGPIO;
	miGPIO.puerto = PUERTOC;
	miGPIO.pin = 13;
	miGPIO.modo = MODO_SALIDA;
	miGPIO.tipo_modo = SALIDA_PROP_GEN;
	miGPIO.velocidad = VEL_2MHZ;
	
	gpio_iniciar(miGPIO);
	
	//uso de las interrupciones
	configurar_interrupcion_gpio(PUERTOB, 0, FLANCO_DE_SUBIDA);
	
	activar_interrupcion_gpio(0, EXTI0_IRQn);
	
	while(1){
		/*
		GPIOC->BSRR = (1<<13);    //set pin 13 is high
		for(int i = 0; i < 5000000; i++);
		GPIOC->BSRR = 1<<(13+16);
		for(int i = 0; i < 5000000; i++);
		*/
		gpio_intercambiar(PUERTOC, 13);
		for(int i = 0; i < 1000000; i++);
	}
	
}

void EXTI0_IRQ_IRQHandler(){    //administradora de la interrupcion
	
	limpiar_interrupcion_gpio(0);
}

