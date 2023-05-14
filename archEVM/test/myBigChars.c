#include <myBigChars/myBigChars.h>

#include "../thirdparty/ctest.h"
#define SUCCESS 0
#define FAIL -1

CTEST (chars, printA)
{
  int expCode = SUCCESS;
  int curCode = bc_printA ('k');
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (chars, bc_box)
{
  int expCode = SUCCESS;
  int curCode = bc_box (0, 0, 12, 45);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_box (-1, 0, -12, 45); // только положительные координаты.
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (chars, bc_setbigcharpos)
{
  int big[2] = { 0x6080817E, 0x7E818060 };
  int expCode = SUCCESS;
  int curCode = bc_setbigcharpos (big, 6, 4, 0);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_setbigcharpos (big, 10, -1, 1); // диапазон бит в пределах 8.
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_setbigcharpos (big, 2, 2, 6); // значение от 0 до 1
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_setbigcharpos (NULL, 2, 2, 6); // необходим символ.
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (chars, bc_getbigcharpos)
{
  int big[2] = { 0x6080817E, 0x7E818060 };
  int value;
  int expCode = SUCCESS;
  int curCode = bc_getbigcharpos (big, 6, 4, &value);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_getbigcharpos (big, 6, 4, NULL); // нулль нельзя передавать.
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_getbigcharpos (big, 20, 10, &value); // диапазон бит до 8.
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = bc_getbigcharpos (NULL, 4, 3, &value); // большой символ требуется.
  ASSERT_EQUAL (expCode, curCode);
}
