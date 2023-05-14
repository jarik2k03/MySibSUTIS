#include "SBtranslator.h"
#include <bstree/bstree.h>
#include <draw/draw.h>
#include <retranslators/SAtranslator/SAtranslator.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char commandSet[][8]
    = { "REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END" };

int16_t SBcounter = 0;
int8_t lastnum = 0;
FILE *tempSA = NULL;

struct command *program;

int gotoCounter = -1;
struct command *gotoRecords;

int variableCount = 0;
struct variable Variables[52] = { 0 };

int8_t
sb_read_program (char *in)
{
  if (sb_check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  char path1[50] = "sc_files/basic/";
  strcat (path1, in);

  FILE *SBprog = fopen (path1, "r+");

  if (SBprog == NULL)
    {
      erropenfile ("Указанный файл не найден.");
      return -1;
    }

  char line[120];
  while (!feof (SBprog))
    {
      fgets (line, 120, SBprog);
      if (sb_string_check (line) == -1)
        return -1;
    }
  lastnum = 0;
  fclose (SBprog);
  return 0;
}

int8_t
sb_write_program (char *in)
{
  if (sb_check_filename (in) == -1)
    {
      erropenfile ("Принимаются только *.sb файлы!");
      return -1;
    }
  char path1[50] = "sc_files/basic/";
  char path2[50] = "sc_files/assembler/";
  strcat (path1, in);
  strcat (path2, in);
  path2[strlen (path2) - 1] = 'a';

  if (tempSA == NULL)
    tempSA = fopen (path2, "a+");

  FILE *SBprog = fopen (path1, "r+");

  if (SBprog == NULL)
    {
      erropenfile ("Указанный файл не найден!");
      return -1;
    }
  char command[8];
  int16_t num;
  char *args = calloc (100, sizeof (char));

  char line[120];
  while (!feof (SBprog))
    {

      fgets (line, 120, SBprog);
      sscanf (line, "%hd %s %[^\n]s ", &num, command, args);
      // printf ("%hd %s %s\n", num, command, args);
      sb_write (num, command, args);
    }
  fclose (SBprog);
  fclose (tempSA);
  tempSA = NULL;
  variableCount = 0;
  return 0;
}
int8_t
sb_write (int16_t num, char *command, char *args)
{
  if (strcmp (command, "INPUT") == 0)
    INPUT (args);
  else if (strcmp (command, "PRINT") == 0)
    PRINT (args);
  else if (strcmp (command, "LET") == 0)
    LET (args);
  else if (strcmp (command, "IF") == 0)
    IF (args);
  else if (strcmp (command, "END") == 0)
    END ();
}

int8_t
sb_check_filename (char *in)
{
  uint8_t ext = strlen (in) - 3;
  char exp_in[] = ".sb";
  for (uint8_t i = 0; i < 3; i++)
    if (in[ext + i] != exp_in[i])
      return -1;

  return 0;
}

int8_t
sb_string_check (char *str)
{
  char command[8];
  char *args = calloc (100, sizeof (char));
  int16_t num;
  sscanf (str, "%hd %s %[^\n]s ", &num, command, args);
  int8_t returns = 0;
  if (sb_cell_check (num, lastnum) == -1)
    returns = -1;
  else if (sb_command_check (command) == -1)
    returns = -1;

  lastnum = num;
  free (args);
  return returns;
}

int8_t
sb_command_check (char *command)
{
  for (uint8_t i = 0; i < 8; i++)
    if (strcmp (command, commandSet[i]) == 0)
      return 0;

  erropenfile ("Ошибка - неопознанная команда.");
  return -1; // не нашли
}

int8_t
sb_cell_check (int16_t num, int16_t lastnum)
{
  if (num <= lastnum)
    {
      erropenfile ("Ошибка - ячейки должны быть последовательны");
      lastnum = 0;
      return -1;
    }
  if (num < 0 || num > 99)
    {
      erropenfile ("Ошибка - указанная ячейка не в пределах памяти.");
      return -1;
    }
  return 0;
}

int8_t
alloc (char var)
{
  for (int i = 0; i < 52; i++)
    {
      if (Variables[i].Name == var)
        return Variables[i].Address;

      if (Variables[i].Name == '\0')
        {
          Variables[i].Name = var;
          Variables[i].Address = 99 - i;
          variableCount++;
          return Variables[i].Address;
        }
    }
}

int8_t
INPUT (char *args)
{
  if (args[0] < 'A' || args[0] > 'Z')
    {
      erropenfile ("INPUT -- переменная начинается с заг. буквы");
      return -1;
    }
  if (args[1] != ' ' && args[1] != '\0')
    {
      erropenfile ("INPUT -- переменная только из одной буквы");
      return -1;
    }
  fprintf (tempSA, "%.2i READ \t\t%d\n", SBcounter, alloc (args[0]));
  SBcounter++;

  return 0;
}

int8_t
PRINT (char *args)
{
  if (args[0] < 'A' || args[0] > 'Z')
    {
      erropenfile ("PRINT -- переменная начинается с заг. буквы");
      return -1;
    }
  if (args[1] != ' ' && args[1] != '\0')
    {
      erropenfile ("PRINT -- переменная только из одной буквы");
      return -1;
    }
  fprintf (tempSA, "%.2i WRITE \t\t%d\n", SBcounter, alloc (args[0]));
  SBcounter++;
  return 0;
}

int8_t
LET (char *args)
{
  char *fin = malloc (255 * sizeof (char));
  char var;
  var = preCalcProcessing (args);
  translateToRPN (args, fin);
  parsRPN (fin, &var);
  return 0;
}

int8_t
IF (char *args)
{
  int mySign = -1;
  int before = 0;
  int after = 0;

  for (int i = 0; i < strlen (args); i++)
    {
      if ((args[i] == '>') || (args[i] == '<') || (args[i] == '='))
        {

          mySign = i;

          if (!(args[i - 1] == ' '))
            before = 1;
          if (!(args[i + 1] == ' '))
            after = 1;
          break;
        }
    }

  char *expression = (char *)malloc (sizeof (char) * 255);
  if (!(before) && !(after))
    {
      expression = strtok (args, "");
    }
  else
    {
      int j = 0;
      for (int i = 0; i < strlen (args); i++)
        {
          if (i == mySign)
            {
              if (before)
                {
                  expression[j] = ' ';
                  j++;
                }
              expression[j] = args[i];
              j++;
              if (after)
                {
                  expression[j] = ' ';
                  j++;
                }
            }
          else
            {
              expression[j] = args[i];
              j++;
            }
        }
      expression[j] = '\0';
    }

  char *ptr = strtok (expression, " ");
  char *operand1 = ptr;
  char operand1Name;

  if (strlen (operand1) > 1)
    {
      if (atoi (operand1))
        operand1Name = intToConstant (atoi (operand1));
    }
  else
    {
      if ((operand1[0] >= '0') && (operand1[0] <= '9'))
        {
          operand1Name = intToConstant (atoi (operand1));
        }
      else if ((operand1[0] >= 'A') && (operand1[0] <= 'Z'))
        {
          operand1Name = operand1[0];
        }
      else
        {
          erropenfile ("Wrong statement!\n");
          return -1;
        }
    }

  ptr = strtok (NULL, " ");
  char *logicalSign = ptr;

  ptr = strtok (NULL, " ");
  char *operand2 = ptr;
  char operand2Name;

  if (strlen (operand2) > 1)
    {
      if (atoi (operand2))
        operand2Name = intToConstant (atoi (operand2));
    }
  else
    {
      if ((operand2[0] >= '0') && (operand2[0] <= '9'))
        operand2Name = intToConstant (atoi (operand2));
      else if ((operand2[0] >= 'A') && (operand2[0] <= 'Z'))
        operand2Name = operand2[0];
      else
        {
          erropenfile ("Wrong statement!\n");
          return -1;
        }
    }

  int falsePosition = -1;

  if (logicalSign[0] == '<')
    {
      fprintf (tempSA, "%.2i LOAD \t\t%d\n", SBcounter, alloc (operand1Name));
      SBcounter++;
      fprintf (tempSA, "%.2i SUB  \t\t\t%d\n", SBcounter,
               alloc (operand2Name));
      SBcounter++;
      fprintf (tempSA, "%.2i JNEG \t\t%d\n", SBcounter, SBcounter + 16);
      SBcounter++;
      falsePosition = SBcounter;
      SBcounter++;
    }
  else if (logicalSign[0] == '>')
    {
      fprintf (tempSA, "%.2i LOAD \t\t%d\n", SBcounter, alloc (operand2Name));
      SBcounter++;
      fprintf (tempSA, "%.2i SUB  \t\t\t%d\n", SBcounter,
               alloc (operand1Name));
      SBcounter++;
      fprintf (tempSA, "%.2i JNEG \t\t%d\n", SBcounter, SBcounter + 16);
      SBcounter++;
      falsePosition = SBcounter;
      SBcounter++;
    }
  else if (logicalSign[0] == '=')
    {
      fprintf (tempSA, "%.2i LOAD \t\t%d\n", SBcounter, alloc (operand1Name));
      SBcounter++;
      fprintf (tempSA, "%.2i SUB  \t\t\t%d\n", SBcounter,
               alloc (operand2Name));
      SBcounter++;
      fprintf (tempSA, "%.2i JZ \t\t%d\n", SBcounter, SBcounter + 16);
      SBcounter++;
      falsePosition = SBcounter;
      SBcounter++;
    }

  ptr = strtok (NULL, " ");
  char *command = ptr;
  ptr = strtok (NULL, "");
  char *commandArguments = ptr;

  if (strcmp (command, "IF") == 0)
    {
      erropenfile ("Multiple IF!\n");
      return -1;
    }

  else if (strcmp (command, "GOTO") != 0)
    function (command, commandArguments);
  else
    {
      gotoCounter++;
      gotoRecords
          = realloc (gotoRecords, sizeof (struct command) * gotoCounter + 1);
      struct command gotoCommand;
      gotoCommand.Address = SBcounter;
      char *buff = (char *)malloc (sizeof (char) * 255);
      sprintf (buff, "%d GOTO %s", falsePosition, commandArguments);
      gotoCommand.Command = buff;
      gotoRecords[gotoCounter] = gotoCommand;
      SBcounter++;
    }

  fprintf (tempSA, "%.2i JUMP \t\t%d\n", falsePosition + 16,
           atoi (commandArguments));
  // SBcounter++;
}

int8_t
GOTO (int address, int operand)
{
  for (int i = 0; i < 100; i++)
    {
      if (program[i].Number == operand)
        {
          fprintf (tempSA, "%.2i JUMP \t\t%d\n", address, operand);
          return 0;
        }
    }
  erropenfile ("Reference to an inspecifed memory location.\n");
  return -1;
}

int8_t
END ()
{
  fprintf (tempSA, "%.2i HALT \t\t00\n", SBcounter);
  SBcounter++;
}

void
parsRPN (char *rpn, char *var)
{
  int i = 0;
  int depth = 0;
  int operand1, operand2;
  char memoryCounter = '1';
  while (rpn[i] != '\0' && rpn[i] != '\n')
    {
      char x = rpn[i];
      if ((x >= 'a' && x <= 'z') || x >= 'A' && x <= 'Z')
        {
          fprintf (tempSA, "%.2i LOAD \t\t%d\n", SBcounter, alloc (x));
          SBcounter++;
          fprintf (tempSA, "%.2i STORE \t\t%d\n", SBcounter,
                   alloc (memoryCounter));
          memoryCounter++;
          SBcounter++;
          depth++;
        }
      if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          if (depth < 2)
            {
              erropenfile (
                  "Uncorrect LET statement, check your expression.\n");
              return;
            }
          else
            {
              operand1 = alloc (memoryCounter - 2);
              operand2 = alloc (memoryCounter - 1);
              fprintf (tempSA, "%.2i LOAD \t\t%d\n", SBcounter,
                       operand1); //закидываем самый правый операнд в акк
              SBcounter++;
              switch (x)
                {
                case '+':
                  fprintf (tempSA, "%.2i ADD \t\t%d\n", SBcounter, operand2);
                  SBcounter++;
                  break;
                case '-': // SUB
                  fprintf (tempSA, "%.2i SUB \t\t%d\n", SBcounter, operand2);
                  SBcounter++;
                  break;
                case '/': // DIVIDE
                  fprintf (tempSA, "%.2i DIVIDE \t\t%d\n", SBcounter,
                           operand2);
                  SBcounter++;
                  break;
                case '*': // MUL
                  fprintf (tempSA, "%.2i MUL \t\t%d\n", SBcounter, operand2);
                  SBcounter++;
                  break;
                }
              fprintf (tempSA, "%.2i STORE \t\t%d\n", SBcounter,
                       alloc (memoryCounter - 2));
              SBcounter++;
              depth--;
              memoryCounter = memoryCounter - 1;
            }
        }
      i++;
    }
  if (depth != 1) //??
    {
      erropenfile ("Uncorrect LET statement, check your expression.\n");
      return;
    }
  fprintf (tempSA, "%.2i STORE \t\t%d\n", SBcounter, alloc (*var));
  SBcounter++;
}

char
intToConstant (int value)
{
  char lastConstantName = 'a';
  for (int i = 0; i < 52; i++)
    {
      if (Variables[i].Name == '\0')
        {
          Variables[i].Name = lastConstantName;
          lastConstantName++;
          Variables[i].Address = 99 - i;
          Variables[i].Value = value;
          fprintf (tempSA, "%.2i = \t\t\t%x\n", Variables[i].Address,
                   abs (Variables[i].Value));
          variableCount++;
          return Variables[i].Name;
        }

      if (Variables[i].Value != 0)
        {
          if (Variables[i].Value == value)
            {
              return Variables[i].Name;
            }
        }
    }
}

char
preCalcProcessing (char *args)
{
  char *ptr = strtok (args, " =");
  char val;
  sscanf (ptr, "%c", &val);
  if (!((val) >= 'A' && (val) <= 'Z'))
    {
      erropenfile ("NOT CORRECT!\n");
      return -1;
    }
  ptr = strtok (NULL, " ");
  char *eq = ptr;
  if (strcmp (eq, "=") != 0)
    {
      erropenfile ("Wrong expression!\n");
      return -1;
    }
  ptr = strtok (NULL, "");
  char *exp = ptr;
  int i = 0;
  int pos = 0;
  int j = 0;
  int operat = 0;
  int flg = 1;
  int m = 0;
  char *assign = (char *)malloc (sizeof (char) * 255);
  for (int k = 0; k < strlen (exp); k++)
    {
      if (exp[k] == '-' && flg)
        {
          assign[m] = '0';
          m++;
        }
      flg = 0;
      if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*')
        operat++;
      if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*'
          || exp[k] == '(')
        flg = 1;

      assign[m] = exp[k];
      m++;
    }
  if (operat == 0)
    sprintf (exp, "0 + %s", assign);
  else
    sprintf (exp, "%s", assign);
  while (exp[i] != '\n' && exp[i] != '\0')
    {
      if (exp[i] >= '0' && exp[i] <= '9')
        {
          char num[256];
          j = 0;
          num[j] = exp[i];
          j++;
          pos = i;
          exp[i] = ' ';
          i++;
          while (exp[i] >= '0' && exp[i] <= '9')
            {
              num[j] = exp[i];
              j++;
              exp[i] = ' ';
              i++;
            }
          num[j] = '\0';
          exp[pos] = intToConstant (atoi (num));
        }
      i++;
    }
  sprintf (args, "%s", exp);

  return val;
}

char *
translateToRPN (char *inf, char *rpn)
{
  // char rpn[255] = "\0";
  Node *root = NULL;
  int i = 0;
  int j = 0;
  while (inf[i] != '\0' && inf[i] != '\n')
    {
      char x = inf[i];
      if ((x >= 'a' && x <= 'z') || x >= 'A' && x <= 'Z')
        {
          rpn[j] = x;
          j++;
          // printf("%s\n", rpn);
        }
      else if (x == '(')
        {
          stack_push (x, &root);
        }
      else if (x == ')')
        {
          while (stack_top (root) != '(')
            {
              char c = stack_pop (&root);
              if (c != 0)
                {
                  rpn[j] = c;
                  j++;
                }
            }
          stack_pop (&root);
        }
      else if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          while (root != NULL
                 && checkPriority (root->data) >= checkPriority (x))
            {
              char c = stack_pop (&root);
              if (c != 0)
                {
                  rpn[j] = c;
                  j++;
                }
            }
          stack_push (x, &root);
        }
      else if (x != ' ')
        {
          // free(rpn);
          erropenfile ("Wrong expression!\n");
          return NULL;
        }
      i++;
    }
  while (root != NULL)
    {
      char c = stack_pop (&root);
      if (c != 0)
        {
          rpn[j] = c;
          j++;
        }
    }
  for (int k = 0; k < j; k++)
    {
      if (rpn[k] == '(' || rpn[k] == ')')
        {
          erropenfile ("Check your expression!\n");
          return NULL;
        }
    }
  rpn[j] = '\0';
  return rpn;
}

void
stack_push (char data, Node **top)
{
  Node *tmp = (Node *)malloc (sizeof (Node));
  tmp->data = data;
  tmp->next = *top;
  *top = tmp;
}

char
stack_pop (Node **top)
{
  Node *tmp;
  char d;
  if (*top == NULL)
    return -1;
  else
    {
      tmp = *top;
      *top = tmp->next;
      d = tmp->data;
      free (tmp);
      return d;
    }
}

char
stack_top (Node *top)
{
  if (top != NULL)
    return top->data;
}

int
checkPriority (char sign)
{
  switch (sign)
    {
    case '*':
    case '/':
      return 4;
    case '+':
    case '-':
      return 2;
    case '(':
    case ')':
      return 1;
    }
}

void
function (char *command, char *args)
{

  if (strcmp (command, "REM") == 0)
    ;
  else if (strcmp (command, "INPUT") == 0)
    INPUT (args);
  else if (strcmp (command, "PRINT") == 0)
    PRINT (args);
  else if (strcmp (command, "IF") == 0)
    IF (args);
  else if (strcmp (command, "LET") == 0)
    LET (args);
  else if (strcmp (command, "END") == 0)
    END ();
  else
    erropenfile ("Unknown  command.\n");
}
