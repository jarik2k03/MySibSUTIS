#include "myBigChars.h"

#include <fcntl.h>
#include <malloc.h>
#include <myTerm/myTerm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define TERMINAL_PATH "/dev/tty"
#define NULLBIT 0x0
#define BIT 0x1
#define MASK 0x7f
#define BUFSIZE 15
#define CLEAR "\033[H\033[2J"
#define FGCOLOR "\033[0;38m"
#define BGCOLOR "\033[0;48m"

#define RECT 'a'
#define HR 'q'
#define VT_BOUND 'x'
#define RB 'j'
#define RU 'k'
#define LU 'l'
#define LB 'm'

static char *bufer;

int
bc_printA (char str)
{
  bufer = malloc (8);
  sprintf (bufer, "\033(0%c\033(B", str);
  write (1, bufer, strlen (bufer));

  free (bufer);

  return 0;
}

int
bc_printNL ()
{
  write (1, "\n", strlen ("\n"));
  return 0;
}

int
bc_printUB (int len)
{
  if (bc_printA (LU) == -1)
    return -1;

  for (int i = 0; i != len; ++i)
    if (bc_printA (HR) == -1)
      return -1;

  if (bc_printA (RU) == -1)
    return -1;

  bc_printNL ();
  return 0;
}

int
bc_printLB (int len)
{
  if (bc_printA (LB) == -1)
    return -1;

  for (int i = 0; i != len; ++i)
    if (bc_printA (HR) == -1)
      return -1;

  if (bc_printA (RB) == -1)
    return -1;

  bc_printNL ();

  return 0;
}

int
bc_printES (int len)
{

  for (int i = 0; i != len; ++i)
    write (1, " ", 2);

  return 0;
}

int
bc_box (int x1, int y1, int x2, int y2)
{
  // без буферизации.
  setvbuf (stdout, NULL, _IONBF, 0);

  if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0)
    return -1;

  if (mt_gotoXY (x1, y1) == -1)
    return -1;

  // чердак.
  if (bc_printUB (y2) == -1)
    return -1;
  ++x1;

  for (int i = 0; i != x2; ++i)
    {
      mt_gotoXY (x1++, y1);

      if (bc_printA (VT_BOUND) == -1)
        return -1;

      bc_printES (y2);

      if (bc_printA (VT_BOUND) == -1)
        return -1;

      bc_printNL ();
    }

  mt_gotoXY (x1++, y1);

  // подвал.
  if (bc_printLB (y2) == -1)
    return -1;

  return 0;
}

int
bc_printbigchar (int arr[2], int x, int y, int front, int back)
{
  int a[2];
  a[0] = arr[0];
  a[1] = arr[1];

  mt_setbgcolor (back);
  mt_setbgcolor (front);
  mt_gotoXY (x, y);

  for (int i = 0; i != 2; i++)
    {
      for (int k = 0; k != sizeof (int); ++k)
        {
          mt_gotoXY (x++, y);
          for (int rad = 0; rad != 8; ++rad)
            {
              int val = a[i] & BIT;
              bc_printA (' ' + (val * 65));
              a[i] >>= 1;
            }
          bc_printNL ();
        }
    }

  return 0;
}

// sc_files/binary/b.o
int
bc_setbigcharpos (int *big, int x, int y, int value)
{
  if (big == NULL || x < 1 || x > 8 || y < 1 || y > 8
      || (value != 0 && value != 1))
    return -1;

  // первые 4 строки выделены под первый инт.
  if (x < (8 / 2 + 1))
    {
      (value == BIT) ? (big[0] |= (1 << ((8 * (x - 1) + y) - 1)))
                     : (big[0] &= (~(1 << (((8 * (x - 1) + y)) - 1))));
    }
  // следующие под второй инт.
  else
    {
      (value == BIT) ? (big[1] |= (1 << ((8 * (x - 1) + y) - 1)))
                     : (big[1] &= (~(1 << (((8 * (x - 1) + y)) - 1))));
    }

  return 0;
}

int
bc_getbigcharpos (int *big, int x, int y, int *value)
{
  if (big == NULL || x < 1 || x > 8 || y < 1 || y > 8 || value == NULL)
    {
      if (value != NULL)
        {
          *value = 0;
        }
      return -1;
    }

  if (x < 8 / 2 + 1)
    {
      *value = (big[0] >> ((8 * (x - 1) + y) - 1)) & BIT;
    }
  else
    {
      *value = (big[1] >> ((8 * (x - 1) + y) - 1)) & BIT;
    }

  return 0;
}

int
bc_bigcharwrite (int fd, int *big, int count)
{
  if (big == NULL || count < 1)
    {
      return -1;
    }

  if (write (fd, big, sizeof (int) * 2 * count) == -1)
    {
      return -1;
    }

  return 0;
}

int
bc_bigcharread (int fd, int *big, int need_count, int *count)
{
  if (big == NULL || need_count < 1 || count == NULL)
    {
      if (count != NULL)
        {
          *count = 0;
        }
      return -1;
    }

  if (read (fd, big, need_count * sizeof (int) * 2) == -1)
    {
      return -1;
    }

  return 0;
}