#include "euler.h"
#include <commons/commons.h>
#include <math.h>
#include <stdio.h>

FILE *datafile;

int allowToWrite = 1;

void
euler_modified (int a, int b, int h, double *S, double *E, double *I,
                double *R, double *D)
{
  if (h == 0)
    h = 1;

  int n = (int)ceil ((b - a) / h) + 1;
  double s = *S, e = *E, i = *I, r = *R, d = *D;
  double si, ei, ii, ri, di;
  double s1, e1, i1, r1, d1;

  for (int k = 0; k < n; k++)
    {
      s1 = s + h * dS_dt (s, e, i, r, d);
      e1 = e + h * dE_dt (s, e, i, r, d);
      i1 = i + h * dI_dt (s, e, i, r, d);
      r1 = r + h * dR_dt (s, e, i, r, d);
      d1 = d + h * dD_dt (s, e, i, r, d);
      // printf ("\ts1:%.1f e1:%.1f i1:%.1f r1:%.1f d1:%.1f\n", s1, e1, i1, r1,
      //        d1);
      si = s + (h / 2) * (dS_dt (s, e, i, r, d) + dS_dt (s1, e1, i1, r1, d1));
      ei = e + (h / 2) * (dE_dt (s, e, i, r, d) + dE_dt (s1, e1, i1, r1, d1));
      ii = i + (h / 2) * (dI_dt (s, e, i, r, d) + dI_dt (s1, e1, i1, r1, d1));
      ri = r + (h / 2) * (dR_dt (s, e, i, r, d) + dR_dt (s1, e1, i1, r1, d1));
      di = d + (h / 2) * (dD_dt (s, e, i, r, d) + dD_dt (s1, e1, i1, r1, d1));
      // printf ("\tsi:%.1f ei:%.1f ii:%.1f ri:%.1f di:%.1f\n", si, ei, ii, ri,
      //        di);
      s = s1;
      e = e1;
      i = i1;
      r = r1;
      d = d1;
      if (allowToWrite)
        fprintf (datafile, "%d\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\n", k, s, e, i, r,
                 d);
    }
  *S = s;
  *E = e;
  *I = i;
  *R = r;
  *D = d;
  allowToWrite = 0;
}