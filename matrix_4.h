#ifndef MATRIX_4_H
#define MATRIX_4_H


class MATRIX_4
{
public:
    MATRIX_4(double a11,double a12,double a13,double x,double a21,double a22,double a23,double y,double a31,double a32,double a33,double z);
    int Getdimensions();
    double GetXYZ();
    void inverse();
    void DisplayMAT();
    double *GetPara();
    double *transposition();
    int RankMAT();
    double DetMAT();//calculate the matrix value
    int InverseNumber();
    double *ExchangeRows();
    double *AdjointMatrix();
    double CalculateDet(double *array);

private:
    double A11,A12,A13,X,A21,A22,A23,Y,A31,A32,A33,Z;
    double A41,A42,A43,one;
    double MAT[16];
    double TransposMAT[16];
    double AdjoinMAT[16];
    double MAT_DET;
    double InverseMAT[16];
};

#endif // MATRIX_4_H
