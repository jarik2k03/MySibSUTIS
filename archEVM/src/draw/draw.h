#pragma once
#include <inttypes.h>

typedef struct NUM
{
  int N[2];
} NUM;

void display_bigchar (int value, int x, int shift);
void draw_frame (int x, int y, int x1, int y1, char *title);
void print_regCell (int address, uint8_t value);
void print_accumulator (int32_t accumulator);
void print_counter ();
void print_operation (int sign, int command, int operand);
void print_display (int value, int command, int operand);
void print_interface ();
void print_cell (int16_t address, int32_t value, int16_t command,
                 int16_t operand);
void print_ccell (int16_t address, int32_t value);
void erropenfile (char *message);

void hist_cursor (int8_t hcounter);
void mainpos_cursor ();
void input_cursor ();
void input_eraser (int length);
void hist_eraser (int length);
void mainpos_eraser (int length);
