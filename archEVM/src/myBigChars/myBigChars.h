#pragma once

int bc_printA (char str);
int bc_box (int x1, int y1, int x2, int y2);
int bc_printNL ();
int bc_printUB (int len);
int bc_printLB (int len);
int bc_printES (int len);
int bc_printbigchar (int a[2], int x, int y, int front, int back);
int bc_setbigcharpos (int *big, int x, int y, int value);
int bc_getbigcharpos (int *big, int x, int y, int *value);
int bc_bigcharwrite (int fd, int *big, int count);
int bc_bigcharread (int fd, int *big, int need_count, int *count);
