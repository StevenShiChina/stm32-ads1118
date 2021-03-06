/*******************************************************************

*******************************************************************/
#ifndef _ADS1118_H
#define _ADS1118_H
#include "stdint.h"
#include "stm32f10x.h"

#define		DINT()		__disable_irq()
#define		EINT()		__enable_irq()

//配置寄存器( Config Register )
//Operational status/single-shot conversion start
#define     CONFIG_BIT_OS       (1<<15)
//MUX[2:0]: Input multiplexer configuration
#define     CONFIG_BIT_MUX      (7<<12)
//PGA[2:0]: Programmable gain amplifier configuration
#define     CONFIG_BIT_PGA      (7<<9)
//MODE: Device operating mode
#define     CONFIG_BIT_MODE     (1<<8)
//DR[2:0]: Data rate
#define     CONFIG_BIT_DR       (7<<5)
//TS_MODE: Temperature sensor mode
#define     CONFIG_BIT_TS_MODE  (1<<4)
//PULL_UP_EN: Pull-up enable
#define     CONFIG_BIT_PULLUP_EN    (1<<3)
//NOP: No operation
#define     CONFIG_BIT_NOP      (3<<1)
//CONFIG_BIT_RESV: default 
#define     CONFIG_BIT_RESV   (1<<0)

#define ADS1118_CONST_6_144V_LSB_mV  (0.1875)
#define ADS1118_CONST_4_096V_LSB_mV  (0.125)
#define ADS1118_CONST_2_048V_LSB_mV  (0.0625)
#define ADS1118_CONST_1_024V_LSB_mV  (0.03125)
#define ADS1118_CONST_0_512V_LSB_mV  (0.015625)
#define ADS1118_CONST_0_256V_LSB_mV  (0.0078125)

#define SAMPLECOUNTER		9//每次采集9个数进行一次中位值滤波

// ------ Public data type declarations ----------------------------
/* Private typedef -----------------------------------------------------------*/

typedef union
{
    struct
    {
        volatile unsigned char    RESV  			:1;   //low
        volatile unsigned char    NOP         :2;
        volatile unsigned char    PULLUP      :1;
        volatile unsigned char    TS_MODE     :1;
        volatile unsigned char    DR          :3;
        volatile unsigned char    MODE        :1;
        volatile unsigned char    PGA         :3;
        volatile unsigned char    MUX         :3;
        volatile unsigned char    OS          :1;   //high
    } stru;
    volatile unsigned int  word;
    volatile unsigned char byte[2];
} ADS_InitTypeDef;

typedef enum
{
    CONVERING = 0x1,          //for read
    SINGLE_CONVER_START = 0x1 //for write
} ADS_OS_TypeDef;

typedef enum
{
    AINPN_0_1 	= 	0x0,
    AINPN_0_3 	=   0x1,
    AINPN_1_3 	=   0x2,
    AINPN_2_3 	=   0x3,
    AINPN_0_GND	=  	0x4,
    AINPN_1_GND	=  	0x5,
    AINPN_2_GND	=  	0x6,
    AINPN_3_GND	=  	0x7
} ADS_MUX_TypeDef;

typedef enum
{
    PGA_6144 	= 0x0,
    PGA_4096 	= 0x1,
    PGA_2048 	= 0x2,
    PGA_1024 	= 0x3,
    PGA_512 	= 0x4,
    PGA_256 	= 0x5
} ADS_PGA_TypeDef;

typedef enum
{
    CONTIOUS  	=  0x0,
    SIGNLE_SHOT = 	0x1
} ADS_MODE_TypeDef;

typedef enum
{
    DR_8_SPS   =   0x0,
    DR_16_SPS  =   0x1,
    DR_32_SPS  =   0x2,
    DR_64_SPS  =   0x3,
    DR_128_SPS =   0x4,
    DR_250_SPS =   0x5,
    DR_475_SPS =   0x6,
    DR_860_SPS =   0x7
} ADS_DATARATE_TypeDef;

typedef enum
{
    ADC_MODE    			=   0x0,
    TEMPERATURE_MODE 	=  	0x1
} ADS_TSMODE_TypeDef;

typedef enum
{
    PULL_UP_DIS = 0x0,
    PULL_UP_EN  = 0x1
} ADS_PULL_TypeDef;

typedef enum
{
    DATA_VALID      = 0x1,
    DATA_INVALID    = 0x2
} ADS_NOP_TypeDef;

typedef enum
{
    DATA_READY 	= 0x0,
    DATA_NREADY = 0x1
} ADS_RDY_TypeDef;

typedef struct {
  double temp_C;
  double mV;
} type_k_thermo_lookup_entry_table;
#define TYPE_K_THERMO_LOOKUP_SIZE 16
#define DEFAULT_PGA		PGA_2048
#define ADS1118_DISABLE									GPIO_SetBits(SPI_MASTER_GPIO, SPI_MASTER_PIN_NSS) //CS拉高
#define ADS1118_ENABLE									GPIO_ResetBits(SPI_MASTER_GPIO, SPI_MASTER_PIN_NSS)//CS拉低
/***************SPI*******************/
#define SPI_MASTER                   SPI2
#define SPI_MASTER_CLK               RCC_APB1Periph_SPI2
#define SPI_MASTER_GPIO              GPIOB
#define SPI_MASTER_GPIO_CLK          RCC_APB2Periph_GPIOB  
#define	SPI_MASTER_PIN_NSS					 GPIO_Pin_12
#define SPI_MASTER_PIN_SCK           GPIO_Pin_13
#define SPI_MASTER_PIN_MISO          GPIO_Pin_14
#define SPI_MASTER_PIN_MOSI          GPIO_Pin_15
#define SPI_MASTER_IRQn              SPI2_IRQn
/************************************/
/******函数***************/
void ads1118_config(void);
void SPI_config(void);

float ads1118_get_temperature(void);
void ads1118_set_config_reg(ADS_InitTypeDef* ConfigReg);
uint16_t ads1118_convert(unsigned char channel);
uint16_t ads1118_get_differential_0_1(uint8_t PGA);
uint16_t ads1118_get_differential_2_3(uint8_t PGA);
double	ads1118_get_differential_0_1_mv(uint8_t PGA);
double	ads1118_get_differential_2_3_mv(uint8_t PGA);
uint8_t SPI_send_Byte(uint8_t byte);
uint16_t SPI_read_write_Reg(uint16_t CofigReg);
uint16_t SPI_read_write_Byte(uint16_t TxData);
void SPI_RCC_Configuration(void);
void SPI_GPIO_Configuration(void);
void SPI_NVIC_Configuration(void);
int8_t  ads1118_get_temp_thermo_type_k( double input_voltage_mV, double input_cold_junction_C, double *output_hot_junction_C);
uint16_t ads1118_median_filter(uint16_t *pbuffer);
double ads1118_median_average_filter(double *pbuffer);
#endif

