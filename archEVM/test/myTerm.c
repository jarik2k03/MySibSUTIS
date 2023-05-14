#include <myTerm/myTerm.h>

#include "../thirdparty/ctest.h"
#define SUCCESS 0
#define FAIL -1

CTEST (term, clrscr)
{
  int expCode = SUCCESS;
  int curCode = mt_clrscr ();
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (term, gotoXY)
{
  int expCode = SUCCESS;
  int curCode = mt_gotoXY (30, 20);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = mt_gotoXY (-3, 20); // каретка начинается с 0.
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (term, getscreensize)
{
  int row, col;
  int expCode = SUCCESS;
  int curCode = mt_getscreensize (&row, &col);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = mt_getscreensize (&row, NULL); // защита "от дурака".
  ASSERT_EQUAL (expCode, curCode);
}
