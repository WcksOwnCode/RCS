/**************************************************************************************************/

/*此处存放的函数主要是用来计算曲线特征点的函数，为了在mainwindow里看起来简单一些
 * 主要是距离计算  斜率计算  斜率简化 等函数组成，目的在于找到曲线行走必须的点
 *
/**************************************************************************************************/
#include "funcitons.h"
#include <cmath>
#include <QMessageBox>
#include"mainwindow.h"
#include"worldvalues.h"
#include<QDatetime>
int FindMinorMax(QVector<int>input,int M)
{
    /*
     * M默认0，返回较小值的脚标
     * 如果M=1，返回最大值的脚标
     */


    int min=input[0];
    int max=input[0];
    int minspot=0;
    int maxspot=0;
    for(int i=0;i<input.length();i++)
    {
        if(input[i]<=min)
        {
            min=input[i];
            minspot=i;
        }
        if(input[i]>=max)
        {
            max=input[i];
            maxspot=i;
        }
    }

    if(M==0){
        return minspot;
    }
    else{
        return maxspot;
    }

}
float TwoPointdis(float x1,float y1,float x2,float y2)
{
    float toreturn;

    toreturn=std::sqrt(std::pow(y2-y1,2)+std::pow(x2-x1,2));


    return toreturn;

}

double DisCalFuc(int x1, int y1, int x2, int y2)
{
    double re=0;
    re=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
    return re;
}
QVector<int> ReorderArray(QVector<int>p, int n)
{

    int length=p.length();

    if(n==0){

        //from maxmum to minmum

        for(int i=0;i<length;i++)
        {
            for(int j=i;j<length;j++)
            {

                if(p[i]<p[j])
                {

                    SwapT_int_int(p[i],p[j]);
                }

            }

        }
    }
    else if(n==1)
    {
        //from minmum to maxmum

        for(int i=0;i<length;i++)
        {
            for(int j=i;j<length;j++)
            {
                if(p[i]>p[j])
                {
                    SwapT_int_int(p[i],p[j]);
                }
            }

        }
    }
    else if(n!=1&&n!=0)
    {
        QMessageBox::information(NULL,"notice!","the third parameter is inappropriate!");
        exit(0);
    }
    return p;
}
QVector<double> ReorderArray(QVector<double> Array, int n)
{
    int length= Array.length();

    if(n==0){

        //from maxmum to minmum

        for(int i=0;i<length;i++)
        {
            for(int j=i;j<length;j++)
            {

                if( Array[i]< Array[j])
                {

                    SwapT( Array[i], Array[j]);
                }

            }

        }
    }
    else if(n==1)
    {
        //from minmum to maxmum

        for(int i=0;i<length;i++)
        {
            for(int j=i;j<length;j++)
            {
                if( Array[i]> Array[j])
                {
                    SwapT( Array[i], Array[j]);
                }
            }

        }
    }
    else if(n!=1&&n!=0)
    {
        QMessageBox::information(NULL,"notice!","the third parameter is inappropriate!");
        exit(0);
    }
    return  Array;
}

void SwapT(double &a,double &b)
{
    double x;
    x=a;
    a=b;
    b=x;
}
void SwapT_int_int(int &a,int &b)
{
    int x;
    x=a;
    a=b;
    b=x;
}
bool comparison(double x, double y)
{
    return x>=y?true:false;
}
void SmoothFunctions(QVector<QVector2D> SF)
{
    //N   给定点数，函数设定N一定等于5
    //  double y[n] 存放n个等距观测点上的观测数据
    //  doulilc yy[n]   返回n个等距观测点上的平滑结果
    //  void kspt() 过程
    //step1
    //五等分所有数据
    int length=SF.length();
    //double y[5];
    // double *d_y=new double[length];
    if(length%5==0)
    {//恰好是5的倍数个点
        int q=length/5;
        for(int i=0;i<q;i++)
        {
            for(int j=i*5;j<5*(i+1);j++);
        }
    }
    /*  if (n<5)
    {
        for (int i=0; i<= n-1; i++)
            yy[i]=y[i];
    }
    else
    {
        yy[0] = 69.0*y[0] + 4.0*y[1] - 6.0*y[2] + 4.0*y[3] - y[4];
        yy[0]=yy[0]/70.0;
        yy[1]=2.0*y[0]+27.0*y[1]+12.0*y[2]-8.0*y[3];
        yy[1]=(yy[1]+2.0*y[4])/35.0;

        for (int i=2; i<=n-3; i++)
        {
            yy[i]=-3.0*y[i-2]+12.0*y[i-1]+17.0*y[i];
            yy[i]=(yy[i]+12.0*y[i+1]-3.0*y[i+2])/35.0;
        }
        yy[n-2]=2.0*y[n-5]-8.0*y[n-4]+12.0*y[n-3];
        yy[n-2]=(yy[n-2]+27.0*y[n-2]+2.0*y[n-1])/35.0;
        yy[n-1]=-y[n-5]+4.0*y[n-4]-6.0*y[n-3];
        yy[n-1]=(yy[n-1]+4.0*y[n-2]+69.0*y[n-1])/70.0;
    }
      */

}

QVector<QVector2D> DerectionCalFunc(QVector<QVector2D> DCF)
{
    bool outtotxt=false;
    QString outaddr="F:/output/Derections.txt";
    int length=DCF.length();
    if(length==0)
    {
        QMessageBox::information(NULL,"notice","length is zero!  (DRECTIONCalFunc)");
        return DCF;
    }

    QVector2D ThisDerec;
    QVector<QVector2D> num_Dere;
    for(int i=1;i<length;i++)
    {
        int tempsss=DCF[i].x()-DCF[i-1].x();
        int tempyyy=DCF[i].y()-DCF[i-1].y();
        switch (tempsss)
        {
        case -1:
            switch (tempyyy)
            {
            case -1:
                ThisDerec.setX(i);
                ThisDerec.setY(1);
                num_Dere.push_back(ThisDerec);
                break;
            case 0:
                ThisDerec.setX(i);
                ThisDerec.setY(8);
                num_Dere.push_back(ThisDerec);
                break;
            default:
                ThisDerec.setX(i);
                ThisDerec.setY(7);
                num_Dere.push_back(ThisDerec);
                break;
            }
            break;
        case 0:
            switch (tempyyy)
            {
            case -1:
                ThisDerec.setX(i);
                ThisDerec.setY(2);
                num_Dere.push_back(ThisDerec);
                break;
            case 1:
                ThisDerec.setX(i);
                ThisDerec.setY(6);
                num_Dere.push_back(ThisDerec);
                break;
            }
            break;
        default:
            switch (tempyyy)
            {
            case -1:
                ThisDerec.setX(i);
                ThisDerec.setY(3);
                num_Dere.push_back(ThisDerec);
                break;
            case 0:
                ThisDerec.setX(i);
                ThisDerec.setY(4);
                num_Dere.push_back(ThisDerec);
                break;
            default:
                ThisDerec.setX(i);
                ThisDerec.setY(5);
                num_Dere.push_back(ThisDerec);
                break;
            }
            break;
        }
    }
    if(outtotxt){
        QFile *outflie=new QFile;
        outflie->setFileName(outaddr);
        bool ok3=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
        if(ok3){
            QTextStream out(outflie);
            for(int i=0;i<num_Dere.length();i++){
                int tempint=num_Dere[i].y();
                QString outstr;
                outstr.append(QString::number(tempint));
                out<<outstr;
                out<<endl;
            }
            outflie->close();
        }
    }
    return num_Dere;
}
QVector<int>GetBreakPoints(QVector<QVector2D>Dec, QVector<QVector2D> Outline)
{
    QTime BPtime;
    BPtime.start();
    
    int length=Dec.length();
    if(length==0)
    {
        QMessageBox::information(NULL,"notice","length is zero!  (GetBreakPoints)");

    }
    bool outtotxt=false;
    QString outaddr="F:/output/Derections.txt";
    QVector<int> toreturn;
    for(int i=0;i<length;i++)
    {
        if(i>0)
        {
            if(Dec[i].y()!=Dec[i-1].y())
            {
                toreturn.push_back(i-1);
            }
        }
    }
    if(toreturn.length()!=0)
    {
        toreturn.push_back(toreturn[0]);
    }
    else{
        QMessageBox::information(NULL,"notice","Breakpoints length is zero!  (GetBreakPoints)");
    }

    if(outtotxt&&toreturn.length()!=0)
    {
        QFile *outflie=new QFile;
        outflie->setFileName(outaddr);
        bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
        if(ok){
            QTextStream out(outflie);
            for(int i=0;i<toreturn.length();i++){
                int tempint=Outline[toreturn[i]].x();
                QString outstr;
                outstr.append(QString::number(tempint));
                outstr.append(" ");

                tempint=Outline[toreturn[i]].y();
                outstr.append(QString::number(tempint));
                out<<outstr;
                out<<endl;
            }
            outflie->close();
            delete outflie;
        }
    }
    //去除相同的点，防止意外;
    QVector<int>::iterator end_unique=std::unique(toreturn.begin(),toreturn.end());
    toreturn.erase(end_unique,toreturn.end());
    qDebug()<<"get break point time used: "<<QString::number(BPtime.elapsed());
    return toreturn;
}
double SingelSlopeCalculate(QVector2D P2,QVector2D zero)
{
    //用于计算两点斜率用的
    //P2\zero分别是两个需要计算的点的坐标，都是二维的xy
    //zero为计算的原点
    //返回两点的斜率弧度
    //注意： atan函数返回-90到+90的弧度，要调整到0-360弧度

    double toreturn;
    if(P2.x()!=zero.x())//分母不为0
    {
        if(P2.y()!=zero.y())//分子不为0，为0就是水平
        {
            toreturn=atan(abs(P2.y()-zero.y())/abs(P2.x()-zero.x()));
            double RelativeX=P2.x()-zero.x();
            double RelativeY=P2.y()-zero.y();

            if(RelativeY>0&&RelativeX<0)//二象限
            {
                toreturn=CV_PI-toreturn;
            }
            else if(RelativeY<0&&RelativeX<0)//三象限
            {
                toreturn=CV_PI+toreturn;
            }
            else if(RelativeY<0&&RelativeX>0)//四象限
            {
                toreturn=CV_PI*2-toreturn;
            }

        }
        else
        {
            if(P2.x()>=zero.x())
            {
                toreturn=0.0;
            }
            else
            {
                toreturn=CV_PI;
            }

        }
    }
    else
    {
        if(P2.y()>=zero.y())
        {
            toreturn=CV_PI/2;
        }
        else if(P2.y()<zero.y())
        {
            toreturn=1.5*CV_PI;
        }

    }
    return toreturn;
}

QVector<double> Slope(QVector<QVector2D> S, int d, bool iscriecal)
{
    //S是传入的点阵序列，
    //d是间隔求取的的数量
    //iscircal,默认传入的点是围圆圈，若是true会计算一个起始点和末位点的斜率

    int length=S.length();

    //qDebug()<<"crash test length:  "<<length;

    bool outtotxt=false;

    QString outaddr="F:/output/slope.txt";

    double *d_slope;

    QVector<double> toreturn;

    if(length>=2){
        d_slope=new double[length-1];
    }
    else{
        //qDebug()<<"go else yet";
        return toreturn;
    }

    if(length>=2){

        for(int i=1;i<length;i=i+d)
        {
            d_slope[i-1]=SingelSlopeCalculate(S[i],S[i-1]);


        }
        for(int i=0;i<length-1;i++)
        {
            toreturn.push_back(d_slope[i]);
        }

        if(iscriecal){
            double lastslope=SingelSlopeCalculate(S[length-1],S[0]);
            toreturn.push_back(lastslope);
        }
    }



    if(outtotxt)
    {
        QFile *outflie=new QFile;
        outflie->setFileName(outaddr);
        bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
        if(ok){
            QTextStream out(outflie);
            for(int i=0;i<length-1;i++){
                double tempint=d_slope[i];
                QString outstr;
                outstr.append(QString::number(tempint));
                out<<outstr;
                out<<endl;
            }
            outflie->close();
            delete outflie;
        }
    }
    delete []d_slope;
    d_slope=NULL;
    return toreturn;
}

QVector<QVector2D> SimplifySlope(QVector<double>S_Slope, QVector<QVector2D> inputP)
{
    //Slope是计算出的斜率数据

    //inputP 输入的坐标点

    //返回简化后的坐标点序列

    //简化方式是进行斜率匹配

    int length=S_Slope.length();

    if(length<3)
    {
        QMessageBox::information(NULL,"notice","length is not enough!  (Simplelify the slopes)");

    }

    bool outtotxt=false;

    QString outaddr="F:/output/SimplifySlope.txt";

    QVector<int> toreturn;

    const double miss=4.0;//正负miss°的角度匹配值
    const double disthresh=3.0;

    QVector<double>inputPdis;

    inputPdis=Distance(inputP,0);//calculate all distance

    for(int i=0;i<inputPdis.length();i++)
    {
        if(inputPdis[i]<disthresh)
        {

        }
    }

    for(int i=0;i<length;i+=2)
    {

        if(abs(S_Slope[i])-abs(S_Slope[i+2])<=miss)
        {
            // toreturn.push_back(inputP[i-1]);

            //  toreturn.push_back(inputP[i+1]);
        }

        else

        {
            //  toreturn.push_back(inputP[i-1]);

            //   toreturn.push_back(inputP[i]);

            //  toreturn.push_back(inputP[i+1]);
        }
    }

    qSort(toreturn.begin(),toreturn.end());

    QVector<int>::iterator end_unique=std::unique(toreturn.begin(),toreturn.end());

    toreturn.erase(end_unique,toreturn.end());




    if(outtotxt)
    {
        QFile *outflie=new QFile;
        outflie->setFileName(outaddr);
        bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
        if(ok){
            QTextStream out(outflie);
            for(int i=0;i<length-1;i++){
                double tempintx=toreturn[i];//error

                QString outstr;
                outstr.append(QString::number(tempintx));
                out<<outstr;
                out<<endl;
            }
            outflie->close();
            delete outflie;
        }
    }

    return inputP;


}

QVector<int> SimplifySlope(QVector<double>S_Slope, QVector<int> BP)
{
    //Slope是计算出的斜率数据

    //BP对应的转折点

    //返回简化后的转折点(即特征点)

    //简化方式是进行斜率匹配

    int length=S_Slope.length();

    if(length<3)
    {
        QMessageBox::information(NULL,"notice","length is not enough!  (Simplelify the slopes)");

    }

    bool outtotxt=false;

    QString outaddr="F:/output/SimplifySlope.txt";

    QVector<int> toreturn;

    const double miss=4.0;//正负miss°的角度匹配值

    for(int i=1;i<length;i++)
    {

        if(abs(S_Slope[i])-abs(S_Slope[i]-1)<=miss)
        {
            toreturn.push_back(BP[i-1]);

            toreturn.push_back(BP[i+1]);
        }

        else

        {
            toreturn.push_back(BP[i-1]);

            toreturn.push_back(BP[i]);

            toreturn.push_back(BP[i+1]);
        }
    }

    qSort(toreturn.begin(),toreturn.end());

    QVector<int>::iterator end_unique=std::unique(toreturn.begin(),toreturn.end());

    toreturn.erase(end_unique,toreturn.end());

    if(outtotxt)
    {
        QFile *outflie=new QFile;
        outflie->setFileName(outaddr);
        bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
        if(ok){
            QTextStream out(outflie);
            for(int i=0;i<length-1;i++){
                double tempintx=toreturn[i];//error

                QString outstr;
                outstr.append(QString::number(tempintx));
                out<<outstr;
                out<<endl;
            }
            outflie->close();
            delete outflie;
        }
    }

    return toreturn;
}
QVector<double> Distance(QVector<QVector2D> Into,int mode=0)
{
    //into 是传入的点阵位置矩阵，求出两个点之间的距离
    int length=Into.length();
    if(length<3)
    {
        QMessageBox::information(NULL,"notice","length is not enough!  (Distance function)");

    }
    bool outtotxt=false;
    QString outaddr="F:/output/distance.txt";
    QVector<double> toreturn;
    double temp;

    if(mode==0){//mode=0 that means calculate Adjacent two points distance
        for(int i=1;i<length;i++)
        {
            temp=DisCalFuc(Into[i].x(),Into[i].y(),Into[i-1].x(),Into[i-1].y());
            toreturn.push_back(temp);
        }

    }
    else{//mode!=0 that means calculate every two points distance
        for(int i=0;i<length;i++)
        {
            for(int j=i+1;j<length;j++)
            {
                temp=DisCalFuc(Into[i].x(),Into[i].y(),Into[j].x(),Into[j].y());
                toreturn.push_back(temp);
            }

        }



    }
    if(outtotxt)
    {
        QFile *outflie=new QFile;
        outflie->setFileName(outaddr);
        bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
        if(ok){
            QTextStream out(outflie);
            for(int i=0;i<length-1;i++){
                double tempint=toreturn[i];
                QString outstr;
                outstr.append(QString::number(tempint));
                out<<outstr;
                out<<endl;
            }
            outflie->close();
            delete outflie;
        }
    }
    return toreturn;
}
double FindMidValue(QVector<double>Array,int sort,int Ratio)
{
    /*
     * sort 1从大到小排列
     * sort 其他，从小到大
*/

    if(Ratio>100)
    {
        Ratio=Ratio%100;
    }
    if(Ratio<0)
    {
        Ratio=0;
    }
    if(sort==1)//1就是从大到小排列，取大小超过ratio比例的数
    {
        QVector<double>retu= ReorderArray(Array,0);
        int p=floor(retu.length()*Ratio/100);
        return retu[p];
    }
    else
    {//从小到大排列，取小于Ratio比例的数
        QVector<double>retu= ReorderArray(Array,1);
        int p=floor(retu.length()*Ratio/100);
        return retu[p];
    }
}

QVector<int>  FindKeypoints(QVector<int>BP,  QVector<QVector2D> OOL,
                            int MinL,QVector<int>&ReturnKey)
{
    QVector<QVector2D>Curve_2D=TransSequenceTo2D(OOL,BP);
    Output2File(Curve_2D,"F:/output/CurveP"+QString::number(qrand())+".txt");
    QVector<QVector2D>gap_point;
    QVector<double>Gap_slope;
    QVector<QVector2D>KeyPoints;
    QVector<int>KeyPoints_int;
    QVector<int>Toreturn;
    bool Contained=false;
    int orispot=-1;
    foreach (int q,BP)
    {
        orispot++;
        if(q==0)
        {
            Contained=true;//认为曲线点过了原点
            qDebug()<<"the curve through the origin points;";
            break;
        }
    }
    //Output2File(Curve_2D,"F:/output/testcurvepoints.txt");
    int Gap=10;
    samplingGap=Gap;
    for(int k=0;k<Curve_2D.length();k+=Gap)
    {
        gap_point.push_back(Curve_2D[k]);
    }
    Gap_slope=Slope(gap_point);
    samplingcount+=gap_point.length();
    QVector<double>Gap_slope_change;
    for(int i=0;i<Gap_slope.length()-1;i++)
    {
        Gap_slope_change.push_back(abs(Gap_slope[i+1]-Gap_slope[i]));
    }

    QVector< double >SampleContainer;


    QVector<QVector2D>NeedFielter;

    Output2File(gap_point,"F:/output/Gap_points"+QString::number(qrand())+".txt");
    Output2File(Gap_slope_change,"F:/output/Gap_slopechanges"+QString::number(qrand())+".txt");
    Output2File(Gap_slope,"F:/output/Gap_slope"+QString::number(qrand())+".txt");
    //Output2File(Gap_slope,"F:/output/Gapslope"+QString::number(qrand())+".txt");
    for(int i=0;i<Gap_slope_change.length();i+=6)
    {
        int count=0;
        for(int j=0;j<6&&j+i<Gap_slope_change.length();j++)
        {
            SampleContainer.push_back(Gap_slope_change[i+j]);
            count++;
        }

        double Change_MidValue=FindMidValue(SampleContainer,1,50);//找出斜率速的中位数
        KeyPoints.push_back(Curve_2D[0]);
        for(int j=0;j<count;j++)
        {
            qDebug()<<"spot is"<<i;
            if(SampleContainer[j]>=Change_MidValue&&AngelCompare(Gap_slope[i+j],Gap_slope[i+j+1],0.7)==2)
            {
                NeedFielter.push_back(gap_point[i+j]);
                NeedFielter.push_back(gap_point[i+j+1]);
                NeedFielter.push_back(gap_point[i+j+2]);
                //  QMessageBox::information(NULL,"notice",QString::number(Gap_slope[i])+"  "+QString::number(Gap_slope[i+1]));
                QVector<QVector2D>kk=KeyPointFilter_RCS(NeedFielter,OOL,Gap);
                KeyPoints.push_back(kk[0]);
                NeedFielter.clear();
            }
        }
        SampleContainer.clear();
    }
    KeyPoints=Unique_2D(KeyPoints);
    KeyPoints.push_back(Curve_2D[Curve_2D.length()-1]);
    KeyPoints_int=TransSequence2D_ToInt(OOL,KeyPoints);
    //qDebug()<<"Keypoints count :"<<KeyPoints.length();
    Output2File(KeyPoints,"F:/output/keypoints"+QString::number(QTime::currentTime().msec())+".txt");

    //将关键点返回（作图）

ReturnKey=VecAddVec(ReturnKey, KeyPoints_int);
    //找到关键点后对关键点之间的点进行等距离散
    //should add some method here to correct the arlgorim

    QVector<int>totest;
    totest =CurveCheck(Curve_2D,BP,KeyPoints_int,KeyPoints,OOL,MinL);

    qDebug()<<"back from curve check!";
    Toreturn.clear();
    Toreturn=totest;
   // Toreturn=TransSequence2D_ToInt(OOL,KeyPoints);



    // Output2File(totest,"F:/output/KEYKEY"+QString::number(QTime::currentTime().msec())+".txt");
    qDebug()<<"OUT the function check point in line!";
    return Toreturn;//temp return
}






QVector<int>  CheckPointInline(QVector<int>BP,
                               QVector<QVector2D> OOL, QVector<int>BreakP,int MinL)
{
    //此函数用在进行了斜率简化后的对长线段之间的点进行筛查用的
    //主要检测两个长线段之间的点是否有必要保留
    //BP是曲线点序号
    //OOL是边界点坐标，OrderdOutline
    //int Pcount是曲线点个数
    // TSlope是曲线两端直线斜率
    //OOL 是OrderdOutLine
    //BreakP 是轮廓点全部的转折点
    //MinL 是 最小离散长度
    //返回 Toreturn 是离散好的关键点序号
    qDebug()<<"===================================>Enter the function check point in line!";
    //qDebug()<<BP;

    if(BP.length()<5){
        qDebug()<<"BP is null";
        exit(0);
    }
    QVector<int> Toreturn;
    QVector<QVector2D>CurvePoints;
    QVector<int>Break_int;
    QVector<QVector2D>Break_2D;
    QVector<int>Break_Gap;
    QVector<QVector2D>PreThree;
    QVector<QVector2D>AfterThree;
    QVector<double>APslope;

    //判断曲线是不是跨越的起点的点
    bool Contained=false;
    int orispot=-1;
    foreach (int q,BP)
    {
        orispot++;
        if(q==0)
        {
            Contained=true;//认为曲线点过了原点
            qDebug()<<"the curve through the origin points;";
            //qDebug()<<"BP:     "<<BP;
            // qDebug()<<"OOL length:  "<<OOL.length();
            // QVector< QVector2D> OriCurve=TransSequenceTo2D(OOL,BP);
            //Output2File(OriCurve,"F:/output/OriCurve.txt");
            //qDebug()<<"orispot:    "<<orispot;
            break;
        }
    }


    qsrand(QTime::currentTime().msec());

    if(Contained)
    {
        //qDebug()<<"BP IS       "<<BP;
        //qDebug()<<"BreakP is   "<<BreakP;
        //qDebug()<<OOL.length()<<"length of OOL";
        OOL= VectorTransposition(OOL,-orispot-2);
        QVector<int>newbp;
        foreach (int k, BreakP)
        {
            if(k+orispot+2>OOL.length()-1)
            {
                k=k-OOL.length();

            }
            newbp.push_back(k+orispot+2);

        }
        BreakP.clear();
        BreakP=newbp;
        newbp.clear();
        foreach (int k, BP) {
            if(k+orispot+2>OOL.length()-1)
            {
                k=k-OOL.length();

            }
            newbp.push_back(k+orispot+2);
        }
        BP.clear();
        BP=newbp;
        //qDebug()<<"BP IS       "<<BP;
        //qDebug()<<"BreakP is   "<<BreakP;
        //qDebug()<<OOL.length()<<"length of OOL";
        //exit(0);
    }

    Break_2D.push_back(OOL[BP[0]]);//认为曲线点也是转折点

    Break_int.push_back(BP[0]);

  //  qDebug()<<"Piece 1    [check point in line]";
    //取出曲线中的转折点
    for(int i=0;i<BP.length();i++)
    {
        CurvePoints.push_back(OOL[BP[i]]);//get the coordinate of the curve points;
        for(int j=0;j<BreakP.length();j++)
        {
            if(BP[i]==BreakP[j])
            {
                Break_2D.push_back(OOL[BreakP[j]]);
                Break_int.push_back(BreakP[j]);
            }
        }

    }

    if(Break_int[Break_int.length()-1]!=BP[BP.length()-1])
    {
        Break_2D.push_back(OOL[BP[BP.length()-1]]);//认为曲线点也是转折点
        Break_int.push_back(BP[BP.length()-1]);
    }

    QVector<QVector2D>BPS;
    //所有转折点换成xy坐标放在BPS
    foreach (int kk, BreakP)
    {
        BPS.push_back(OOL[kk]);
    }

    //Output2File(BPS,"F:/output/BPS"+QString::number(QTime::currentTime().msec())+".txt");


    for(int k=1;k<Break_int.length();k++)
    {
        Break_Gap.push_back(Break_int[k]-Break_int[k-1]);
    }

    // Output2File(Break_int,"F:/output/Gap"+QString::number(QTime::currentTime().msec())+".txt");
    // Output2File(Break_2D,"F:/output/Curvebreakp"+QString::number(QTime::currentTime().msec())+".txt");

   // qDebug()<<"Curve point length: "<<CurvePoints.length();

    qDebug()<<"to get the subscript and coordinate of breakp";

    if(CurvePoints.length()==0){
        exit(0);
    }


    // Output2File(CurvePoints,"F:/output/TestforCurvePoint"+QString::number(QTime::currentTime().msec())+".txt");

    qDebug()<<"Finish the geting!";

    QVector<double>slo;


    slo=Slope(Break_2D,1,false);

    // Output2File(slo,"F:/output/slo"+QString::number(QTime::currentTime().msec())+".txt",1);

    QVector<QVector2D> P_turn;//转折点 xy

    QVector<int>P_turn_int;//转折点序号

    /*a new try for find the characteristic points*/


    //need  to fix here,to filter the points which will be choose near by the real key point

    for(int i=1;i<Break_int.length()-1;i++)//首尾点是曲线端点，不予考虑break_int包含了曲线端点
    {
        //qDebug()<<i<<"i is this ";

        //qDebug()<<Break_int[i]<<"break_int i";
        //  qDebug()<<"ool length   "<<OOL.length();

        if(Break_int[i]-9>=0&&Break_int[i]+9<OOL.length())
        {
            // qDebug()<<"case one";
            PreThree.push_back(OOL[Break_int[i]-9]);
            PreThree.push_back(OOL[Break_int[i]-6]);
            PreThree.push_back(OOL[Break_int[i]-3]);
            AfterThree.push_back(OOL[Break_int[i]+3]);
            AfterThree.push_back(OOL[Break_int[i]+6]);
            AfterThree.push_back(OOL[Break_int[i]+9]);
        }
        else if(Break_int[i]-9<0&&Break_int[i]+9<OOL.length())
        {
            // qDebug()<<"case two";
            int leng=Break_int[i];
            if(leng>=1&&leng<=9){
                PreThree.push_back(OOL[Break_int[i]-leng]);
                PreThree.push_back(OOL[Break_int[i]-leng/2]);
                PreThree.push_back(OOL[Break_int[i]-leng/4]);
                AfterThree.push_back(OOL[Break_int[i]+3]);
                AfterThree.push_back(OOL[Break_int[i]+6]);
                AfterThree.push_back(OOL[Break_int[i]+9]);
            }
        }else if(Break_int[i]-9>=0&&Break_int[i]+9>=OOL.length())
        {
            //  qDebug()<<"case three";
            int leng=OOL.length()-Break_int[i]-1;
            // qDebug()<<leng;
            if(leng>=1&&leng<=9){
                PreThree.push_back(OOL[Break_int[i]-9]);
                PreThree.push_back(OOL[Break_int[i]-6]);
                PreThree.push_back(OOL[Break_int[i]-3]);

                AfterThree.push_back(OOL[Break_int[i]+leng/4]);
                AfterThree.push_back(OOL[Break_int[i]+leng/2]);
                AfterThree.push_back(OOL[Break_int[i]+leng]);
            }
        }
        /*  qDebug()<<"got the pre and after!";
        qDebug()<<"debug test"<<OOL[Break_int[i]];
        qDebug()<<PreThree;*/
        for(int j=0;j<3;j++)
        {
            double mi;
            mi=SingelSlopeCalculate(OOL[Break_int[i]],PreThree[j]);
            APslope.push_back(mi);
        }
        // qDebug()<<"Debug order";
        for(int j=0;j<3;j++)
        {
            double mi;
            mi=SingelSlopeCalculate(AfterThree[j],OOL[Break_int[i]]);
            APslope.push_back(mi);
        }
        // qDebug()<<"Debug order";
        if(slo.length()>2)
        {
            bool ok=AngelCompare(APslope,i,slo,0.3);
            if(ok)
            {
                //qDebug()<<OOL.length();
                //qDebug()<<Break_int[i];
                //qDebug()<<"该点满足条件: "<<OOL[Break_int[i]];
                P_turn.push_back(OOL[Break_int[i]]);
                P_turn_int.push_back(Break_int[i]);
            }

            qDebug()<<"ingnore";

        }
        else
        {
            /* qDebug()<<"slo is     "<<slo;
             qDebug()<<"break_2D    "<<Break_2D;
            qDebug()<<"Break int     "<<Break_int;
             qDebug()<<"Curvepoints:     "<<CurvePoints;*/
            // exit(0);
        }
        APslope.clear();
        PreThree.clear();
        AfterThree.clear();
    }
    if(P_turn.length()!=0){

        // Output2File(P_turn,"F:/output/turnpoints"+QString::number(QTime::currentTime().msec())+".txt");

        //找到关键点后对关键点之间的点进行等距离散
        //should add some method here to correct the arlgorim
        // Output2File(P_turn,"F:/output/Pturn_old"+QString::number(qrand())+".txt");

        // P_turn=  KeyPointFilter_RCS(P_turn);
        P_turn_int=TransSequence2D_ToInt(OOL,P_turn);

        Output2File(P_turn,"F:/output/Pturn_new"+QString::number(qrand())+".txt");
        //创建一个离散创建函数？
        QVector<int>totest;

        qDebug()<<"go to curve check!";

        totest =CurveCheck(CurvePoints,BP,P_turn_int,P_turn,OOL,MinL);

        Toreturn.clear();

        Toreturn=totest;

        qDebug()<<"back from curve check!";

        // Output2File(totest,"F:/output/KEYKEY"+QString::number(QTime::currentTime().msec())+".txt");

    }
    else
    {
        QVector2D LineSE;
        LineSE.setX(BP[0]);
        LineSE.setY(BP[BP.length()-1]);
        QVector<int>Noturn;
        Noturn=CurveDisperce(OOL,LineSE,MinL);
        //没有就直接等距离散
        //调用Curvedisperce
        Toreturn.clear();
        Toreturn=Noturn;
        qDebug()<<"no turn points";

    }

    if(Contained)
    {
        QVector<int >newtoreturn;
        foreach (int q, Toreturn) {
            if(q-orispot-2<0)
            {
                q=OOL.length()+q;

            }



            newtoreturn.push_back(q-orispot-2);
        }
        Toreturn.clear();
        Toreturn=newtoreturn;
    }
    qDebug()<<"OUT the function check point in line!";
    return Toreturn;//temp return
}
QVector<QVector2D>KeyPointFilter_RCS(QVector<QVector2D>KP, QVector<QVector2D>ALL,int Gap)
{
    /*
     * KP 关键点xy坐标，一次传入三个，关键点前一个离散点，和后一个离散点
     * ALL 全轮廓点
     * Gap 取间距点的时候的间距数值
     * 返回审查后的关键点xy坐标
     */
    // qDebug()<<"length"<<KP.length();
    QVector<QVector2D>Toreturn;
    qDebug()<<"Enter the function keypoints filter!";
    QVector <QVector2D>virtualLine;
    //qDebug()<<ALL.length();
   // qDebug()<<KP.length()<<"Keypoints length";

        virtualLine.push_back(KP[0]);
        virtualLine.push_back(KP[2]);
        QVector<int> Threespot=TransSequence2D_ToInt(ALL,KP);
        //qDebug()<<"threespot "<<Threespot;
        QVector<QVector2D>MidP;
        for(int i=Threespot[0];i<=Threespot[2];i++)
        {
            MidP.push_back(ALL[i]);
        }
        QVector<double>Alldis=  PointToLineDis(virtualLine,MidP);

        double maxva=-1;
        int where=-1;
       // qDebug()<<"All  dis length:   "<<Alldis.length();
       // qDebug()<<"mid p length  "<<MidP.length();
        for (int j=0;j<Alldis.length();j+=2)
        {
            if(Alldis[j]>maxva)
            {
                maxva=Alldis[j];
                where=j;
            }
        }
        if(where!=-1){
            Toreturn.clear();
            Toreturn.push_back(MidP[where/2]);
        }
        else{
            Toreturn.clear();
            Toreturn.push_back(KP[1]);
        }

        return Toreturn;

}

QVector<int> CurveCheck(QVector<QVector2D>CurP_2D, QVector<int>CurP_int,
                        QVector<int>keyP_int, QVector<QVector2D>keyP_2D,QVector<QVector2D>All,int MinL)
{
    /*
   * 该函数对曲线进行离散。
   * CurP_2D是曲线点的xy坐标，
   * CurP_int是曲线点的序号（对全局）
   * keyP_int是相对于全部轮廓的序号.
   * keyP_2D 是关键点xy坐标
   * MinL 是离散长度
   * 返回 曲线离散点序号
*/
    QTime testtimer;
    testtimer.start();
    QVector<int>newCurve_int;
    QVector<int>Toreturn;
    if(keyP_int.length()>0){
        qDebug()<<"keypoints length is long enough";
        newCurve_int.push_back(CurP_int[0]);//放入曲线起始点
        foreach (int kk, keyP_int)
        {
            newCurve_int.push_back(kk);//把关键点放入newCurve

        }
        newCurve_int.push_back(CurP_int.length()-1);
    }
    else
    {
        //没有中间的关键点
        qDebug()<<"no mid key points";
        newCurve_int.push_back(CurP_int[0]);//直接放入曲线首尾点
        newCurve_int.push_back(CurP_int[CurP_int.length()-1]);
    }
    QVector<QVector2D>new2D=TransSequenceTo2D(All,newCurve_int);//带曲线起止点和关键点的曲线xy得出
    QVector<int>new_int=TransSequence2D_ToInt(CurP_2D,new2D);//以原来曲线为模板重新编号
    Output2File(new2D,"F:/output/new2D"+QString::number(qrand())+".txt");
    //qDebug()<<CurP_int;
//qDebug()<<"Curvelength"<<CurP_2D.length();
   // qDebug()<<"new   int"<<new_int;
   // qDebug()<<"new curve int"<<newCurve_int;
    // exit(0);
    //here need corrected
    //开始考察曲线间隔点

    for(int i=0;i<=new_int.length()-2;i++)
    {
        int stp=new_int[i];

        int endp=new_int[i+1];

        int Pointin=endp-stp-1;//中间间隔点数，之后对中间间隔点数进行考察
        // qDebug()<<"point in stp and endp length:    "<<Pointin;
        QVector<int>InsetP;//插点序列

        if(Pointin>MinL-2)
        {
            //曲线超过了最短点数，需要插点
            qDebug()<<"curve is long and need insert some point!";
            //此处做一个插点数量计算函数
            InsetP=InsertCalculate(stp,endp,CurP_2D.length()-1,MinL);//该函数 返回 int型的插值点后序列
            Toreturn=VecAddVec(Toreturn,InsetP);
            InsetP.clear();
        }
        else//曲线很短，只有首尾端点
        {
            qDebug()<<"curve is short!";
            QVector<int> sine;

            sine.push_back(new_int[i]);

            sine.push_back(new_int[i+1]);//取出首尾点

            Toreturn=VecAddVec(Toreturn,sine);

        }
    }

    QVector<QVector2D>tt=TransSequenceTo2D(CurP_2D,Toreturn);
    if(tt.length()>=2){
        Unique_2D(tt);
    }
    Output2File(tt,"F:/output/dis"+QString::number(qrand())+".txt");
    Toreturn=TransSequence2D_ToInt(All,tt);
    qDebug()<<"Returned from the function Curvecheck;       **********[CurveCheck]*************";
    qDebug()<<"Curvecheck time used:    "<<QString::number(testtimer.elapsed());
    return Toreturn;
}



QVector<int>CurveDisperce(QVector<QVector2D>AllOutline,
                          QVector2D CurveEndpoints,int MinL)
{
    /*
 *AllOutline 是全部轮廓点的xy坐标
 * CurveEndpoints 是传入的曲线的起止点序号
 *MinL 是默认最小离散长度。

    */
    qDebug()<<"===========>Enter the function CurveDisperce!           [CurveDisperce]";
    int length=CurveEndpoints.y()-CurveEndpoints.x()+1;//获取曲线长度
    int DisperceNum=length/MinL;//获取需要离散的段数
    //qDebug()<<"MinL               "<<MinL;
    QVector<int> Toreturn;

    QVector<QVector2D>Twoends;
    //heere

    Twoends.push_back(AllOutline[CurveEndpoints.x()]);
    Twoends.push_back(AllOutline[CurveEndpoints.y()]);

    QVector<QVector2D>AllPoints;
    QVector<QVector2D>MidPoints;

    QVector<double>P2Pdis;//储存距离

    //取出XY坐标形势的曲线全部点和除去端点后的

    for(int i=CurveEndpoints.x();i<=CurveEndpoints.y();i++)
    {
        AllPoints.push_back(AllOutline[i]);
        if(i>CurveEndpoints.x()&&i<CurveEndpoints.y()){
            MidPoints.push_back(AllOutline[i]);
        }
    }
    qDebug()<<"get the midpoints is finished!   [CurveDisperce]";

    P2Pdis=PointToLineDis(Twoends,MidPoints);//获取中间点到直线距离

    qDebug()<<"get the distance is dawn!     [CurveDisperce]";

    QVector<int> spotstory;

    for(int k=0;k<P2Pdis.length();k++)
    {
        if(P2Pdis[k]>3)
        {
            spotstory.push_back(k);//讲距离大于3的点的位置标记
        }
    }

    qDebug()<<"check the points which are not good     [CurveDisperce]";

    if(spotstory.length()==0){
        //没有超出的点
        qDebug()<<"case one  [CurveDisperce]";
        Toreturn.push_back(CurveEndpoints.x());
        Toreturn.push_back(CurveEndpoints.y());
        qDebug()<<"Returned from the curvedisperce case **one**!        [CurveDisperce]";
        return Toreturn;
    }
    else if(spotstory.length()==1)
    {
        qDebug()<<"case two  [CurveDisperce]";
        Toreturn.push_back(CurveEndpoints.x());
        Toreturn.push_back(spotstory[0]+CurveEndpoints.x());
        Toreturn.push_back(CurveEndpoints.y());
        qDebug()<<"Returned from the curvedisperce case **Two**!        [CurveDisperce]";
        return Toreturn;
    }else
    {
        qDebug()<<"case three  [CurveDisperce]";
        int aver;
        foreach (int m, spotstory) {
            aver=aver+m;
        }
        aver=aver/spotstory.length();


        Toreturn.push_back(CurveEndpoints.x());
        Toreturn.push_back(aver+CurveEndpoints.x());
        Toreturn.push_back(CurveEndpoints.y());
        qDebug()<<"Returned from the curvedisperce case **Three**!        [CurveDisperce]";
        return Toreturn;


    }

    qDebug()<<"Out from the function CurveDisperce!           [CurveDisperce]";
}
QVector<int>TransSequence2D_ToInt(QVector<QVector2D>Alloutline,QVector<QVector2D>input2D)
{
    QVector<int>Toreturn;
    foreach (QVector2D k, input2D)
    {
        bool notfind=true;
        for (int i=0;i< Alloutline.length();i++)
        {
            QVector2D ou=Alloutline[i];
            if(k==ou)
            {
                notfind=false;
                Toreturn.push_back(i);
                break;
            }
        }
        if(notfind)
        {
            qDebug()<<"no 100% match  [TransSequence2DToint]";
            double mind=99999;
            int minspot=-5;
            for(int i=0;i< Alloutline.length();i++)
            {
                for(int j=0;j<Toreturn.length();j++)
                {
                    if(i!=Toreturn[j])
                    {
                        double dis=DisCalFuc(k.x(),k.y(),Alloutline[i].x(),Alloutline[i].y());
                        if(dis<mind)
                        {
                            mind=dis;
                            minspot=i;
                        }
                    }
                }

            }
            Toreturn.push_back(minspot);
        }

    }
    qDebug()<<"returned from transSequence2DToint  [TransSequence2DToint]";
    return Toreturn;
}

QVector<QVector2D>TransSequenceTo2D(QVector<QVector2D>Alloutline,QVector<int>input)
{
    //qDebug()<<"===================================>Enter function transSequenceTo2D  [TransSequenceTo2D]";
    QVector<QVector2D>Toreturn;
    foreach (int k, input) {

        if(k<0||k>Alloutline.length())
        {
            qDebug()<<" OUT OF THE RANGE! please check the points!!int to 2D";
          //  qDebug()<<k;
            //qDebug()<<Alloutline.length();

        }else{
            Toreturn.push_back(Alloutline[k]);
        }
    }
    qDebug()<<"returned from transSequenceTo2D  [TransSequenceTo2D]";
    return Toreturn;
}

QVector<QVector3D>PeriodDetection(QVector<int> PD)
{
    QVector<QVector3D> ToReturn;
    enum PeriodType{
        Increasing,
        Decreasing,
        Circulation
    };
    return ToReturn;
}
QVector<QVector3D>PeriodDetection(QVector<double> PD)
{
    QVector<QVector3D> ToReturn;
    enum PeriodType{
        Increasing,
        Decreasing,
        Circulation
    };
    return ToReturn;
}

QVector<QVector3D>Curvature(QVector<QVector2D> OUTLINE)
{
    //该函数用于检测曲线曲率变化
    //OUTLINE是传入的顺序坐标点
    int length=OUTLINE.length();
    if(length<3){
        QMessageBox::warning(NULL,"warning","length is not enough! Curvature function");
        exit(0);
    }
    int Curdis=3;//间隔斜率检测
    QVector<double> Angels3;
    QVector<double> AngelChange3;
    Angels3=Slope(OUTLINE,Curdis);
    Curdis++;
    QVector<double> Angels4;
    QVector<double> AngelChange4;
    Angels4=Slope(OUTLINE,Curdis);

    for(int i=1;i<Angels3.length();i++)
    {
        AngelChange3.push_back(Angels3[i]-Angels3[i-1]);
    }

    for(int i=1;i<Angels4.length();i++)
    {
        AngelChange4.push_back(Angels4[i]-Angels4[i-1]);
    }

}
QVector<int> InsertCalculate(int startP, int endP, int maxor,int MinL)
{
    /*
     *  startP 是传入曲线段起点序号
     *  endP 是传入曲线段终止点序号
     *  MinL 最小离散长度
     * maxor 是最大序号
     *  返回从startP开始到endP结束的离散点序号
*/
    qDebug()<<"===================================>Enter the InsetCalculate function!";
    QVector<int>toreturn;

    //说明曲线没有跨越起点
    qDebug()<<"the curve is not contain the origin point";
    toreturn.push_back(startP);//返回的点先放入起点
    int SumofPoints=abs(endP-startP)+1;//获取总共点数
    bool ok=true;
    int remainder=(SumofPoints-1)%(MinL);

    int spot=startP;
    if(SumofPoints>2*MinL){
        if(remainder==0)
        {//刚好均分，就均分点放入返回
            qDebug()<<"no remainder!";
            for(int i=spot;i<=SumofPoints+spot;i+=MinL)
            {

                toreturn.push_back(i);
                if(spot>endP)
                {
                    QMessageBox::information(NULL,"out","OUT OF THE RANGE");
                    exit(0);
                }
            }
            toreturn.push_back(endP);
        }
        else
        {
            //有余数
            //快速计算分段数据
            ok=true;
            int change=0;
            int Restore[4];
            Restore[0]=remainder;
            while(remainder/MinL>=0.2&&remainder/MinL<=0.8)
            {
                MinL+=1;
                change++;
                if(change>=4)
                {
                    break;
                }
                Restore[change]=(SumofPoints-1)%(MinL);
            }
            int cut=(SumofPoints-1)/(MinL);//分段数
            int minval=999;
            int maxval=-1;
            int minspot=-1;
            int maxspot=-1;
            for(int i=0;i<4;i++)
            {
                if(minval>Restore[i])
                {
                    minval=Restore[i];
                    minspot=i;
                }
                if(maxval<Restore[i])
                {
                    maxval=Restore[i];
                    maxspot=i;
                }
            }
            int type_remi=0;
            double ratio_min=minval/(MinL-3+minspot);
            double ratio_max=maxval/(MinL-3+maxspot);
            if(1-ratio_max<ratio_min)
            {
                //大值更好
                MinL=MinL-3+maxspot;
                type_remi=1;

            }
            else
            {
                MinL=MinL-3+minspot;
                type_remi=2;
            }
            int cutcount=0;
            if(remainder<=cut)
            {
                cutcount=remainder;
            }
            for(int i=spot;i<=SumofPoints+spot;i+=MinL)
            {
                //qDebug()<<i;
                if(remainder<=cut&&cutcount>0)
                {
                    i=i+1 ;
                    cutcount--;
                }



                toreturn.push_back( i);
            }
            if(type_remi==2&&remainder>cut)
            {
                toreturn.remove(toreturn.length()-1,1);
            }
        }
        toreturn.push_back(endP);
    }

    else if(SumofPoints<=1.6*MinL)
    {
        toreturn.push_back(startP);
        toreturn.push_back(endP);
    }
    else if(SumofPoints>1.6*MinL&&SumofPoints<=2*MinL)
    {
        toreturn.push_back(startP);
        toreturn.push_back(0.5*(startP+endP));

        toreturn.push_back(endP);
    }
    qDebug()<<"out the InsetCalculate function!";
    ////Output2File(toreturn,"F:/output/insetcalculation"+QString::number(toreturn.length())+".txt");
    // QMessageBox::information(NULL,"InsertCalculation Called","This Function is called and check the output window!");
    return toreturn;
}
QVector<QVector2D>Performance( QVector<double> change)
{
    //返回是2维int数组，第一个标签，1表示合格，0表示不合格，第二列是合格点
    float thresh=15.0;//误差阈值15°
    QVector<QVector2D>toreturn;
    QVector2D  toomax;//记录满足误差阈值的点在change中的位置（过度角度便宜误差）
    for(int q=0;q<change.length();q++)
    {
        if(abs(change[q])<thresh)//正向角度误差0~15°
        {
            toomax.setX(1);
            toomax.setY(q);
            toreturn.push_back(toomax);
        }else
        {
            toomax.setX(0);
            toomax.setY(q);
            toreturn.push_back(toomax);
        }
    }
    if(toreturn.length()==0)
    {
        toomax.setX(-8);
        toomax.setY(-8);
        toreturn.push_back(toomax);
    }
    return toreturn;
}
QVector<QVector2D> HoughTransform(QImage OutlineImage, int PointCount, int minmumLine)
{
    // this function is for hough transform to get the curve and strait line

    QImage newtocv=OutlineImage;

    for(int i=0;i<OutlineImage.width();i++)
    {
        for(int j=0;j<OutlineImage.height();j++)
        {
            QColor it=OutlineImage.pixelColor(i,j);
            if(it.red()==255&&it.blue()==255){
                newtocv.setPixel(i,j,qRgb(0,0,0));
            }else
            {
                newtocv.setPixel(i,j,qRgb(255,255,255));
            }
        }
    }

    //part one strait line check
    cv::Mat mat,graymat,drawmat;

    mat=QImage2cvMat(newtocv);

    cvtColor(mat,graymat, CV_RGBA2GRAY);// change to one channels gray image
    // imshow("mat",graymat);


    drawmat=graymat.clone();
    std::vector<cv::Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合

    cv::HoughLinesP(graymat, lines, 1, CV_PI/180,PointCount,minmumLine*1.5,20); //(in,out,rho,theta,threshold,MinLength,maxgap)



    QVector<QVector4D>SLines;//stroe all lines （p1.x,p1.y;p2.x,p2.y）
    QVector4D Pis;
    for( size_t i = 0; i < lines.size(); i++ )
    {
        cv::Vec4i l = lines[i];
        line( drawmat, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(186,88,255), 5, CV_AA);
        Pis.setX(l[0]);
        Pis.setY(l[1]);
        Pis.setZ(l[2]);
        Pis.setW(l[3]);
        SLines.push_back(Pis);
    }

    //qDebug()<<"Slines length is:     "<<SLines.length();
    ////Output2File(SLines,"F:/output/oriSLines.txt");
    cv::imwrite("F:/output/cvsave.png",drawmat);
   // imshow("hough",drawmat);

    /* for(size_t i=0;i<SLines.size();i++)
  {
     qDebug()<<SLines[i]<<"Lines is this";
  }*/

    QVector<QVector2D> Sline2D;
    for(int i=0;i<SLines.length();i++)
    {
        QVector2D tit;
        tit.setX(SLines[i].x());
        tit.setY(SLines[i].y());
        Sline2D.push_back(tit);
        tit.setX(SLines[i].z());
        tit.setY(SLines[i].w());
        Sline2D.push_back(tit);

    }

    //   imshow("lines",drawmat);


    return Sline2D;




}

QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
       // qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
cv::Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;

    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        QMessageBox::warning(NULL,"warning","format is not right");
        exit(0);
    }
    return mat;
}
void Output2File(QVector<QPointF> InputArray,QString Outputadd)
{
    if(Outputadd.isEmpty()){
        QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="F:/output/outputfiles.txt";;
    }
    int length=InputArray.length();


    QFile *outflie=new QFile;
    outflie->setFileName(Outputadd);
    bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok)
    {
        QTextStream out(outflie);
        for(int i=0;i<length;i++)
        {

            QString outstr;
            outstr=QString::number(InputArray[i].x());
            outstr.push_back(" ");
            outstr.push_back(QString::number(InputArray[i].y()));
            out<<outstr;
            out<<endl;

        }
        outflie->close();
        delete outflie;
    }
}

void Output2File(QVector<QVector2D>InputArray,QString Outputadd)
{
    if(Outputadd.isEmpty()){
        QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="F:/output/outputfiles.txt";;
    }
    int length=InputArray.length();


    QFile *outflie=new QFile;
    outflie->setFileName(Outputadd);
    bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok)
    {
        QTextStream out(outflie);
        for(int i=0;i<length;i++)
        {

            QString outstr;
            outstr=QString::number(InputArray[i].x());
            outstr.append(" ");
            outstr.append(QString::number(InputArray[i].y()));
            out<<outstr;
            out<<endl;

        }
        outflie->close();
        delete outflie;
    }
}
void Output2File(QVector<QVector4D>InputArray,QString Outputadd)
{
    if(Outputadd.isEmpty()){
        QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="F:/output/outputfiles.txt";;
    }
    int length=InputArray.length();


    QFile *outflie=new QFile;
    outflie->setFileName(Outputadd);
    bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok){

        QTextStream out(outflie);
        for(int i=0;i<length;i++)
        {

            QString outstr;
            outstr=QString::number(InputArray[i].x());
            outstr.append(" ");
            outstr.append(QString::number(InputArray[i].y()));
            out<<outstr;
            out<<endl;
            outstr.clear();
            outstr=QString::number(InputArray[i].z());
            outstr.append(" ");
            outstr.append(QString::number(InputArray[i].w()));
            out<<outstr;
            out<<endl;

        }

        outflie->close();
        delete outflie;
    }
}
void Output2File(QVector<int>InputArray,QString Outputadd,int newline)
{
    //newline means that every line can only contain 1 word
    if(Outputadd.isEmpty()){
        // QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="F:/output/outputfiles"+QString::number(qrand())+".txt";
    }
    int length=InputArray.length();


    QFile *outflie=new QFile;
    outflie->setFileName(Outputadd);
    bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok)
    {
        int count=0;
        QTextStream out(outflie);
        for(int i=0;i<length;i++)
        {

            QString outstr;
            outstr=QString::number(InputArray[i]);
            outstr.append(" ");

            out<<outstr;
            count++;
            if(count==newline){
                out<<endl;
                count=0;
            }

        }
        outflie->close();
        delete outflie;
    }
}
QVector<QVector2D> PointReorder(QVector<QVector2D>input,QVector<QVector2D>templateArray,int MinL)
{

    int templength=templateArray.length();

    int inputLength=input.length();
    QVector<int>tempor;



    QVector<QVector2D>Toreturn;

    if(inputLength==0){
        //  QMessageBox::information(NULL,
        // "length","the length of the input is zero!");
        exit(0);
    }
    if(templength==0){
        //  QMessageBox::information(NULL,
        //    "length","the length of the template array is zero!");
        exit(0);
    }



    tempor=PointReorder_Rint(input,templateArray,MinL);



    for(int i=0;i<tempor.length();i++)
    {
        for(int j=0;j<templength;j++){
            if(tempor[i]==j){

                Toreturn.push_back(templateArray[j]);
            }
        }
    }


    qDebug()<<"out of function PointReordered in 2D";
    return Toreturn;

}

QVector<int> PointReorder_Rint(QVector<QVector2D>input,QVector<QVector2D>templateArray,int MinL)
{
    QVector<int>Toreturn;
    //进行点转编号
    Toreturn=TransSequence2D_ToInt(templateArray,input);
    //qDebug()<<"befor reorder function: "<<endl<<Toreturn;
    if(Toreturn.length()%2!=0){
        exit(0);
    }
    //直线内交换大小
    for(int i=0;i<Toreturn.length();i+=2)
    {
        if(Toreturn[i]>Toreturn[i+1])
        {
            SwapT_int_int(Toreturn[i],Toreturn[i+1]);
        }
    }
    if(Toreturn.length()==2)
    {
        return Toreturn;
    }
    QVector<int>toorder;
    QVector<int>getorder;
    for(int i=0;i<Toreturn.length();i+=2)
    {
        toorder.push_back(Toreturn[i]);
    }
    getorder=ReorderArray(toorder,1);
    //上述是把直线起点从小到大排一次
    QVector<int>newtoreturn;
    for(int i=0;i<getorder.length();i++)
    {
        for(int j=0;j<Toreturn.length();j+=2)
        {
            if(getorder[i]==Toreturn[j])
            {
                newtoreturn.push_back(Toreturn[j]);
                newtoreturn.push_back(Toreturn[j+1]);
            }
        }
    }


    //需要判断是否有直线包含起点
    //下面是判断直线是否包含起点
    //下面是处理第一条直线跨越起始点
    bool cont=false;
    Toreturn.clear();
    int longlength=abs(newtoreturn[1]-newtoreturn[0])+1;
    if(newtoreturn[1]>newtoreturn[2]&&longlength>templateArray.length()*0.35)
    {//如果第一条直线末端序号大于下一条直线起点序号并且跨越长度大于总共点数的35%
        //则说明直线包含原点
        QVector<int>replace_int;
        Toreturn.push_back(newtoreturn[1]);
        Toreturn.push_back(newtoreturn[0]);
        cont=true;
        //把第一个第二个点切去
        for(int i=2;i<newtoreturn.length();i++)
        {
            replace_int.push_back(newtoreturn[i]);
        }
        newtoreturn.clear();
        newtoreturn=replace_int;
        replace_int.clear();
    }
    //可能有直线嵌套
    //先处理完全嵌套

    //嵌套处理有问题
    // qDebug()<<"new"<<newtoreturn;
    // qDebug()<<TransSequenceTo2D(templateArray,newtoreturn);
    //


    QVector<QVector2D>LineIn2D;
    QVector<double>Line2Dslope;

    bool inside=true;
    int out1=0;
    //qDebug()<<"before inside :"<<newtoreturn;
    while(inside)
    {
        int insidewhere=-5;
        LineIn2D=TransSequenceTo2D(templateArray,newtoreturn);
        Line2Dslope=Slope(LineIn2D,1,true);
        for(int i=1;i<newtoreturn.length()-1;i+=2)
        {
            inside=false;
            if(newtoreturn[i]>newtoreturn[i+1])
            {
                insidewhere=i;
                inside=true;
                break;
            }

        }

        if(!inside){
            qDebug()<<"inside is false";
            break;
        }
        //qDebug()<<Line2Dslope<<"lieslope";//偶数项是直线斜率
        // qDebug()<<LineIn2D.length()<<"2d";//此时上面slope个数和这个相同
        //qDebug()<<"inside :"<<newtoreturn;
        //qDebug()<<"inside where:"<<insidewhere;
        if(inside)
        {
            QVector<int>tempus;
            QVector<int>IngnoreP;

            if(newtoreturn[insidewhere]>newtoreturn[insidewhere+2])
            {
                //后一段完全嵌套在里面
                //检查两者斜率，如果很近，直接抹去嵌入段
                if(AngelCompare(Line2Dslope[insidewhere-1],Line2Dslope[insidewhere+1],0.15)==1)
                {
                    //两者斜率很相近，直接抹去
                    IngnoreP.push_back(insidewhere+2);
                    IngnoreP.push_back(insidewhere+1);
                }
                else{
                    //应该不可能有这种情况
                    //qDebug()<<"special case 1"<<  newtoreturn[insidewhere+1]<<"    "<<newtoreturn[insidewhere];
                    newtoreturn[insidewhere+1]=newtoreturn[insidewhere];

                }

            }
            else if(newtoreturn[insidewhere]<newtoreturn[insidewhere+2])
            {
                //此为非完全嵌套

                if(AngelCompare(Line2Dslope[insidewhere-1],Line2Dslope[insidewhere+1],0.15)==1)
                {
                    //两者斜率很相近，直接抹去
                    IngnoreP.push_back(insidewhere);
                    IngnoreP.push_back(insidewhere+1);
                }
                else{
                    //应该不可能有这种情况
                    //  qDebug()<<"special case  2 "<<  newtoreturn[insidewhere+1]<<"    "<<newtoreturn[insidewhere];
                    newtoreturn[insidewhere+1]=newtoreturn[insidewhere];
                }
            }
            else if(newtoreturn[insidewhere]==newtoreturn[insidewhere+2]&&newtoreturn[insidewhere-1]==newtoreturn[insidewhere+1])
            {
                IngnoreP.push_back(insidewhere);
                IngnoreP.push_back(insidewhere+1);
            }


            for(int j=0;j<newtoreturn.length();j++)
            {
                bool ok=true;
                for(int k=0;k<IngnoreP.length();k++)
                {
                    if(j==IngnoreP[k]){
                        ok=false;
                    }
                }
                if(ok)
                {
                    tempus.push_back(newtoreturn[j]);
                }

            }
            newtoreturn.clear();
            newtoreturn=tempus;
            tempus.clear();
            //qDebug()<<toshow<<"to check is opreated!!";
            //qDebug()<<newtoreturn<<"new to return!!";

            out1++;
          //  qDebug()<<"out1:    "<<out1;
            if(out1>25)
            {
                qDebug()<<"too much iterater!";
                exit(0);
                break;

            }

        }

    }

    // QVector<QVector2D>tete=TransSequenceTo2D(templateArray,newtoreturn);
    // Output2File(tete,"F:/output/sss.txt");
    if(cont){
        QVector<int>rep;
        foreach (int kl,Toreturn) {

            rep.push_back(kl);
        }
        foreach (int kl, newtoreturn) {
            rep.push_back(kl);
        }
        Toreturn.clear();
        Toreturn=rep;
    }else
    {
        Toreturn.clear();
        Toreturn=newtoreturn;
    }

    QVector<int>exchange;
    for(int i=0;i<Toreturn.length();i+=2)
    {
        if(cont&&i==0)
        {
            if(templateArray.length()-Toreturn[0]+Toreturn[1]>=2*MinL)
            {
                exchange.push_back(Toreturn[0]);
                exchange.push_back(Toreturn[1]);
            }
        }
        if(i>=0){
            if(Toreturn[i+1]-Toreturn[i]>=2*MinL)
            {
                exchange.push_back(Toreturn[i]);
                exchange.push_back(Toreturn[i+1]);
            }
        }
    }
    Toreturn.clear();
    Toreturn=exchange;

    //qDebug()<<"After the reorder:    "<<endl<<Toreturn;
    return Toreturn;



}

QVector2D Find_Center(QVector<QVector2D>Circle, QVector<double>cent, double radiuss)
{
    //to find all the points center
    QVector2D centerp;
    int length=Circle.length();

    for(int i=0;i<length;i++)
    {

    }







return centerp;





}
QVector<int> LineMerge(QVector<int>input_int,QVector<QVector2D>input_Point,
                       QVector<QVector2D>allp,QVector<int>BreakP,bool bisect,
                       QVector<int>&KeyP,int MinL)
{
    /*after houghlinesP function ,we will get some strait line which is represeted by two points(start
    * and end points)
    *input_int 直线端点的位置序号
    *input_Point 直线端点直接坐标
    *BreakP 是轮廓点全部的转折点
    * bisect bool 值，对应界面上曲线等分checkbox，如果为true，则曲线直接等分，不考虑转折点
   *MinL 是最小离散长度
  */
    qDebug()<<"===================================>Enter function line merge";
    QTime testtimer;

    testtimer.start();

    QVector<int> CharacterPoints_int=input_int;//设定所有直线点为特征点

    QVector<QVector2D> CharacterPoints_2D=input_Point;//设定所有直线点为特征点

    if(input_int.length()!=input_Point.length())
    {
        QMessageBox::information(NULL,"LineMergeFunction","Two input array's length is not the same!");
        exit(0);
    }

    QVector<int> Toreturn;

    int length=input_int.length();

    if(length<2)
    {
        QMessageBox::information(NULL,"LineMergeFunction","Length is not enough for a line!");
        return input_int;
    }


    bool Linejump=false;

    if(input_int[0]>input_int[1]&&input_int[0]!=0&&input_int[1]!=0){
        Linejump=true;//这个bool参数是用来检测第一条直线是不是跨越了默认的起点的
        qDebug()<<"the first line contained the start point!";
    }

    QVector<int>fortemp;
    const double degreeT=CV_PI*10/180;
    QVector<double>lineslope;
    lineslope= Slope(input_Point,1,true);

    //当把所有直线作为环型处理时，即第三个参数是true，lineslope的元素应该和直线条数相同
    //且，脚标偶数是直线斜率，奇数是直线之间的斜率

    //qDebug()<<"   lineslope is this  :"<<lineslope;

    int slopelength=lineslope.length();

    QVector<int>RemoveP_int;

    for(int i=1;i<length-1;i+=2)
    {
        if(abs(input_int[i]-input_int[i+1])<=1)
        {
            qDebug()<<"the gap is very small and merge!";
            //qDebug()<<"length is"<<length;
            // qDebug()<<"and now ths i is "<<i;
            if(i+1<=slopelength)
            {//to check will here index out of range
                if(abs(lineslope[i-1]-lineslope[i+1])<degreeT)
                {
                    // mergetwo line

                    RemoveP_int.push_back(input_int[i]);

                    RemoveP_int.push_back(input_int[i+1]);
                }
                else
                {
                    //该点转折,把两个点融合
                    //判定领域方式再融合
                    CharacterPoints_int[i]=input_int[i+1];
                    CharacterPoints_2D[i]=input_Point[i+1];
                    KeyP.push_back(CharacterPoints_int[i]);
                }
            }

            else{
                // QMessageBox::information(NULL,"notice","Out of range ,slope check is here!");
                exit(0);
            }
            qDebug()<<"out from 1 gap!";
        }
        else if(abs(input_int[i]-input_int[i+1])<=MinL&&abs(input_int[i]-input_int[i+1])>1&&i%2!=0)//直线不会短于MinL
        {
            //i必须为奇数，不然就不是检测的直线之间的点
            qDebug()<<"middle gap two point merge";
            //qDebug()<<"length is"<<length;
            // qDebug()<<"and now ths i is "<<i;
            if(i+1<=slopelength)
            {//to check will here index out of range
                if(qAbs(lineslope[i-1]-lineslope[i+1])<degreeT)
                {
                    RemoveP_int.push_back(input_int[i]);
                    RemoveP_int.push_back(input_int[i+1]);
                }
                else
                {
                    //直线做直接拼接
                    CharacterPoints_int[i]=(input_int[i+1]+input_int[i])/2;
                    CharacterPoints_int[i+1]=(input_int[i+1]+input_int[i])/2;
                     KeyP.push_back(CharacterPoints_int[i]);
                }
            }
            else
            {
                // QMessageBox::information(NULL,"notice","Out of range ,slope check is here!");
                exit(0);
            }
            qDebug()<<"out from small gap!";
        }
        else if(abs(input_int[i]-input_int[i+1])>MinL&&i%2!=0)
        {
            //gap is longer than MinL
            //但是这里没有检查起始点和末位点之间的距离，放在下面的if中
            //长度过长的直接送入CheckPointsinline函数
            //预处理：
            //Gap_Point二维存放曲线起止点位置序号，也就是两段直线，前直线的尾点，和后直线的起点

            qDebug()<<"need curve check!";

            QVector<int> Gap_Point;//Curve point's order in OrderdOutLine
            for(int qq=input_int[i];qq<=input_int[i+1];qq++)    //获取所有曲线点位置序号
            {
                Gap_Point.push_back(qq);
            }

            //获取曲线两端直线的斜率
            QVector<double>TwoSlope;
            TwoSlope.push_back(lineslope[i-1]);//脚标需要对应查询
            TwoSlope.push_back(lineslope[i+1]);
            if(Gap_Point.length()<MinL-3)
            {
            }
            else{
                qDebug()<<"case one send to check point inline:";
                fortemp=FindKeypoints(Gap_Point,allp,MinL,KeyP);
                //fortemp= CheckPointInline(Gap_Point,allp,BreakP,MinL);
                qDebug()<<"out from large gap!";
            }

            foreach (int k, fortemp) {
                CharacterPoints_int.push_back(k);
            }
            fortemp.clear();

        }
        else
        {
            qDebug()<<"can not be this situation!";
            //qDebug()<<"abs(input_int[i]-input_int[i+1]):    "<<abs(input_int[i]-input_int[i+1]);
            //qDebug()<<"i:     "<<i;
            exit(0);
        }
        //qDebug()<<"for is one circle and i is:"<<i;
    }

    qDebug()<<"All point are checked!";


    if(Linejump)
    {   //第一条直线跨界起点
        qDebug()<<"excute the other type!";
        if(abs(input_int[0]-input_int[length-1])>MinL)
        {

            //此处标明，第一条直线起点和最后一条直线尾点之间存在曲线
            QVector<int> Gap_Point;//Curve point's order in OrderdOutLine
            for(int qq=input_int[length-1];qq<input_int[0];qq++)
            {
                Gap_Point.push_back(qq);
            }//获取所有曲线点位置序号
            //获取曲线两端直线的斜率
            QVector<double>TwoSlope;

            TwoSlope.push_back(lineslope[0]);//脚标需要对应查询
            TwoSlope.push_back(lineslope[lineslope.length()-2]);
            if(Gap_Point.length()<MinL){
                //  QMessageBox::information(NULL,"WRONG","Gappoint length is not enough!");
                //qDebug()<<input_int[0]<<"   "<<input_int[length-1];
            }
            qDebug()<<"case 2 send to checkpoint in line";
            if(Gap_Point.length()>1){
                fortemp=FindKeypoints(Gap_Point,allp,MinL,KeyP);


                foreach (int k, fortemp) {
                    CharacterPoints_int.push_back(k);
                }
            }
            fortemp.clear();
        }

    }
    else//第一条直线没有跨界,即有一条曲线包含了起始点
    {
        qDebug()<<"the first line didnt jumped the first point;";

        QVector<int> Gap_Point;//Curve point's order in OrderdOutLine

        for(int qq=input_int[input_int.length()-1];qq<=allp.length()-1;qq++)
        {
            Gap_Point.push_back(qq);
        }
        for(int qq=0;qq<=input_int[0];qq++)
        {
            Gap_Point.push_back(qq);
        }
        qDebug()<<"to get the two line slope";
        //获取所有曲线点位置序号
        //获取曲线两端直线的斜率
        QVector<double>TwoSlope;
        if(Gap_Point.length()<MinL){

            CharacterPoints_int[length-1]=(input_int[length-1]+input_int[0])/2;
            //CharacterPoints_int[0]=(input_int[input_int.length()-1]+input_int[0])/2;
        }
        else
        {
            TwoSlope.push_back(lineslope[0]);//脚标需要对应查询
            TwoSlope.push_back(lineslope[lineslope.length()-2]);
            qDebug()<<"case 3 send to checkpoint in line";
            fortemp=FindKeypoints(Gap_Point,allp,MinL,KeyP);


            foreach (int k, fortemp) {
                CharacterPoints_int.push_back(k);
            }
            fortemp.clear();
        }
    }
    //去除 忽略点

    if(RemoveP_int.length()==0)//没有忽略点
    {
        Toreturn=CharacterPoints_int;
    }
    else//有忽略点
    {
        for(int i=0;i<length;i++)
        {

            bool Nooth=false;

            for(int j=0;j<RemoveP_int.length();j++)
            {
                if(input_int[i]==RemoveP_int[j])
                {
                    Nooth=true;
                }
            }
            if(!Nooth){
                Toreturn.push_back(input_int[i]);
            }
        }
    }

    RemoveP_int.clear();



    foreach (int k, CharacterPoints_int) {
        Toreturn.push_back(k);
    }

    //qDebug()<<"original to return is "<<Toreturn;
    Toreturn=ReorderArray(Toreturn,1);
    //qDebug()<<"Orderedtoreturn is "<<Toreturn;
    Toreturn=Unique_Int(Toreturn);

    //  qDebug()<<"uniqued return is "<<Toreturn;
    QVector<QVector2D>To2D=TransSequenceTo2D(allp,Toreturn);

    //qDebug()<<"2D is"<<To2D;

    Output2File(To2D,"F:/output/To2D.txt");
    qDebug()<<"out function linemerge!";
    qDebug()<<"Linemerge time used:    "<<QString::number(testtimer.elapsed());
    return Toreturn;
}
QVector<QVector2D> CircularStitching(QVector<QVector2D>Line1,QVector<QVector2D>Line2){
    return Line1;
}
QVector<double> PointToLineDis(QVector<QVector2D> LinePoint,QVector<QVector2D>Points){

    /*
 * Linepoint是确定直线的两个点xy坐标
 * Points是需要考察到上述直线距离的点xy坐标
 */
    int length=Points.length();
    QVector<double>Toreturn;

    double ss;

    ss=SingelSlopeCalculate(LinePoint[1],LinePoint[0]);

    //qDebug()<<"Slope is done!                 [PointToLineDis]";

    //判断斜率情况，写出Ax+By+C=0的直线方程
    //返回的距离参数，同时还标明方向，设定为，直线方向的左手边为+1，右手边为-1,在直线上设定为0

    if(ss== CV_PI/2){
        //竖直
        //qDebug()<<"case one;        [PointToLineDis]";
        for(int i=0;i<Points.length();i++)
        {
            double dis=abs(Points[i].x()-LinePoint[0].x());
            Toreturn.push_back(dis);
            if(Points[i].x()<LinePoint[0].x()){
                Toreturn.push_back(1.0);
            }else if(Points[i].x()==LinePoint[0].x()){
                Toreturn.push_back(0);
            }else{
                Toreturn.push_back(-1);
            }

        }

    }else if(ss==CV_PI*1.5){

        //qDebug()<<"case two;        [PointToLineDis]";

        //竖直
        for(int i=0;i<Points.length();i++)
        {
            double dis=abs(Points[i].x()-LinePoint[0].x());

            Toreturn.push_back(dis);

            if(Points[i].x()<LinePoint[0].x()){

                Toreturn.push_back(-1.0);

            }else if(Points[i].x()==LinePoint[0].x()){

                Toreturn.push_back(0);

            }
            else
            {

                Toreturn.push_back(1.0);
            }
        }

    }

    else{

        double k=tan(ss);

        double c=LinePoint[0].y()-k*LinePoint[0].x();

        for(int i=0;i<Points.length();i++)
        {
            double d=abs((k*Points[i].x()-Points[i].y()+c)/sqrt(pow(k,2)+1));

            Toreturn.push_back(d);

            if(SingelSlopeCalculate(LinePoint[0],Points[i])-ss>0&&SingelSlopeCalculate(LinePoint[0],Points[i])-ss<CV_PI/2)
            {
                Toreturn.push_back(1.0);
            }
            else if(SingelSlopeCalculate(LinePoint[0],Points[i])<ss||SingelSlopeCalculate(LinePoint[0],Points[i])-ss>CV_PI/2)
            {
                Toreturn.push_back(-1.0);
            }
            else
            {
                Toreturn.push_back(0);
            }
        }

    }
    //qDebug()<<"returned from PointToLineDis                       [PointToLineDis]";
    return Toreturn;

}
int AngelCompare(double slope1,double slope2,double tolerance)
{
    //this function is to identify the two slope near or not!

    //this function is to identify the two slope near or not!

    //返回参数设定成 -1 方向相反，超过公差
    //              0 方向相反，在公差内
    //              1 方向同向，在公差内
    //              2 方向同向，超过公差
    //             -2 反向相差180°，如果正向则在公差内
    //qDebug()<<"===================================>Enter the Campare";
    //qDebug()<<"slope1 is  "<<slope1<<"slope2 is  "<<slope2;
    if(slope1>1.5*CV_PI&&slope2<0.5*CV_PI)
    {
        //一、四象限做比较
        //qDebug()<<"case one";
        double dvalue=slope2+(2*CV_PI-slope1);
        //qDebug()<<dvalue<<"  dvalue ";
        if(dvalue<tolerance){
           // qDebug()<<"out the Campare";
          //  qDebug()<<"return is "<<1;
            return 1;
        }else{
         //   qDebug()<<"out the Campare";
            return 2;
          //  qDebug()<<"return is "<<2;
        }
    }
    else if(slope2>1.5*CV_PI&&slope1<0.5*CV_PI)
    {
        //同上
       // qDebug()<<"case two";
        double dvalue=slope1+(2*CV_PI-slope2);
        //qDebug()<<dvalue<<"  dvalue ";
        if(dvalue<tolerance){
            //qDebug()<<"out the Campare(Two para)";
         //   qDebug()<<"return is "<<1;
            return 1;
        }else{
            //qDebug()<<"out the Campare(Two para)";
           // qDebug()<<"return is "<<2;
            return 2;
        }
    }
    else
    {
        //qDebug()<<"case three";
        double dvalue=abs(slope1-slope2);
       // qDebug()<<dvalue<<"  dvalue ";
        if(dvalue<tolerance){

            //qDebug()<<"out the Campare(Two para)";
        //    qDebug()<<"return is "<<1;
            return 1;
        }else{
            //qDebug()<<"out the Campare(Two para)";
          //  qDebug()<<"return is "<<2;
            QMessageBox::information(NULL,"a",QString::number(slope1)+QString::number(slope2));
            return 2;
        }
    }


}

void Output2File(QVector<double>InputArray,QString Outputadd,int newline)
{
    //newline means that every line can only contain 1 word
    if(Outputadd.isEmpty()){
        //   QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="F:/output/outputfiles"+QString::number(qrand())+".txt";
    }
    int length=InputArray.length();


    QFile *outflie=new QFile;
    outflie->setFileName(Outputadd);
    bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok)
    {
        int count=0;
        QTextStream out(outflie);
        for(int i=0;i<length;i++)
        {

            QString outstr;
            outstr=QString::number(InputArray[i]);
            outstr.append(" ");

            out<<outstr;
            count++;
            if(count==newline){
                out<<endl;
                count=0;
            }

        }
        outflie->close();
        delete outflie;
    }
}

bool AngelCompare(QVector<double>Asl,int spot,QVector<double> BSlope,double tolerance)
{
    /*
        Asl 是六个斜率
        spot 是当前考察的Breakpoint的位置序号
        BSlope 是该考察点相对于前后Breakpoint的斜率
        tolerance 是允许的角度公差
   */
    qDebug()<<"===================================>Enter the function AngelComapare origin one!";
    double *Preslo=new  double [3];
    double *Suffixslo=new  double [3];

    for(int i=0;i<6;i++)
    {
        if(i<3){
            Preslo[i]=Asl[i];
        }else{
            Suffixslo[i-3]=Asl[i];
        }
    }
    double WeightedAverage_Pre=0.3*Preslo[0]+0.5*Preslo[1]+0.2*Preslo[2];
    double WeightedAverage_Be=0.2*Suffixslo[0]+0.5*Suffixslo[1]+0.3*Suffixslo[2];

    int ans= AngelCompare(WeightedAverage_Pre,WeightedAverage_Be,tolerance);

    if(ans==1)
    {
        //角度偏差小
        //该点忽略
        qDebug()<<"out from the function AngelComapare Vector one(Vector  and return bool)!";
        delete[]Preslo;
        delete[]Suffixslo;
        return false;

    }else if(ans==2)
    {
        //角度偏差大
        //不可忽略
        qDebug()<<"out from the function AngelComapare Vector one(Vector  and return bool)!";
        delete[]Preslo;
        delete[]Suffixslo;
        return true;
    }
    qDebug()<<"out from the function AngelComapare origin one!(Vector  and return bool)";
}
QVector<double>DistanceEvaluate(QVector<double>dis)
{
    qDebug()<<"===================================>Enter the function *** DistanceEvaluate! ***";
    //这个函数用来给距离序列进行评估，设定为距离为0的得分10;距离计算公式:10-dis/10,低于0分的直接给0分;
    double Thresh=10.0;

    QVector<double>Toreturn;
    for(int i=0;i<dis.length();i++)
    {

    }
    qDebug()<<"out from the *** funtion distanceevaluate ***";
    return Toreturn;
}
QVector<int>Unique_Int(QVector<int>Input)
{
    //该函数按照顺序删除相同的元素
    QVector<int>Toreturn;
    Toreturn.push_back(Input[0]);
    bool ok;
    for(int i=1;i<Input.length();i++)
    {
        ok=false;
        foreach (int k, Toreturn)
        {
            if(Input[i]==k)
            {
                ok=true;
                break;
            }
        }
        if(!ok)
        {
            Toreturn.push_back(Input[i]);
        }
    }


    return Toreturn;

}
QVector<QVector2D>VectorTransposition(QVector<QVector2D>point_2D,int Transpos)
{
    /*
     * 该函数的作用是进行xy坐标在point_2D 向量中位置进行环移
     * point_2D 是输入的一圈坐标点xy
     * Transpos 是移动距离，+6表示Point_2D[6]换到Point2D[11];
     */
    qDebug()<<"===================================>Enter the VectorTranposition    [VectorTransposition]";
    QVector<QVector2D>Toreturn;
    if(Transpos==0)
    {
        qDebug()<<"OUT the VectorTranposition    [VectorTransposition]";

        return point_2D;
    }
    else if(Transpos<0)
    {

        for(int i=point_2D.length()+Transpos;i<point_2D.length();i++)
        {
            Toreturn.push_back(point_2D[i]);
        }
        for(int q=0;q<point_2D.length()+Transpos;q++)
        {
            Toreturn.push_back(point_2D[q]);
        }
        qDebug()<<"OUT the VectorTranposition    [VectorTransposition]";
        return Toreturn;

    }
    else
    {

        for(int q=Transpos;q<point_2D.length();q++)
        {
            Toreturn.push_back(point_2D[q]);
        }
        for(int i=0;i<Transpos;i++)
        {
            Toreturn.push_back(point_2D[i]);
        }
        qDebug()<<"OUT the VectorTranposition    [VectorTransposition]";
        return Toreturn;
    }


}
QVector<QVector2D>Unique_2D(QVector<QVector2D>Input)
{
    //该函数按照顺序删除相同的元素
    QVector<QVector2D>Toreturn;
    Toreturn.push_back(Input[0]);
    bool ok;
    for(int i=1;i<Input.length();i++)
    {
        ok=false;
        foreach (QVector2D k, Toreturn)
        {
            if(Input[i]==k)
            {
                ok=true;
                break;
            }
        }
        if(!ok)
        {
            Toreturn.push_back(Input[i]);
        }
    }
    return Toreturn;
}
void QimageSave(QImage Tosave,QString add,int width,int height)
{
    QString pres="F:/output/";
    pres.push_back(add);
    pres.push_back(".png");
    Tosave.scaled(width,height);

    Tosave.save(pres,"PNG",100);
}
bool IsEvenNumber(int input)
{
    if(input%2==1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
double SumofArray(QVector<double>inputarray)
{
    double Toreturn=0;
    foreach (double val, inputarray) {
        Toreturn+=val;
    }


    return Toreturn;




}
QVector<int>VecAddVec(QVector<int>MainV,QVector<int>lowV,int n)
{

    if(n==1)
    {
        foreach (int k, lowV) {
            MainV.push_back(k);
        }
        return MainV;
    }
    if(n==0)
    {
        foreach (int k, MainV) {
            lowV.push_back(k);
        }
        return lowV;
    }


}
