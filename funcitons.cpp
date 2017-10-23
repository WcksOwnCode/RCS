/**************************************************************************************************/

/*此处存放的函数主要是用来计算曲线特征点的函数，为了在mainwindow里看起来简单一些
 * 主要是距离计算  斜率计算  斜率简化 等函数组成，目的在于找到曲线行走必须的点
/**************************************************************************************************/


#include "funcitons.h"
#include <cmath>
#include <QMessageBox>
#include"mainwindow.h"
#include"worldvalues.h"

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
    QString outaddr="C:/Users/duke/Desktop/Derections.txt";
    int length=DCF.length();
    if(length==0)
    {
        QMessageBox::information(NULL,"notice","length is zero!  (DRECTIONCalFunc)");
        return DCF;
    }
    QVector2D tempDere;
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
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(1);
                num_Dere.push_back(ThisDerec);
                break;
            case 0:
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(8);
                num_Dere.push_back(ThisDerec);
                break;
            default:
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(7);
                num_Dere.push_back(ThisDerec);
                break;
            }
            break;
        case 0:
            switch (tempyyy)
            {
            case -1:
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(2);
                num_Dere.push_back(ThisDerec);
                break;
            case 1:
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(6);
                num_Dere.push_back(ThisDerec);
                break;
            default:
                break;
            }
            break;
        default:
            switch (tempyyy)
            {
            case -1:
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(3);
                num_Dere.push_back(ThisDerec);
                break;
            case 0:
                ThisDerec.setX(tempDere.x());
                ThisDerec.setY(4);
                num_Dere.push_back(ThisDerec);
                break;
            default:
                ThisDerec.setX(tempDere.x());
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
    int length=Dec.length();
    if(length==0)
    {
        QMessageBox::information(NULL,"notice","length is zero!  (GetBreakPoints)");

    }
    bool outtotxt=false;
    QString outaddr="C:/Users/duke/Desktop/Derections.txt";
    QVector<int> toreturn;
    for(int i=0;i<length;i++)
    {
        if(i>0)
        {
            if(Dec[i]!=Dec[i-1])
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
    return toreturn;
}
double SingelSlopeCalculate(QVector2D SSC1,QVector2D SSC2)
{
    //用于计算两点斜率用的
    //SSC1\SSC2分别是两个需要计算的点的坐标，都是二维的xy
    //返回两点的斜率角度0-360
    double toreturn;
    if(SSC1.x()!=SSC2.x())//分母不能为0
    {
        if(SSC1.y()!=SSC2.y())//分子不为0，为0就是水平
        {
            toreturn=atan((SSC1.y()-SSC2.y())/(SSC1.x()-SSC2.x()));
        }
        else
        {
            if(SSC1.y()>SSC2.y())
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
        if(SSC1.y()>SSC2.y())
        {
            toreturn=CV_PI/2;
        }
        else if(SSC1.y()<SSC2.y())
        {
            toreturn=1.5*CV_PI;
        }

    }
    return toreturn;
}

QVector<double> Slope(QVector<QVector2D> S, int d)
{
    //S是传入的点阵序列，
    //d是间隔求取的的数量
    int length=S.length();

    if(length<3)
    {
        QMessageBox::information(NULL,"notice","length is not enough!  (Simplelify the slopes)");

    }
    bool outtotxt=false;

    QString outaddr="C:/Users/duke/Desktop/slope.txt";

    double *d_slope=new double[length-1];
    QVector<double> toreturn;

    for(int i=1;i<length;i=i+d)
    {
        if(S[i].x()!=S[i-1].x())//分母不能为0
        {
            if(S[i].y()!=S[i-1].y())//分子不为0，为0就是水平
            {
                d_slope[i-1]=atan((S[i].y()-S[i-1].y())/(S[i].x()-S[i-1].x()));
            }
            else
            {
                if(S[i].y()>S[i-1].y())
                {
                    d_slope[i-1]=0.0;
                }
                else
                {
                    d_slope[i-1]=CV_PI;
                }

            }
        }
        else
        {
            if(S[i].y()>S[i-1].y())
            {
                d_slope[i-1]=CV_PI/2;
            }
            else if(S[i].y()<S[i-1].y())
            {
                d_slope[i-1]=1.5*CV_PI;
            }
            else//超级特殊情况，分子分母都为零,说明两个特征点重合，要避免，我会在前面家一个unique，这个不会发生什么，但是报警一下
            {
                qDebug()<<"there are two same points here to calculate:"<<i<<"   "<<i-1<<"(slope function)";
                QMessageBox::warning(NULL,"Warning","two point is the same ,no slope ,please check!") ;
            }
        }



    }

    for(int i=0;i<length-1;i++)
    {
        toreturn.push_back(d_slope[i]);
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

    QString outaddr="C:/Users/duke/Desktop/SimplifySlope.txt";

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

    QString outaddr="C:/Users/duke/Desktop/SimplifySlope.txt";

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
    QString outaddr="C:/Users/duke/Desktop/distance.txt";
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
QVector<int> CheckPointInline(QVector<int>BP, int Pcount, QVector<double>TSlope, QVector<QVector2D> OOL, int MinL)
{
    //此函数用在进行了斜率简化后的对长线段之间的点进行筛查用的
    //主要检测两个长线段之间的点是否有必要保留
    //BP是曲线点序号
    //OOL是边界点坐标，OrderdOutline

    const double Lneed=18.0;//视为直线最小长度；
    const double miss=10.0;//正负miss°的角度匹配值

    int inidecrese=9;

    QVector<int>line;//储存检测出的长度足够的线
    QVector<int>IgnoreP;//储存可以忽略的点
    QVector<int>ToCurve;//送去检测曲线的点
    QVector<QVector2D>CurveRe;//储存Curvecheck返回的点
    QVector<int> Toreturn;



    if(Pcount<=minL)
    {//先查看是不是可能为有一点小波动的直线

        QVector<QVector2D>Linep;
        QVector<QVector2D>midPoints;
        Linep.push_back(OOL[BP[0]]);
        Linep.push_back(OOL[BP[Pcount-1]]);
        for(int i=1;i<Pcount-1;i++)
        {
            midPoints.push_back(OOL[BP[i]]);
        }
       QVector<double>alldis= PointToLineDis(Linep,midPoints);
       double sumdis;
       double maxdis;
       int maxpos;
       //对alldis进行考察：
       maxdis=0;
       maxpos=0;
       for(int k=0;k<alldis.length();k+=2)
       {
           sumdis=sumdis+alldis[k];
           if(maxdis<alldis[k])
           {
               maxdis=alldis[k];
               maxpos=k;
           }
       }
       if(sumdis<10&&maxdis<4){
          //视作直线
           Toreturn.push_back(BP[0]);
           Toreturn.push_back(BP.end());
       }else
       {
           Toreturn.push_back(BP[maxpos]);
       }

    }
    else
    {
        //对曲线进行9个点依次的离散


    }






    return Toreturn;//temp return
}

QVector<QVector2D>CurveCheck(QVector<QVector2D>Outline,QVector<int>P,bool isgetcurve)
{
    //该函数检测输入的点是不是曲线点，检测方式是斜率渐变。
    //Outline是全部的边界点数据，
    //P是全部的候选点序列
    //返回类型是QVector2D的序列，记录方式是[曲线1起点，曲线1结束点]，[曲线2起点，曲线2结束点]....
    //该函数应该放置在直线检测之后，检测直线之间的点是否是曲线点
    // isgetcurve 默认参数是false，这个参数表示该函数是否进行曲线检测，false是进行，true是直接返回曲线离散点(等距)
    //返回 曲线离散点的时候 记录方式每一个2D记录数据相同
    int CurveD=2;//间隔多少个点进行一次求取
    int tempCurved=2;//当CerveD太大，用tempCurved来离散
    int length=P.length();
    int num=P[length]-P[0];//获取总共点数
    QVector<QVector2D> ToReturn;
    float thresh=15.0;
    CurveD=P2Pcalculate(num);//计算获取间距

    if(!isgetcurve){

        QVector<QVector2D> Vd_Toslope;
        QVector<double> Vd_slope;
        QVector2D needed;
        for(int i=0;i<num;i++)
        {
            needed.setX(Outline[P[i]].x());
            needed.setY(Outline[P[i]].y());
            Vd_Toslope.push_back(needed);//从Outline中提取出需要的点
        }


        Vd_slope=Slope(Vd_Toslope,CurveD);//获取多间隔斜率
        //检测斜率规律
        QVector<double>change;

        for(int q=1;q<Vd_slope.length();q++)
        {
            //step one 渐变检测，看是不是逐渐增加或者逐渐减少
            change.push_back(Vd_slope[q]-Vd_slope[q-1]);
        }
        QVector<QVector2D>checkreturn;
        QVector<int>unqualified;
        QVector<int>qualified;
        checkreturn=Performance(change);
        for(int x=0;x<checkreturn.length();x++)
        {
            if(checkreturn[x].x()==1){
                qualified.push_back(checkreturn[x].y());
            }else if(checkreturn[x].x()==-8)
            {
                qDebug()<<"checkreturn is zero! wrong!";
                exit(0);
            }
            else
            {
                unqualified.push_back(checkreturn[x].y());
            }
        }
        //分理出合格和不合格的点
        //对上述提取的四种数据进行分析
        //考虑是不是做函数
        //先对跨距太大的点进行测试
        if(unqualified.length()!=1||unqualified[0]!=-8)
        {
            for(int l=0;l<unqualified.length();l++)
            {
                int pstart;
                int pend;
                int shortcount;
                pstart=unqualified[l];
                if(unqualified[l]+1==unqualified[l+1])
                {
                    pend=unqualified[l+1]+1;
                    l++;
                    continue;
                }
                else
                {
                    pend=unqualified[l]+2;

                }
                shortcount=pend-pstart;

                tempCurved=P2Pcalculate(shortcount);//recalculate a new gap

                //用新的间距重新离散
                QVector<QVector2D>  tempcheckreturn;
                QVector<int> tempunqualified;

                for(int x=0;x<tempcheckreturn.length();x++)
                {
                    if(tempcheckreturn[x].x()==1){
                        qualified.push_back(tempcheckreturn[x].y());
                    }else if(tempcheckreturn[x].x()==-8)
                    {
                        qDebug()<<"tempcheckreturn is zero! wrong!";
                        exit(0);
                    }
                    else
                    {
                        tempunqualified.push_back(checkreturn[x].y());
                    }
                }
                QVector<int>tttoomax;
                int outcount=0;
                do{
                    Vd_Toslope.clear();//清除数据
                    Vd_slope.clear();
                    for(int i=0;i<num;i++)
                    {
                        needed.setX(Outline[P[i]].x());
                        needed.setY(Outline[P[i]].y());
                        Vd_Toslope.push_back(needed);//从Outline中提取出需要的点
                    }

                    Vd_slope=Slope(Vd_Toslope,tempCurved);//获取多间隔斜率
                    change.clear();
                    for(int q=1;q<Vd_slope.length();q++)
                    {
                        //step one 渐变检测，看是不是逐渐增加或者逐渐减少
                        change.push_back(Vd_slope[q]-Vd_slope[q-1]);
                    }
                    tttoomax.clear();
                    for(int q=0;q<change.length();q++)
                    {
                        if(abs(change[q])>thresh)//正向角度误差0~15°
                        {//超过15°的阈值，可能是变化激增，很小的圆，但是跨距选大了
                            tttoomax.push_back(q);
                        }
                    }
                    tempCurved--;
                    outcount++;
                    if(outcount>15||tempCurved==2)
                    {
                        QMessageBox::information(NULL,"NOTICE","循环次数太多，无法达到最好的分段");
                        break;
                    }
                }while(tttoomax.length()>1);

            }

        }
    }
    else//直接进行曲线等距离散化，返回离散化后的关键点
    {
        for(int i=0;i<=num;i=i+CurveD)
        {
            QVector2D tt;
            tt.setX(i);
            tt.setY(i);
            ToReturn.push_back(tt);
        }
    }
    return ToReturn;

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
int P2Pcalculate(int pnum)
{

    int toreturn=0;

    if(pnum<8)
    {
        toreturn=2;
    }else if(pnum>=8&&pnum<24)
    {
        toreturn=4;
    }
    else if(pnum>=24&&pnum<56)
    {
        toreturn=8;
    }
    else if(pnum>=56&&pnum<112)
    {
        toreturn=12;
    }
    else if(pnum>=112&&pnum<176)
    {
        toreturn=16;
    }
    else if(pnum>=176&&pnum<256)
    {
        toreturn=20;
    }
    else{
        toreturn=20+(pnum-246)/10;
    }
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
    imshow("mat",graymat);


    drawmat=graymat.clone();
    std::vector<cv::Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合

    cv::HoughLinesP(graymat, lines, 1, CV_PI/180,PointCount,minmumLine*2,200); //(in,out,rho,theta,threshold,minlength,maxgap)



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

    qDebug()<<"Slines length is:     "<<SLines.length();
    Output2File(SLines,"C:/Users/duke/Desktop/oriSLines.txt");
    cv::imwrite("C:/Users/duke/Desktop/cvsave.png",drawmat);

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

    imshow("lines",drawmat);


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
        qDebug() << "CV_8UC4";
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

void Output2File(QVector<QVector2D>InputArray,QString Outputadd)
{
    if(Outputadd.isEmpty()){
        QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="C:/Users/duke/Desktop/outputfiles.txt";;
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
        Outputadd="C:/Users/duke/Desktop/outputfiles.txt";;
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

QVector<QVector2D> PointReorder(QVector<QVector2D>input,QVector<QVector2D>templateArray)
{
    int templength=templateArray.length();

    int inputLength=input.length();

    int *spoto=new int[inputLength];


    QVector<QVector2D>Toreturn;

    if(inputLength==0){
        QMessageBox::information(NULL,
                                 "length","the length of the input is zero!");
        exit(0);
    }
    if(templength==0){
        QMessageBox::information(NULL,
                                 "length","the length of the template array is zero!");
        exit(0);
    }

    double mindis;
    double tempdis;

    for(int j=0;j<inputLength;j++)
    {
        bool find=false;

        for(int i=0;i<templength;i++)
        {

            if(input[j].x()==templateArray[i].x()&&input[j].y()==templateArray[i].y())
            {
                spoto[j]=i;
                find=true;
                break;
            }
        }
        if(!find)
        {
            //means no correctly same points
            mindis=DisCalFuc(input[j].x(),input[j].y(),templateArray[0].x(),templateArray[0].y());

            spoto[j]=0;

            for(int i=1;i<templength-1;i++)
            {
                tempdis=DisCalFuc(input[j].x(),input[j].y(),templateArray[i].x(),templateArray[i].y());

                if(mindis>=tempdis)
                {
                    mindis=tempdis;

                    spoto[j]=i;
                }
            }
        }

    }


    qDebug()<<"input length:    "<<inputLength;
    for(int j=0;j<inputLength;j++)
    {
        int mins=0;
        int minv=spoto[0];

        for(int i=1;i<inputLength;i++)
        {

            if(minv>spoto[i])
            {
                mins=i;
                minv=spoto[i];
            }

        }

        Toreturn.push_back(input[mins]);

        spoto[mins]=52000;

    }





    //then check the length
    if(Toreturn.length()!=inputLength)
    {
        //that means the some the input points may not in outline points,it has fault
        QMessageBox::information(NULL,"NOTICE",
                                 "Here is not right,some of the poits are not in template array,please check");

        exit(0);


    }


    qDebug()<<"toreturen:    "<<Toreturn;

    qDebug()<<Toreturn.length()<<"this is toreturn's length ";

    delete[] spoto;

    spoto=NULL;

    return Toreturn;

}

QVector<int> PointReorder_Rint(QVector<QVector2D>input,QVector<QVector2D>templateArray)
{

    int templength=templateArray.length();

    int inputLength=input.length();

    QVector<int>Toreturn;

    if(inputLength==0){
        QMessageBox::information(NULL,
                                 "length","the length of the input is zero!");
        exit(0);
    }
    if(templength==0){
        QMessageBox::information(NULL,
                                 "length","the length of the template array is zero!");
        exit(0);
    }

    double mindis;
    double tempdis;

    for(int j=0;j<inputLength;j++)
    {
        bool find=false;

        for(int i=0;i<templength;i++)
        {

            if(input[j].x()==templateArray[i].x()&&input[j].y()==templateArray[i].y())
            {
                Toreturn.push_back(i);
                find=true;
                break;
            }
        }
        if(!find)
        {
            //means no correctly same points
            mindis=DisCalFuc(input[j].x(),input[j].y(),templateArray[0].x(),templateArray[0].y());

            int tempxx=0;

            for(int i=1;i<templength-1;i++)
            {
                tempdis=DisCalFuc(input[j].x(),input[j].y(),templateArray[i].x(),templateArray[i].y());

                if(mindis>=tempdis)
                {
                    mindis=tempdis;

                    tempxx=i;
                }
            }
            Toreturn.push_back(tempxx);
        }

    }


    qDebug()<<"befor reorder function: "<<endl<<Toreturn;

    if(Toreturn.length()%2!=0){
        QMessageBox::warning(NULL,"warning","the length of the is not odd");
        exit(0);
    }

    for(int i=0;i<Toreturn.length();i+=2)
    {
        if(Toreturn[i]>Toreturn[i+1])
        {
            SwapT_int_int(Toreturn[i],Toreturn[i+1]);
        }
    }
    QVector<int>toorder;
    QVector<int>getorder;
    for(int i=0;i<Toreturn.length();i+=2)
    {
        toorder.push_back(Toreturn[i]);
    }
    getorder=ReorderArray(toorder,1);

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


    Toreturn.clear();
    Toreturn=newtoreturn;

    qDebug()<<"After the reorder:    "<<endl<<Toreturn;

    //then check the length
    if(Toreturn.length()!=inputLength)
    {
        //that means the some the input points may not in outline points,it has fault
        QMessageBox::information(NULL,"NOTICE",
                                 "Here is not right,some of the poits are not in template array,please check");

        exit(0);


    }




    // qDebug()<<Toreturn.length()<<"this is toreturn's length ";





    return Toreturn;
}

void Find_Center(QVector<QVector2D>Circle, QVector<double>cent, double radiuss)
{
    //to find all the points center
    int length=Circle.length();

    for(int i=0;i<length;i++)
    {

    }













}
QVector<int> LineMerge(QVector<int>input_int,QVector<QVector2D>input_Point,QVector<QVector2D>allp,int minL=10)
{
    //after houghlinesP function ,we will get some strait line which is represeted by two points(start
    // and end points)

    if(input_int.length()!=input_Point.length())
    {
        QMessageBox::information(NULL,"LineMergeFunction","Two input array's length is not the same!");
        exit(0);
    }
    int length=input_int.length();


    if(length<2)
    {
        QMessageBox::information(NULL,"LineMergeFunction","Length is not enough for a line!");
        return input_int;
    }
    QVector<QVector2D>CurveP;//store the Curve start point and end point;send it to Curve Check;
    QVector<int> Toreturn;
    double disthresh=5;//if there are two adjacent points distance is miner than this ,they will be merged
    double dis;
    const double degreeT=CV_PI*10/180;
    QVector<double>lineslope;

    //get the slope between lines;

    lineslope= Slope(input_Point);
    qDebug()<<lineslope<<"   lineslope is this!";
    int slopelength=lineslope.length();



    QVector<int>RemoveP_int;

    for(int i=0;i<length-1;i++)
    {
        if(abs(input_int[i]-input_int[i+1])<=1)
        {


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

                    input_int[i]=input_int[i+1];
                    input_Point[i]=input_Point[i+1];



                }

            }

            else{
                QMessageBox::information(NULL,"notice","Out of range ,slope check is here!");
                exit(0);
            }

        }
        else if(abs(input_int[i]-input_int[i+1])<=minL&&abs(input_int[i]-input_int[i+1]>1))//直线不会短于minL
        {
            if(i+1<=slopelength)
            {//to check will here index out of range
                if(qAbs(lineslope[i-1]-lineslope[i+1])<degreeT)
                {
                    //int tempomid=(input_int[i]+input_int[i+1])/2;
                    qDebug()<<"the slope is : "<<qAbs(lineslope[i-1]-lineslope[i+1]);
                    RemoveP_int.push_back(input_int[i]);

                    RemoveP_int.push_back(input_int[i+1]);
                }
                else
                {
                    //直线做圆弧拼接
                    input_int[i]=(input_int[i+1]+input_int[i])/2;

                }
            }
            else{
                QMessageBox::information(NULL,"notice","Out of range ,slope check is here!");
                exit(0);
            }
        }else
        {
            //gap is longer than minL

            QVector2D Gap_Point;
            Gap_Point.setX(input_int[i]);
            Gap_Point.setY(input_int[i+1]);
            QVector<QVector2D>Curve_Points;
            QVector<double>Lslope;
            Lslope.push_back(lineslope[i-1]);
            Lslope.push_back(lineslope[i+1]);
            int CurveP_count=input_int[i+1]-input_int[i]+1;
            for(int k=0;k<=CurveP_count;k++)
            {
                Curve_Points.push_back(allp[input_int[i]+k]);
            }
           // CheckPointInline(Curve_Points,CurveP_count,Lslope,allp);

        }


    }

    for(int i=0;i<length;i++)
    {
        if(RemoveP_int.length()==0)
        {
            Toreturn.push_back(input_int[i]);
        }
        else
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


    return Toreturn;
}
QVector<QVector2D> CircularStitching(QVector<QVector2D>Line1,QVector<QVector2D>Line2){
    return Line1;
}
QVector<double> PointToLineDis(QVector<QVector2D> LinePoint,QVector<QVector2D>Points){

    int length=Points.length();



    QVector<double>Toreturn;
    double ss;
    ss=SingelSlopeCalculate(LinePoint[0],LinePoint[1]);
    //判断斜率情况，写出Ax+By+C=0的直线方程
    //返回的距离参数，同时还标明方向，设定为，直线方向的左手边为+1，右手边为-1,在直线上设定为
    switch (ss) {
    case CV_PI/2:
        //竖直
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



        break;
    case CU_PI*1.5:
        //竖直
        for(int i=0;i<Points.length();i++)
        {
            double dis=abs(Points[i].x()-LinePoint[0].x());
            Toreturn.push_back(dis);
            if(Points[i].x()<LinePoint[0].x()){
                Toreturn.push_back(-1.0);
            }else if(Points[i].x()==LinePoint[0].x()){
                Toreturn.push_back(0);
            }else{
                Toreturn.push_back(1.0);
            }
        }
        break;
    default:

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


        break;
    }

    return Toreturn












}
