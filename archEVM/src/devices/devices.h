#pragma once
#include <inttypes.h>

void CU ();
void ALU (int command, int operand);
void READ (int operand);
void WRITE (int operand);
void LOAD (int operand);
void STORE (int operand);
void JUMP (int operand);
void JNEG (int operand);
void JZ (int operand);
void HALT (int operand);
void OR (int operand);
void XOR (int operand);