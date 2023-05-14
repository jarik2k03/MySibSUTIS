#define MU 0.0188  // коэф. смертности от COVID-19
#define BETA 0.999 // скорость выздоровления заражённых случаев
#define RO 0.952 // скорость восстановления выявленных случаев
#define ALPHA_E                                                               \
  0.999 // коэф. заражения между бессимптомно-инфицированным и восприимчивым
        // населением
#define ALPHA_I                                                               \
  0.999 // коэф. заражения между инфицированным и восприимчивым населением
        // (социальные факторы)
#define K 0.042 // частота появления симптомов в открытых случаях
#define N0 2798170 // население Новосибирской области
#define E0 99 // начальное количество бессимптомно инфицированных
#define R0 24 // начальное количество вылеченных
#define GAMMA                                                                 \
  0 // скорость повторного заражения, раз (0 - устойчивый иммунитет)
#define C                                                                     \
  1 // ограничение на передвижения граждан (изначально - 1 + C_ISOL * (...),
    // сокращена до 1, т.к. C_ISOL = 0)
// N - вся популяция, S - восприимчивые, E - заражённые бессимптомные, I -
// инфицированные с симптомами, R - вылеченные, D - умершие

double dS_dt (double S, double E, double I, double R, double D);
double dE_dt (double S, double E, double I, double R, double D);
double dI_dt (double S, double E, double I, double R, double D);
double dR_dt (double S, double E, double I, double R, double D);
double dD_dt (double S, double E, double I, double R, double D);
