#pragma once
enum colors
{
  darkgrey = 0, // "\e[0;30m"
  red,          // "\e[0;31m"
  green,        // "\e[0;32m"
  yellow,       // "\e[0;33m"
  blue,         // "\e[0;34m"
  magenta,      // "\e[0;35m"
  cyan,         // "\e[0;36m"
  grey,         // "\e[0;37m"
  white,        // "\e[0;38m"
};

int mt_clrscr (void);
int mt_gotoXY (int x, int y);
int mt_getscreensize (int *rows, int *cols);
int mt_setfgcolor (enum colors c);
int mt_setbgcolor (enum colors c);
int num_length (float x);
