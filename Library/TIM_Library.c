#include "TIM_Library.h"
#include "stm32f429xx.h"

#include <stdbool.h>
#include <stddef.h>


//	/************** STEPS TO FOLLOW *****************
//	1. Enable Timer clock
//	2. Set the prescalar and the ARR
//	3. Enable the Timer, and wait for the update Flag to set
//	************************************************/
void TIM4Config(void){
	// Output PWM: GPIO_PD12 **************************************************
	// Here we are using Timer so we need to init clock for timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN ;//| RCC_AHB1ENR_GPIODEN; 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	
	// Setoutput port GPIODA as AF
	GPIOD->MODER |= (1 << (12 * 2 + 1)); // 2 bit for each pin
	GPIOD->MODER &= ~(1 << (12 * 2)); // 2 bit for each pin
	
	GPIOD->OTYPER |= (1 << 12); // Pushpull
	GPIOD->OSPEEDR |= (GPIO_OSPEED_VERY_HIGH << (12 * 2)); // 2 bit for each pin
	
	GPIOD->AFR[1] |= GPIO_AFRH_AFSEL12_1;
	GPIOD->AFR[1] &= ~(GPIO_AFRH_AFSEL12_0 | GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL12_3);

	// Enable Timer 4  GPIO_PD12 **********************************************
	// Disabled update for configuration
	TIM4->CR1 |= TIM_CR1_UDIS;
	// Setup Timer
	TIM4->CCER |= TIM_CCER_CC1E; // Enable channel 1
	
	// CR1: control register: modify timer behaviour, routine, 
	// such as re/ preload, direction, enable...
	TIM4->CR1 |= TIM_CR1_ARPE; // Auto reload preload enable
	
	// CR2: advance. unknown
	
	// Setup mode: compare capture -> PWM and output compare
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE; 
	
	// Timer paramter
	TIM4->PSC = 89;
	//TIM4->CR1 &= ~TIM_CR1_UDIS; // Enable update
	TIM4->ARR = 1000;
	TIM4->CCR1 = 200; // Duty: counter compared register

	// Start Timer
	TIM4->EGR |= TIM_EGR_UG; // Update generation: reinit counter
	TIM4->EGR |= TIM_EGR_CC1G; // 
	
	TIM4->CR1 |= TIM_CR1_CEN; // Start Timer
	//while(!(TIM4->SR & (1 << 0)));
}


void TIM6Config(void){
	// 1. Enable Timer Clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	
	// 2. Set prescaler and ARR
	TIM6->PSC = 89; 
	TIM6->ARR = 0xffff;
	
	// 3. Enable the timer and wait for flag to be set
	TIM6->CR1 |= 1 << 0 ; //TIM_CR1_CEN;
	while(!(TIM6->SR & (1 << 0)));
}


void delay_us(uint16_t us){
	// Reset the counter
	TIM6->CNT = 0;
	while(TIM6->CNT < us);
}

void delay_ms(uint16_t ms){
	for(int i = 0; i < ms; i++){
		delay_us(1000);
	}
}






