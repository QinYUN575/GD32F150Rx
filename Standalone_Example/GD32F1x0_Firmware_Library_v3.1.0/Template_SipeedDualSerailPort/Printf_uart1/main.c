/*!
    \file  main.c
    \brief USART printf
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0.h"
#include <stdio.h>
#include "systick.h"
#include "gd32f1x0_eval.h"
#include "main.h"
/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0;

    if(timingdelaylocal){

        if(timingdelaylocal < 500){
            gd_eval_led_on(LED2);
        }else{
            gd_eval_led_off(LED2);
        }

        timingdelaylocal--;
    }else{
        timingdelaylocal = 1000;
    }
}

#define USART1_CLOCK				RCU_USART1
#define	USART1_PORT					GPIOA
#define USART1_TX_PIN				GPIO_PIN_2
#define USART1_RX_PIN				GPIO_PIN_3

/**
  * @������       USART1_Init
  * @��  ��       ��ʼ��USART1
  * @��  ��       ��
  * @����ֵ       ��
  */
void USART1_Init(void)
{
	/* ʹ��GPIOA����ʱ�� */
	rcu_periph_clock_enable( RCU_GPIOA);
	
	/* ʹ��USART1����ʱ�� */
	rcu_periph_clock_enable(USART1_CLOCK);
	
	/* ����USART��ص�RX��TX������Ϊ���ù��� */
	gpio_af_set(USART1_PORT, GPIO_AF_1, USART1_TX_PIN);
	gpio_af_set(USART1_PORT, GPIO_AF_1, USART1_RX_PIN);
	
	gpio_mode_set(USART1_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART1_TX_PIN);
	gpio_output_options_set(USART1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, USART1_TX_PIN);
	
	gpio_mode_set(USART1_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART1_RX_PIN);
	gpio_output_options_set(USART1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, USART1_RX_PIN);
	
	/* ����USART */
	usart_deinit(USART1);	//�˿ں�
	usart_baudrate_set(USART1,115200U);	//������115200
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);	//ʹ�ܷ���
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);	//ʹ�ܽ���
	usart_enable(USART1);	//ʹ��USART1
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gd_eval_led_init(LED2);
    /* USART configuration */
    gd_eval_com_init(EVAL_COM1);
    USART1_Init();
    
    printf("-------------------------------\r\n");
    printf("Sipeed@QinYUN575\r\n");
    printf("Compile@ %s %s\r\n", __DATE__, __TIME__);
    printf("-------------------------------\r\n");

    systick_config();
    uint8_t i =0;
    while (1)
    {
        printf(".");
        i++;
        if (i >=10)
        {
            printf("\r\n");
            i = 0;
        }
        
        gd_eval_led_toggle(LED2);
        gd_eval_led_toggle(LED1);
        delay_1ms(500);
    };
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}
