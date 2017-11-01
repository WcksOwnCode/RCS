/**************************************************************************************************/

/*此处存放的函数主要是用来计算曲线特征点的函数，为了在mainwindow里看起来简单一些
 * 主要是距离计算  斜率计算  斜率简化 等函数组成，目的在于找到曲线行走必须的点
/**************************************************************************************************/


#include "funcitons.h"
#include <cmath>
#include <QMessageBox>
#include"mainwindow.h"
#include"worldvalues.h"
#include<QDatetime>
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
    QString outaddr="C:/Users/duke/Desktop/visiontest/Derections.txt";

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
    int length=Dec.length();
    if(length==0)
    {
        QMessageBox::information(NULL,"notice","length is zero!  (GetBreakPoints)");

    }
    bool outtotxt=false;
    QString outaddr="C:/Users/duke/Desktop/visiontest/Derections.txt";
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
            toreturn=atan((P2.y()-zero.y())/(P2.x()-zero.x()));
            double RelativeX=P2.x()-zero.x();
            double RelativeY=P2.y()-zero.y();

            if(RelativeY>0&&RelativeX<0)//象限转换
            {
                toreturn=CV_PI+toreturn;
            }
            else if(RelativeY<0&&RelativeX<0)
            {
                toreturn=CV_PI+toreturn;
            }else if(RelativeY<0&&RelativeX>0)
            {
                toreturn=CV_PI*2+toreturn;
            }

        }
        else
        {
            if(P2.y()>zero.y())
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
        if(P2.y()>zero.y())
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

    qDebug()<<"crash test length:  "<<length;

    bool outtotxt=false;

    QString outaddr="C:/Users/duke/Desktop/visiontest/slope.txt";

    double *d_slope;

    QVector<double> toreturn;

    if(length>=2){
        d_slope=new double[length-1];
    }
    else{
        qDebug()<<"go else yet";
        return toreturn;
    }

    if(length>=2){

        for(int i=1;i<length;i=i+d)
        {


            d_slope[i-1]=SingelSlopeCalculate(S[i],S[i-1]);

            /*  if(S[i].x()!=S[i-1].x())//分母不能为0
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
                    qDebug()<<"there are two same points here to calculate:"<<i<<"   "<<i-1<<"and the point is:"<<
                              S[i]<<"  "<<S[i-1]<<"(slope function)";
                    // QMessageBox::warning(NULL,"Warning","two point is the same ,no slope ,please check!") ;
                }
            }*/
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

    QString outaddr="C:/Users/duke/Desktop/visiontest/SimplifySlope.txt";

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

    QString outaddr="C:/Users/duke/Desktop/visiontest/SimplifySlope.txt";

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
    QString outaddr="C:/Users/duke/Desktop/visiontest/distance.txt";
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
QVector<int> CheckPointInline(QVector<int>BP, int Pcount, QVector<double>TSlope,
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


    QVector<int> Toreturn;
    QVector<QVector2D>CurvePoints;
    QVector<int>Break_int;
    QVector<QVector2D>Break_2D;
    QVector<int>Break_Gap;
    QVector<QVector2D>PreThree;
    QVector<QVector2D>AfterThree;
    QVector<double>APslope;
    QVector<double>todis;

    qsrand(QTime::currentTime().msec());

    Break_2D.push_back(OOL[BP[0]]);//认为曲线点也是转折点
    Break_int.push_back(BP[0]);
    bool lastone=false;
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

    foreach (int kk, BreakP)
    {
      BPS.push_back(OOL[kk]);
    }

    Output2File(BPS,"C:/Users/duke/Desktop/output/BPS"+QString::number(QTime::currentTime().msec())+".txt");


    for(int k=1;k<Break_int.length();k++)
    {
        Break_Gap.push_back(Break_int[k]-Break_int[k-1]);
    }

    Output2File(Break_int,"C:/Users/duke/Desktop/output/Gap"+QString::number(QTime::currentTime().msec())+".txt");
    Output2File(Break_2D,"C:/Users/duke/Desktop/output/Curvebreakp"+QString::number(QTime::currentTime().msec())+".txt");

    qDebug()<<"Curve point length: "<<CurvePoints.length();

    qDebug()<<"to get the subscript and coordinate of breakp";

    if(CurvePoints.length()==0){
        exit(0);
    }


    Output2File(CurvePoints,"C:/Users/duke/Desktop/output/TestforCurvePoint"+QString::number(QTime::currentTime().msec())+".txt");



    qDebug()<<"Finish the geting!";

    QVector<double>slo;


        slo=Slope(Break_2D,1,false);

        Output2File(slo,"C:/Users/duke/Desktop/output/slo"+QString::number(QTime::currentTime().msec())+".txt",1);




    QVector<QVector2D> P_turn;







    /*a new try for find the characteristic points*/
    for(int i=1;i<Break_int.length()-1;i++)//首尾点是曲线端点，不予考虑
    {
        if(Break_int[i]-12>0&&Break_int[i]+12<OOL.length())
        {
            PreThree.push_back(OOL[Break_int[i]-12]);
            PreThree.push_back(OOL[Break_int[i]-8]);
            PreThree.push_back(OOL[Break_int[i]-4]);
            AfterThree.push_back(OOL[Break_int[i]+4]);
            AfterThree.push_back(OOL[Break_int[i]+8]);
            AfterThree.push_back(OOL[Break_int[i]+12]);
        }
        else if(Break_int[i]-12<0&&Break_int[i]+12<OOL.length())
        {
            int leng=Break_int[i];
            if(leng>1&&leng<12){
                PreThree.push_back(OOL[Break_int[i]-leng]);
                PreThree.push_back(OOL[Break_int[i]-leng/2]);
                PreThree.push_back(OOL[Break_int[i]-leng/4]);
                AfterThree.push_back(OOL[Break_int[i]+4]);
                AfterThree.push_back(OOL[Break_int[i]+8]);
                AfterThree.push_back(OOL[Break_int[i]+12]);
            }
        }else if(Break_int[i]-12>0&&Break_int[i]+12<OOL.length())
        {
            int leng=Break_int[i];
            if(leng>1&&leng<12){
                PreThree.push_back(OOL[Break_int[i]-12]);
                PreThree.push_back(OOL[Break_int[i]-8]);
                PreThree.push_back(OOL[Break_int[i]-4]);
                AfterThree.push_back(OOL[Break_int[i]+leng/4]);
                AfterThree.push_back(OOL[Break_int[i]+leng/2]);
                AfterThree.push_back(OOL[Break_int[i]+leng]);
            }
        }

        for(int j=0;j<3;j++)
        {
            double mi;
            mi=SingelSlopeCalculate(Break_2D[i],PreThree[j]);
            APslope.push_back(mi);
        }
        for(int j=0;j<3;j++)
        {
            double mi;
            mi=SingelSlopeCalculate(AfterThree[j],Break_2D[i]);
            APslope.push_back(mi);
        }
        qDebug()<<APslope<<"APslope at time:"<<QTime::currentTime().msecsSinceStartOfDay();
        qDebug()<<"and the points is    "<<OOL[Break_int[i]];
        if(slo.length()>2){
            bool ok=AngelCompare(APslope,i,slo,0.2);
             if(ok)
             {
                 P_turn.push_back(OOL[Break_int[i]]);
             }

        }else{
            qDebug()<<"slo is "<<slo;
            qDebug()<<"break_2D"<<Break_2D;
            qDebug()<<"Break int   "<<Break_int;
            qDebug()<<"Curvepoints:   "<<CurvePoints;
            exit(0);
        }

        APslope.clear();
        PreThree.clear();
        AfterThree.clear();
    }


    if(P_turn.length()!=0){
        Output2File(P_turn,"C:/Users/duke/Desktop/output/turnpoints"+QString::number(QTime::currentTime().msec())+".txt");
    }else{
        QMessageBox::information(NULL,"notice","no turnpoint!");

    }


    /*a new try for find the characteristic points*/






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

    cv::HoughLinesP(graymat, lines, 1, CV_PI/180,PointCount,minmumLine*2,30); //(in,out,rho,theta,threshold,minlength,maxgap)



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
    Output2File(SLines,"C:/Users/duke/Desktop/visiontest/oriSLines.txt");
    cv::imwrite("C:/Users/duke/Desktop/visiontest/cvsave.png",drawmat);

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
        Outputadd="C:/Users/duke/Desktop/visiontest/outputfiles.txt";;
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
        Outputadd="C:/Users/duke/Desktop/visiontest/outputfiles.txt";;
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
        QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="C:/Users/duke/Desktop/visiontest/outputfiles"+QString::number(qrand())+".txt";
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
QVector<QVector2D> PointReorder(QVector<QVector2D>input,QVector<QVector2D>templateArray)
{

    int templength=templateArray.length();

    int inputLength=input.length();
    QVector<int>tempor;



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



    tempor=PointReorder_Rint(input,templateArray);



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

    //可能有直线嵌套
    //先处理完全嵌套
    QVector<int>toshow=newtoreturn;
    QVector<int> deletp;
    bool inside=true;
    int out1=0;
    while(inside)
    {
        for(int i=3;i<newtoreturn.length()-1;i+=2)
        {
            inside=false;
            if(newtoreturn[i]>newtoreturn[i+1])
            {
                inside=true;
            }

        }

        if(inside)
        {
            QVector<int>tempus;
            for(int i=3;i<newtoreturn.length()-1;i+=2)
            {
                if(newtoreturn[i]>newtoreturn[i+1])
                {


                    if(newtoreturn[i-1]<newtoreturn[i+2])
                    {
                        deletp.push_back(i+1);
                        deletp.push_back(i+2);
                        break;
                    }
                    else
                    {
                        deletp.push_back(i+1);
                        deletp.push_back(i);
                        break;
                    }


                }
            }
            for(int j=0;j<newtoreturn.length();j++)
            {
                bool ok=true;
                for(int k=0;k<deletp.length();k++)
                {
                    if(j==deletp[k]){
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
            qDebug()<<toshow<<"to check is opreated!!";
            qDebug()<<newtoreturn<<"new to return!!";

            out1++;
            qDebug()<<"out1:    "<<out1;
            if(out1>15)
            {
                exit(0);
                break;

            }
        }

    }


    //下面是处理第一条直线跨越起始点
    Toreturn.clear();
    if(newtoreturn[1]>newtoreturn[2])
    {
        Toreturn.push_back(newtoreturn[1]);
        Toreturn.push_back(newtoreturn[0]);
        for(int i=2;i<newtoreturn.length();i++)
        {
            Toreturn.push_back(newtoreturn[i]);
        }
    }
    else
    {
        Toreturn=newtoreturn;
    }



    qDebug()<<"After the reorder:    "<<endl<<Toreturn;



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
QVector<int> LineMerge(QVector<int>input_int,QVector<QVector2D>input_Point,
                       QVector<QVector2D>allp,QVector<int>BreakP,int minL=10)
{
    //after houghlinesP function ,we will get some strait line which is represeted by two points(start
    // and end points)
    //input_int 直线端点的位置序号
    //input_Point 直线端点直接坐标
    //BreakP 是轮廓点全部的转折点
    //MinL 是最小离散长度
    qDebug()<<"enter function line merge";

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

    int TheLastOrder=allp.length();

    bool Linejump=false;

    if(input_int[0]>input_int[1]){
        Linejump=true;//这个bool参数是用来检测第一条直线是不是跨越了默认的起点的
        qDebug()<<"the first line jumped the start point!";
    }

    QVector<int> Toreturn;

    QVector<int>fortemp;

    const double degreeT=CV_PI*10/180;

    QVector<double>lineslope;


    lineslope= Slope(input_Point,1,true);

    //当把所有直线作为环型处理时，即第三个参数是true，lineslope的元素应该和直线条数相同
    //且，脚标偶数是直线斜率，奇数是直线之间的斜率


    //qDebug()<<lineslope<<"   lineslope is this!";

    int slopelength=lineslope.length();


    QVector<int>RemoveP_int;

    for(int i=1;i<length-1;i+=2)
    {
        if(abs(input_int[i]-input_int[i+1])<=1)
        {
            qDebug()<<"the gap is very small and merge!";
            qDebug()<<"length is"<<length;
            qDebug()<<"and now ths i is "<<i;


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
            qDebug()<<"out from 1 gap!";
        }
        else if(abs(input_int[i]-input_int[i+1])<=minL&&abs(input_int[i]-input_int[i+1])>1&&i%2!=0)//直线不会短于minL
        {//i必须为奇数，不然就不是检测的直线之间的点
            qDebug()<<"middle gap two point merge";
            qDebug()<<"length is"<<length;
            qDebug()<<"and now ths i is "<<i;
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
                    //直线做直接拼接
                    input_int[i]=(input_int[i+1]+input_int[i])/2;

                }
            }
            else
            {
                QMessageBox::information(NULL,"notice","Out of range ,slope check is here!");
                exit(0);
            }
            qDebug()<<"out from small gap!";
        }
        else if(abs(input_int[i]-input_int[i+1])>minL&&i%2!=0)
        {
            //gap is longer than minL
            //但是这里没有检查起始点和末位点之间的距离，放在下面的if中
            //长度过长的直接送入CheckPointsinline函数
            //预处理：
            //Gap_Point二维存放曲线起止点位置序号，也就是两段直线，前直线的尾点，和后直线的起点
            //
            //
            //

            qDebug()<<"need curve check!";
            qDebug()<<"length is"<<length;
            qDebug()<<"and now ths i is "<<i;


            QVector<int> Gap_Point;//Curve point's order in OrderdOutLine


            for(int qq=input_int[i];qq<=input_int[i+1];qq++)
            {
                Gap_Point.push_back(qq);
            }//获取所有曲线点位置序号
            //获取曲线两端直线的斜率

            QVector<double>TwoSlope;


            TwoSlope.push_back(lineslope[i-1]);//脚标需要对应查询
            TwoSlope.push_back(lineslope[i+1]);


            if(Gap_Point.length()<minL){

                QMessageBox::information(NULL,"WRONG","Gappoint length is not enough!");
            }

            qDebug()<<"case one send to check point inline:";
            fortemp= CheckPointInline(Gap_Point,Gap_Point.length(),TwoSlope,allp,BreakP,10);
            qDebug()<<"out from large gap!";
        }
        else
        {
            qDebug()<<"can not be this situation!";
            qDebug()<<"abs(input_int[i]-input_int[i+1]):    "<<abs(input_int[i]-input_int[i+1]);
            qDebug()<<"i:     "<<i;
            exit(0);
        }
        qDebug()<<"for is one circle and i is:"<<i;
    }

    qDebug()<<"All point are checked!";


    if(Linejump)
    {   //第一条直线跨界起点
        qDebug()<<"excute the other type!";
        if(abs(input_int[0]-input_int[length-1])>minL)
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
            if(Gap_Point.length()<minL){
                QMessageBox::information(NULL,"WRONG","Gappoint length is not enough!");
                qDebug()<<input_int[0]<<"   "<<input_int[length-1];
            }
            qDebug()<<"case 2 send to checkpoint in line";
            fortemp= CheckPointInline(Gap_Point,Gap_Point.length(),TwoSlope,allp,BreakP,10);
        }

    }
    else//第一条直线没有跨界
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

        if(Gap_Point.length()<minL){
            input_int[input_int.length()-1]=(input_int[input_int.length()-1]+input_int[0])/2;
            input_int[0]=(input_int[input_int.length()-1]+input_int[0])/2;
        }else{
            TwoSlope.push_back(lineslope[0]);//脚标需要对应查询
            TwoSlope.push_back(lineslope[lineslope.length()-2]);
            qDebug()<<"case 3 send to checkpoint in line";
            fortemp=CheckPointInline(Gap_Point,Gap_Point.length(),TwoSlope,allp,BreakP,10);
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
    RemoveP_int.clear();
    qDebug()<<"out function linemerge!";
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

    if(ss= CV_PI/2){
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

    }else if(ss=CV_PI*1.5){



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
    qDebug()<<"enter the Campare";
    if(slope1>1.5*CV_PI&&slope2<0.5*CV_PI)
    {
        //一、四象限做比较
        double dvalue=slope2+CV_PI-slope1;
        if(dvalue<tolerance){


           qDebug()<<"out the Campare";
            return 1;
        }else{
             qDebug()<<"out the Campare";
            return 2;
        }
    }
    else if(slope2>1.5*CV_PI&&slope1<0.5*CV_PI)
    {
        //同上
        double dvalue=slope1+CV_PI-slope2;
        if(dvalue<tolerance){
             qDebug()<<"out the Campare";
            return 1;
        }else{
             qDebug()<<"out the Campare";
            return 2;
        }
    }
    else
    {
        double dvalue=abs(slope1-slope2);
        if(dvalue<tolerance){
             qDebug()<<"out the Campare";
            return 1;
        }else{
             qDebug()<<"out the Campare";
            return 2;
        }
    }


}

void Output2File(QVector<double>InputArray,QString Outputadd,int newline){
    //newline means that every line can only contain 1 word
    if(Outputadd.isEmpty()){
        QMessageBox::information(NULL,"warning","no out put file address!");
        Outputadd="C:/Users/duke/Desktop/visiontest/outputfiles"+QString::number(qrand())+".txt";
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
    qDebug()<<"enter the function AngelComapare origin one!";
    qDebug()<<"spot      "<<spot;
    qDebug()<<"BSlope:       "<<BSlope;
   double *Preslo=new  double [3];
   double *Suffixslo=new  double [3];
   double Aslope=BSlope[spot-1];
   double Beslope=BSlope[spot];
    qDebug()<<"here";
   for(int i=0;i<6;i++)
   {
       if(i<3){
           Preslo[i]=Asl[i];
       }else{
           Suffixslo[i-3]=Asl[i];
       }
   }
   qDebug()<<"here2";

   double WeightedAverage_Pre=0.2*Preslo[0]+0.5*Preslo[1]+0.3*Preslo[2];
   double WeightedAverage_Be=0.2*Suffixslo[0]+0.5*Suffixslo[1]+0.3*Suffixslo[2];

   qDebug()<<"**************************************************";
    qDebug()<<"WeightedAverage_Pre:     "<<WeightedAverage_Pre;
    qDebug()<<"WeightedAverage_Be:     "<<WeightedAverage_Be;
   qDebug()<<"**************************************************";

  int ans= AngelCompare(WeightedAverage_Pre,WeightedAverage_Be,0.3);

  if(ans==1)
  {
     //角度偏差小
      //该点忽略
      qDebug()<<"out from the function AngelComapare origin one!";
      delete[]Preslo;
      delete[]Suffixslo;
      return false;

  }else if(ans==2)
  {
      //角度偏差大
      //不可忽略
      qDebug()<<"out from the function AngelComapare origin one!";
      delete[]Preslo;
      delete[]Suffixslo;
      return true;
  }




   /*int *flag=new int[4];
     flag[0]=AngelCompare(Preslo[0],Preslo[1],2*tolerance);
     flag[1]=AngelCompare(Preslo[1],Preslo[2],2*tolerance);

     flag[2]=AngelCompare(Suffixslo[0],Suffixslo[1],2*tolerance);
     flag[3]=AngelCompare(Suffixslo[1],Suffixslo[2],2*tolerance);
 delete[]flag;
   int */









    qDebug()<<"out from the function AngelComapare origin one!";

}
