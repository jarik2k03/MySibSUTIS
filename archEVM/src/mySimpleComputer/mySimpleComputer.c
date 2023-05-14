#include "mySimpleComputer.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#define RAM 100
#define NULLBIT 0x0
#define BIT 0x1
#define MASK 0x7f

static int *memory = NULL;
static int registr; // переменная, хранящая флаги

static int commandSet[] = {
  10, 11, 20, 21, 30, 31, 32, 33, 40, 41, 42, 43, 51, 52, 53, 54, 55, 56, 57,
  58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
};

int
sc_init ()
{
  memory = calloc (RAM, sizeof (int));
  sc_regInit ();
  if (memory != NULL)
    return 0;
  else
    return -1;
}

int
sc_memorySave (char *filename)
{
  FILE *output_file = fopen (filename, "wb");

  if (output_file == NULL)
    return -1;

  if (fwrite (memory, sizeof (int), 100, output_file) != 100)
    return -1;

  fclose (output_file);

  return 0;
}

int
sc_memoryLoad (char *filename)
{
  FILE *input_file = fopen (filename, "rb");

  if (input_file == NULL)
    return -1;

  if (fread (memory, sizeof (int), 100, input_file) != 100)
    return -1;

  fclose (input_file);

  return 0;
}

int
sc_memoryGet (int address, int *value)
{
  if (value == NULL || address < 0 || address > 99)
    {
      sc_regSet (memoryfault, 1);
      return -1;
    }
  *value = memory[address];
  return 0;
}

int
sc_memorySet (int address, int value) // reg - номер разряда
{
  if (address < 0 || address > 99)
    {
      sc_regSet (memoryfault, 1);
      return -1;
    }
  memory[address] = value;
  return 0;
}

int
sc_regGet (int reg, int *value) // reg - номер разряда
{
  if (reg < 1 || reg > 5)
    return -1;
  if (value == NULL)
    return -1;

  *value = (registr >> (reg - 1)) & BIT;
  // printf("%d", registr >> (reg - 1));

  return 0;
}

int
sc_regSet (int reg, int value) // reg - номер разряда
{
  if (reg < 1 || reg > 5)
    return -1;
  if (value != 0 && value != 1)
    return -1;

  if (value == 1)
    registr |= (1 << (reg - 1)); // задвигаем единичку на нужную позицию и
                                 // записываем в регистр
  else
    registr &= (~(1 << (reg - 1))); // задвигаем единичку на нужную позицию и
                                    // записываем в регистр

  return 0;
}

void
sc_regInit ()
{
  registr = 0;
}

void
sc_outputMemory ()
{
  for (uint8_t i = 0; i != 100; ++i)
    {
      if (i % 10 == 0)
        {
          printf ("\n");
        }
      printf ("%3d ", memory[i]);
    }

  printf ("\n");
}

int
sc_commandEncode (int command, int operand, int *value)
{
  int length = sizeof (commandSet) / sizeof (commandSet[0]);
  int *found = bsearch (&command, commandSet, length, sizeof (int), compare);
  if (found == NULL)
    return -1;
  if (value == NULL || operand > MASK || command > MASK)
    return -1;

  *value = NULLBIT;

  *value = command << 7;
  *value |= operand;
  return 0;
}

int
sc_commandDecode (int value, int *command, int *operand)
{
  if (command == NULL || operand == NULL)
    return -1;
  int sign = value & 0x4000;
  if (sign == 1)
    sc_regSet (unknown, 1);

  *operand = *command = NULLBIT;

  *operand = value & MASK;
  value >>= 7;
  *command = value;

  return sign;
}

int
compare (const void *n1, const void *n2)
{
  return (*(int *)n1 - *(int *)n2);
}

void
sc_memoryRand ()
{
  for (uint8_t i = 0; i < 50; i++)
    sc_memorySet (rand () % 100, rand () % 32767);
}

void
sc_free ()
{
  free (memory);
}
