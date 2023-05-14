#include <commons/commons.h>
#include <euler/euler.h>
#include <math.h>
#include <stdio.h>

#define GREEN "\033[1;32m"
#define WHITE "\033[1;30m"

extern FILE *datafile;
int
main ()
{
  // начальные данные
  int a = 0, b = 90;
  double eps = 1e-2, h = 1;
  double e = E0, i = 0, r = R0, d = 0;
  double s = N0 - i - e - r - d;

  datafile = fopen ("exp/data.dat", "a");
  printf ("Результаты метода Эйлера-Коши (метода Эйлера с пересчётом):\n");
  printf ("%sвсё население %sN0 = %d \n", GREEN, WHITE, N0);
  printf ("%sвосприимчивое население %sS0 = %.0f \n", GREEN, WHITE, s);
  printf ("%sбессимптомно-инфицированное население %sE0 = %.0f \n", GREEN,
          WHITE, e);
  printf ("%sвыявленные или инфицированные с симптомами %sI0 = %.0f \n", GREEN,
          WHITE, i);
  printf ("%sвылечившиеся %sR0 = %d \n", GREEN, WHITE, R0);
  printf ("%sумершие %sD0 = %.0f \n", GREEN, WHITE, d);
  printf ("промежуток [%d;%d] с шагом %.1f\n", a, b, h);
  euler_modified (a, b, h, &s, &e, &i, &r, &d); // первая итерация

  double delta = 1, d1 = d;
  while (delta > eps)
    {
      h = h / 2;
      e = E0, i = 0, r = R0, d = 0, s = N0 - i - e - r - d;
      euler_modified (a, b, h, &s, &e, &i, &r, &d);
      delta = fabs (d - d1);
      d1 = d;
      // printf ("Delta = %f; H = %f\n", delta, h);
    }

  printf ("Результаты метода Эйлера-Коши (метода Эйлера с пересчётом):\n");
  printf ("%sвсё население %sN0 = %d \n", GREEN, WHITE, N0);
  printf ("%sвосприимчивое население %sS0 = %.0f \n", GREEN, WHITE, s);
  printf ("%sбессимптомно-инфицированное население %sE0 = %.0f \n", GREEN,
          WHITE, e);
  printf ("%sвыявленные или инфицированные с симптомами %sI0 = %.0f \n", GREEN,
          WHITE, i);
  printf ("%sумершие %sD0 = %.0f \n", GREEN, WHITE, d);
  double n = s + e + i + r + d;
  printf ("ИТОГО (N)= %.0f = %d (N0)\n", n, N0);

  fclose (datafile);
  return 0;
}