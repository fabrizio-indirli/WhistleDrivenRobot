/**************************************************************************
 * Copyright (C) 2014 Riccardo Binetti, Guido Gerosa, Alessandro Mariani  *
 *                                                                        *
 * This program is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU General Public License as published by   *
 * the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                    *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 * Edited by Fabrizio Indirli in 2018 to:                                 *
 *  - Perform FFT analysis to recognize frequency and amplitude of sound  *
 *  - Reduce decimation factor to 16 instead of 64 to increase speed      *
 **************************************************************************/

/*
 * File:   Microphone.h
 * Author: grp51
 *
 * Created on December 29, 2013, 4:54 PM
 *
 * This class defines a simple interface for recording audio with the embedded
 * microphone on the STM32F4 Discovery board.
 *
 * It works by specifying which user-defined function should process the PCM samples
 * and how many of them. Then the driver can start recording and the defined function
 * will be called repeatedly each time the specified number of PCM samples have
 * been produced.
 *
 * Internally, it takes care of copying the microphone output in RAM via DMA and
 * transcoding such output from PDM to PCM. The latter is done via CIC filtering.
 *
 * Added on April 2018:
 * it now also performs the FFT analysis on the acquired samples, and recognizes
 * frequency and amplitude of each sample.
 * It is also possible to ignore frequencies too high by setting the LP_DIV value.
 */

#ifndef MICROPHONE_H
#define	MICROPHONE_H

#include "miosix.h"
#include <tr1/functional>
#include <cstdio>
#include <stdbool.h>
#include "miosix/kernel/scheduler/scheduler.h"
#include "util/software_i2c.h"
#include <math.h>
#include "miosix/arch/common/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h"
#include <stdio.h>
#include <iostream>
#define ARM_MATH_CM4
#include "miosix/arch/common/CMSIS/Include/arm_math.h"

#define SAMPLES 8192 //audio samples acquired each time
#define FFT_SIZE SAMPLES/2 //we store only the real parts of the samples

#define DECIMATION_FACTOR       64
#define SAMPLING_FREQ            44000

/*this is the low-pass divider: it defines which portion of the output[] vector will be taken into account
* when computing the strongest frequency of each sample.
* For example, if LP_DIV=1, all the frequencies (the entire output[] vector) will be taken into account;
* if LP_DIV = 2, only the first half of the frequencies will be taken into account
* if LP_DIV = 4, only the first quarter of the output[] vector will be taken into account
*/
#define LP_DIV 4

using namespace std::tr1;

/*
 * The Microphone class is the singleton that handles everything: the device
 * configuration, the recording and the transcoding from PDM to PCM
 */
class Microphone {
public:

    /*
     * \return the instance of the Microphone class to be used for recording.
     */
    static Microphone& instance();


    /*
     * Initialize the driver for recording and handling of the audio
     *
     * \param cback the callback function that will be executed (repeatedly) when
     * the driver produces FFT_SIZE PCM samples
     * \param freqVar the address of the variable where the frequency of the last acquired sample will be stored
     * \param amplitudeVar the address of the variable where the amplitude of the last acquired sample will be stored
     *
     */
    void init(function<void ()> cback, float32_t* freqVar, float32_t* amplitudeVar);

    /*
     * Starts the recording. When start() is called the devices configuration
     * registers are set and the DMA starts copying the microphone PDM samples in RAM.
     * The call is non-blocking, the processing from PDM to PCM and the callbacks
     * are executed in threads.
     */
    void start();

    /*
     * Wait for the last chunk of PCM samples to be processed, stop the DMA and
     * reset the configuration registers.
     */
    void stop();

private:
    Microphone(); // Microphone is a singleton, the constructor is private
    Microphone(const Microphone& orig);
    virtual ~Microphone();
    function<void ()> callback;
    // the buffers handling the double buffering "callback-side"
    unsigned short* readyBuffer;
    unsigned short* processingBuffer;
    // variables used to track  and store the transcoding progess
    unsigned int PCMsize;
    unsigned int PCMindex;

    volatile bool recording;
    pthread_t mainLoopThread;
    bool processPDM(const unsigned short *pdmbuffer, int size);
    unsigned short PDMFilter(const unsigned short* PDMBuffer, unsigned int index);
    void mainLoop();
    void execCallback();
    static void* callbackLauncher(void* arg);
    static void* mainLoopLauncher(void* arg);


};

#endif	/* MICROPHONE_H */