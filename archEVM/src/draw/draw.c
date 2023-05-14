#include "draw.h"

#include <malloc.h>
#include <myBigChars/myBigChars.h>
#include <myTerm/myTerm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static NUM NUMS[] = { /* + */ { .N[0] = 0xFF181818, .N[1] = 0x181818FF },
                      /* - */ { .N[0] = 0xFF000000, .N[1] = 0x000000FF },
                      /* 0 */ { .N[0] = 0x8181817e, .N[1] = 0x7e818181 },
                      /* 1 */ { .N[0] = 0x8890A0C0, .N[1] = 0x80808080 },
                      /* 2 */ { .N[0] = 0x2040827C, .N[1] = 0xFE040810 },
                      /* 3 */ { .N[0] = 0x6080817E, .N[1] = 0x7E818060 },
                      /* 4 */ { .N[0] = 0xFF818181, .N[1] = 0x80808080 },
                      /* 5 */ { .N[0] = 0x7F0101FF, .N[1] = 0x7F808080 },
                      /* 6 */ { .N[0] = 0x0101817E, .N[1] = 0x7E81817F },
                      /* 7 */ { .N[0] = 0x204080FE, .N[1] = 0x02040810 },
                      /* 8 */ { .N[0] = 0x7E81817E, .N[1] = 0x7E818181 },
                      /* 9 */ { .N[0] = 0x7E81817E, .N[1] = 0x7E808080 },
                      /* A */ { .N[0] = 0x7E42423C, .N[1] = 0x42424242 },
                      /* B */ { .N[0] = 0x3E42423E, .N[1] = 0x3E424242 },
                      /* C */ { .N[0] = 0x0101017E, .N[1] = 0x7E010101 },
                      /* D */ { .N[0] = 0x4242221E, .N[1] = 0x1E224242 },
                      /* E */ { .N[0] = 0x7E02027E, .N[1] = 0x7E020202 },
                      /* F */ { .N[0] = 0x7E02027E, .N[1] = 0x02020202 } };

void
print_hints ()
{
  int x = 14, y = 48;
  mt_gotoXY (x, y);
  printf ("s\t- save");
  mt_gotoXY (x++, y);
  printf ("l\t- load");
  mt_gotoXY (x++, y);
  printf ("s\t- save");
  mt_gotoXY (x++, y);
  printf ("r\t- run");
  mt_gotoXY (x++, y);
  printf ("t\t- step");
  mt_gotoXY (x++, y);
  printf ("i\t- reset");
  mt_gotoXY (x++, y);
  printf ("F5\t- accumulator");
  mt_gotoXY (x++, y);
  printf ("F6\t- instructionCounter");
  mt_gotoXY (x++, y);
  printf ("F7\t- SB retranslator");
}
void
display_bigchar (int value, int x, int shift)
{
  int bc_id;
  if (x == 2)
    (value & 0x4000) ? (bc_id = 1) : (bc_id = 0);
  else
    bc_id = 2 + ((value >> shift) & 0b1111);
  bc_printbigchar (NUMS[bc_id].N, 14, x, green, yellow);
  mt_setbgcolor (darkgrey);
}

void
draw_frame (int x, int y, int x1, int y1, char *title)
{
  bc_box (y, x, y1 - y, x1 - x);
  mt_gotoXY (y, (x1 + x) / 2 - (strlen (title) / 2));
  printf ("%s", title);
}
void
print_regCell (int address, uint8_t value)
{
  if (address < 1 || address > 5)
    return;

  char regs[] = "OZMTU";
  mt_gotoXY (11, 74 + address);
  if (value == 1)
    mt_setbgcolor (green);
  printf ("%c", *(regs + address - 1));
  mt_setbgcolor (darkgrey);
}

void
print_accumulator (int32_t accumulator)
{
  mt_gotoXY (2, 74);
  char sign = (accumulator & 0x4000) ? '-' : '+';
  printf ("%c%04X", sign, accumulator);
}

void
print_counter (int counter)
{

  mt_gotoXY (5, 74);
  char sign = (counter & 0x4000) ? '-' : '+';
  printf ("%c%04X", sign, counter);
}

void
print_operation (int sign, int command, int operand)
{
  mt_gotoXY (8, 74);
  printf ("%c%02X : %02X", sign, command, operand);
}

void
print_display (int value, int command, int operand)
{
  int x = 2;
  display_bigchar (value, x, 0);
  display_bigchar (command, x += 9, 4);
  display_bigchar (command, x += 9, 0);
  display_bigchar (operand, x += 9, 4);
  display_bigchar (operand, x += 9, 0);
}

void
print_interface ()
{
  draw_frame (1, 1, 60, 11, "Memory");
  draw_frame (63, 1, 90, 2, "accumulator");
  draw_frame (63, 4, 90, 5, "instructionCounter");
  draw_frame (63, 7, 90, 8, "Operation");
  draw_frame (63, 10, 90, 11, "Flags");
  draw_frame (1, 13, 45, 21, "");
  draw_frame (47, 13, 90, 21, "Keys:  ");
  draw_frame (1, 23, 90, 24, "Execution requests");
  draw_frame (1, 25, 90, 30, "History");
  draw_frame (1, 32, 90, 33, "");

  print_hints ();
}

void
print_cell (int16_t address, int32_t value, int16_t command, int16_t operand)
{
  int row, col;
  char *buf = malloc (7 * sizeof (char));

  row = address / 10;
  col = address % 10;

  snprintf (buf, 7, "%c%02X%02X\n", (value & 0x4000) ? '-' : '+', command,
            operand);

  mt_gotoXY (2 + row, 2 + col * 6);
  write (1, buf, 5);

  free (buf);
  return;
}

void
print_ccell (int16_t address, int32_t value)
{
  int row, col;
  char *buf = malloc (7 * sizeof (char));

  row = address / 10;
  col = address % 10;

  snprintf (buf, 7, "%c%04X\n", (value & 0x4000) ? '-' : '+', value);

  mt_gotoXY (2 + row, 2 + col * 6);
  write (1, buf, 5);

  free (buf);
  return;
}

void
erropenfile (char *message)
{
  input_cursor ();
  mt_setbgcolor (red);
  write (0, message, 60);
  mt_setbgcolor (darkgrey);
  sleep (1);
  input_cursor ();
  input_eraser (strlen (message));
}

void
mainpos_cursor ()
{
  mt_gotoXY (24, 2);
}

void
input_cursor ()
{
  mt_gotoXY (33, 2);
}

void
hist_cursor (int8_t hcounter)
{
  int8_t curlist = (hcounter % 5) + 26;
  mt_gotoXY (curlist, 2);
  hist_eraser (70);
  mt_gotoXY (curlist, 2);
}

void
input_eraser (int length)
{
  char *buffer = malloc (length * sizeof (char));
  for (int i = 0; i < length; i++)
    *(buffer + i) = ' ';
  input_cursor ();
  write (0, buffer, length);
  free (buffer);
}

void
mainpos_eraser (int length)
{
  char *buffer = malloc (length * sizeof (char));
  for (int i = 0; i < length; i++)
    *(buffer + i) = ' ';
  mainpos_cursor ();
  write (0, buffer, length);
  free (buffer);
}

void
hist_eraser (int length)
{
  char *buffer = malloc (length * sizeof (char));
  for (int i = 0; i < length; i++)
    *(buffer + i) = ' ';
  write (0, buffer, length);
  free (buffer);
}
