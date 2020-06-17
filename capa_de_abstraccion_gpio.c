#include "capa_de_abstraccion_gpio.h"
#include <stdint.h>

uint32_t POSPIN[16] = {
	(0x00),
	(0x04),
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C),    //pin 8
	(0x00),    //pin 9
	(0x04),
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C),
};

static void config_pin (GPIO_TypeDef *puerto, uint32_t numeroPin, uint32_t tipo_modo){    //modo = entrada/salida, tipo_modo = pu_pd, etc.
	
	if(numeroPin >= 8){    //control high register
		
		switch(tipo_modo){
			//modos de salida y entrada
			case SALIDA_PROP_GEN | ENTRADA_ANALOG:
				puerto->CRH &= ~( (1<<CNF_BIT1) | (1<<CNF_BIT2) );
			break;
			
			case SALIDA_OD | ENTRADA_FLOTANTE:
				puerto->CRH &= ~( 1<<CNF_BIT2 );
				puerto->CRH |= ( 1<<CNF_BIT1 );
			break;
			
			case SALIDA_FUNCION_ALT | ENTRADA_PU_PD:
				puerto->CRH |= SALIDA_FUNCION_ALT<<(CNF_BIT1);
			break;
			
			case SALIDA_FUNCION_ALT_OD:
				puerto->CRH |= SALIDA_FUNCION_ALT_OD<<(CNF_BIT1);
			break;
		}    //fin switch
	}else{    //control low register
		switch(tipo_modo){
			//modos de salida y entrada
			case SALIDA_PROP_GEN | ENTRADA_ANALOG:
				puerto->CRL &= ~( (1<<CNF_BIT1) | (1<<CNF_BIT2) );
			break;
			
			case SALIDA_OD | ENTRADA_FLOTANTE:
				puerto->CRL &= ~( 1<<CNF_BIT2 );
				puerto->CRL |= ( 1<<CNF_BIT1 );
			break;
			
			case SALIDA_FUNCION_ALT | ENTRADA_PU_PD:
				puerto->CRL |= SALIDA_FUNCION_ALT<<(CNF_BIT1);
			break;
			
			case SALIDA_FUNCION_ALT_OD:
				puerto->CRL |= SALIDA_FUNCION_ALT_OD<<(CNF_BIT1);
			break;
		}    //fin switch
	}
}


static void config_vel_pin(GPIO_TypeDef *puerto, uint32_t numeroPin, uint32_t velocidad, uint32_t tipo_modo){
	
	if(numeroPin >= 8){
		if(tipo_modo == MODO_ENTRADA)    //ajustar el registro control high como entrada
			puerto->CRH &= ~( 1<<(POSPIN[numeroPin]) | 1<<(POSPIN[numeroPin] + 1) );
		else
			puerto->CRH |= (velocidad << (POSPIN[numeroPin]));    //ajustar el registro control high en modo salida a la velocidad determinada
	}else{
		if(tipo_modo == MODO_ENTRADA)
			puerto->CRL &= ~( 1<<(POSPIN[numeroPin]) | 1<<(POSPIN[numeroPin] + 1) );
		else
			puerto->CRL	|= (velocidad << (POSPIN[numeroPin])); 
	}
}


void gpio_escribir(GPIO_TypeDef *puerto, uint32_t numeroPin, uint8_t estado){
	
	if(estado){
		puerto->BSRR = (1<<numeroPin);
	}else{
		puerto->BSRR = (1<<(numeroPin + 16));
	}
}


void gpio_intercambiar(GPIO_TypeDef *puerto, uint32_t numeroPin){
	
	puerto->ODR ^= (1<<numeroPin);
}


void gpio_iniciar(TIPO_GPIO tipo_gpio){
	
	if(tipo_gpio.puerto == PUERTOA)
		GPIO_HABILITAR_RELOJ_PA;
	
	if(tipo_gpio.puerto == PUERTOB)
		GPIO_HABILITAR_RELOJ_PB;
	
	if(tipo_gpio.puerto == PUERTOC)
		GPIO_HABILITAR_RELOJ_PC;
	
	if(tipo_gpio.puerto == PUERTOD)
		GPIO_HABILITAR_RELOJ_PD;
	
	config_pin(tipo_gpio.puerto, tipo_gpio.pin, tipo_gpio.tipo_modo);
	config_vel_pin(tipo_gpio.puerto, tipo_gpio.pin, tipo_gpio.velocidad, tipo_gpio.tipo_modo);
}


void configurar_interrupcion_gpio(GPIO_TypeDef *puerto, uint32_t numeroPin, elegir_flanco flanco){
	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	if(puerto == PUERTOA){
		switch(numeroPin){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PA;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PA;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PA;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PA;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PA;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PA;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PA;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PA;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PA;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PA;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PA;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PA;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PA;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PA;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PA;
			break;
		}//fin switch
	}//fin if
	
	if(puerto == PUERTOB){
		switch(numeroPin){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PB;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PB;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PB;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PB;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PB;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PB;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PB;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PB;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PB;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PB;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PB;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PB;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PB;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PB;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PB;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PB;
			break;
		}//fin switch
	}//fin if
	
	if(puerto == PUERTOC){
		switch(numeroPin){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PC;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PC;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PC;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PC;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PC;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PC;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PC;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PC;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PC;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PC;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PC;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PC;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PC;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PC;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PC;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PC;
			break;
		}//fin switch
	}//fin if
	
	if(puerto == PUERTOD){
		switch(numeroPin){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PD;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PD;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PD;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PD;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PD;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PD;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PD;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PD;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PD;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PD;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PD;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PD;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PD;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PD;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PD;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PD;
			break;
		}//fin switch
	}//fin if
	
	if(puerto == PUERTOE){
		switch(numeroPin){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PE;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PE;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PE;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PE;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PE;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PE;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PE;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PE;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PE;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PE;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PE;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PE;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PE;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PE;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PE;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PE;
			break;
		}//fin switch
	}//fin if
	
	if(flanco == FLANCO_DE_SUBIDA)
		EXTI->RTSR |= 1<<numeroPin;
	if(flanco == FLANCO_DE_BAJADA)
		EXTI->FTSR |= 1<<numeroPin;
	if(flanco == AMBOS_FLANCOS){
		EXTI->RTSR |= 1<<numeroPin;
		EXTI->FTSR |= 1<<numeroPin;
	}
}

void activar_interrupcion_gpio(uint32_t numeroPin, IRQn_Type numeroIrq){
	
	//activar la interrupcion en EXTI
	EXTI->IMR |= 1<<numeroPin;
	//activar la interrupcion en NVIC
	NVIC_EnableIRQ(numeroIrq);
}

void limpiar_interrupcion_gpio(uint32_t numeroPin){
	
	//limpiar la interrupcion escribiendo un 1
	EXTI->PR |= 1<<numeroPin;
}

