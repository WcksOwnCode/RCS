/**************************************************************************************************/

/*此处存放的函数主要是用来计算曲线特征点的函数，为了在mainwindow里看起来简单一些
 * 主要是距离计算  斜率计算  斜率简化 等函数组成，目的在于找到曲线行走必须的点
/**************************************************************************************************/


#include "funcitons.h"
#include <cmath>
#include <QMessageBox>

double DisCalFuc(int x1, int y1, int x2, int y2)
{
    double re=0;
    re=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
    return re;
}
void ReorderArray(int *p, int length, int n)
{

    if(n=0){
        //从小到大排列
        for(int i=length;i>0;i--)
        {
            for(int j=0;j<i;j++)
            {
                if(p[i]>p[j])
                {
                    //  SwapT(p[i],p[j]);
                }
            }

        }
    }
    else if(n=1)
    {
        //从da到xiao排列
        for(int i=length;i>0;i--)
        {
            for(int j=0;j<i;j++)
            {
                if(p[i]<p[j])
                {
                    //SwapT(p[i],p[j]);
                }
            }

        }
    }
    else
    {
        QMessageBox::information(NULL,"notice!","the third parameter is inappropriate!");
        exit(0);
    }
}

void SwapT(double &a,double &b)
{
    double x;
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
                toreturn=180.0;
            }

        }
    }
    else
    {
        if(SSC1.y()>SSC2.y())
        {
            toreturn=90.0;
        }
        else if(SSC1.y()<SSC2.y())
        {
            toreturn=270.0;
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
                    d_slope[i-1]=180.0;
                }

            }
        }
        else
        {
            if(S[i].y()>S[i-1].y())
            {
                d_slope[i-1]=90.0;
            }
            else if(S[i].y()<S[i-1].y())
            {
                d_slope[i-1]=270.0;
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



QVector<int> SimplifySlope(QVector<double>S_Slope,QVector<int> BP)
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

QVector<double> Distance(QVector<QVector2D> Into)
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
    for(int i=1;i<length;i++)
    {
        temp=DisCalFuc(Into[i].x(),Into[i].y(),Into[i-1].x(),Into[i-1].y());
        toreturn.push_back(temp);
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
QVector<int> CheckPointInline(QVector<double> dis, QVector<double> slope, QVector<int>BP, QVector<QVector2D> OOL)
{
    //此函数用在进行了斜率简化后的对长线段之间的点进行筛查用的
    //主要检测两个长线段之间的点是否有必要保留
    //BP是转折点的位置序号
    //dis是转折点之间两两距离
    //slope是转折点之间两两斜率
    //OOL是边界点坐标，OrderdOutline
    int length=slope.length();
    if(length<3)
    {
        QMessageBox::information(NULL,"notice","length is not enough!  (CheckPointInline Function)");

    }
    const double Lneed=18.0;//视为直线最小长度；
    const double miss=10.0;//正负miss°的角度匹配值
    QVector<int>line;//储存检测出的长度足够的线
    QVector<int>IgnoreP;//储存可以忽略的点
    QVector<int>ToCurve;//送去检测曲线的点
    QVector<QVector2D>CurveRe;//储存Curvecheck返回的点
    for(int i=0;i<length;i++)
    {
        if(dis[i]>=Lneed)
        {
            line.push_back(i);//将长度足够的编号放置到line中
        }
    }
    if(line.length()>1)//至少有两条长度足够的直线
    {
        line.push_back(line[0]);//把第一个找到的放入，形成环形检测
        //监测方案1 将直线段1尾部和直线段2的开头，假设为直线，看中间的转折点分布，如果在很均匀在两边则视为全职线
        // double d_bslope;
        double d_dis;
        for(int i=1;i<line.length();i++)
        {
            int i_x1=OOL[BP[line[i]+1]].x();//第一个线断的尾点
            int i_y1=OOL[BP[line[i]+1]].y();
            int i_x2=OOL[BP[line[i+1]]].x();//第二个线断的起点
            int i_y2=OOL[BP[line[i+1]]].y();
            int orderdis=line[i+1]-line[i]-1;
            d_dis=sqrt(pow(i_y2-i_y1,2)+pow(i_x1-i_x2,2));
            //先检查斜率是不是近似
            if(abs(slope[i]-slope[i-1])<=miss)
            {
                //斜率偏差10°以内
                //再检查距离，如果很近，直接忽略断点
                if(orderdis>5)//点数大于8
                {
                    for(int j=line[i]+1;j<=line[i+1];j++)
                    {
                        ToCurve.push_back(j);
                        CurveRe.clear();
                        //CurveRe=CurveCheck(OOL,ToCurve);//送去曲线检测
                        ToCurve.clear();
                    }
                }else if(orderdis<=5&&d_dis<10)//点数小于3,距离小于10
                {

                    for(int j=line[i]+1;j<=line[i+1];j++)
                    {
                        IgnoreP.push_back(BP[j]);//将两段直线中间两点压如忽略点
                    }

                    //  double gD=SingelSlopeCalculate(OOL[BP[line[i]]],OOL[BP[line[i+1]+1]]);

                }
            }
            else//说明两条线段有角度
            {
                if(orderdis>5)//点数大于8
                {
                    for(int j=line[i]+1;j<=line[i+1];j++)
                    {
                        ToCurve.push_back(j);

                        CurveRe.clear();
                        //  CurveRe=CurveCheck(OOL,ToCurve);//送去曲线检测
                        ToCurve.clear();
                    }
                }else if(orderdis<=5&&d_dis<10)//点数小于3,距离小于10
                {
                    //  double gD=SingelSlopeCalculate(OOL[BP[line[i]]],OOL[BP[line[i+1]+1]]);


                    if(orderdis>1)
                    {
                        for(int j=line[i]+2;j<=line[i+1]-1;j++)
                        {
                            IgnoreP.push_back(BP[j]);//将两段直线中间两点压如忽略点
                        }
                    }
                }
            }
            if(i_x1==i_x2)//x相同
            {
            }
        }
    }
    else if(line.length()==1)//仅有一条直线
    {
        ToCurve.push_back(line[0]+1);
        ToCurve.push_back(BP.last());
        CurveRe=CurveCheck(OOL,ToCurve,true);
    }
    else if(line.length()==0)//全为曲线
    {
        CurveRe=CurveCheck(OOL,BP,true);
    }
    return BP;//temp return
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
        if(unqualified.length()!=1||unqualified.x()!=-8)
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
                QVector<int>  tttoomax;
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

        toreturn.push_back((-8,-8));
    }
    return toreturn;
}
