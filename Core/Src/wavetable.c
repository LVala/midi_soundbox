#include "wavetable.h"

float sine_wavetable[WAVETABLE_LEN];
static uint8_t wavetables_initialized = 0;

float CHROMATIC_BASE = pow(2.0f, 1.0f / 12.0f);

static float PitchToFreq(uint8_t pitch_midi) {
  return 440*pow(CHROMATIC_BASE, (float)pitch_midi - 69);
}

static void SineWavetableInit() {
  float d_phase = (2.0f * (float)M_PI) / WAVETABLE_LEN;
  float phase = 0;
  for (uint16_t i = 0; i < WAVETABLE_LEN; i++) {
    sine_wavetable[i] = sin(phase);
	phase += d_phase;
  }
}

void Wavetable_Init(Wavetable_State *state, uint8_t wave) {
  if (!wavetables_initialized) {
    SineWavetableInit();
  }

  state->wave = wave;
  state->active = 0;
  state->phase = 0;
  state->pitch_midi = 0;
  state->pitch_hz = 0;
  state->d_phase = (state->pitch_hz/SAMPLE_RATE) * WAVETABLE_LEN;
}

void Wavetable_NoteOn(Wavetable_State *state, uint8_t pitch_midi) {
  state->active = 1;
  state->phase = 0;
  state->pitch_midi = pitch_midi;
  state->pitch_hz = PitchToFreq(pitch_midi);
  state->d_phase = (state->pitch_hz/SAMPLE_RATE) * WAVETABLE_LEN;
}

void Wavetable_NoteOff(Wavetable_State *state, uint8_t pitch_midi) {
  if (state->pitch_midi == pitch_midi) {
    state->active = 0;
	state->phase = 0;
	state->pitch_midi = 0;
	state->pitch_hz = 0;
	state->d_phase = (state->pitch_hz/SAMPLE_RATE) * WAVETABLE_LEN;
  }
}

void Wavetable_GetSamples(Wavetable_State *state, float *buffer, int num_frames) {
  float sample;
  for (uint32_t i = 0; i<num_frames; i++) {
	// TODO this if is temporary
	if (!state->active) {
	  buffer[2*i] = 0;
	  buffer[2*i+1] = 0;
	  continue;
	}
    sample = sine_wavetable[(uint32_t)state->phase];
	buffer[2*i] = sample;
	buffer[2*i+1] = sample;
	state->phase += state->d_phase;
	if (state->phase > WAVETABLE_LEN) {
      state->phase -= WAVETABLE_LEN;
	}
  }
}
