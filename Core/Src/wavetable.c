#include "wavetable.h"

float sine_wavetable[WAVETABLE_LEN];
static uint8_t wavetables_initialized = 0;

static void sineWavetableInit() {
  float d_phase = (2.0f * (float)M_PI) / WAVETABLE_LEN;
  float phase = 0;
  for (uint16_t i = 0; i < WAVETABLE_LEN; i++) {
    sine_wavetable[i] = sin(phase);
	phase += d_phase;
  }
}

void Wavetable_Init(Wavetable_State *state, uint8_t wave) {
  if (!wavetables_initialized) {
    sineWavetableInit();
  }

  state->wave = wave;
  // state->active = 1;
  state->phase = 0;
  state->pitch_hz = 93.75;
  state->d_phase = (state->pitch_hz/SAMPLE_RATE) * WAVETABLE_LEN;
}

void Wavetable_NoteOn(Wavetable_State *state, float pitch_hz) {
	// state->active = 1;
	state->phase = 0;
	state->pitch_hz = pitch_hz;
	state->d_phase = (state->pitch_hz/SAMPLE_RATE) * WAVETABLE_LEN;
}

void Wavetable_NoteOff(Wavetable_State *state) {
	// state->active = 0;
	state->phase = 0;
	state->pitch_hz = 0;
	state->d_phase = (state->pitch_hz/SAMPLE_RATE) * WAVETABLE_LEN;
}

void Wavetable_GetSamples(Wavetable_State *state, float *buffer, int num_frames) {
  float sample;
  for (uint32_t i = 0; i<num_frames; i++) {
    sample = sine_wavetable[(uint32_t)state->phase];
	buffer[2*i] = sample;
	buffer[2*i+1] = sample;
	state->phase += state->d_phase;
	if (state->phase > WAVETABLE_LEN) {
      state->phase -= WAVETABLE_LEN;
	}
  }
}
