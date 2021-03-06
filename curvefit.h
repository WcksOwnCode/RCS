#ifndef CURVEFIT_H
#define CURVEFIT_H
#include<QVector>
#include<cmath>
#include<QDebug>
#include<QVector2D>




//functions
double N(int k, int i, double u);
double N1(int i, double u);
double N2(int i, double u);
double N3(int i, double u);   // 三次B样条的基函数

//最小二乘法曲线拟合

QVector<double> polyfit(QVector<QVector2D>Points);
void gauss_solve(int n,double A[],double x[],double b[]);


//Fitting the curve with least squares
QVector<QVector2D> LeastSquaresMethod(QVector<QVector2D> LSM);
QVector<QVector2D> B_spline(QVector<QVector2D> B_line);
#endif // CURVEFIT_H
