#pragma once

#include <inttypes.h>

enum keys
{
  load,
  save,
  run,
  step,
  reset,
  f5,
  f6,
  up,
  down,
  left,
  right,
  enter,
  f7,
  quit,
  etc
};

int rk_readkey (enum keys *k);
int rk_termsave (const char *const file_name);
int rk_termrestore (const char *const file_name);
int rk_mytermregime (int8_t regime, int8_t vtime, int8_t vmin, int8_t echo,
                     int8_t sigint);