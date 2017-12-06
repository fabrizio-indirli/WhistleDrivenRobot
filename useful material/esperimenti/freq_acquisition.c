
#include "initialization.h"
#include "stm32f4_discovery.h"

#define DECIMATION_FACTOR       64
#define OUT_FREQ                32000
#define PDM_Input_Buffer_SIZE   ((OUT_FREQ/1000)*DECIMATION_FACTOR/8)
#define PCM_Output_Buffer_SIZE  (OUT_FREQ/1000)

#define SPECTRUM_BG_COLOR       Black
#define SPECTRUM_FREQ_S_kHz     32.0
#define SPECTRUM_HEIGHT         150
#define SPECTRUM_NR_SAMPLES     512
#define SPECTRUM_X_LABEL        "[kHz]"

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

void main{
extern uint32_t Data_Status;

  unsigned int freq, i, z;

//l'implementazione delle seguenti 4 funzioni è nel file 'initialization.c'
RCC_Configure(); //Abilita il generatore di clock
NVIC_Configure(); //Configura l'interrupt controller (da rimuovere o modificare?)
GPIO_Configure(); //Configura il GPIO per lavorare con il microfono
I2S_Configure(); //imposta modalità I2S sulla Serial Peripheral Interface (SPI) e la configura

PDMFilter_InitStruct Filter;//crea la struttura usata dalla libreria PDM

PDM_Lib_Init(OUT_FREQ, 16000, 10, &Filter); //Inizializza libreria PDM (usa funzione in 'initialization.c')

// crea le strutture usata dalla FFT
arm_rfft_instance_f32 S;
arm_cfft_radix4_instance_f32 S_CFFT;


// e le inizializza, dove:
//  arg#3 – The number of samples in the input buffer; Possible values are: 128, 512, 2048
//  arg#4 – Direction of transformation: 0 – straight, 1 – reverse
//  arg#5 – How values are sorted in the output buffer: 0 – inverted, 1 - normal
arm_rfft_init_f32(&S, &S_CFFT, 512, 0, 1);

// Clear arrays
    for(i=0; i<256; ++i){
      buffer_input_copy[i] = 0;
      buffer_output_mag_copy[i] = 0;
    }


// Enabling data collection by the I2S system
I2S_Cmd(SPI2, ENABLE);

while(1){
  // If the button is pressed, end the spectrum display
  /*if(STM_EVAL_PBGetState(BUTTON_USER) == SET){
    while(STM_EVAL_PBGetState(BUTTON_USER) == SET);
    break;
  }*/

  


  // If the new data packet in the form of a PCM signal is ready
  if(Data_Status){
    // Copy the data of the package to the buffer
    for(i=0; i<(OUT_FREQ/1000); i++){
      buffer_input[i+(OUT_FREQ/1000)*z] = (float32_t) PCM_Output_Buffer[i];
    }

    ++z;
    if(z > 512/(OUT_FREQ/1000)){
      z = 0;



      // ************************************************************
      //PERFORMING FFT ANALYSIS
      // Determination of the Fourier transform
      arm_rfft_f32(&S, buffer_input, buffer_output);
      // Calculation of modules (magnitude)
      arm_cmplx_mag_f32(buffer_output, buffer_output_mag, 512);
      // Finding the harmonic component of the signal with the highest amplitude
      arm_max_f32(&(buffer_output_mag[1]), 512, &maxvalue, &maxvalueindex);
      // Scaling the values ​​of modules
      for(i=0; i<512; ++i){
        buffer_output_mag[i+1] = 140*buffer_output_mag[i+1]/20000000;
      }

    }
    Data_Status = 0;
  }

  // Display the frequency of the harmonic component with the highest amplitude
  sprintf(text,"F=%2.2f[kHz]   ",(maxvalueindex+1)*32.0/512);

  // Here, a loop can be inserted to wait if necessary
}

// Turn off data reception
I2S_Cmd(SPI2, DISABLE);
}
