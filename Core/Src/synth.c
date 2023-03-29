#include "synth.h"

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t num_frames);

uint16_t float2uint16_t(float f) {
  return (uint16_t)(((int16_t)(32767*f + 32768.5)) - 32768);
}

uint16_t audio_buffer[AUDIO_BUFFER_SAMPLES];
float sample_N;

// temporary, single voice
Wavetable_State wavetable;

void Synth_Init() {
  Wavetable_Init(&wavetable, WAVE_SINE);
  UpdateAudioBuffer(0, AUDIO_BUFFER_FRAMES);
}

void Synth_Play() {
  HAL_I2S_Transmit_DMA(&hi2s3, audio_buffer, AUDIO_BUFFER_SAMPLES);
}

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t num_frames) {
  static float buffer[AUDIO_BUFFER_SAMPLES];
  Wavetable_GetSamples(&wavetable, buffer, num_frames);

  uint16_t i = 0;
  for (uint32_t frame = start_frame; frame<start_frame+num_frames; frame++) {
    audio_buffer[2*frame] = (int16_t)(buffer[2*i] * 32000);
	audio_buffer[2*frame+1] = (int16_t)(buffer[2*i+1] * 32000);
	i++;
  }

//  float d_phase = (2.0f * (float)M_PI) / AUDIO_BUFFER_FRAMES;
//  float phase = 0;
//  for (uint16_t i = 0; i < AUDIO_BUFFER_FRAMES; i++) {
//	float sample = sinf(phase);
//	int16_t sample_int = (int16_t)(sample * 32000);
//	audio_buffer[2*i] = sample_int;
//	audio_buffer[2*i+1] = sample_int;
//	phase += 8*d_phase;
//  }

//  float val;
//  float f_sample = 48000;
//  float f_out = 700;
//  float sample_dt = f_out/f_sample;
//  sample_N = f_sample/f_out;
//  for (uint16_t i=0; i<sample_N; i++) {
//	  val = sinf(i*2*(float)M_PI*sample_dt);
//	  audio_buffer[2*i] = val * 32000;
//	  audio_buffer[2*i+1] = val * 32000;
//  }
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(0, AUDIO_BUFFER_FRAMES/2);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(AUDIO_BUFFER_FRAMES/2, AUDIO_BUFFER_FRAMES/2);
}
