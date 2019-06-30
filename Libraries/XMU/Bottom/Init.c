#include  "common.h"
#include  "headfile.h"
#include  "isr.h"
#include  "Init.h"
#include   "TFCard.h"








//*    PIT_Init(void)
//*	@brief:		PIT��ʼ��
//*	@param:		void
//*	@return:	void 
//* @note:      void
//*
void PIT_Init(void)
{
    pit_init_ms(pit0,2);                                //��ʼ��PIT0,��ʱ2ms  
    enable_irq(PIT0_IRQn);                              // ʹ��PIT�ж�   
} 






//*    communication_Init(void)
//*	@brief:		������ʼ��
//*	@param:		void
//*	@return:	void 
//* @note:      void
//*
void communication_Init(void)
{
    uart_init(uart4,115200);
    uart_rx_irq_en (uart4);                                  //�����ڽ����ж�
}



//*    Encoder_Init(void)
//*	@brief:		��������ʼ��
//*	@param:		void
//*	@return:	void 
//* @note:      void
//*
void encoder_Init(void)
{								//ʹ��
    //  ��������  FTM1  A8��A9
    ftm_quad_init(ftm1);   
    ftm_quad_init(ftm2);   
}

//=================================================================//
//  @brief:	���PWM��ʼ��
//  @param: 	
//  @return: 
//  @note: 
//=================================================================//
void DriveInit()      

{
   ftm_pwm_init(ftm0, ftm_ch4, 16000,0);
   ftm_pwm_init(ftm0, ftm_ch5, 16000,0);
   ftm_pwm_init(ftm0, ftm_ch6, 16000,0);
   ftm_pwm_init(ftm0, ftm_ch7, 16000,0);
}




//=================================================================//
//  @brief:	���뿪�عܽų�ʼ��
//  @param: 	
//  @return: 
//  @note: 
//=================================================================//
void DialSwitchInit()
{
    //�ܽų�ʼ��
    gpio_init(DIALSWITCH_PIN1, GPI, 1);   
    gpio_init(DIALSWITCH_PIN2, GPI, 1);
    gpio_init(DIALSWITCH_PIN3, GPI, 1);
    gpio_init(DIALSWITCH_PIN4, GPI, 1);    
    port_init_NoAlt(DIALSWITCH_PIN1,PULLUP);    
    port_init_NoAlt(DIALSWITCH_PIN2,PULLUP);
    port_init_NoAlt(DIALSWITCH_PIN3,PULLUP);    
    port_init_NoAlt(DIALSWITCH_PIN4,PULLUP);


}

//*    System_Init(void)
//*	@brief:		��ʼ��
//*	@param:		void
//*	@return:	void 
//* @note:      void
//*
uint8 Mpu_Error = 1;
uint16 dat;


void system_Init(void)
{
#if i2c
    IIC_init();
    dat = simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_WHO_AM_I,IIC);
    icm20602_init();
    
#else
    Mpu_Error = icm20602_Init();
#endif 
//        sensor.gyr_calibrate = 1;

     communication_Init();
//     OLED_Init();
     DriveInit();  
     encoder_Init();
     lcd_init();
    camera_init();
    gpio_init(C8,GPI,0);
     adc_init(ADC1_SE8);
     adc_init(ADC1_SE9);
     adc_init(ADC0_SE12);
     adc_init(ADC0_SE13);
     adc_init(ADC1_SE10);
     adc_init(ADC1_SE17);
     adc_init(ADC0_SE18);
     
//     adc_init(ADC1_SE12);               //�����ʼ��
//     //sd_init();
     SD_BMP_Init();
////     crcInit();
     DialSwitchInit();
     	INTR_Keyboard_Init();		//���̳�ʼ��
     BluetoothInt();
     PIT_Init();    
    /**********�����ж����ȼ�**********/
    NVIC_SetPriorityGrouping(3);   
    NVIC_SetPriority(PORTC_IRQn,0);            //���ж�
    NVIC_SetPriority(PIT0_IRQn,1);            //PIT
    NVIC_SetPriority(DMA0_IRQn,2);            //DMA0
    NVIC_SetPriority(PORTA_IRQn,3);
}