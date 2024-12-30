#include "../../src/gpio.h"
#include "../../src/spi.h"
#include "../../src/timer.h"
#include "../../src/headphonejack.h"
#include "../../lib/dsp/sound_effects/reverb.h"

#define DC_OFFSET 2048
#define GAIN 14
#define EFFECT_SWITCH_PIN 25

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23

DSPReverb sfxReverb;

unsigned int readAudioInput()
{
  unsigned int analogValue;

  // 16 bits SPI transaction to read from the MCP3201
  spiBeginTransaction();
  analogValue = spiTransfer(0x00); // Value sent here doesn't matter since MOSI is not connected to the MCP3201
  analogValue = analogValue & 0x1F;
  analogValue = analogValue << 7;
  analogValue = analogValue | ((spiTransfer(0x00) & 0xFE) >> 1); // Value sent here doesn't matter since MOSI is not connected to the MCP3201
  spiEndTransaction();

  // Only the last 12 bits contain valid data
  analogValue = analogValue & 0x0FFF;
  return analogValue;
}

void setup()
{
  initReverb(&sfxReverb);

  // Enable / Disable the effect by reading EFFECT_SWITCH_PIN
  pinMode(EFFECT_SWITCH_PIN, INPUT);

  spiInit();
  spiSetClockDivisor(250); // SYSCLOCK = 250MHz -> SYSCLOCK/250 = 1MHz

  // Direct (non-FIFO) mode to avoid buffer underrun noise
  headphoneDirectOutputInit();
}

void loop()
{
  // Run this loop at 44.1 kHz
  unsigned int loopStartTime = micros();

  // Read the analog input value
  unsigned int audioInputValue = readAudioInput();

  // Convert to a signed value scaled to the appropriate amplitude
  int audioInputValueSigned = audioInputValue;
  audioInputValueSigned -= DC_OFFSET;
  audioInputValueSigned = GAIN * audioInputValueSigned;

  // Apply the audio effect and add some gain
  int audioProcessedSigned = (3 * processReverb(&sfxReverb, audioInputValueSigned)) / 2;

  // Convert to unsigned for audio output
  unsigned int audioProcessedUnsigned = (audioProcessedSigned + 32768);

  // Send the sample to the headphone jack output queue (and convert 12 bit to 16 bit)
  if (digitalRead(EFFECT_SWITCH_PIN) == HIGH) {
    // Reverb
    headphoneDirectOutputWrite(audioProcessedUnsigned);
  } else {
    // No reverb (passthrough)
    headphoneDirectOutputWrite(audioInputValue << 4);
  }

  // Wait if necessary to ensure that this loop doesn't run any faster than 44.1 kHz
  while ((loopStartTime - micros()) < LOOP_PERIOD_US) {}
}
