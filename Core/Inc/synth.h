#ifndef __SYNTH_H
#define __SYNTH_H

#include <math.h>
#include <stdint.h>
#include "i2s.h"
#include "stm32f4xx_hal.h"
#include "wavetable.h"

#define POLY_MAX 2
// temp 256
#define AUDIO_BUFFER_FRAMES   128
#define AUDIO_BUFFER_CHANNELS 2
#define AUDIO_BUFFER_SAMPLES AUDIO_BUFFER_FRAMES * AUDIO_BUFFER_CHANNELS

void Synth_Init();
void Synth_Play();


#endif
