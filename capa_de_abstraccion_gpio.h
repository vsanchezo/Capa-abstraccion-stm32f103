#ifndef capa_de_abstraccion_gpio
#define capa_de_abstraccion_gpio

#include "stm32f10x.h"                  // Device header

#define APG 0
#define ENC 1

//definir el nombre de los puertos
#define PUERTOA    GPIOA
#define PUERTOB    GPIOB
#define PUERTOC    GPIOC
#define PUERTOD    GPIOD
#define PUERTOE    GPIOE
#define PUERTOF    GPIOF
#define PUERTOG    GPIOG

//Modo de los pins
#define MODO_SALIDA ((uint32_t) 0x01)    //los valores (0x01) no significan nada... solo deben ser diferentes
#define MODO_ENTRADA ((uint32_t) 0x02)

//Tipos de entrada
#define ENTRADA_ANALOG    ((uint32_t) 0x00)
#define ENTRADA_FLOTANTE    ((uint32_t) 0x01)    //predeterminada al reset
#define ENTRADA_PU_PD    ((uint32_t) 0x02)    //entrada con pull up o pull down

//Tipos de salida
#define SALIDA_PROP_GEN    ((uint32_t) 0x00)    //salida de proposito general
#define SALIDA_OD    ((uint32_t) 0x01)    //salida open drain
#define SALIDA_FUNCION_ALT    ((uint32_t) 0x02)    //función alterna
#define SALIDA_FUNCION_ALT_OD     ((uint32_t) 0x03)    

//velocidades de los pins
#define VEL_2MHZ    ((uint32_t) 0x02)
#define VEL_10MHZ    ((uint32_t) 0x01)
#define VEL_50MHZ    ((uint32_t) 0x03)

//habilitar reloj
#define GPIO_HABILITAR_RELOJ_ALT    (RCC->APB2ENR |= (1<<0))
#define GPIO_HABILITAR_RELOJ_PA    (RCC->APB2ENR |= (1<<2))
#define GPIO_HABILITAR_RELOJ_PB    (RCC->APB2ENR |= (1<<3))
#define GPIO_HABILITAR_RELOJ_PC    (RCC->APB2ENR |= (1<<4))
#define GPIO_HABILITAR_RELOJ_PD    (RCC->APB2ENR |= (1<<5))
//#define GPIO_HABILITAR_RELOJ_PE    (RCC->APB2ENR |= (1<<6))

//Bits de CNF (config)
#define CNF_BIT1 (POSPIN[numeroPin] + 2)
#define CNF_BIT2 (POSPIN[numeroPin] + 3)

//Estructura de configuración
typedef struct{
	GPIO_TypeDef *puerto;
	
	uint32_t pin;
	uint32_t modo;
	uint32_t tipo_modo;
	uint32_t pull;
	uint32_t velocidad;
	uint32_t func_alt;
} TIPO_GPIO;

typedef enum{
	FLANCO_DE_SUBIDA,
	FLANCO_DE_BAJADA,
	AMBOS_FLANCOS
}elegir_flanco;

//prototipos de funciones
//********************************************************
//configuración gpio
static void config_pin (GPIO_TypeDef *puerto, uint32_t numeroPin, uint32_t modo);

static void config_vel_pin(GPIO_TypeDef *puerto, uint32_t numeroPin, uint32_t velocidad, uint32_t modo);

//gpio funciones de pin para el usuario
void gpio_escribir(GPIO_TypeDef *puerto, uint32_t numeroPin, uint8_t estado);

void gpio_intercambiar(GPIO_TypeDef *puerto, uint32_t numeroPin);

void gpio_iniciar(TIPO_GPIO tipo_gpio);

//funciones de las interrupciones
void configurar_interrupcion_gpio(GPIO_TypeDef *puerto, uint32_t numeroPin, elegir_flanco flanco);

void activar_interrupcion_gpio(uint32_t numeroPin, IRQn_Type numeroIrq);

void limpiar_interrupcion_gpio(uint32_t numeroPin);


#endif

