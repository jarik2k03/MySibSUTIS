#include <mySimpleComputer/mySimpleComputer.h>

#include "../thirdparty/ctest.h"
#define SUCCESS 0
#define FAIL -1

CTEST (memory, init)
{
  int expCode = SUCCESS;
  int curCode = sc_init ();
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (memory, get)
{
  int val = 24;

  int expCode = SUCCESS;
  int curCode = sc_memoryGet (50, &val);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = sc_memoryGet (110, &val); // попыткаобращения
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (reg, get)
{
  int val = 1;
  int expCode = SUCCESS;
  int curCode = sc_regGet (1, &val);
  ASSERT_EQUAL (expCode, curCode);

  expCode = FAIL;
  curCode = sc_regGet (1, NULL); // защита "от дурака".
  ASSERT_EQUAL (expCode, curCode);

  val = 0;
  expCode = FAIL;
  curCode = sc_regGet (7, &val); // диапазон разрядов от 0 до 5.
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (command, encode)
{
  int command = 41, operand = 51;
  int value = 0;
  int expValue = 5299;

  int expCode = SUCCESS;
  int curCode = sc_commandEncode (command, operand, &value);
  ASSERT_EQUAL (expCode, curCode);
  ASSERT_EQUAL (expValue, value); // закодированная команда.

  command = 27;
  expCode = FAIL;
  curCode = sc_commandEncode (command, operand,
                              &value); // не существует такой команды.
  ASSERT_EQUAL (expCode, curCode);

  command = 20;
  operand = 130;
  expCode = FAIL;
  curCode
      = sc_commandEncode (command, operand, &value); // операнд превышает 0x7f.
  ASSERT_EQUAL (expCode, curCode);
}

CTEST (command, decode)
{
  int expCommand = 41, expOperand = 51;
  int command = 0, operand = 0;
  int value = 5299;

  int expCode = SUCCESS;
  int curCode = sc_commandDecode (value, &command, &operand);
  ASSERT_EQUAL (expCode, curCode);
  ASSERT_EQUAL (expCommand, command); // раскодированная команда.
  ASSERT_EQUAL (expOperand, operand); // полученный операнд.

  command = 27;
  expCode = FAIL;
  curCode = sc_commandDecode (value, NULL, &operand); // защита от "дурака".
  ASSERT_EQUAL (expCode, curCode);
}
