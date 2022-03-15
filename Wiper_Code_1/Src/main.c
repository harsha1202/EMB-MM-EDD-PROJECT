#include <stdio.h>

#define EXTI0_IRQn 6

#define RCC_APB2ENR_AFIOEN 0x00000001

#define SYSCFG_EXTICR1_EXTI0_PA 0x0000

#define NVIC_ISER_BASE_ADDRESS *(volatile unsigned int *)(0xE000E100)
#define NVIC_ICER_BASE_ADDRESS *(volatile unsigned int *)(0xE000E180)

#define PORTA_BASE_ADDRESS 0x40020000
#define PORTA_IDR_OFFSET 0x10
#define PORTA_IDR *(volatile unsigned int *)(PORTA_BASE_ADDRESS + PORTA_IDR_OFFSET)

#define PORTD_BASE_ADDRESS 0x40020C00
#define PORTD_ODR_OFFSET 0x14
#define PORTD_ODR *(volatile unsigned int *)(PORTD_BASE_ADDRESS + PORTD_ODR_OFFSET)

#define PORTA_MODER_OFFSET 0x00
#define PORTA_MODER *(volatile unsigned int *)(PORTA_BASE_ADDRESS + PORTA_MODER_OFFSET)

#define PORTD_MODER_OFFSET 0x00
#define PORTD_MODER *(volatile unsigned int *)(PORTD_BASE_ADDRESS + PORTD_MODER_OFFSET)

#define RCC_BASE_ADDRESS 0x40023800
#define RCC_AHB1ENR_OFFSET 0x30
#define RCC_AHB1ENR *(volatile unsigned int *)(RCC_BASE_ADDRESS + RCC_AHB1ENR_OFFSET)

#define RCC_APB2ENR_OFFSET 0x44
#define RCC_APB2ENR *(volatile unsigned int *)(RCC_BASE_ADDRESS + RCC_APB2ENR_OFFSET)

#define SYSCFG_BASE_ADDRESS 0x40013800
#define SYSCFG_EXTICR1_OFFSET 0x08
#define SYSCFG_EXTICR1 *(volatile unsigned int *)(SYSCFG_BASE_ADDRESS + SYSCFG_EXTICR1_OFFSET)

#define EXTI_BASE_ADDRESS 0x40013C00
#define EXTI_RTSR_OFFSER 0x08
#define EXTI_RTSR *(volatile unsigned int *)(EXTI_BASE_ADDRESS + EXTI_RTSR_OFFSER)

#define EXIT_IMR_OFFSET 0x00
#define EXIT_IMR *(volatile unsigned int *)(EXTI_BASE_ADDRESS + EXIT_IMR_OFFSET)

#define EXIT_PR_OFFSET 0x14
#define EXIT_PR *(volatile unsigned int *)(EXTI_BASE_ADDRESS + EXIT_PR_OFFSET)

#define EXTI_FTSR_OFFSET 0x0C
#define EXTI_FTSR *(volatile unsigned int *)(EXTI_BASE_ADDRESS + EXTI_FTSR_OFFSET)

static void Init_Function(void);
static void Intr_Function_Init(void);
static void Intr_Clean_Function(void);
static void Freq_1_Function(void);
static void Freq_2_Function(void);
static void Freq_3_Function(void);

volatile unsigned int switch_status;
volatile long flag_status=0,freq_status=0;
volatile int flag_1 = -2;

void EXTI0_IRQHandler(void)
{
	volatile long i;

	/*PORTD_ODR |= (1<<12);
	for(i=0;i<1000000;i++);
	Intr_Clean_Function();*/
	switch_status = PORTA_IDR & (1<<0);
	while(switch_status)
	{
		/*PORTD_ODR |= (1<<12);
		for(i=0;i<1000000;i++);
		Intr_Clean_Function();*/
		switch_status = PORTA_IDR & (1<<0);
		i++;

		if(i>500000)
		{
			i=0;
			PORTD_ODR ^= (1<<14);//red
			flag_status = ~flag_1;
			if (flag_status == 0)
			{
				PORTD_ODR = (0x00 << 12);
			}
			break;
		}
		else;
	}
	i=0;
	freq_status++;
	if (freq_status > 3)
	{
		freq_status = 1;
	}
	else;
	Intr_Clean_Function();
}

static void Init_Function(void)
{
	RCC_AHB1ENR |= (1<<3)|(1<<0);
	//RCC_AHB1ENR |= (1<<3);
	PORTA_MODER &= 0x00000000;
	PORTD_MODER &= 0x00000000;
	PORTA_MODER |= (0<<1)|(0<<0);
	PORTD_MODER |= (1<<24)|(1<<26)|(1<<28)|(1<<30);
	//PORTD_ODR &= ~(0x0F << 12);
	PORTD_ODR = 0x0000;
}

static void Intr_Function_Init(void)
{
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;
	SYSCFG_EXTICR1 |= SYSCFG_EXTICR1_EXTI0_PA;
	EXTI_RTSR |= (1<<0);
	//EXTI_FTSR |= (1<<0);
	EXIT_IMR |= (1<<0);
	NVIC_ISER_BASE_ADDRESS |= (1<<EXTI0_IRQn);
}

static void Intr_Clean_Function(void)
{
	EXIT_PR |= (1<<0);
}

static void Freq_1_Function(void)
{
	volatile unsigned int i;

	PORTD_ODR = (0x05 << 12);
	for(i=0;i<100000;i++);
	PORTD_ODR = (0x06 << 12);
	for(i=0;i<100000;i++);
	PORTD_ODR = (0x0C << 12);
	for(i=0;i<100000;i++);
	PORTD_ODR = (0x06 << 12);
	for(i=0;i<100000;i++);
}

static void Freq_2_Function(void)
{
	volatile unsigned int i;

	PORTD_ODR = (0x05 << 12);
	for(i=0;i<500000;i++);
	PORTD_ODR = (0x06 << 12);
	for(i=0;i<500000;i++);
	PORTD_ODR = (0x0C << 12);
	for(i=0;i<500000;i++);
	PORTD_ODR = (0x06 << 12);
	for(i=0;i<500000;i++);
}

static void Freq_3_Function(void)
{
	volatile unsigned int i;

	PORTD_ODR = (0x05 << 12);
	for(i=0;i<1000000;i++);
	PORTD_ODR = (0x06 << 12);
	for(i=0;i<1000000;i++);
	PORTD_ODR = (0x0C << 12);
	for(i=0;i<1000000;i++);
	PORTD_ODR = (0x06 << 12);
	for(i=0;i<1000000;i++);
}

int main()
{
	//volatile unsigned int switch_status;

	Init_Function();
	Intr_Function_Init();
	while(1)
	{
		/*switch_status = PORTA_IDR & (1<<0);

		if (switch_status == 1)
		{
			PORTD_ODR |= (1<<12);
		}
		else
			PORTD_ODR &= ~(1<<12);*/
			//PORTD_ODR &= ~(1<<12);


		if (flag_status == 1)
		{
			flag_1 = -1;

			if (freq_status == 1) // freq_1
			{
				Freq_1_Function();
			}
			else if (freq_status == 2) // freq_2
			{
				Freq_2_Function();
			}
			else if (freq_status == 3) // freq_3
			{
				Freq_3_Function();
			}
			else;
		}
		else;
	}
	return 0;
}
