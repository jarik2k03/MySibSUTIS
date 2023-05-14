#include "events.h"

#include <devices/devices.h>
#include <draw/draw.h>
#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>
#include <retranslators/SAtranslator/SAtranslator.h>
#include <retranslators/SBtranslator/SBtranslator.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define RAM 100

struct itimerval nval = { .it_interval.tv_sec = 2,
                          .it_interval.tv_usec = 0,
                          .it_value.tv_sec = 0,
                          .it_value.tv_usec = 800000 },
                 oval = { .it_interval.tv_sec = 0,
                          .it_interval.tv_usec = 0,
                          .it_value.tv_sec = 0,
                          .it_value.tv_usec = 0 };

int current = 0;
int accumulator = 0;
int registr = 0;
int counter = 0;

int
decode_and_print (int address)
{
  int value;
  int command, operand;
  int sign;
  sc_memoryGet (address, &value);

  sign = sc_commandDecode (value, &command, &operand);
  command >>= 1;

  if (address == current)
    mt_setbgcolor (green);

  print_cell (address, value, command, operand);
  mt_setbgcolor (darkgrey);
  return sign;
}

void
decode_and_display_bc_ (int lastaddress, int address)
{
  int value, lastvalue;
  int command, operand;
  sc_memoryGet (address, &value);
  sc_memoryGet (lastaddress, &lastvalue);
  if (lastvalue == value)
    return;
  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  print_display (value, command, operand);
}

void
decode_and_display_bc (int address)
{
  int value;
  int command, operand;
  sc_memoryGet (address, &value);

  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  print_display (value, command, operand);
}

void
operation_event (int address)
{
  int value;
  int command, operand;
  sc_memoryGet (address, &value);
  sc_commandDecode (value, &command, &operand);
  command >>= 1;
  char sign = (value & 0x4000) ? '-' : '+';
  print_operation (sign, command, operand);
}

void
register_event (int address)
{
  int value;
  sc_regGet (address, &value);
  print_regCell (address, value);
  mainpos_cursor ();
}

void
counter_event ()
{
  int buffer;
  rk_mytermregime (0, 0, 1, 1, 1);
  input_cursor ();
  write (0, "Set counter: ", 14);

  scanf ("%x", &buffer);

  if (buffer < 0xffff && buffer >= 0)
    {
      counter = buffer;
      print_counter (counter);
    }
  input_eraser (50);
  mainpos_cursor ();
}

void
enter_event ()
{
  int buffer;
  rk_mytermregime (0, 0, 1, 1, 1);
  input_cursor ();
  write (0, "Задайте значение текущей ячейке: ", 62);

  scanf ("%x", &buffer);

  if (buffer < 0xffff && buffer >= 0)
    {
      sc_memorySet (current, buffer);
      decode_and_print (current);
    }
  input_eraser (50);
  mainpos_cursor ();
}

void
sat_event ()
{
  char *request = malloc (30 * sizeof (char));
  rk_mytermregime (1, 0, 1, 1, 1);
  input_cursor ();
  write (0, "Выберите файл для компиляции: ", 56);
  read (0, request, 30);
  request[strlen (request) - 1] = '\0';
  if (sa_read_program (request) == -1)
    {
      input_eraser (50);
      mainpos_cursor ();
      free (request);
      return;
    }
  sa_write_program (request);
  free (request);
  input_eraser (50);
  mainpos_cursor ();
}

void
sbt_event ()
{
  char *request = malloc (30 * sizeof (char));
  rk_mytermregime (1, 0, 1, 1, 1);
  input_cursor ();
  write (1, "Выберите файл для компиляции: ", 56);
  read (0, request, 30);
  request[strlen (request) - 1] = '\0';

  if (sb_read_program (request) == -1)
    {
      input_eraser (50);
      mainpos_cursor ();
      free (request);
      return;
    }
  if (sb_write_program (request) != -1 && sa_read_program (request) != -1)
    {
      sa_write_program (request);
    }
  free (request);
  input_eraser (50);
  mainpos_cursor ();
}

void
move_event (enum keys *k)
{
  int last = current;
  switch (*k)
    {
    case right:
      current++;
      break;
    case left:
      current--;
      break;
    case up:
      current -= 10;
      break;
    case down:
      current += 10;
      break;
    default:
      break;
    }

  if (current < 0)
    current = 0;
  else if (current > 99)
    current = 99;

  if (last != current)
    {
      decode_and_print (last);
      decode_and_print (current);
      decode_and_display_bc_ (last, current);
      operation_event (current);
      mainpos_cursor ();
    }
}

void
saveload_event (enum keys *k)
{
  char *filename = malloc (30 * sizeof (char));
  rk_mytermregime (1, 0, 1, 1, 1);
  input_cursor ();
  if (*k == save)
    write (0, "Save to: ", 10);
  else if (*k == load)
    write (0, "Load from: ", 12);
  read (0, filename, 30);
  uint8_t endstr = strlen (filename) - 1;
  filename[endstr] = '\0';

  if (*k == save)
    {
      if (sc_memorySave (filename) == -1)
        erropenfile ("Указанный файл не найден или поврежден!");
    }
  else if (*k == load)
    {
      if (sc_memoryLoad (filename) == -1)
        erropenfile ("Указанный файл не найден или поврежден!");
      for (int i = 0; i < RAM; i++)
        decode_and_print (i);
      decode_and_display_bc (current);
    }
  free (filename);
  input_eraser (50);
  mainpos_cursor ();
}

void
accumulator_event ()
{
  int buffer;
  rk_mytermregime (0, 0, 1, 1, 1);
  input_cursor ();
  write (0, "Set accumulator: ", 18);

  scanf ("%x", &buffer);

  if (buffer < 0xffff && buffer >= 0)
    {
      accumulator = buffer;
      print_accumulator (accumulator);
    }
  input_eraser (50);
  mainpos_cursor ();
}

void
reset_event ()
{
  sc_free ();
  sc_init ();
  sc_regInit ();
  accumulator = 0;
  current = 0;
  registr = 0;
  counter = 0;
  print_accumulator (accumulator);
  operation_event (0);
  print_counter (counter);
  for (int i = 1; i < 6; i++)
    register_event (i);
  for (int i = 0; i < RAM; i++)
    decode_and_print (i);
  decode_and_display_bc (current);
  mainpos_cursor ();
}

void
all_events ()
{
  print_interface ();
  for (uint8_t i = 0; i < RAM; i++)
    decode_and_print (i);
  for (int i = 1; i < 6; i++)
    register_event (i);

  print_counter ();
  print_accumulator (accumulator);
  decode_and_display_bc (current);
  operation_event (current);
  print_counter (counter);
}

void
run_event ()
{
  int value;
  int ignore;
  int oldcell = current;
  int command, operand;

  current = counter;
  decode_and_print (oldcell);
  signal (SIGALRM, *CU);

  for (;;)
    {
      sc_regGet (ticks, &ignore); // 4 - ignore N
      if (ignore == 1)
        {
          alarm (0);
          return;
        }

      if (sc_memoryGet (counter, &value) == -1
          || sc_commandDecode (value, &command, &operand) != 0)
        {
          alarm (0);
          break;
        }
      setitimer (ITIMER_REAL, &nval, &oval); // start.
      pause ();
    }
}

void
step_event ()
{
  int oldcell = current;
  current = counter;
  decode_and_print (oldcell);
  CU ();
}

void
quit_event ()
{
  rk_mytermregime (1, 1, 1, 1, 1);
  mt_clrscr ();
  quick_exit (EXIT_SUCCESS);
}

void
event_listener (enum keys *k)
{
  if (*k >= 7 && *k <= 10)
    move_event (k);
  else if (*k == f6)
    counter_event ();
  else if (*k == load || *k == save)
    saveload_event (k);
  else if (*k == reset)
    reset_event ();
  else if (*k == f5)
    accumulator_event ();
  else if (*k == f7)
    sbt_event ();
  else if (*k == run)
    run_event (k);
  else if (*k == step)
    step_event ();
  else if (*k == enter)
    enter_event ();
  else if (*k == quit)
    quit_event (k);
}
