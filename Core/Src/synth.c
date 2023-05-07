#include "synth.h"

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t num_frames);

uint16_t audio_buffer[AUDIO_BUFFER_SAMPLES];
float sample_N;

uint16_t base = 16000;
uint16_t scaled_base = 16000 / POLY_MAX;

// temporary, single voice
Wavetable_State wavetables[POLY_MAX];

void Synth_Init() {
  for(int note = 0; note<POLY_MAX; note++) {
	  Wavetable_Init(&wavetables[note], WAVE_SINE);
	  Wavetable_NoteOn(&wavetables[note], wavetables[note].pitch_hz * (note+1));
  }
  UpdateAudioBuffer(0, AUDIO_BUFFER_FRAMES);
}

void Synth_Play() {
  HAL_I2S_Transmit_DMA(&hi2s3, audio_buffer, AUDIO_BUFFER_SAMPLES);
}

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t end_frame) {
  // start_frame is inclusive, end_frame exclusive
  static float buffer[AUDIO_BUFFER_SAMPLES];
  for(int i = 0; i<AUDIO_BUFFER_SAMPLES; i++) {buffer[i] = 0;}
  static float _buffer[AUDIO_BUFFER_SAMPLES];

  for(int note = 0; note < POLY_MAX; note++) {
	  Wavetable_GetSamples(&wavetables[note], _buffer, end_frame - start_frame);
	  for(int i = 0; i < end_frame - start_frame; i++) {
		  buffer[2*i] += _buffer[2*i];
		  buffer[2*i+1] += _buffer[2*i+1];
	  }
  }

  uint32_t i = 0;
  for (uint32_t frame = start_frame; frame < end_frame; frame++) {
	float scaled = buffer[2*i] * scaled_base;
	uint16_t value = (uint16_t)(base + scaled);

    audio_buffer[2*frame] = value;
	audio_buffer[2*frame+1] = value;
	i++;
  }

}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(0, AUDIO_BUFFER_FRAMES/2);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(AUDIO_BUFFER_FRAMES/2, AUDIO_BUFFER_FRAMES);
}
