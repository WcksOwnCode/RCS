#include"curvefit.h"

QVector<QVector2D> LeastSquaresMethod(QVector<QVector2D> LSM)
{
    int length=LSM.length();
    return LSM;
}
QVector<QVector2D> B_spline(QVector<QVector2D> B_line)
{
    int length=B_line.length();
    return B_line;
}
double N(int k, int i, double u)
{
    switch(k)
    {
    case 1:
        return N1(i,u);
    case 2:
        return N2(i,u);
    case 3:
        return N3(i,u);
    }
}

double N1(int i, double u)
{
    double t = u-i;

    if(0<=t && t<1)
        return t;
    if(1<=t && t<2)
        return 2-t;
    return 0;
}

double N2(int i, double u)
{
    double t = u-i;

    if(0<=t && t<1)
        return 0.5*t*t;
    if(1<=t && t<2)
        return 3*t-t*t-1.5;
    if(2<=t && t<3)
        return 0.5*pow(3-t,2);
    return 0;
}

double N3(int i, double u)
{
    double t = u-i;
    double a = 1.0/6.0;

    if(0<=t && t<1)
        return a*t*t*t;
    if(1<=t && t<2)
        return a*(-3*pow(t-1,3) + 3*pow(t-1,2) + 3*(t-1) +1);
    if(2<=t && t<3)
        return a*(3*pow(t-2,3) - 6*pow(t-2,2) +4);
    if(3<=t && t<4)
        return a*pow(4-t,3);
    return 0;
}
/************************************************************************************************/
/*
 *
 *
 * *************************************************************************************************/
//最小二乘法函数
