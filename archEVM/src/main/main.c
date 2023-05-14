#include <draw/draw.h>
#include <events/events.h>

#include <myBigChars/myBigChars.h>
#include <myReadKey/myReadKey.h>
#include <mySimpleComputer/mySimpleComputer.h>
#include <myTerm/myTerm.h>

#include <stdio.h>
#include <unistd.h>

int
main (void)
{
  enum keys k;
  int cols, rows;
  uint8_t smallWindow = 1;
  mt_clrscr ();
  sc_init ();
  mt_setfgcolor (white);
  // sc_memoryRand();
  while (1)
    {
      mt_getscreensize (&rows, &cols);
      if (rows >= 32 && cols >= 96)
        {
          if (smallWindow == 1)
            {
              all_events ();
              mainpos_cursor ();
              smallWindow = 0;
            }
          rk_readkey (&k);
          event_listener (&k);
        }
      else
        {
          smallWindow = 1;
          mt_clrscr ();
          printf ("РАЗВЕРНИТЕ ПРИЛОЖЕНИЕ НА ПОЛНЫЙ ЭКРАН!");
          getchar ();
        }
    }
  sc_free ();
  return 0;
}
