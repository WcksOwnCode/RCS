#include "matrix_4.h"
#include<qDebug>

#include<math.h>
#include<stddef.h>
#include<iostream>
#include<iomanip>
MATRIX_4::MATRIX_4(double a11,double a12,double a13,double x,double a21,double a22,double a23,double y,double a31,double a32,double a33,double z)
{
    A11=a11;
    A12=a12;
    A13=a13;
    X=x;
    A21=a21;
    A22=a22;
    A23=a23;
    Y=y;
    A31=a31;
    A32=a32;
    A33=a33;
    Z=z;
    A41=0;
    A42=0;
    A43=0;
    one=1;
    /**/
    MAT[0]=A11;
    MAT[1]=A12;
    MAT[2]=A13;
    MAT[3]=X;
    MAT[4]=A21;
    MAT[5]=A22;
    MAT[6]=A23;
    MAT[7]=Y;
    MAT[8]=A31;
    MAT[9]=A32;
    MAT[10]=A33;
    MAT[11]=Z;
    MAT[12]=0;
    MAT[13]=0;
    MAT[14]=0;
    MAT[15]=1;
    MAT_DET=this->DetMAT();
   // this->AdjointMatrix();
   // this->inverse();


}
int MATRIX_4::Getdimensions()
{
    return 4;
}
double MATRIX_4::GetXYZ()
{
    return X,Y,Z;
}


void MATRIX_4::inverse()
{
    for(int i=0;i<16;i++)
    {

    InverseMAT[i]=AdjoinMAT[i]/MAT_DET;

    }
    std::cout<<"Inverse MATRIX:"<<std::endl;

     for(int i=0;i<16;i++)
     {
         std::cout<<std::setw(10)<<InverseMAT[i];
         if(i==3||i==7||i==11)
         {
             std::cout<<std::endl;
         }
     }
     std::cout<<std::endl;


}









void MATRIX_4::DisplayMAT()
{
    std::cout<<"MAT:"<<std::endl;

    for(int i=0;i<16;i++)
    {
        std::cout<<std::setw(10)<<MAT[i];
        if(i==3||i==7||i==11)
        {
            std::cout<<std::endl;
        }
    }
std::cout<<std::endl;
}
double* MATRIX_4::GetPara()
{
    double *N=MAT;
    return N;
    delete N;
    N=NULL;
}
double *MATRIX_4::transposition()
{

    TransposMAT[0]=MAT[0];
    TransposMAT[1]=MAT[4];
    TransposMAT[2]=MAT[8];
    TransposMAT[3]=MAT[12];
    TransposMAT[4]=MAT[1];
    TransposMAT[5]=MAT[5];
    TransposMAT[6]=MAT[9];
    TransposMAT[7]=MAT[13];
    TransposMAT[8]=MAT[2];
    TransposMAT[9]=MAT[6];
    TransposMAT[10]=MAT[10];
    TransposMAT[11]=MAT[14];
    TransposMAT[12]=MAT[3];
    TransposMAT[13]=MAT[7];
    TransposMAT[14]=MAT[11];
    TransposMAT[15]=MAT[15];





    double *p=TransposMAT;
    return p;
    delete p;

}

int MATRIX_4::RankMAT()//to calculate the rank of this matrix
{
    void Exchange_rows();
    int Storepoint;
    for(int i=0;i<4;i++)//to find the first element which is not equeal 0;
    {
        if(MAT[i*4]!=0)
        {
            Storepoint=i*4;
            break;
        }
    }



    return 1;
}
double MATRIX_4::DetMAT()
{
    double det=0;
    double temp[4][4];
    for(int i=0;i<4;i++)//convert the to 4 * 4 Matrix
    {
        for(int j=0;j<4;j++)
        {
            temp[i][j]=MAT[i*4+j];
        }
    }
   /* for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
           qDebug()<< temp[i][j]<<"  ";
        }
    }*/

    //to get the cofactor
    double B00[3][3]={{temp[1][1],temp[1][2],temp[1][3]},{temp[2][1],temp[2][2],temp[2][3]},{temp[3][1],temp[3][2],temp[3][3]}};
    double B01[3][3]={{temp[1][0],temp[1][2],temp[1][3]},{temp[2][0],temp[2][2],temp[2][3]},{temp[3][0],temp[3][2],temp[3][3]}};
    double B02[3][3]={{temp[1][0],temp[1][1],temp[1][3]},{temp[2][0],temp[2][1],temp[2][3]},{temp[3][0],temp[3][1],temp[3][3]}};
    double B03[3][3]={{temp[1][0],temp[1][1],temp[1][2]},{temp[2][0],temp[2][1],temp[2][2]},{temp[3][0],temp[3][1],temp[3][2]}};

    double B00_value=B00[0][0]*B00[1][1]*B00[2][2]+B00[0][1]*B00[1][2]*B00[2][0]+B00[1][0]*B00[2][1]*B00[0][2];
    B00_value=B00_value-B00[0][2]*B00[1][1]*B00[2][0]-B00[0][1]*B00[1][0]*B00[2][2]-B00[1][2]*B00[2][1]*B00[0][0];
    double B01_value=B01[0][0]*B01[1][1]*B01[2][2]+B01[0][1]*B01[1][2]*B01[2][0]+B01[1][0]*B01[2][1]*B01[0][2];
    B01_value=B01_value-B01[0][2]*B01[1][1]*B01[2][0]-B01[0][1]*B01[1][0]*B01[2][2]-B01[1][2]*B01[2][1]*B01[0][0];
    double B02_value=B02[0][0]*B02[1][1]*B02[2][2]+B02[0][1]*B02[1][2]*B02[2][0]+B02[1][0]*B02[2][1]*B02[0][2];
    B02_value=B02_value-B02[0][2]*B02[1][1]*B02[2][0]-B02[0][1]*B02[1][0]*B02[2][2]-B02[1][2]*B02[2][1]*B02[0][0];
    double B03_value=B03[0][0]*B03[1][1]*B03[2][2]+B03[0][1]*B03[1][2]*B03[2][0]+B03[1][0]*B03[2][1]*B03[0][2];

    B03_value=B03_value-B03[0][2]*B03[1][1]*B03[2][0]-B03[0][1]*B03[1][0]*B03[2][2]-B03[1][2]*B03[2][1]*B03[0][0];

  /*  for(int i=0;i<3;i++){
        for(int j=0;j<3;j++)
        {
            qDebug()<<B03[i][j]<<" ";
        }
    }*/

    det=B00_value*temp[0][0]-B01_value*temp[0][1]+B02_value*temp[0][2]-B03_value*temp[0][3];//to get the det
   /* qDebug()<<"B00_value: "<<B00_value<<endl;
    qDebug()<<"B01_value: "<<B01_value<<endl;
    qDebug()<<"B02_value: "<<B02_value<<endl;
    qDebug()<<"B03_value: "<<B03_value<<endl;*/
    std::cout<<"det: "<<std::endl;
    return det;


}
int MATRIX_4::InverseNumber()
{
    int k=0;
    return k;
}
double *MATRIX_4::ExchangeRows(){
double *p =new double;
return p;
delete p;
p=NULL;
}





double * MATRIX_4::AdjointMatrix()
{
    double* p= AdjoinMAT;
    if(MAT_DET==0){
        return false;
    }
    double temp[4][4];
    for(int i=0;i<4;i++)//put MAT to temp array
    {
        for(int j=0;j<4;j++)
        {
            temp[i][j]=MAT[i*4+j];
        }
    }
    for(int k=0;k<4;k++)//to get the adjoint matrix
    {
        for(int m=0;m<4;m++)
        {
            double Store[3][3];
            int row=0;
            int col=0;
            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    if(i==k||j==m){
                        continue;
                    }
                    else
                        {
                            Store[row][col]=temp[i][j];
                            if(col!=2)
                            {
                                 col++;
                            }
                            else
                            {
                                col=0;
                                row++;
                            }
                        }
                }
            }
/*to check store array*/

          /*  for(int i=0;i<3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    std::cout<<std::setw(10)<<Store[i][j];

                }
                std::cout<<std::endl;
            }*/












            double *sto=new double[9];
            for(int i=0;i<3;i++)
            {
                for(int j=0;j<3;j++)
                {
                    sto[i*3+j]=Store[i][j];
                }
            }
        p[k*4+m]=pow(-1,m+k)*CalculateDet(sto);
        }

    }
    double transtemp[4][4];
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++)
        {
            transtemp[i][j]=p[i*4+j];
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++)
        {
            temp[j][i]=transtemp[i][j];
        }
    }

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++)
        {
            p[i*4+j]=temp[i][j];
        }
    }





std::cout<<"bansuijuzhen:"<<std::endl;
for(int i=0;i<16;i++)
{
    std::cout<<std::setw(10)<<p[i];
    if(3==i||7==i||11==i)
      {
        std::cout<<std::endl;
      }
}
std::cout<<std::endl<<std::endl;

for(int i=0;i<16;i++)
{
   AdjoinMAT[i]=p[i];
}

    return p;
    delete p;
    p=NULL;

}

double MATRIX_4::CalculateDet(double *array)
{
    double temp[3][3];
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
           temp[i][j]=array[i*3+j];
        }

    }
  double answer=temp[0][0]*temp[1][1]*temp[2][2]+temp[0][1]*temp[1][2]*temp[2][0]+temp[1][0]*temp[2][1]*temp[0][2];
  answer=answer-temp[0][2]*temp[1][1]*temp[2][0]-temp[0][1]*temp[1][0]*temp[2][2]-temp[1][2]*temp[2][1]*temp[0][0];
  //qDebug()<<"Ansewer:"<<answer<<endl;
  return answer;


}

