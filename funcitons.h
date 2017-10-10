#include"mainwindow.h"
#include"matrix_4.h"
#ifndef FUNCITONS_H
#define FUNCITONS_H
//here to write some funcitons

double DisCalFuc(int x1,int y1,int x2,int y2);

void ReorderArray(int *p,int length,int n);

void ReorderArray(double *p,int n);

void SwapT(double &a,double &b);

bool comparison(double x,double y);

void SmoothFunctions(QVector<QVector2D> SF);

QVector<QVector2D> DerectionCalFunc(QVector<QVector2D> DCF);//计算传入的矩阵的两点间方向

double SingelSlopeCalculate(QVector2D SSC1, QVector2D SSC2);

QVector<double> Slope(QVector<QVector2D> S,int d=1);//计算斜率

QVector<int> SimplifySlope(QVector<double> S_Slope, QVector<int> BP);//斜率精简

QVector<double> Distance(QVector<QVector2D> Into);//计算全部距离

QVector<int> CheckPointInline(QVector<double> dis, QVector<QVector2D> slope, QVector<int> BP,QVector<QVector2D>OOL);

QVector<int>GetBreakPoints(QVector<QVector2D>Dec,QVector<QVector2D>Outline);

QVector<QVector2D> CurveCheck(QVector<QVector2D>Outline, QVector<int>P, bool isgetcurve=false);


QVector<QVector3D>PeriodDetection(QVector<int> PD);

QVector<QVector3D>PeriodDetection(QVector<double> PD);


QVector<QVector3D>Curvature(QVector<QVector2D> OUTLINE);


int P2Pcalculate(int pnum);

QVector<QVector2D> Performance( QVector<double> change);

void HoughTransform(QVector<QVector2D> Outlines);

QImage cvMat2QImage(const cv::Mat& mat);

cv::Mat QImage2cvMat(QImage image);

#endif // FUNCITONS_H
