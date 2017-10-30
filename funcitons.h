#include"mainwindow.h"
#include"matrix_4.h"

#include <opencv2/opencv.hpp>
#ifndef FUNCITONS_H
#define FUNCITONS_H
#pragma once
//here to write some funcitons

double DisCalFuc(int x1,int y1,int x2,int y2);

QVector<int> ReorderArray(QVector<int> Array, int n);

void ReorderArray(double *p,int n);

void SwapT(double &a,double &b);
void SwapT_int_int(int &a,int &b);
bool comparison(double x,double y);

void SmoothFunctions(QVector<QVector2D> SF);



QVector<QVector2D> DerectionCalFunc(QVector<QVector2D> DCF);//计算传入的矩阵的两点间方向

double SingelSlopeCalculate(QVector2D P2, QVector2D zero);

QVector<double> Slope(QVector<QVector2D> S,int d=1,bool iscriecal=false);//计算斜率

QVector<int> SimplifySlope(QVector<double> S_Slope, QVector<int> BP);//斜率精简
QVector<QVector2D> SimplifySlope(QVector<double> S_Slope, QVector<QVector2D> inputP);//斜率精简2
QVector<int> SimplifySlope(QVector<double>S_Slope, QVector<int> BP);
QVector<double> Distance(QVector<QVector2D> Into, int mode);//计算全部距离

QVector<int> CheckPointInline(QVector<int>BP, int Pcount, QVector<double>TSlope,
                              QVector<QVector2D> OOL, QVector<int> BreakP, int MinL=10);
QVector<int>GetBreakPoints(QVector<QVector2D>Dec,QVector<QVector2D>Outline);

QVector<QVector2D> CurveCheck(QVector<QVector2D>Outline, QVector<int>P, bool isgetcurve=false);


QVector<QVector3D>PeriodDetection(QVector<int> PD);

QVector<QVector3D>PeriodDetection(QVector<double> PD);


QVector<QVector3D>Curvature(QVector<QVector2D> OUTLINE);


int P2Pcalculate(int pnum);

QVector<QVector2D> Performance( QVector<double> change);

QVector<QVector2D> HoughTransform(QImage OutlineImage,int PointCount,int minmumLine=10);

QImage cvMat2QImage(const cv::Mat& mat);

cv::Mat QImage2cvMat(QImage image);

void Output2File(QVector<QVector2D>InputArray,QString Outputadd="");
void Output2File(QVector<QVector4D>InputArray,QString Outputadd="");
QVector<QVector2D> PointReorder(QVector<QVector2D>input,QVector<QVector2D>templateArray);
QVector<int> PointReorder_Rint(QVector<QVector2D>input,QVector<QVector2D>templateArray);
QVector<int> LineMerge(QVector<int> input_int,QVector<QVector2D>input_Point, QVector<QVector2D> allp, QVector<int> BreakP, int minL);

void Find_Center(QVector<QVector2D>Circle, QVector<double>cent, double radiuss=0);

QVector<QVector2D> CircularStitching(QVector<QVector2D>Line1,QVector<QVector2D>Line2);
QVector<double> PointToLineDis(QVector<QVector2D> LinePoint, QVector<QVector2D>Points);
#endif // FUNCITONS_H
