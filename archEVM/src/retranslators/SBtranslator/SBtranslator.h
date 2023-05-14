#pragma once
#include <inttypes.h>

typedef struct NODE
{
  char data;
  struct NODE *next;
} Node;

struct command
{
  int Number;
  char *Command;
  int Address;
};

struct variable
{
  char Name;
  int Address;
  int Value;
};

int8_t sb_read_program (char *in);
int8_t sb_check_filename (char *in);
int8_t sb_string_check (char *str);
int8_t sb_check_filename (char *in);
int8_t sb_command_check (char *str);
int8_t sb_cell_check (int16_t num, int16_t lastnum);

int8_t sb_write (int16_t num, char *command, char *args);
int8_t sb_write_program (char *in);
int8_t alloc (char var);
int8_t INPUT (char *args);
int8_t PRINT (char *args);
int8_t LET (char *args);
int8_t IF (char *args);
int8_t GOTO (int address, int operand);
int8_t END ();

void function (char *command, char *args);
char preCalcProcessing (char *args);
char intToConstant (int value);
void parsRPN (char *rpn, char *var);

void stack_push (char data, Node **top);
void stack_print (Node *top);
char stack_pop (Node **top);
char stack_top (Node *top);
int checkPriority (char sign);
char *translateToRPN (char *inf, char rpn[]);
