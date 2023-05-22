#ifndef __WAVETABLE_H
#define __WAVETABLE_H


#include <math.h>
#include <stdint.h>
#include <adsr.h>

#define SAMPLE_RATE 48000
#define WAVETABLE_LEN 4048
#define WAVE_SINE 0
#define WAVE_SAW 1

typedef struct {
	uint8_t wave;
	uint8_t active;
	float phase;
	float d_phase;
	float pitch_hz;

	adsr_state_t adsr_state;
} Wavetable_State;

void Wavetable_Init(Wavetable_State *state, uint8_t wave);
void Wavetable_NoteOn(Wavetable_State *state, float pitch_hz);
void Wavetable_NoteOff(Wavetable_State *state);
void Wavetable_GetSamples(Wavetable_State *state, float *buffer, int num_frames);
void Wavetable_SetActive(Wavetable_State *state, int active);

#endif
