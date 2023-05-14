#pragma once
#include <inttypes.h>

int8_t sa_read_program (char *in);
int8_t sa_write_program (char *in);
int8_t sa_check_filename (char *in);
int8_t sa_write_to_memory (int16_t *num, char *command, int16_t *operand);
int8_t sa_save_memory (char *in);
int8_t sa_string_check (char *str, int16_t *num, char *command,
                        int16_t *operand);
int8_t sa_comment_check (char *com);
int8_t sa_command_check (char *command);
int8_t sa_operand_check (int16_t operand, char *command);
int8_t sa_cell_check (int16_t num);
int8_t sa_init_commands ();