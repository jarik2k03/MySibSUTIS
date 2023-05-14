#include "myTerm.h"

#include <fcntl.h>
#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define TERMINAL_PATH "/dev/tty"
#define CLEAR "\033[H\033[2J"
#define FGCOLOR "\033[0;38m"
#define BGCOLOR "\033[0;48m"

char *buffer;

int
mt_clrscr (void)
{
  int terminal = open (TERMINAL_PATH, O_WRONLY); // дескриптор для записи
  if (terminal == -1)
    return -1;

  const int bufsize = sizeof (CLEAR);
  buffer = malloc (bufsize * sizeof (char));

  sprintf (buffer, CLEAR);           //запись в буфер
  write (terminal, buffer, bufsize); // запись в терминал

  free (buffer);
  close (terminal);

  return 0;
}

int
mt_gotoXY (int x, int y)
{
  if (x < 0 || y < 0)
    return -1;

  int terminal = open (TERMINAL_PATH, O_WRONLY); // дескриптор для записи
  if (terminal == -1)
    return -1;

  const int bufsize = sizeof ("\033[;f]") + num_length (x) + num_length (y);
  buffer = malloc (bufsize * sizeof (char));

  sprintf (buffer, "\033[%d;%df", x, y); //запись в буфер
  write (terminal, buffer, bufsize);     // запись в терминал

  free (buffer);
  close (terminal);

  return 0;
}

int
mt_getscreensize (int *rows, int *cols)
{
  struct winsize ws;
  if (cols == NULL || rows == NULL)
    return -1;
  if (ioctl (1, TIOCGWINSZ, &ws))
    return -1;

  *rows = ws.ws_row;
  *cols = ws.ws_col;
  return 0;
}

int
mt_setfgcolor (enum colors c)
{
  int terminal = open (TERMINAL_PATH, O_WRONLY);
  if (terminal == -1)
    return -1;

  const int bufsize = sizeof (FGCOLOR);
  buffer = malloc (bufsize * sizeof (char));

  sprintf (buffer, "\033[0;3%dm", c); //запись в буфер
  write (terminal, buffer, bufsize);  // запись в терминал

  free (buffer);
  close (terminal);
  return 0;
}

int
mt_setbgcolor (enum colors c)
{
  int terminal = open (TERMINAL_PATH, O_WRONLY);
  if (terminal == -1)
    return -1;

  const int bufsize = sizeof (BGCOLOR);
  buffer = malloc (bufsize * sizeof (char));

  sprintf (buffer, "\033[0;4%dm", c); //запись в буфер
  write (terminal, buffer, bufsize);  // запись в терминал

  free (buffer);
  close (terminal);
  return 0;
}

int
num_length (float x)
{
  if (x == 0)
    return 1;

  int degree = 0;
  for (float i = 1; (x / i) >= 1; i *= 10)
    degree++;

  return degree;
}
