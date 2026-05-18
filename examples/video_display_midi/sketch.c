#include <stddef.h>
#include "../../src/uart.h"
#include "../../src/timer.h"
#include "../../src/video.h"
#include "../../src/graphics_canvas.h"
#include "../../lib/midi/midi.h"

#include "../../src/video_1024x768.h"

#define MIDI_PARSER_TIMEOUT_MS 250
#define PRINT_KEY_X_POSITION 0
#define PRINT_VALUE_X_POSITION 450
#define FONT_SIZE 3

const char* hexChars = "0123456789abcdef";

videoOutput display;
graphicsCanvas canvas;

MIDIParser midiParser;

unsigned int millisecond_tick;

void stringPrintUnsignedDec_8bit(char* str, unsigned char val) {
  str[0] = hexChars[(val / 100) % 10];
  str[1] = hexChars[(val / 10) % 10];
  str[2] = hexChars[(val / 1) % 10];
  str[3] = 0;
}

void stringPrintUnsignedDec_16bit(char* str, unsigned int val) {
  str[0] = hexChars[(val / 10000) % 10];
  str[1] = hexChars[(val / 1000) % 10];
  str[2] = hexChars[(val / 100) % 10];
  str[3] = hexChars[(val / 10) % 10];
  str[4] = hexChars[(val / 1) % 10];
  str[5] = 0;
}

void setup()
{

  // Initialize the UART
  uartInit(31250);

  // Initialize the MIDI Parser
  initMidiParser(&midiParser, MIDI_PARSER_TIMEOUT_MS);

  // Initialize the video output
  if (videoOutputInit(&display) != 0) {
    // Block indefinitely if video output initialization fails
    while(1) {}
  }

  // Clear the display
  clearDisplay(&display);

  // Get a graphicsCanvas to draw on
  canvas = getGraphicsCanvas(&display);

  // Create a font for printing to the graphicsCanvas
  fontConfig font = graphicsCanvasDefaultFontConfig();
  font.size = FONT_SIZE;
  graphicsCanvasDrawString(canvas, 0, 0, font, "Waiting for MIDI message...");

  millisecond_tick = micros();
}

void loop()
{
  if (uartAvailable()) {
    feedMidiParser(&midiParser, uartRead());
    if (midiParser.message_ready) {
      // Black rectangle to erase any pervious text
      graphicsCanvasFillRectangle(canvas, 0, 0, 1024,  4 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, 0, 0, 0);

      // Print the decoded information about the received MIDI event
      fontConfig font = graphicsCanvasDefaultFontConfig();
      font.size = FONT_SIZE;

      char value_string_repr[6];

      graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 0 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Channel:");
      stringPrintUnsignedDec_8bit(value_string_repr, midiParser.channel + 1); // MIDI channels start at 1, not 0
      graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 0 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);
      
      graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Event type:");

      if (midiParser.message_type == MIDI_MESSAGE_TYPE_NOTE_OFF) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "NOTE OFF");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Note:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_note_off.note);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Velocity:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_note_off.velocity);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_NOTE_ON) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "NOTE ON");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Note:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_note_on.note);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Velocity:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_note_on.velocity);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_POLYPHONIC_AFTERTOUCH) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "POLYPHONIC AFTERTOUCH");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Note:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_polyphonic_aftertouch.note);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Pressure:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_polyphonic_aftertouch.pressure);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_CONTROL_CHANGE) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "CONTROL CHANGE");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Controller Number:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_control_change.controller_number);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Controller Value:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_control_change.controller_value);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 3 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_PROGRAM_CHANGE) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "PROGRAM CHANGE");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Program Number:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_program_change.program_number);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_CHANNEL_AFTERTOUCH) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "CHANNEL AFTERTOUCH");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Pressure:");
        stringPrintUnsignedDec_8bit(value_string_repr, midiParser.message_channel_aftertouch.pressure);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_PITCH_WHEEL) {
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 1 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "PITCH WHEEL");

        graphicsCanvasDrawString(canvas, PRINT_KEY_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, "Value:");
        stringPrintUnsignedDec_16bit(value_string_repr, midiParser.message_pitch_wheel.value);
        graphicsCanvasDrawString(canvas, PRINT_VALUE_X_POSITION, 2 * GRAPHICS_CANVAS_CHAR_HEIGHT * FONT_SIZE, font, value_string_repr);

      }
    }
  }

  if ((micros() - millisecond_tick) > 1000) {
    countdownMidiParser(&midiParser);
    millisecond_tick = micros();
  }

}
