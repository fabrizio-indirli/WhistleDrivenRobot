/**
  * Audio Spectrum Analyzer
  * Author: Jan Szemiet
  */ 

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include "arm_math.h"               // Required to use float32_t type
#include "main.h"
#include "pdm_filter.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t  PDM_Input_Buffer[PDM_Input_Buffer_SIZE];
uint16_t PCM_Output_Buffer[PCM_Output_Buffer_SIZE];

float32_t buffer_input[1024];
float32_t buffer_input_copy[256];
float32_t buffer_output[1024];
float32_t buffer_output_mag[1024];
float32_t buffer_output_mag_copy[256];
float32_t maxvalue;
uint32_t  maxvalueindex;
uint8_t   mode;

uint16_t  g_SPECTRUM_Xpos;
uint8_t   g_SPECTRUM_Ypos;
char      text[100];

arm_rfft_instance_f32 S;
arm_cfft_radix4_instance_f32 S_CFFT;
PDMFilter_InitStruct Filter;

/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configure(void);
static void I2S_Configure(void);
static void NVIC_Configure(void);
static void RCC_Configure(void);

/* Function prototypes -------------------------------------------------------*/
void DrawSpectrum_Prepare(uint16_t xpos, uint8_t ypos);
void DrawSpectrum_Update(uint16_t color);

/* Functions -----------------------------------------------------------------*/
int main(void)
{
  extern uint32_t Data_Status;

  unsigned int freq, i, z;
  float dt;

  RCC_Configure();
  NVIC_Configure();
  GPIO_Configure();
  I2S_Configure();

  // Initialize PDM filter
  Filter.Fs = OUT_FREQ;
  Filter.HP_HZ = 10;
  Filter.LP_HZ = 16000;
  Filter.In_MicChannels = 1;
  Filter.Out_MicChannels = 1;
  PDM_Filter_Init(&Filter);

  arm_rfft_init_f32(&S, &S_CFFT, 512, 0, 1);

  // Initialize User Button
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

  STM32f4_Discovery_LCD_Init();
  LCD_Clear(Black);

  LCD_SetBackgroundColor(Black);

  LCD_SetFont(&Font12x12);
  LCD_SetTextColor(Green);
  x_LCD_DrawText(160, 30, "Audio Spectrum Analyzer", CenterJustify, NormalOrientation);
  x_LCD_DrawText(160, 50, "on STM32F407VG", CenterJustify, NormalOrientation);
  LCD_SetFont(&Font8x8);
  LCD_SetTextColor(Green);
  x_LCD_DrawText(160, 70, "- DSP/FFT Library", CenterJustify, NormalOrientation);
  x_LCD_DrawText(160, 90, "- PDM Library", CenterJustify, NormalOrientation);

  LCD_SetFont(&Font8x8);
  LCD_SetTextColor(Yellow);
  x_LCD_DrawText(160, 150, "Press button to continue...", CenterJustify, NormalOrientation);
  while(STM_EVAL_PBGetState(BUTTON_USER) == RESET);
  while(STM_EVAL_PBGetState(BUTTON_USER) == SET);

  while(1){
    LCD_SetForegroundColor(White);
    LCD_SetTextColor(Grey);

    LCD_Clear(Black);

    x_LCD_DrawText(32, 5, "Demo - one sine wave", LeftJustify, NormalOrientation);
    x_LCD_DrawRectangle(31,19,258,63);
    x_LCD_DrawFilledRectangle(32, 20, 256, 61, Black);
    x_LCD_DrawRectangle(31,129,258,103);
    x_LCD_DrawFilledRectangle(32, 130, 256, 101, Black);

    // Clear arrays
    for(i=0; i<256; ++i){
      buffer_input_copy[i] = 0;
      buffer_output_mag_copy[i] = 0;
    }

    dt = 0.001;
    freq = 0;
    mode = 1;

    while(mode == 1 || mode == 2 || mode == 3 || mode == 4){
      ++freq;
      if(freq > 512*2)
        freq = 1;

      if(STM_EVAL_PBGetState(BUTTON_USER) == SET){
        while(STM_EVAL_PBGetState(BUTTON_USER) == SET);

        ++mode;

        if(mode == 2)
          x_LCD_DrawText(32, 5, "Demo - two sine waves", LeftJustify, NormalOrientation);
        else if(mode == 3)
          x_LCD_DrawText(32, 5, "Demo - three sine waves", LeftJustify, NormalOrientation);
        else if(mode == 4)
          x_LCD_DrawText(32, 5, "Demo - four sine waves", LeftJustify, NormalOrientation);

        if(mode > 4)
          break;

        freq = 1;
      }

      // ************************************************************
      // Generate signal
      if(mode == 1){
        for(i=0; i<512; ++i){
          buffer_input[i] = (float32_t) 30*sin(2*PI*freq*i*dt);
        }
      }
      else if(mode == 2){
        for(i=0; i<512; ++i){
          buffer_input[i] = (float32_t) 20*sin(2*PI*freq*i*dt) + 10*sin(2*PI*2*freq*i*dt);
        }
      }
      else if(mode == 3){
        for(i=0; i<512; ++i){
          buffer_input[i] = (float32_t) 15*sin(2*PI*freq*i*dt) + 10*sin(2*PI*2*freq*i*dt) + 5*sin(2*PI*3*freq*i*dt);
        }
      }
      else if(mode == 4){
        for(i=0; i<512; ++i){
          buffer_input[i] = (float32_t) 15*sin(2*PI*freq*i*dt) + 10*sin(2*PI*2*freq*i*dt) + 3*sin(2*PI*3*freq*i*dt) + 2*sin(2*PI*4*freq*i*dt);
        }
      }

      // ************************************************************
      // Hide old signal
      LCD_SetForegroundColor(Black);
      for(i=0; i<255; ++i){
        x_LCD_DrawLine_2(i + 32, (uint16_t)(buffer_input_copy[i] + 50),
                         i + 33, (uint16_t)(buffer_input_copy[i+1] + 50));
      }
      // Show new signal
      LCD_SetForegroundColor(Yellow);
      for(i=0; i<255; ++i){
        x_LCD_DrawLine_2(i + 32, (uint16_t)(buffer_input[i] + 50),
                         i + 33, (uint16_t)(buffer_input[i+1] + 50));
      }
      // Show frequencies of signal
      if(mode == 1){
        sprintf(text,"F=%4d [1]",freq);
        x_LCD_DrawText(32, 100, text, LeftJustify, NormalOrientation);
      }
      else if(mode == 2){
        sprintf(text,"F1=%4d [1], F2=%4d [1]",freq,freq*2);
        x_LCD_DrawText(32, 100, text, LeftJustify, NormalOrientation);
      }
      else if(mode == 3){
        sprintf(text,"F1=%4d [1], F2=%4d [1]",freq,freq*2);
        x_LCD_DrawText(32, 100, text, LeftJustify, NormalOrientation);
        sprintf(text,"F3=%4d [1]",freq*3);
        x_LCD_DrawText(32, 110, text, LeftJustify, NormalOrientation);
      }
      else if(mode == 4){
        sprintf(text,"F1=%4d [1], F2=%4d [1]",freq,freq*2);
        x_LCD_DrawText(32, 100, text, LeftJustify, NormalOrientation);
        sprintf(text,"F3=%4d [1], F4=%4d [1]",freq*3,freq*4);
        x_LCD_DrawText(32, 110, text, LeftJustify, NormalOrientation);
      }

      // ************************************************************
      // Update mirror buffers
      for(i=0; i<256; ++i){
        buffer_input_copy[i] = buffer_input[i];
        buffer_output_mag_copy[i] = buffer_output_mag[i];
      }

      // ************************************************************
      // Calculate Real FFT
      arm_rfft_f32(&S, buffer_input, buffer_output);
      // Calculate magnitude
      arm_cmplx_mag_f32(buffer_output, buffer_output_mag, 512);
      // Get maximum value of magnitude
      arm_max_f32(buffer_output_mag, 512, &maxvalue, &maxvalueindex);
      // Scale magnitude values
      for(i=0; i<512; ++i){
        buffer_output_mag[i] = 100*buffer_output_mag[i]/maxvalue;
      }

      // ************************************************************
      // Hide old FFT
      LCD_SetForegroundColor(Black);
      for(i=0; i<256; ++i){
        x_LCD_DrawLine_2(i + 32, 130 + 100,
                         i + 32, 130 + (uint16_t)(100 - (uint16_t)buffer_output_mag_copy[i+1]));
      }
      // Show new FFT
      LCD_SetForegroundColor(Green);
      for(i=0; i<256; ++i){
        x_LCD_DrawLine_2(i + 32, 130 + 100,
                         i + 32, 130 + (uint16_t)(100 - (uint16_t)buffer_output_mag[i+1]));
      }

      // ************************************************************
      // Wait some time
      for(i=0; i<0x20000; ++i);
    }

    LCD_SetForegroundColor(White);
    LCD_SetTextColor(Grey);

    LCD_Clear(Black);

    x_LCD_DrawText(100, 5, "Audio Spectrum", LeftJustify, NormalOrientation);

    // Clear arrays
    for(i=0; i<256; ++i){
      buffer_input_copy[i] = 0;
      buffer_output_mag_copy[i] = 0;
    }

    z = 0;

    DrawSpectrum_Prepare(21,20);

    I2S_Cmd(SPI2, ENABLE);

    while(1){
      if(STM_EVAL_PBGetState(BUTTON_USER) == SET){
        while(STM_EVAL_PBGetState(BUTTON_USER) == SET);
        break;
      }

      if(Data_Status){
        for(i=0; i<(OUT_FREQ/1000); i++){
          buffer_input[i+(OUT_FREQ/1000)*z] = (float32_t) PCM_Output_Buffer[i];
        }

        ++z;
        if(z > 512/(OUT_FREQ/1000)){
          z = 0;

          // ************************************************************
          // Hide old FFT
          DrawSpectrum_Update(Black);

          // ************************************************************
          // Calculate Real FFT
          arm_rfft_f32(&S, buffer_input, buffer_output);
          // Calculate magnitude
          arm_cmplx_mag_f32(buffer_output, buffer_output_mag, 512);
          // Get maximum value of magnitude
          arm_max_f32(&(buffer_output_mag[1]), 512, &maxvalue, &maxvalueindex);
          // Scale magnitude values
          for(i=0; i<512; ++i){
            //buffer_output_mag[i] = 100*buffer_output_mag[i]/maxvalue;
            buffer_output_mag[i+1] = 140*buffer_output_mag[i+1]/20000000;
          }

          // ************************************************************
          // Show new FFT
          DrawSpectrum_Update(Green);
        }
        Data_Status = 0;
      }

      sprintf(text,"F=%2.2f[kHz]   ",(maxvalueindex+1)*32.0/512);
      x_LCD_DrawText(100, 15, text, LeftJustify, NormalOrientation);

      // Wait some time
      for(i=0; i<0x10000; ++i);
    }

    I2S_Cmd(SPI2, DISABLE);
  }
}

void DrawSpectrum_Prepare(uint16_t xpos, uint8_t ypos){
  int i;

  g_SPECTRUM_Xpos = xpos;
  g_SPECTRUM_Ypos = ypos;

  // Draw spectrum plot border
  LCD_SetForegroundColor(White);
  x_LCD_DrawRectangle(xpos + 10, ypos + 10, SPECTRUM_NR_SAMPLES/2 + 2, SPECTRUM_HEIGHT+2);

  // Fill spectrum plot background
  x_LCD_DrawFilledRectangle(xpos + 11, ypos + 11, SPECTRUM_NR_SAMPLES/2, SPECTRUM_HEIGHT, SPECTRUM_BG_COLOR);

  // Draw horizontal divisions
  LCD_SetFont(&Font8x8);
  LCD_SetTextColor(Grey);
  for(i=0; i<=16; ++i){
    sprintf(text,"%.2f",i*16*SPECTRUM_FREQ_S_kHz/SPECTRUM_NR_SAMPLES);

    x_LCD_DrawLine(xpos + 10 + i*16, ypos + 10 + SPECTRUM_HEIGHT+2, 5, Vertical);
    x_LCD_DrawText(xpos + 10 + i*16 + 4, ypos + 10 + SPECTRUM_HEIGHT+2 + 10, text, LeftJustify, RightOrientation);
  }
  x_LCD_DrawText(xpos + 10 + i*16 + 4, ypos + 10 + SPECTRUM_HEIGHT+2 + 10, SPECTRUM_X_LABEL, LeftJustify, RightOrientation);

  // Draw vertical divisions
  for(i=0; i<=6; ++i){
    x_LCD_DrawLine(xpos + 10 - 5, ypos + 10 + SPECTRUM_HEIGHT+2 - 1 - i*25, 5, Horizontal);
  }
}

void DrawSpectrum_Update(uint16_t color){
  int i;

  LCD_SetForegroundColor(color);
  // Show niddles
  for(i=0; i<256; ++i){
      x_LCD_DrawLine_2(g_SPECTRUM_Xpos + 11 + i, g_SPECTRUM_Ypos + 160,
                       g_SPECTRUM_Xpos + 11 + i, g_SPECTRUM_Ypos + (uint16_t)(160 - (uint16_t)buffer_output_mag[i+1]));
  }
}

/* Private functions ---------------------------------------------------------*/
static void GPIO_Configure(void){
  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure MP45DT02's CLK / I2S2_CLK (PB10) line
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure MP45DT02's DOUT / I2S2_DATA (PC3) line
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);  // Connect pin 10 of port B to the SPI2 peripheral
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);   // Connect pin 3 of port C to the SPI2 peripheral
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

  // Enable the Rx buffer not empty interrupt
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
}

static void NVIC_Configure(void){
  NVIC_InitTypeDef NVIC_InitStructure;

  // Configure the interrupt priority grouping
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // Configure the SPI2 interrupt channel
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

static void RCC_Configure(void){
  // Enable CRC module - required by PDM Library
  //RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

  /********/
  /* AHB1 */
  /********/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC |
                         RCC_AHB1Periph_CRC, ENABLE);

  /********/
  /* APB1 */
  /********/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  RCC_PLLI2SCmd(ENABLE);
}
