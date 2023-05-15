#include "usb_host.h"
#include "main.h"
#include "../../Drivers/USBH_midi_class/Inc/usbh_MIDI.h"

#define RX_BUFF_SIZE 64

extern USBH_HandleTypeDef hUsbHostFS;
uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer


void start_midi(void)
{
  USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE);
}

// ======================================================================
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost)
{
  // each USB midi package is 4 bytes long
  uint16_t numberOfPackets = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostFS) / 4;

  for(uint16_t i = 0; i < numberOfPackets; ++i) {
    uint8_t cin_cable   = MIDI_RX_Buffer[4*i+0];
    uint8_t midi_cmd    = MIDI_RX_Buffer[4*i+1];
    uint8_t midi_param0 = MIDI_RX_Buffer[4*i+2];
    uint8_t midi_param1 = MIDI_RX_Buffer[4*i+3];
    if(cin_cable == 0) {
      continue;
    }
    HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);

  }
  // start a new reception
  USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE);
}
