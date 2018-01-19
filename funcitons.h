#include"mainwindow.h"
#include"matrix_4.h"

#include <opencv2/opencv.hpp>
#ifndef FUNCITONS_H
#define FUNCITONS_H
#pragma once
//here to write some funcitons

double DisCalFuc(int x1,int y1,int x2,int y2);

float TwoPointdis(float x1, float y1, float x2, float y2);

QVector<int> ReorderArray(QVector<int> Array, int n);
QVector<double> ReorderArray(QVector<double> Array, int n);

int FindMinorMax(QVector<int>input,int M=0);
int FindMinorMax(QVector<double>input,int M=0);
void ReorderArray(double *p,int n);

void SwapT(double &a,double &b);
void SwapT_int_int(int &a,int &b);
bool comparison(double x,double y);

void SmoothFunctions(QVector<QVector2D> SF);

QVector<int> LineMake(QVector<int>input_int,QVector<QVector2D>input_Point,
                       QVector<QVector2D>allp,QVector<int>&KeyP,int MinL);

QVector<QVector2D> DerectionCalFunc(QVector<QVector2D> DCF);//计算传入的矩阵的两点间方向

double SingelSlopeCalculate(QVector2D P2, QVector2D zero);

QVector<double> Slope(QVector<QVector2D> S,int d=1,bool iscriecal=false);//计算斜率

QVector<int> SimplifySlope(QVector<double> S_Slope, QVector<int> BP);//斜率精简
QVector<QVector2D> SimplifySlope(QVector<double> S_Slope, QVector<QVector2D> inputP);//斜率精简2
QVector<int> SimplifySlope(QVector<double>S_Slope, QVector<int> BP);
QVector<double> Distance(QVector<QVector2D> Into, int mode);//计算全部距离
QVector<int>  FindKeypoints(QVector<int>BP,  QVector<QVector2D> OOL, int MinL, QVector<int> &ReturnKey);
QVector<int> CheckPointInline(QVector<int>BP,
                              QVector<QVector2D> OOL, QVector<int> BreakP, int MinL);
QVector<int>GetBreakPoints(QVector<QVector2D>Dec,QVector<QVector2D>Outline);

QVector<int> CurveCheck(QVector<QVector2D>CurP_2D, QVector<int>CurP_int,
                              QVector<int>keyP_int, QVector<QVector2D>keyP_2D,
                              QVector<QVector2D> All, int MinL);

QVector<int> CurveDisperce(QVector<QVector2D>AllOutline,
                                QVector2D CurveEndpoints,int MinL=10);

QVector<QVector3D>PeriodDetection(QVector<int> PD);

QVector<QVector3D>PeriodDetection(QVector<double> PD);


QVector<QVector3D>Curvature(QVector<QVector2D> OUTLINE);


QVector<int> InsertCalculate(int startP, int endP, int maxor, int MinL);

QVector<QVector2D> Performance( QVector<double> change);

QVector<QVector2D> HoughTransform(QImage OutlineImage,int PointCount,int minmumLine);

QImage cvMat2QImage(const cv::Mat& mat);

cv::Mat QImage2cvMat(QImage image);

void Output2File(QVector<QVector2D>InputArray,QString Outputadd="");
void Output2File(QVector<QVector4D>InputArray,QString Outputadd="");
void Output2File(QVector<int>InputArray,QString Outputadd="",int newline=1);
void Output2File(QVector<double>InputArray,QString Outputadd="",int newline=1);
void Output2File(QVector<QPointF> InputArray,QString Outputadd="");



QVector<QVector2D> PointReorder(QVector<QVector2D>input, QVector<QVector2D>templateArray, int MinL);
QVector<int> PointReorder_Rint(QVector<QVector2D>input, QVector<QVector2D>templateArray, int MinL);
QVector<int> LineMerge(QVector<int> input_int, QVector<QVector2D>input_Point,
                       QVector<QVector2D> allp, QVector<int> BreakP, bool bisect,
                       QVector<int> & KeyP, int MinL);

QVector2D Find_Center(QVector<QVector2D>Circle, QVector<double>cent, double radiuss=0);

QVector<QVector2D> CircularStitching(QVector<QVector2D>Line1,QVector<QVector2D>Line2);
QVector<double> PointToLineDis(QVector<QVector2D> LinePoint, QVector<QVector2D>Points);

double PointToLineDis(QVector<QVector2D> LinePoint, QVector2D Points);

int AngelCompare(double slope1,double slope2,double tolerance);

bool AngelCompare(QVector<double>Asl, int spot, QVector<double> BSlope, double tolerance);


QVector<double> DistanceEvaluate(QVector<double>dis);
QVector<QVector2D>TransSequenceTo2D(QVector<QVector2D>Alloutline,QVector<int>input);
QVector<int>TransSequence2D_ToInt(QVector<QVector2D>Alloutline,QVector<QVector2D>input2D);
QVector<int>Unique_Int(QVector<int>Input);
QVector<QVector2D>Unique_2D(QVector<QVector2D>Input);
QVector<QVector2D>VectorTransposition(QVector<QVector2D>point_2D,int Transpos);

void QimageSave(QImage Tosave, QString Name, int width=800, int height=600);

QVector<QVector2D>KeyPointFilter_RCS(QVector<QVector2D>KP, QVector<QVector2D> ALL, int Gap);

bool IsEvenNumber(int input);

double SumofArray(QVector<double>inputarray);

QVector<int>VecAddVec(QVector<int>MainV,QVector<int>lowV,int n=1);

double FindMidValue(QVector<double>Array, int sort, int Ratio);


int FindMidValue(QVector<int>Array, int sort, int Ratio);
#endif // FUNCITONS_H
