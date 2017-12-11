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
QVector<double> polyfit_RCS(QVector<QVector2D>Points)
{
    /*
     * Points是数据点
     * n是多项式项数
     * 返回拟合结果的各项系数，从低到高        y=a0+a1*x+a2*x^2+……+apoly_n*x^poly_n     a即是Toreturn
   */
    int poly_n=Points.length()-1;
    QVector<double>Toreturn;
    int n=Points.length();//获取点数
    int i,j;

   double *x =new double [n];
    double *y=new double [n];

  for(int k=0;k<n;k++)
    {
        x[k]=Points[k].x();
        y[k]=Points[k].y();
    }


    double *para=new double[poly_n+1];//系数
    double *tempx=new double[n];
    double *sumxx=new double [poly_n*2+1];
    double *tempy=new double [n];
    double *sumxy=new double [poly_n*2+1];
    double *ata=new double[(poly_n+1)*(poly_n+1)];
    for (i=0;i<n;i++)
         {
          tempx[i]=1;
          tempy[i]=y[i];
         }
     for (i=0;i<2*poly_n+1;i++)
         for (sumxx[i]=0,j=0;j<n;j++)
       {
        sumxx[i]+=tempx[j];
        tempx[j]*=x[j];
       }
     for (i=0;i<poly_n+1;i++)
        for (sumxy[i]=0,j=0;j<n;j++)
       {
        sumxy[i]+=tempy[j];
        tempy[j]*=x[j];
       }
     for (i=0;i<poly_n+1;i++)
         for (j=0;j<poly_n+1;j++)
      ata[i*(poly_n+1)+j]=sumxx[i+j];

    gauss_solve(poly_n+1,ata,para,sumxy);
    for(int q=0;q<poly_n+1;q++)
    {
        Toreturn.push_back(para[q]);
    }

//去除多余项目
    bool ok=true;
    QVector<double>tempvec;
    for(int k=Toreturn.length()-1;k>=0;k--)
    {
        if(abs(Toreturn[k])<0.1&&ok)//认为该系数无效
        {
            continue;
        }else
        {
            ok=false;
            tempvec.push_back(Toreturn[k]);
        }
    }

    delete [] tempx;
    delete [] sumxx;
    delete [] tempy;
    delete [] sumxy;
    delete [] ata;
    delete[]x;
   delete []y;
    delete [] para;

  //  Toreturn.clear();
 //   Toreturn=tempvec;
    return Toreturn;
}

void gauss_solve(int n,double A[],double x[],double b[])
{
    int i,j,k,r;
     double max;
     for (k=0;k<n-1;k++)
         {
          max=fabs(A[k*n+k]); /*find maxmum*/
          r=k;
          for (i=k+1;i<n-1;i++)
       if (max<fabs(A[i*n+i]))
          {
           max=fabs(A[i*n+i]);
           r=i;
          }
          if (r!=k)
      for (i=0;i<n;i++)         /*change array:A[k]&A[r]  */
          {
           max=A[k*n+i];
           A[k*n+i]=A[r*n+i];
           A[r*n+i]=max;
          }
          max=b[k];                    /*change array:b[k]&b[r]     */
          b[k]=b[r];
          b[r]=max;
          for (i=k+1;i<n;i++)
       {
        for (j=k+1;j<n;j++)
            A[i*n+j]-=A[i*n+k]*A[k*n+j]/A[k*n+k];
        b[i]-=A[i*n+k]*b[k]/A[k*n+k];

       }
         }

     for (i=n-1;i>=0;x[i]/=A[i*n+i],i--)
         for (j=i+1,x[i]=b[i];j<n;j++)
      x[i]-=A[i*n+j]*x[j];
}





