#include "initialization.h"

//CONFIGURING THE MIC PERIPHERAL
static void RCC_Configure(void){
  /********/
  /* AHB1: abilita il clock per GPIOB, GPIOC e CRC */
  /********/
  // Enabling the CRC clock signal is required by the PDM library
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_CRC, ENABLE);

  /********/
  /* APB1 */
  /********/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  // Turning on the clock signal for the I2S system
  RCC_PLLI2SCmd(ENABLE);
}

static void NVIC_Configure(void){
    //configura l'interrupt controller
  NVIC_InitTypeDef NVIC_InitStructure;

  // Configuration of the priority group
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // Configuration of interrupts from SPI2 (also for I2S2)
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

static void GPIO_Configure(void){
  GPIO_InitTypeDef GPIO_InitStructure;

  // Configuration of the PB10 line connected to the CLK of the MP45DT02 chip - the I2S2_CLK function
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configuration of the PC3 line connected to the DOUT of the MP45DT02 chip - the I2S2_DATA function
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);  // Connection of PB10 to SPI2
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);   // Connecting PC3 to SPI2
}

static void I2S_Configure(void){
  I2S_InitTypeDef I2S_InitStructure;

  SPI_I2S_DeInit(SPI2);
  I2S_InitStructure.I2S_AudioFreq = OUT_FREQ*2;
  I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_Init(SPI2, &I2S_InitStructure);

  // Enabling interrupts from filling the I2S2 receive buffer
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
}

static void PDM_Lib_Init(uint16_t sampling_freq, float HPfilter_cut_freq, float LPfilter_cutoff_freq, PDMFilter_InitStruct* Filter)
{
//INITIALIZE PDM LIBRARY


Filter->Fs = sampling_freq;       // Sampling frequency, should be OUT_FREQ
Filter->HP_HZ = HPfilter_cut_freq;          // high pass filter cut frequency, was 10 in the example
Filter->LP_HZ = LPfilter_cutoff_freq;       // low-pass filter cutoff frequency, was 16000 in the example
Filter->In_MicChannels = 1;  // number of microphones in the input stream
Filter->Out_MicChannels = 1; // number of microphones in the output stream
PDM_Filter_Init(Filter);   // Initializes the filter
}



