#include "synth.h"

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t num_frames);

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

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t end_frame) {
  // start_frame is inclusive, end_frame exclusive
  static float buffer[AUDIO_BUFFER_SAMPLES];
  Wavetable_GetSamples(&wavetable, buffer, end_frame - start_frame);

  uint32_t i = 0;
  for (uint32_t frame = start_frame; frame < end_frame; frame++) {
    audio_buffer[2*frame] = (int16_t)(buffer[2*i] * 32000);
	audio_buffer[2*frame+1] = (int16_t)(buffer[2*i+1] * 32000);
	i++;
  }

//  float d_phase = (2.0f * (float)M_PI) / AUDIO_BUFFER_FRAMES;
//  float phase = 0;
//  for (uint16_t i = 0; i < 2*AUDIO_BUFFER_FRAMES; i++) {
//	float sample = sinf(phase);
//	int16_t sample_int = (int16_t)(sample * 32000);
//	audio_buffer[2*i] = sample_int;
//	audio_buffer[2*i+1] = sample_int;
//	phase += 0.5*d_phase;
//  }
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(0, AUDIO_BUFFER_FRAMES/2);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(AUDIO_BUFFER_FRAMES/2, AUDIO_BUFFER_FRAMES);
}
