#include "synth.h"

static void UpdateAudioBuffer(uint32_t start_frame, uint32_t num_frames);

uint16_t audio_buffer[AUDIO_BUFFER_SAMPLES];
float sample_N;

// temporary, single voice
Wavetable_State wavetables[POLY_MAX];

void Synth_Init() {
  for(int note = 0; note<POLY_MAX; note++) {
	  Wavetable_Init(&wavetables[note], WAVE_SINE);
	  Wavetable_NoteOn(&wavetables[note], wavetables[note].pitch_hz + note*20);
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
//	int16_t v = (int16_t)32767 + (buffer[2*i] * 32000 / POLY_MAX);
	uint16_t v = (int16_t)(32767 + (buffer[2*i] * 32000 / POLY_MAX)) / 10;

    audio_buffer[2*frame] = v;
	audio_buffer[2*frame+1] = v;
	i++;
  }

}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(0, AUDIO_BUFFER_FRAMES/2);
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
  UpdateAudioBuffer(AUDIO_BUFFER_FRAMES/2, AUDIO_BUFFER_FRAMES);
}
