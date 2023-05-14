#include "commons.h"
// система дифференциальных уравнений модели SEIR-D
double
dS_dt (double S, double E, double I, double R, double D)
{
  double N = S + E + I + R + D;
  return -C * (ALPHA_I * S * I + ALPHA_E * S * E) / N + GAMMA * R;
}

double
dE_dt (double S, double E, double I, double R, double D)
{
  double N = S + E + I + R + D;
  return C * (ALPHA_I * S * I + ALPHA_E * S * E) / N - (K + RO) * E;
}

double
dI_dt (double S, double E, double I, double R, double D)
{
  return K * E - BETA * I - MU * I;
}

double
dR_dt (double S, double E, double I, double R, double D)
{
  return BETA * I + RO * E - GAMMA * R;
}

double
dD_dt (double S, double E, double I, double R, double D)
{
  return MU * I;
}
