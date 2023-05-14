#pragma once

enum regs
{
  overflow = 1,
  zero,
  memoryfault,
  ticks,
  unknown
};

int sc_init ();
int sc_regGet (int reg, int *value);
int sc_regSet (int reg, int value);
void sc_regInit ();

int sc_memorySave (char *filename);
int sc_memoryLoad (char *filename);
int sc_memoryGet (int address, int *value);
int sc_memorySet (int address, int value);
void sc_memoryRand ();
void sc_outputMemory ();
void sc_free ();

int sc_commandEncode (int command, int operand, int *value);
int sc_commandDecode (int value, int *command, int *operand);
int compare (const void *n1, const void *n2);
