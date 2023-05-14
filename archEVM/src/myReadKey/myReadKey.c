#include "myReadKey.h"

#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#define RAM 100
#define NULLBIT 0x0
#define BIT 0x1
#define MASK 0x7f
#define TERMINAL_PATH "/dev/tty"

#define KEYS "lsrti"
#define F7_KEY "\033[18~"
#define F5_KEY "\033[15~"
#define F6_KEY "\033[17~"
#define DOWN_KEY "\033[B"
#define UP_KEY "\033[A"
#define LEFT_KEY "\033[D"
#define RIGHT_KEY "\033[C"
#define ENTER_KEY '\n'
#define QUIT_KEY "\033"

int
rk_readkey (enum keys *k)
{
  setvbuf (stdout, NULL, _IONBF, 0);
  int terminal = open (TERMINAL_PATH, O_WRONLY);
  if (terminal == -1)
    return -1;

  rk_mytermregime (0, 1, 0, 0, 1);

  char *buffer = malloc (6 * sizeof (char));
  read (0, buffer, 6); // отлавливание введенной клавиши в буфер.

  /* проверка введенной клавиши, записанной в буфер*/
  if (*buffer == 'l')
    *k = load;
  else if (*buffer == 's')
    *k = save;
  else if (*buffer == 'r')
    *k = run;
  else if (*buffer == 't')
    *k = step;
  else if (*buffer == 'i')
    *k = reset;
  else if (*buffer == '\n')
    *k = enter;
  else if (strncmp (buffer, F7_KEY, 4) == 0)
    *k = f7;
  else if (strncmp (buffer, F5_KEY, 4) == 0)
    *k = f5;
  else if (strncmp (buffer, F6_KEY, 4) == 0)
    *k = f6;
  else if (strncmp (buffer, DOWN_KEY, 3) == 0)
    *k = down;
  else if (strncmp (buffer, UP_KEY, 3) == 0)
    *k = up;
  else if (strncmp (buffer, LEFT_KEY, 3) == 0)
    *k = left;
  else if (strncmp (buffer, RIGHT_KEY, 3) == 0)
    *k = right;
  else if (*buffer == '\033')
    *k = quit;
  else
    *k = etc;

  close (terminal);
  free (buffer);

  return 0;
}

int
rk_termsave (const char *const file_name)
{
  FILE *ftermsets = fopen (file_name, "wb");
  if (ftermsets == NULL)
    return -1;
  int terminal = open (TERMINAL_PATH, O_WRONLY);
  if (terminal == -1)
    return -1;

  struct termios termsets;
  if (fwrite (&termsets, sizeof (termsets), BIT, ftermsets) != 1)
    {
      close (terminal);
      return -1;
    }

  fclose (ftermsets);
  close (terminal);
  return 0;
}

int
rk_termrestore (const char *const file_name)
{
  FILE *ftermsets = fopen (file_name, "rb");
  if (ftermsets == NULL)
    return -1;
  int terminal = open (TERMINAL_PATH, O_WRONLY);
  if (terminal == -1)
    return -1;

  struct termios termsets;
  if (fread (&termsets, sizeof (termsets), BIT, ftermsets) != 1)
    {
      close (terminal);
      return -1;
    }
  tcsetattr (0, TCSANOW, &termsets);

  fclose (ftermsets);
  close (terminal);
  return 0;
}

int
rk_mytermregime (int8_t regime, int8_t vtime, int8_t vmin, int8_t echo,
                 int8_t sigint)
{
  int terminal = open (TERMINAL_PATH, O_WRONLY);

  if (terminal == -1 || isatty (terminal) == 0)
    return -1;

  if (regime != 0 && regime != 1)
    return -1;
  if (vtime != 0 && vtime != 1)
    return -1;
  if (vmin != 0 && vmin != 1)
    return -1;
  if (echo != 0 && echo != 1)
    return -1;
  if (sigint != 0 && sigint != 1)
    return -1;
  struct termios termsets;
  tcgetattr (0, &termsets);

  if (regime == 0)
    termsets.c_lflag &= ~ICANON; // настраиваем неканонический режим.
  else
    termsets.c_lflag |= ICANON; // настраиваем канонический режим.

  if (vtime == 0)
    termsets.c_lflag &= ~VTIME;
  else
    termsets.c_lflag |= VTIME;

  if (vmin == 0)
    termsets.c_lflag &= ~VMIN;
  else
    termsets.c_lflag |= VMIN;

  if (echo == 0)
    termsets.c_lflag &= ~ECHO; // отвечает за невидимость введенной клавиши
  else
    termsets.c_lflag |= ECHO;

  if (sigint == 0)
    termsets.c_lflag
        &= ~ISIG; // позволяет принудительно останавливать и убивать программу
  else
    termsets.c_lflag |= ISIG;
  tcsetattr (0, TCSANOW, &termsets);

  close (terminal);
  return 0;
}