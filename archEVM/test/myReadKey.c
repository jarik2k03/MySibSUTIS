#include <myReadKey/myReadKey.h>

#include "../thirdparty/ctest.h"
#define SUCCESS 0
#define FAIL -1

CTEST (key, termmode)
{
  int expCode = SUCCESS;
  int curCode = rk_mytermregime (0, 0, 0, 1, 1);
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (key, termregime)
{
  int expCode = FAIL;
  int curCode = rk_mytermregime (-1, 0, 0, 1, 1); // от 0 до 1.
  ASSERT_EQUAL (expCode, curCode);
}