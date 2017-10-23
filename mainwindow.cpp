#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"funcitons.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"codewindow.h"
#include"worldvalues.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    WhoseTime.start();

    /******************************************************/
    /*                    From API                        */
    /********************************************************/
    ui->setupUi(this);
    ui->BaudRate->setCurrentIndex(0);
    //关闭发送按钮的使能
    ui->SendMesg_Button->setEnabled(false);
    ui->AutoSend_Button->setEnabled(false);
    ui->Creat_code->setEnabled(false);
    ui->SpeedDirection_comboBox->setEnabled(false);
    ui->BoltSpeed_spinBox->setEnabled(false);
    cam     = NULL;
    timer   = new QTimer(this);
    imag    = new QImage();
    WTimer  = new QTimer(this);
    GetpicTimer= new QTimer(this);
    // // origin_image.load(SpaceimageADD);
    //  origin_image.scaled(width,height);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));
    QObject::connect(GetpicTimer, SIGNAL(timeout()), this, SLOT(TakingPhoto()));

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortName->addItem(serial.portName());
            serial.close();
        }

    }



    QObject::connect(ui->SendMesg_Button,SIGNAL(clicked()),this,SLOT(SendMessgOut()));
    QObject::connect(ui->Read_txt_Button,SIGNAL(clicked()),this,SLOT(ReadtxtButton()));
    QObject::connect(ui->Creat_code,SIGNAL(clicked()),this,SLOT(CreatGcodefile()));
    QObject::connect(ui->ReadPicture,SIGNAL(clicked()),this,SLOT(ReadPngButton()));
    QObject::connect(ui->Openserial_Button,SIGNAL(clicked()),this,SLOT(OpenSerialClicked()));
    QObject::connect(ui->AutoSend_Button,SIGNAL(clicked()),this,SLOT(AutoSendClicked()));
    QObject::connect(ui->BoltSpeed_checkBox,SIGNAL(clicked()),this,SLOT(BoltSpeedCheckBox_checked()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));
    QObject::connect(GetpicTimer, SIGNAL(timeout()), this, SLOT(TakingPhoto()));
    QObject::connect(ui->CameraView_Button, SIGNAL(clicked()), this, SLOT(CameraPreView()));
    rows=0;
    columns=1;
    CoorCount=0;
    m_iSendCount=0;
    m_bReadState=false;
    m_bSerialIsOpen=false;
    m_bStrCompare=false;
    m_iSendRepeatedlyCount=0;
    m_dXbase=0.0;
    m_dYbase=0.0;
    m_dZbase=0.0;


    m_dCurrentX=0;
    m_dCurrentY=0;
    m_dCurrentZ=0;
    m_bFirstsend=false;
    qDebug()<<tr("Initialize Success!");
    /***************************************************************/
    /***************************************************************/
    /******************** read a picture **************************/
    //cread grid points
    int initial=WhoseTime.elapsed();

    qDebug()<<"initialize time used: "<<initial;
    /* opencv duqu tupian */
    /*
           cv::Mat rgb;
          cv::Mat image2;
          image2=cv::imread("C:/Users/duke/Desktop/t1.png",1);
          QImage img;
              if(image2.channels()==3)
              {
                  //cvt Mat BGR 2 QImage RGB
                  cvtColor(image2,rgb,CV_BGR2RGB);
                  img =QImage((const unsigned char*)(rgb.data),
                              rgb.cols,rgb.rows,
                              rgb.cols*rgb.channels(),
                              QImage::Format_RGB888);
              }
              else
              {
                  img =QImage((const unsigned char*)(image2.data),
                              image2.cols,image2.rows,
                              image2.cols*image2.channels(),
                              QImage::Format_RGB888);
              }

              ui->final_label->setPixmap(QPixmap::fromImage(img));
                        */
}
MainWindow::~MainWindow()
{

    delete ui;

}
/*||||||||||API||||||||||||||API|||||||||||||||||||||||||||||||||||||||*/
void MainWindow::readmycom() //读串口函数
{
    ui->Message_Label->clear();
    qDebug()<<"Receive";
    QByteArray buf;
    if(m_bReadState){
        QString StrC;
        StrC.append(Array[m_iSendCount]);
        QString StrCheck=StrC.left(2);
        qDebug()<<"strCHECK::"<<StrCheck;
        qDebug()<<"STR "<<StrC;
        QString tocheck;
        tocheck.prepend(StrCheck);
        qDebug()<<"to check "<<tocheck;
        if(tocheck==StrCheck)
        {
            emit Receveid();
            m_bStrCompare=true;
        }
    }
    buf = serial->readAll();
    QString sss;
    sss+=tr(buf);
    QString tocheck=sss.left(3);
    QString StrC;
    StrC.append("G97");
    QString StrCheck=StrC.left(3);
    // qDebug()<<"strCHECK::"<<StrCheck;
    // qDebug()<<"STR "<<StrC;
    QString toFindXYZABC;
    int sx,sy,sz,sa;
    int tempcount=-1;
    //qDebug()<<"to check "<<tocheck;
    if(tocheck==StrCheck)
    {
        QString Xc="X";
        QString Yc="Y";
        QString Zc="Z";
        QString Ac="A";
        for(int i=0;i<sss.length();i++)
        {
            QString K=sss.mid(i,1);
            //qDebug()<<K;
            if(K==Xc){
                // qDebug()<<"enter x";
                tempcount++;
                sx=i;
            }
            if(K==Yc)
            {
                // qDebug()<<"enter y";
                tempcount++;
                sy=i;

            }
            if(K==Zc)
            {
                //qDebug()<<"enter z";
                tempcount++;
                sz=i;
            }
            if(K==Ac)
            {
                tempcount++;
                sa=i;
            }
        }

        toFindXYZABC=sss.mid(sx+2,sy-sx-3) ;
        // qDebug()<<toFindXYZABC;
        m_dXbase=toFindXYZABC.toDouble();
        // qDebug()<<m_dXbase;
        toFindXYZABC=sss.mid(sy+2,sz-sy-3) ;
        m_dYbase=toFindXYZABC.toDouble();
        toFindXYZABC=sss.mid(sz+2,sa-sz-3) ;
        m_dZbase=toFindXYZABC.toDouble();
        //qDebug()<<m_dZbase;
        ui->Xbase_spin->setValue(m_dXbase);
        ui->Ybase_spin->setValue(m_dYbase);
        ui->Zbase_spin->setValue(m_dZbase);

    }
    if(!buf.isEmpty())
    {

        QString str = ui->Message_Label->text();
        str+=tr(buf);
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString Tstr = time.toString("yyyy-MM-dd hh:mm:ss.zzz ");
        Tstr.append(": \n");
        Tstr.append(str);
        ui->Message_Label->clear();
        ui->Message_Label->setText(Tstr);
    }

    buf.clear();
}
void MainWindow::SendMessgOut(){
    qDebug()<<"send button clicked";
    QString senders=ui->textEdit->toPlainText();
    qDebug()<<senders;
    senders.append("\n");
    qDebug()<<senders;
    // serial->write(ui->SendEditor->toPlainText().toLatin1());
    serial->write(senders.toLatin1());
    qDebug()<<"over";

}
void MainWindow::ReadtxtButton()
{
    //预先清空缓存
    Array.clear();

    ui->textBrowser->setText("");

    ui->Message_Label->clear();
    //*EXCEL导入方式//

    /*导入文件读取数据部分*/
    /*********************************************************************/
    QString fileadd=QFileDialog::getOpenFileName(this,"openfile",QDir::currentPath(),"*.xlsx");
    if(fileadd.isEmpty())
    {
        QMessageBox::information(this,"notice","not opened");
        return;
    }
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", QString(fileadd));
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    QAxObject *used_range = work_sheet->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");
    row_start = used_range->property("Row").toInt();  //获取起始行
    column_start = used_range->property("Column").toInt();  //获取起始列
    row_count = rows->property("Count").toInt();  //获取行数
    column_count = columns->property("Count").toInt();  //获取列数


    for(int i=2;i<row_count+1;i++)
    {
        QAxObject *x = work_sheet->querySubObject("Cells(int,int)", i, 1);
        QVariant x_value = x->property("Value");  //获取单元格内容
        QAxObject *y = work_sheet->querySubObject("Cells(int,int)", i, 2);
        QVariant y_value = y->property("Value");  //获取单元格内容
        QAxObject *z = work_sheet->querySubObject("Cells(int,int)", i, 3);
        QVariant z_value = z->property("Value");  //获取单元格内容
        double dX=x_value.toDouble()+m_dXbase;
        double dY=y_value.toDouble()+m_dYbase;
        double dZ=z_value.toDouble()+m_dZbase;
        SetCoordinate(dX,dY,dZ);

    }
    /****************************/

    /*找到从哪里开始做记录*/
    for(int i=5;i<column_count;i++)
    {
        QAxObject *where = work_sheet->querySubObject("Cells(int,int)", 2, i);
        QVariant where_value = where->property("Value");  //获取单元格内容
        //qDebug()<<where_value;
    }
    /*************************/
    work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
    excel.dynamicCall("Quit(void)");
    m_bReadState=true;
    /********************************************************************************/
    //文件导入结束
    /********************************************************************************/
    ui->textBrowser->setText(Todisplay);

    /* double* p=Discoor;

    for(int i=0;i<3*CoorCount;i++){
        std::cout<<p[i]<<" ";
        if((i+1)%3==0){
            std::cout<<std::endl;
        }
    }*/

    CreadOrders();

    QString repor="All total of ";
    repor.append(QString::number(row_count-1));
    repor.append(" points!\nFinished!");

    QMessageBox::information(this,"point Reading",repor);
    if(m_bReadState&&m_bSerialIsOpen){
        ui->AutoSend_Button->setEnabled(true);
    }
    //  delete p;
    //  p=NULL;
    /* std::cout<<"display coordinate"<<std::endl;
    for(int i=0;i<CoorCount;i++){
        std::cout<<Coordinate[0][i]<<" "<<Coordinate[1][i]<<" "<<Coordinate[2][i]<<std::endl;
    }*/


    ///open a thread
    ///
    /*for(int i=0;i<MAXSIZE;i++)
    {
        readthread[i]=new mythread();
    }
    for(int i=0;i<MAXSIZE;i++)
    {
        readthread[i]->start();
    }*/
}
void MainWindow::SetCoordinate(double x, double y, double z)
{
    m_dBoltSpeed=ui->BoltSpeed_spinBox->text().toDouble();
    int dir=ui->SpeedDirection_comboBox->currentIndex();

    /*加入世界坐标*/
    m_dXbase=ui->Xbase_spin->text().toDouble();
    m_dYbase=ui->Ybase_spin->text().toDouble();
    m_dZbase=ui->Zbase_spin->text().toDouble();

    if(ui->BoltSpeed_checkBox->isChecked())
    {
        if(dir==0)
        {
            m_dXspeed=m_dXspeed+m_dBoltSpeed;
        }
        else if(dir==1)
        {
            m_dXspeed=m_dXspeed-m_dBoltSpeed;
        }
        else if(dir==2)
        {
            m_dYspeed=m_dYspeed+m_dBoltSpeed;
        }
        else if(dir==3)
        {
            m_dYspeed=m_dYspeed-m_dBoltSpeed;
        }
        else if(dir==4)
        {
            m_dZspeed=m_dZspeed+m_dBoltSpeed;
        }
        else if(dir==5)
        {
            m_dZspeed=m_dZspeed-m_dBoltSpeed;
        }
    }
    if(ui->BoltSpeed_checkBox->isChecked())
    {
        Coordinate[0][CoorCount]=x+m_dXspeed+m_dXbase;
        Coordinate[1][CoorCount]=y+m_dYspeed+m_dYbase;
        Coordinate[2][CoorCount]=z+m_dZspeed+m_dZbase;
        Discoor[3*CoorCount]=x+m_dXspeed+m_dXbase;
        Discoor[3*CoorCount+1]=y+m_dYspeed+m_dYbase;
        Discoor[3*CoorCount+2]=z+m_dZspeed+m_dZbase;

    }
    else
    {
        Coordinate[0][CoorCount]=x+m_dXbase;
        Coordinate[1][CoorCount]=y+m_dYbase;
        Coordinate[2][CoorCount]=z+m_dZbase;
        Discoor[3*CoorCount]=x+m_dXbase;
        Discoor[3*CoorCount+1]=y+m_dYbase;
        Discoor[3*CoorCount+2]=z+m_dZbase;
    }
    CoorCount++;

    Todisplay.append(QString::number(x,'g',4).toUpper());//double转化为qstring

    Todisplay.append(" ");

    Todisplay.append(QString::number(y,'g',4).toUpper());

    Todisplay.append(" ");

    Todisplay.append(QString::number(z,'g',4).toUpper());

    Todisplay.append('\n');

    //  qDebug()<<Todisplay;

}
void MainWindow::CreadOrders()
{
    int SpeedValue=ui->Speed_Percentage->text().toInt();
    FullOrder.append("G94 F=");
    FullOrder.append(QString::number(SpeedValue));
    FullOrder.append("\n");

    Array.push_back(FullOrder);

    QString DD;

    for(int i=0;i<CoorCount;i++){

        QString X=QString::number(Coordinate[0][i],'g',4);
        QString Y=QString::number(Coordinate[1][i],'g',4);
        QString Z=QString::number(Coordinate[2][i],'g',4);
        QString Xp=" X=";
        QString Yp=" Y=";
        QString Zp=" Z=";
        QString Ap=" A=";
        QString Bp=" B=";
        QString Cp=" C=";
        QString PreOrder="G00 ";
        QString Fix=" A=3.14 B=0 C=0\n";
        DD=PreOrder+Xp+X+Yp+Y+Zp+Z+Fix;
        Array.push_back(DD);
    }

    ui->Creat_code->setEnabled(true);
    ui->AutoSend_Button->setEnabled(true);

}
void MainWindow::CreatGcodefile()
{
    qDebug()<<Array;
    QString filename=QFileDialog::getSaveFileName(this,"savefile",QDir::currentPath());
    // qDebug()<<filename;
    if(filename.isEmpty())
    {
        QMessageBox::information(this,"warning","content is empty");
        return ;
    }
    filename.append(".txt");
    QFile *file=new QFile;
    file->setFileName(filename);
    bool ok=file->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok){
        QTextStream out(file);
        for(int i=0;i<CoorCount+1;i++){
            QString outstr;

            outstr=Array[i];

            out<<outstr<<endl;
        }
        QMessageBox::information(this,"CreatCodeFile","Finish!");
        file->close();
        delete file;
    }
    else{
        QMessageBox::information(this,"notice","savefailed");
        return ;
    }


}
void MainWindow::on_ExitButton_clicked()
{
    cvReleaseCapture(&cam);
    ClearVector();
    if(m_bSerialIsOpen)
    {
        serial->clear();
        serial->close();
        serial->deleteLater();
    }
    timer->stop();
    GetpicTimer->stop();
    exit(0);
}
void MainWindow::OpenSerialClicked()
{
    if(ui->Openserial_Button->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortName->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudRate->currentText().toInt());
        //设置数据位数
        switch(ui->DataByte->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityCheck->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBit->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortName->setEnabled(false);
        ui->BaudRate->setEnabled(false);
        ui->DataByte->setEnabled(false);
        ui->StopBit->setEnabled(false);
        ui->ParityCheck->setEnabled(false);
        ui->Openserial_Button->setText(tr("关闭串口"));
        ui->SendMesg_Button->setEnabled(true);
        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this,  &MainWindow::readmycom);
        m_bSerialIsOpen=true;
        /*QString Firstsend="G97\n";
       serial->write(Firstsend.toLatin1());
       m_bFirstsend=true;*/
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        m_bSerialIsOpen=false;
        //恢复设置使能
        ui->PortName->setEnabled(true);
        ui->BaudRate->setEnabled(true);
        ui->DataByte->setEnabled(true);
        ui->ParityCheck->setEnabled(true);
        ui->StopBit->setEnabled(true);
        ui->Openserial_Button->setText(tr("打开串口"));
        ui->SendMesg_Button->setEnabled(false);
        ui->AutoSend_Button->setEnabled(false);
    }
    if(m_bReadState&&m_bSerialIsOpen)
    {
        ui->AutoSend_Button->setEnabled(true);
    }
}
void MainWindow::AutoSendClicked()
{
    //close all other buttons and initialize
    ui->SendMesg_Button->setEnabled(false);
    ui->Openserial_Button->setEnabled(false);
    ui->Creat_code->setEnabled(false);
    ui->Read_txt_Button->setEnabled(false);
    m_bStrCompare=false;
    ui->progressBar->setRange(0,Array.length());
    ui->progressBar->setValue(0);
    m_iSendRepeatedlyCount=0;
    for(int i=0;i<CoorCount+1;i++)
    {
        QString sendstr;
        sendstr.append(Array[i]);
        //sendstr.append("\n");
        serial->write(sendstr.toLatin1());
        qDebug()<<i<<"   "<<sendstr<<"By Auto Send";

        while(!m_bStrCompare&&m_iSendRepeatedlyCount<10){
            QEventLoop eventloop;
            QTimer::singleShot(500, &eventloop, SLOT(quit()));//5000ms后跳出
            eventloop.exec();
            m_iSendRepeatedlyCount++;
            serial->write(sendstr.toLatin1());
            qDebug()<<m_iSendRepeatedlyCount<<"   "<<sendstr<<"repeat send";
        }
        if(!m_bStrCompare){
            QMessageBox::warning(this,"warning","no reply!");
            ui->SendMesg_Button->setEnabled(true);
            ui->Openserial_Button->setEnabled(true);
            ui->Creat_code->setEnabled(true);
            ui->Read_txt_Button->setEnabled(true);
            m_iSendRepeatedlyCount=0;
            break;
        }
        else{
            m_iSendCount++;
            ui->progressBar->setValue(i+1);
            m_bStrCompare=false;
            m_iSendRepeatedlyCount=0;
        }
        m_iSendRepeatedlyCount=0;
        m_iSendCount=0;
    }
}
void MainWindow::BoltSpeedCheckBox_checked()
{
    if(ui->BoltSpeed_checkBox->isChecked())
    {
        ui->BoltSpeed_spinBox->setEnabled(true);
        ui->SpeedDirection_comboBox->setEnabled(true);
    }else
    {
        ui->BoltSpeed_spinBox->setEnabled(false);
        ui->SpeedDirection_comboBox->setEnabled(false);
    }
}
/*||||||||||API||||||||||||||API|||||||||||||||END||||||||||||||||||*/
/*==============================================================*/
/*=======================================================*/
QImage MainWindow::GaussianBlur(QImage GB)
{
    //高斯模糊函数，
    QTime GBtimer;
    GBtimer.start();
    // QImage newGB=spaceImage;
    QColor GBc;
    int Crgb=0;
    static const double gaussianTemplate[7][7] =
    {
        {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
        {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
        {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
        {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
        {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
        {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
        {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
    };
    int GaussianRadio=3;//  (7-1)/2
    double GrySum=0;
    for(int i=GaussianRadio;i<width-GaussianRadio;i++)
    {
        for(int j=GaussianRadio;j<height-GaussianRadio;j++)
        {
            GrySum=0;
            for(int x=i-GaussianRadio;x<i+GaussianRadio;x++)
            {
                for(int y=j-GaussianRadio;y<j+GaussianRadio;y++)
                {
                    GBc=GB.pixel(x,y);
                    GrySum=GBc.red()*gaussianTemplate[x-i+3][y-j+3]+GrySum;
                }
            }
            Crgb=GrySum;
            GB.setPixel(i,j,qRgb(Crgb,Crgb,Crgb));
        }
    }

    //设置边框
    //边缘由于没有参与高斯模糊，所以去掉，设置为白色的
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            if(x<GaussianRadio||x>=width-GaussianRadio||y<GaussianRadio||y>=height-GaussianRadio)
            {
                GB.setPixel(x,y,qRgb(255,255,255));
            }

        }
    }

    int GBela=GBtimer.elapsed();
    qDebug()<<"GaussianBlur Function time using: "<<GBela;
    return GB;
}
/*================================================================*/
void MainWindow::DomainCalcu(QImage x)
{   //这个函数把所有行连通域找到，并放置到V4Domadin中

    DCtimer.start();
    //to get the connected Domain
    count=0;//连通域的每行序号
    int tempstart=0;
    int tempend=0;
    all=0;//连通域序号储存再all中（总序号）
    int Entercheck;
    /*to get the height and width of the picture*/

    /***********************************************/

    /* * * * * * * * * * * * * * * * * * */
    //下面这循环是获取每行的连通域并记录到V4Domain变量中，储存方式：（行号，连通域编号，起始位置，结束位置）
    QVector3D tempV3;
    int tempst=-5;

    for(int i=0;i<width;i++)
    {
        Entercheck=-5;//reset entercheck;
        for(int j=0;j<height;j++)
        {
            QColor Ppix=QColor(x.pixel(i,j));//get the color of coordinate i j;

            if(Ppix.red()==0&&Entercheck==-5)//判断是不是进入连通域的第一个点
            {
                tempstart=j;
                Entercheck=j;
            }
            else if(Ppix.red()==0&&Entercheck==j-1&&j!=height-1){
                Entercheck++;//如果检测的下一个点是黑色并且j增加了一个，说明是和上一个点连续的
            }
            else if(Ppix.red()!=0&&Entercheck==j-1)
            {//如果检测的点是白色，并且j增加1，说明是连续的，但是黑色区域结束了
                tempend=j-1;
                Entercheck=-5;//重置标志
                tempvec.setX(i);//(rows,order,start,end)
                tempvec.setY(all);
                tempvec.setZ(tempstart);
                tempvec.setW(tempend);
                V4Domain.push_back(tempvec);
                count++;//connected domain detected!
                all++;
                if(tempst==-5){
                    tempst=all;
                }
            }
            else if(j==height-1&&Ppix.red()==0)
            {
                tempend=j;
                Entercheck=-5;//重置标志
                tempvec.setX(i);//(rows,order,start,end)
                tempvec.setY(all);
                tempvec.setZ(tempstart);
                tempvec.setW(tempend);
                V4Domain.push_back(tempvec);
                count++;//connected domain detected!
                all++;
                if(tempst==-5){
                    tempst=all;
                }
            }
        }
        if(count!=0)
        {
            tempV3.setX(i);//放入行
            tempV3.setY(tempst-1);
            tempV3.setZ(all-1);
            LineDomain.push_back(tempV3);
            count=0;//reset count;
            tempst=-5;
        }
    }
    //  qDebug()<<"all:  "<<all;
    //qDebug()<<"VD  "<<V4Domain;
    ConnectDomains();

    int elapsed=DCtimer.elapsed();
    qDebug()<<"DomainFunction used time:"<<elapsed;
}
/*================================================================*/
void MainWindow::ToGray()
{
    /*************************************************************/
    /****************** to gray **********************************/

    for(int y = 0; y<height; y++){
        QRgb * line = (QRgb *)grayImage.scanLine(y);
        for(int x = 0; x<width; x++){
            int average = 0.299*qRed(line[x]) +0.587* qGreen(line[x]) + 0.114*qRed(line[x]);
            grayImage.setPixel(x,y, qRgb(average, average, average));
        }
    }

    grayImage=GaussianBlur(grayImage);

}
/*================================================================*/
void MainWindow::ToTwoColor()
{
    QColor oldColor2;
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            oldColor2 = QColor(grayImage.pixel(x,y));
            int average = (oldColor2.red()+oldColor2.green()+oldColor2.blue())/3;
            if(average>m_iTotwoValue){
                Timage.setPixel(x,y,qRgb(255,255,255));
            }
            else{
                Timage.setPixel(x,y,qRgb(0,0,0));
            }
        }
    }
}
/*================================================================*/
QImage MainWindow::NoiseFilter(QImage x)//过滤单一元素的噪点
{
    //过滤连通域内部空白等操作
    NFtimer.start();
    /* * * * * * * * * * * * * */
    QVector3D whitetempV3;
    int whitecount=0;//连通域的每行序号
    int tempstart=0;
    int tempend=0;
    int whiteall=0;//连通域序号储存再all中（总序号）
    int Entercheck=-5;
    int tempst=-5;
    for(int i=0;i<width;i++)
    {
        Entercheck=-5;//reset entercheck;
        for(int j=0;j<height;j++)
        {
            QColor Ppix=QColor(x.pixel(i,j));//get the color of coordinate i j;

            if(Ppix.red()==255&&Entercheck==-5)//判断是不是进入连通域的第一个点
            {
                tempstart=j;
                Entercheck=j;
            }
            else if(Ppix.red()==255&&Entercheck==j-1&&j!=height-1){
                Entercheck++;//如果检测的下一个点是黑色并且j增加了一个，说明是和上一个点连续的
            }
            else if(Ppix.red()!=255&&Entercheck==j-1)
            {//如果检测的点是白色，并且j增加1，说明是连续的，但是黑色区域结束了
                tempend=j-1;
                Entercheck=-5;//重置标志
                tempvec.setX(i);//(rows,order,start,end)
                tempvec.setY(all);
                tempvec.setZ(tempstart);
                tempvec.setW(tempend);
                V4Domain_white.push_back(tempvec);
                whitecount++;//connected domain detected!
                whiteall++;
                if(tempst==-5){
                    tempst=whiteall;
                }
            }
            else if(j==height-1&&Ppix.red()==0)
            {
                tempend=j;
                Entercheck=-5;//重置标志
                tempvec.setX(i);//(rows,order,start,end)
                tempvec.setY(all);
                tempvec.setZ(tempstart);
                tempvec.setW(tempend);
                V4Domain_white.push_back(tempvec);
                whitecount++;//connected domain detected!
                all++;
                if(tempst==-5){
                    tempst=whiteall;
                }
            }

        }
        if(whitecount!=0)
        {
            whitetempV3.setX(i);//放入行
            whitetempV3.setY(tempst-1);
            whitetempV3.setZ(whiteall-1);
            WhiteLineDomain.push_back(whitetempV3);
            whitecount=0;//reset count;
            tempst=-5;
        }
    }

    int elapsed=NFtimer.elapsed();
    qDebug()<<"NoiseFilter time used :"<<elapsed;
    QImage re=x;
    return re;
}
/*================================================================*/
void MainWindow::ConnectDomains()
{
    QVector4D LastRows;//上一行连通域信息
    QVector4D ThisRows;//这一行连通域信息
    int Ltempx;//取出该连通域信息的起止位置
    int Ltempy;
    int Ttempx;
    int Ttempy;
    //得到每行连通域数量
    for(int m=0;m<LineDomain.length()-1;m++)
    {
        if(LineDomain[m].x()==LineDomain[m+1].x()-1){
            for(int t1=LineDomain[m].y();t1<=LineDomain[m].z();t1++)
            {
                LastRows=V4Domain[t1];//逐个取出上一行的连通域信息
                Ltempx=LastRows.z();//取出该连通域信息的起止位置
                Ltempy=LastRows.w();

                for(int t2=LineDomain[m+1].y();t2<=LineDomain[m+1].z();t2++)
                {
                    ThisRows=V4Domain[t2];//逐个取出本行的连通域信息
                    Ttempx=ThisRows.z();
                    Ttempy=ThisRows.w();
                    //to check
                    if(Ttempx<=Ltempy&&Ttempy>=Ltempx)
                    {
                        if(ThisRows.y()>LastRows.y())
                        {
                            V4Domain[t2].setY(V4Domain[t1].y());//fllow the order of last row
                        }
                        else
                        {
                            for(int q=0;q<=t1;q++)
                            {
                                if(V4Domain[q].y()==LastRows.y())
                                {
                                    V4Domain[q].setY(ThisRows.y());
                                }
                            }
                        }
                    }
                }
            }

        }
    }

    /***********************************************************************/
    //then statistic the numbers of domains and the range of the domain
    /* to get how many domain is here */
    QVector<int>order;
    QVector<int>OrderStatistic;
    int ordercount=0;
    int *temp=new int[V4Domain.length()];
    for(int mm=0;mm<V4Domain.length();mm++)
    {
        temp[mm]=V4Domain[mm].y();//连通域编号放置到temp变量中;
    }
    order.push_back(temp[0]);//将第一个连通域编号放置到order序列;
    OrderStatistic.push_back(1);//记录第一个编号的连通域出现次数为1；
    for(int mm=1;mm<V4Domain.length();mm++)
    {
        bool check=false;//检测是否是相同的编号的标志bool变量
        for(int nn=0;nn<=ordercount;nn++)
        {
            if(order[nn]==temp[mm])
            {//ordercount 从0开始.
                check=false;//如果order序列中的编号和temp中的相同，就认为是相同的，那么就在连通域出现次数上+1
                OrderStatistic[nn]++;//连通域出现次数+1;
                if(!check)
                {//一旦检测到相同，就跳出for进行下一次对比;
                    break;
                }
            }
            else
            {
                check=true;//如果不等，重置check，貌似这剧else没用
            }
        }
        if(check){//如果check是true。说明有新的连通域编号出现
            order.push_back(temp[mm]);  //将新连通域编号追加到order
            OrderStatistic.push_back(1);//并设置其数量为1
            ordercount++;//数数增加
        }

    }
    //delete too small area
    int AllDomainArea=0;//sometimes there has no enouth doomain area,use this one
    QVector<int>OrderArea;//store every connected domain area
    //initial the orderarea
    for(int i=0;i<order.length();i++)
    {
        OrderArea.push_back(1);
    }

    for(int j=0;j<order.length();j++)
    {
        for(int i=0;i<V4Domain.length();i++)
        {
            if(order[j]==V4Domain[i].y())
            {
                OrderArea[j]=OrderArea[j]+(V4Domain[i].w()-V4Domain[i].z());
                AllDomainArea=AllDomainArea+V4Domain[i].w()-V4Domain[i].z();
            }

        }
    }
    double SumArea=0.2;
    int spotpoint=AllDomainArea*SumArea;//面积排查阈值，大于该值才会被认为是有效连通域
    //qDebug()<<"spotpoint: "<<spotpoint;
    QVector<int>Dflag;
    for(int i=0;i<OrderStatistic.length();i++)
    {
        if(OrderArea[i]>spotpoint){
            Dflag.push_back(order[i]);
        }

    }
    //qDebug()<<"DFlag: "<<Dflag;

    // QVector<QVector>OrderStore;
    for(int xx=0;xx<V4Domain.length();xx++){
        int mu=V4Domain[xx].y();
        for(int yy=0;yy<=ordercount;yy++){
            if(mu==order[yy]){
                V4Domain[xx].setY(yy);
            }

        }
        for(int jo=0;jo<Dflag.length();jo++)
        {
            if(mu==Dflag[jo]){
                V4Domain_main.push_back(V4Domain[xx]);
            }
        }

    }
    delete [] temp;
    temp=NULL;
    qDebug()<<"the number of Domains: "<<order.length();
    /*for(int mm=0;mm<V4Domain.length();mm++)
    {
        qDebug()<<"V4Domain:  "<<V4Domain[mm];
    }*/

    //to get the max area domain
    int maxval=OrderArea[0];
    int maxflag=0;
    for(int qq=1;qq<OrderArea.length();qq++)
    {
        if(OrderArea[qq]>maxval)
        {
            maxval=OrderArea[qq];
            maxflag=qq;
        }
    }
    //qDebug()<<"maxflag: "<<maxflag;
    for(int qq=0;qq<V4Domain.length();qq++)
    {
        if(V4Domain[qq].y()==maxflag)
        {
            V4Domain_max.push_back(V4Domain[qq]);
        }

    }
    //qDebug()<<"max: "<<V4Domain_max;
}
/*================================================================*/
QImage MainWindow::GetOutLine(QImage II)
{
    //找出边界线
    /****************/
    /****************/
    QTime GOLtimer;
    GOLtimer.start();
    QColor oldColor3;
    QColor checkcolor1;
    QColor checkcolor2;
    QColor checkcolor3;
    QColor checkcolor4;
    QImage Ix=spaceImage;//要将检测和操作的图分开，不然自相操作会导致检测干扰;
    QImage rx=spaceImage;
    int width3=Ix.width();
    int height3=Ix.height();
    int SumX=0;
    int SumY=0;
    int pixcount=0;
    QVector2D tempV2;
    QVector2D tempV2_s;
    int tempx;
    int tempy;
    /**************/
    /**************/
    /**************/

    for(int x = 1; x<width3-1; x++)
    {
        for(int y = 1; y<height3-1; y++)
        {
            oldColor3 = QColor(II.pixel(x,y));
            checkcolor1=QColor(II.pixel(x+1,y));
            checkcolor2=QColor(II.pixel(x-1,y));
            checkcolor3=QColor(II.pixel(x,y+1));
            checkcolor4=QColor(II.pixel(x,y-1));
            if(oldColor3.red()==0)
            {
                if(checkcolor1.red()!=0||checkcolor2.red()!=0||checkcolor3.red()!=0||checkcolor4.red()!=0)
                {
                    SumX=SumX+x;
                    SumY=SumY+y;
                    pixcount++;
                    Ix.setPixel(x,y,qRgb(1,254,1));
                    tempV2.setX(x);
                    tempV2.setY(y);
                    OutLine.push_back(tempV2);
                    //不加粗边界线，会是下面查找最外边界速度飞升
                    /*if( checkcolor1.red()==0||checkcolor2.red()==0)
                    {
                        Ix.setPixel(x,y+1,qRgb(1,254,1));
                        Ix.setPixel(x,y-1,qRgb(1,254,1));
                        tempV2.setX(x);
                        tempV2.setY(y+1);
                        OutLine.push_back(tempV2);
                        tempV2.setX(x);
                        tempV2.setY(y-1);
                        OutLine.push_back(tempV2);
                    }
                    if(checkcolor3.red()==0||checkcolor4.red()==0)
                    {
                        Ix.setPixel(x+1,y,qRgb(1,254,1));
                        Ix.setPixel(x-1,y,qRgb(1,254,1));
                        tempV2.setX(x+1);
                        tempV2.setY(y);
                        OutLine.push_back(tempV2);
                        tempV2.setX(x-1);
                        tempV2.setY(y);
                        OutLine.push_back(tempV2);
                    }*/
                }
            }

        }

    }
    int firstela=GOLtimer.elapsed();
    qDebug()<<"first step time used "<<firstela;
    // qDebug()<<"Outline:  "<<OutLine.length();
    OutLine_copy=OutLine;//copy the all xy to use

    int *outlineX=new int[OutLine.length()];
    int *outlineY=new int[OutLine.length()];//由于容器类操作速度太慢，尝试使用数组操作
    for(int i=0;i<OutLine.length();i++)
    {
        outlineX[i]=OutLine[i].x();
        outlineY[i]=OutLine[i].y();
    }

    // qDebug()<<"outline_copy lenth: "<<OutLine_copy.length();
    if(m_bOnlyOutline){
        //qDebug()<<"Entered it!";
        int stx=-5;
        int sty=-5;
        for(int x = 1; x<width3-1; x++)
        {
            for(int y = 1; y<height3-1; y++)
            {
                oldColor3 = QColor(Ix.pixel(x,y));
                //checkcolor1=QColor(Ix.pixel(x+1,y));
                //checkcolor2=QColor(Ix.pixel(x-1,y));
                //checkcolor3=QColor(Ix.pixel(x,y+1));
                //checkcolor4=QColor(Ix.pixel(x,y-1));
                if(oldColor3.red()==1&&oldColor3.green()==254)
                {
                    stx=x;
                    sty=y;
                    tempV2.setX(x);
                    tempV2.setY(y);
                    OnlyOutLine.push_back(tempV2);
                    tempV2.setX(0);//from
                    tempV2.setY(0);//to
                    OnlyOutLine_count.push_back(tempV2);
                    for(int q=0;q<OutLine.length();q++)
                    {
                        if(outlineX[q]==x&&outlineY[q]==y)
                        {
                            outlineX[q]=-1;
                            outlineY[q]=-1;
                        }
                    }
                    break;
                    //从外向内检查到第一个边界上的点后，保存起来，然后跳出.
                }

            }
            if(stx!=-5)
            {
                break;
            }
        }
        int fromcounting=0;//记录每次检测出多少个点
        int tocounting=0;//记录第几次检测,上面已经检测一次了，这里至少是第二次
        int allcounting=0;
        //在此处开始用8联通的方式，检测一开始检测到的初始点连通域;

        int timeF=GOLtimer.elapsed()-firstela;
        bool outflag=true;
        for(int k=0;k<OnlyOutLine_count.length();k++)
        {
            for(int j=OnlyOutLine_count[k].x();j<=OnlyOutLine_count[k].y();j++)
            {
                tempV2=OnlyOutLine[j];//取出第一个坐标点
                outflag=true;

                for(int i=0;i<OutLine_copy.length();i++)//找到outline_copy阵列中相邻关系的点
                {
                    tempx=outlineX[i];
                    tempy=outlineY[i];
                    if(abs(tempx-tempV2.x())<=1&&abs(tempy-tempV2.y())<=1)
                    {
                        tempV2_s.setX(tempx);
                        tempV2_s.setY(tempy);
                        OnlyOutLine.push_back(tempV2_s);
                        outlineX[i]=-1;
                        outlineY[i]=-1;
                        allcounting++;
                        if(outflag)
                        {
                            outflag=false;
                        }

                    }
                }
            }
            if(outflag&&allcounting==0)
            {
                // qDebug()<<"there: "<<outflag;
                // qDebug()<<allcounting;
                break;
            }
            else{
                fromcounting=1+OnlyOutLine_count[k].y();
                tocounting=fromcounting+allcounting-1;
                tempV2.setX(fromcounting);
                tempV2.setY(tocounting);
                OnlyOutLine_count.push_back(tempV2);
                allcounting=0;
            }
            // qDebug()<<OnlyOutLine_count;
        }
        int secondst=GOLtimer.elapsed()-timeF;
        qDebug()<<"second step time used:"<<secondst;
        //上色

        if(m_bOnlyOutline)
        {
            // qDebug()<<"draw new color!";
            for(int i=0;i<OnlyOutLine.length();i++)
            {
                rx.setPixel(OnlyOutLine[i].x(),OnlyOutLine[i].y(),qRgb(253,0,0));
            }
        }

        Ix.setPixel(SumX/pixcount,SumY/pixcount,qRgb(255,0,0));

        int alltime=GOLtimer.elapsed();

        qDebug()<<"Get out line time used:"<<alltime;

        delete []outlineX;

        delete []outlineY;


    }
    return rx;
}
/*================================================================*/
void MainWindow::SmoothOutline()
{
    QTime SOtimer;
    SOtimer.start();


    ReOrderOutline(OnlyOutLine);//重新排序边界

    CurveFit(OrderdOutLine);
    /***********************************************************/
    /*             曲线拟合的方式尝试进行边界平滑                   */
    /*                    关键的特征点提取                       */
    /**********************************************************/

    //计算全部边界的两点间方向//9.22尝试

    num_Derec=DerectionCalFunc(OrderdOutLine);


    //去除一些噪点
    DeleteOutlineNoise();



    //   CharacteristicCalculate(BreakPointDistance);

    //得到转折点

    BreakPoints=GetBreakPoints(num_Derec,OrderdOutLine);

    //获取特征点

    CharacteristicCalculate(BreakPoints);//此处未完待续
    /*************************************************************/
    int SOalltime=SOtimer.elapsed();
    qDebug()<<"smooth outline time used:"<<SOalltime;

}
/*void MainWindow::DisplayButton()
{
  //  QTime DStimer;
   // DStimer.start();
    qDebug()<<"DisplayButton";
    int times=15;

    if(tempus<0){
        qDebug()<<"first time";
        xx=spaceImage;
        pp=pp.fromImage(spaceImage);
        ui->final_label->setPixmap(pp);

        tempus++;
    }
    else
    {
        for(int i=0;i<times;i++)
        {
            qDebug()<<"count:  "<<tempus;
            if(tempus==OrderdOutLine.length())
            {
                ui->Display->setEnabled(false);
                break;

            }
            xx.setPixel(OrderdOutLine[tempus].x(),OrderdOutLine[tempus].y(),qRgb(0,0,255));
            pp=pp.fromImage(xx);
            ui->final_label->setPixmap(pp);
            tempus++;

        }
    }

}*/
/**new new new new new**/
void MainWindow::ReadPngButton()
{
    ui->CameraView_Button->setEnabled(false);
    ui->openCamera->setEnabled(false);
    ui->Origin_Label->setPixmap(QPixmap::fromImage(spaceImage));
    ui->final_label->setPixmap(QPixmap::fromImage(spaceImage));
    ClearVector();
    readfileadd=QFileDialog::getOpenFileName(this,"openfile",QDir::currentPath(),"*.png");
    if(readfileadd.isEmpty())
    {
        QMessageBox::information(this,"notice","not opened");
        ui->CameraView_Button->setEnabled(true);
        ui->openCamera->setEnabled(true);
        return;
    }
    int readstart=WhoseTime.elapsed();

    origin_image.load(readfileadd);
    origin_image = origin_image.scaled(width, height);
    spaceImage=origin_image;
    for(int i=0;i<width;i++){//creat a all white but the same size as origin
        for(int j=0;j<height;j++)
        {
            spaceImage.setPixel(i,j,qRgb(255,255,255));
        }
    }
    Timage=origin_image;//二色图
    grayImage=origin_image;//灰度图
    pp=pp.fromImage(origin_image);
    ui->Origin_Label->setPixmap(pp);


    /*********************************************************/
    ToGray();

    /***************** to two color picture *********************/
    ToTwoColor();

    //jisuan liantongyu
    DomainCalcu(Timage);//then we get V4Domain

    QImage DomainTest=spaceImage;

    for(int i=0;i<V4Domain.length();i++)
    {

        for(int j=V4Domain[i].z();j<V4Domain[i].w();j++)
        {
            int x=V4Domain[i].x();
            int y=j;
            DomainTest.setPixel(x,y,qRgb(0,0,0));
        }
    }

    /**************************************************************/
    //过滤沙眼
    QImage mainim=spaceImage;
    for(int i=0;i<V4Domain_main.length();i++)
    {

        for(int j=V4Domain_main[i].z();j<V4Domain_main[i].w();j++)
        {
            int x=V4Domain_main[i].x();
            int y=j;
            mainim.setPixel(x,y,qRgb(0,0,0));
        }
    }

    /******************************************************/
    //画出最大连通域的外轮廓
    /************************************************************/
    OulineImage=spaceImage;
    /*for(int i=0;i<width;i++)
     {
         for(int j=0;j<height;j++)
         {
             OulineImage.setPixel(i,j,qRgb(255,255,255));
         }
     }*/
    for(int i=0;i<V4Domain_max.length();i++)
    {

        for(int j=V4Domain_max[i].z();j<V4Domain_max[i].w();j++)
        {
            int x=V4Domain_max[i].x();
            int y=j;

            OulineImage.setPixel(x,y,qRgb(0,0,0));

        }
    }


    OulineImage=GetOutLine(OulineImage);//get the outline of the max domain


    SmoothOutline();

    SmoothOulineImage=spaceImage;

    for(int i=0;i<OrderdOutLine.length();i++){
        SmoothOulineImage.setPixel(OrderdOutLine[i].x(),OrderdOutLine[i].y(),qRgb(255,0,0));
    }

    //qDebug()<<"CharacteristicPoint"<<CharacteristicPoint;
    for(int i=0;i<CharacteristicPoint.length();i++)
    {
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-1,CharacteristicPoint[i].y()-1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+1,CharacteristicPoint[i].y()+1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-1,CharacteristicPoint[i].y()+1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+1,CharacteristicPoint[i].y()-1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()-1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()+1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-1,CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+1,CharacteristicPoint[i].y(),qRgb(0,255,255));


        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()+2,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()-2,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-2,CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+2,CharacteristicPoint[i].y(),qRgb(0,255,255));

        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()+3,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()-3,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-3,CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+3,CharacteristicPoint[i].y(),qRgb(0,255,255));
    }


    pp=pp.fromImage(SmoothOulineImage);
    ui->final_label->setPixmap(pp);


    ui->CameraView_Button->setEnabled(true);

    ui->openCamera->setEnabled(true);

    //DynamicEncoding(CharacteristicPoint);



    QVector<QVector2D>HoughPoints;

    HoughPoints=HoughTransform(OulineImage,OrderdOutLine.length()/15,minmumLine);
    if(HoughPoints.length()>=4){

        QVector<QVector2D>OrderedSline;

        QVector<int>testorder;
        QVector<QVector2D>test2D;

        testorder=PointReorder_Rint(HoughPoints,OrderdOutLine);

        OrderedSline=PointReorder(HoughPoints,OrderdOutLine);

        Output2File(OrderedSline,"C:/Users/duke/Desktop/OrderedSline.txt");

        Output2File(OrderdOutLine,"C:/Users/duke/Desktop/Orderdoutline.txt");


        QVector<int>m_Int_Line;

        /*****************************************************************/
        /*                                                               */
        /*                                                               */
        /*                                                               */
        m_Int_Line=LineMerge(testorder,OrderedSline,OrderdOutLine);
        /*                                                               */
        /*                                                               */
        /*                                                               */
        /*                                                               */
        /*****************************************************************/
        qDebug()<<m_Int_Line<<"    m_Int_Line is this!";
        qDebug()<<testorder<<"    testorder is this!";

        QVector<QVector2D>testMerge;

        for(int j=0;j<testorder.length();j++)
        {

            test2D.push_back(OrderdOutLine[testorder[j]]);

        }

        for(int j=0;j<m_Int_Line.length();j++)
        {

            testMerge.push_back(OrderdOutLine[m_Int_Line[j]]);

        }

        Output2File(test2D,"C:/Users/duke/Desktop/test2D.txt");

        Output2File(testMerge,"C:/Users/duke/Desktop/testMerge.txt");


        //    OrderedSline=LineMerge(OrderedSline);//this function is not prepared!

    }
    else
    {

        //直线数量不足，说明全部是曲线，直接进行曲线检测


    }
    int elapsed=WhoseTime.elapsed()-readstart;

    m_bReadState=true;
    ui->time_label->setText(QString::number(elapsed)+" ms");

}
void MainWindow::on_openCamera_clicked()
{
    if(ui->openCamera->text()=="自动视觉")
    {

        ui->ReadPicture->setEnabled(false);
        ui->openCamera->setText("关闭相机");
        cam = cvCreateCameraCapture(0);
        timer->start(50);
        GetpicTimer->start(8000);
        qDebug()<<"opened";

    }
    else
    {
        m_bCareraAutoRun=false;
        ui->ReadPicture->setEnabled(true);
        ui->openCamera->setText("自动视觉");
        timer->stop();
        GetpicTimer->stop();
        connect(timer, SIGNAL(timeout()), this, SLOT(EmptySlots()));
        cvReleaseCapture(&cam);
    }

}
void MainWindow::readFarme()
{
    // qDebug()<<"get frame";
    frame = cvQueryFrame(cam);//Grabs each frame from the camera and return
    //qDebug()<<frame->height;
    // catch the frame and transform to QImage,QImage::Format_RGB888different camera should use a different form。
    QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
    image=image.scaled(width,height);
    if(m_bFrompreView){
        for(int i=0;i<width;i++)
        {
            for(int j=0;j<height;j++)
            {
                if(i==0||i==1||i==199||i==200||i==398||i==399||j==0||j==1||j==150||j==149||j==151||j==299||j==298)
                    image.setPixel(i,j,qRgb(255,255,0));
            }

        }


    }
    ui->Origin_Label->setPixmap(QPixmap::fromImage(image));

}
void MainWindow::EmptyFunction()
{
    qDebug()<<"empty function";
}
void MainWindow::TakingPhoto()
{
    frame = cvQueryFrame(cam);
    QImage image2 = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
    //QImage image2((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888);

    origin_image=image2;
    AutoRun();
}
void MainWindow::AutoRun()
{
    AutoRunTimer.restart();
    origin_image = origin_image.scaled(width, height);
    Timage=origin_image;//二色图
    grayImage=origin_image;//灰度图
    pp=pp.fromImage(origin_image);
    ui->Origin_Label->setPixmap(pp);
    spaceImage=origin_image;
    for(int i=0;i<width;i++){//creat a all white but the same size as origin
        for(int j=0;j<height;j++)
        {
            spaceImage.setPixel(i,j,qRgb(255,255,255));
        }
    }
    /*********************************************************/
    ToGray();
    /***************** to two color picture *********************/
    ToTwoColor();
    //jisuan liantongyu
    DomainCalcu(Timage);//then we get V4Domain
    QImage DomainTest=spaceImage;
    for(int i=0;i<V4Domain.length();i++)
    {

        for(int j=V4Domain[i].z();j<V4Domain[i].w();j++)
        {
            int x=V4Domain[i].x();
            int y=j;
            DomainTest.setPixel(x,y,qRgb(0,0,0));
        }
    }

    /**************************************************************/
    //过滤沙眼
    QImage mainim=spaceImage;
    for(int i=0;i<V4Domain_main.length();i++)
    {
        for(int j=V4Domain_main[i].z();j<V4Domain_main[i].w();j++)
        {
            int x=V4Domain_main[i].x();
            int y=j;
            mainim.setPixel(x,y,qRgb(0,0,0));
        }
    }
    /******************************************************/
    //画出最大连通域的外轮廓
    /************************************************************/
    OulineImage=spaceImage;
    /*for(int i=0;i<width;i++)
     {
         for(int j=0;j<height;j++)
         {
             OulineImage.setPixel(i,j,qRgb(255,255,255));
         }
     }*/
    for(int i=0;i<V4Domain_max.length();i++)
    {

        for(int j=V4Domain_max[i].z();j<V4Domain_max[i].w();j++)
        {
            int x=V4Domain_max[i].x();
            int y=j;

            OulineImage.setPixel(x,y,qRgb(0,0,0));
        }
    }
    OulineImage=GetOutLine(OulineImage);//get the outline of the max domain
    // pp=pp.fromImage(OulineImage);
    // ui->final_label->setPixmap(pp);

    SmoothOutline();

    SmoothOulineImage=spaceImage;

    for(int i=0;i<OrderdOutLine.length();i++){
        SmoothOulineImage.setPixel(OrderdOutLine[i].x(),OrderdOutLine[i].y(),qRgb(255,0,0));
    }

    //qDebug()<<"CharacteristicPoint"<<CharacteristicPoint;
    for(int i=0;i<CharacteristicPoint.length();i++)
    {
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-1,CharacteristicPoint[i].y()-1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+1,CharacteristicPoint[i].y()+1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-1,CharacteristicPoint[i].y()+1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+1,CharacteristicPoint[i].y()-1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()-1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()+1,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-1,CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+1,CharacteristicPoint[i].y(),qRgb(0,255,255));


        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()+2,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()-2,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-2,CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+2,CharacteristicPoint[i].y(),qRgb(0,255,255));

        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()+3,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x(),CharacteristicPoint[i].y()-3,qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()-3,CharacteristicPoint[i].y(),qRgb(0,255,255));
        SmoothOulineImage.setPixel(CharacteristicPoint[i].x()+3,CharacteristicPoint[i].y(),qRgb(0,255,255));
    }

    /* for(int i=0;i<BreakPoints.length();i++){
        SmoothOulineImage.setPixel(BreakPoints[i].y(),BreakPoints[i].z(),qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y()-2,BreakPoints[i].z()-2,qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y()+2,BreakPoints[i].z()+2,qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y()-2,BreakPoints[i].z()+2,qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y()+2,BreakPoints[i].z()-2,qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y(),BreakPoints[i].z()-2,qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y(),BreakPoints[i].z()+2,qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y()-2,BreakPoints[i].z(),qRgb(0,0,0));
        SmoothOulineImage.setPixel(BreakPoints[i].y()+2,BreakPoints[i].z(),qRgb(0,0,0));

    }*/
    pp=pp.fromImage(SmoothOulineImage);
    ui->final_label->setPixmap(pp);

    ui->CameraView_Button->setEnabled(true);
    ui->openCamera->setEnabled(true);
    /// DynamicEncoding(CharacteristicPoint);
    int elapsed=AutoRunTimer.elapsed();
    ui->time_label->setText(QString::number(elapsed)+" ms");
    ClearVector();

}
void MainWindow::ClearVector()
{
    V4Domain.clear();
    V4Domain_new.clear();
    V4Domain_max.clear();//keep the max area domain
    V4Domain_main.clear();//保留主要连通域
    V4Domain_white.clear();//反色连通域
    LineDomain.clear();//记录每行连通域的编号的起编号和止编号
    WhiteLineDomain.clear();//记录白色每行连通域的编号的起编号和止编号
    OutLine.clear();
    OutLine_copy.clear();
    OnlyOutLine.clear();
    OnlyOutLine_count.clear();
    OrderdOutLine.clear();
    CharacteristicPoint.clear();
    Segmentboundary.clear();
    GridPoints.clear();
    BreakPoints.clear();
    BreakPointDistance.clear();
    StraitLine.clear();
    StraitLineCount.clear();
    BreakPointDistance.clear();
    Derection.clear();
    num_Derec.clear();
    Array.clear();
    FullOrder.clear();
    ctrlPoints.clear();
    curvePoints.clear();
    CoorCount=0;
}
void MainWindow::CameraPreView()
{
    if(ui->CameraView_Button->text()=="相机预览")
    {
        m_bFrompreView=true;
        ui->openCamera->setEnabled(false);
        connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));
        cam = cvCreateCameraCapture(0);
        timer->start(20);
        ui->CameraView_Button->setText("关闭相机");
    }else
    {
        m_bFrompreView=false;
        ui->CameraView_Button->setText("相机预览");
        timer->stop();
        connect(timer, SIGNAL(timeout()), this, SLOT(EmptyFunction()));
        cvReleaseCapture(&cam);
        ui->openCamera->setEnabled(true);
        ui->Origin_Label->setPixmap(QPixmap::fromImage(spaceImage));
    }

}
void MainWindow::on_GetTheWorldCoordinate_button_clicked()
{   m_bSerialIsOpen=true;
    if(m_bSerialIsOpen){
        qDebug()<<"get!";
        QString sendstr;
        sendstr.append("G97");
        sendstr.append("\n");
        //sendstr.append("\n");
        // serial->write(sendstr.toLatin1());
        m_bWorldCheck=true;
        ResetWorldTimer();
        WTimer->start(2000);
        QObject::connect(WTimer, SIGNAL(timeout()), this, SLOT(ResetWorldTimer()));
    }
    else
    {
        QMessageBox::warning(this,"warning","SerialPort is not open!");
    }

}
void MainWindow::ResetWorldTimer()
{
    m_bWorldCheck=false;
    QMessageBox::information(this,"warning","no reply!");
}
void MainWindow::TransferToWorldCoordinate()
{
    int PointNum=CharacteristicPoint.length();
    for(int i=0;i<PointNum;i++)
    {

    }



}
void MainWindow::DynamicEncoding(QVector <QVector2D> DE)
{
    int Orderlength=DE.length();
    int tempx,tempy;
    int tempz=m_dZbase;
    for(int i=0;i<Orderlength;i++)
    {
        tempx=DE[i].x();
        tempy=DE[i].y();

        SetCoordinate(tempx,tempy,tempz);
    }
    CreadOrders();
}

void MainWindow::on_Canny_button_clicked()
{
    //声明IplImage指针
    IplImage* img = NULL;
    IplImage* cannyImg = NULL;
    char *filename;
    filename="C://Users//duke//Desktop//t2.png";

    img=cvLoadImage(filename,1);
    //载入图像，强制转化为Gray
    if((img = cvLoadImage(filename, 0)) != 0 )
    {

        //为canny边缘图像申请空间
        cannyImg = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
        //图像大小img大小，图像颜色深度8位，1通道图像
        // IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U,IPL_DEPTH_16S, IPL_DEPTH_32S

        //canny边缘检测

        cvCanny(img, cannyImg, 100,220,3);
        //创建窗口
        cvNamedWindow("src", 1);
        cvNamedWindow("canny",1);

        //显示图像
        cvShowImage( "src", img );
        cvShowImage( "canny", cannyImg );
        cvWaitKey(0); //等待按键
        //销毁窗口
        cvDestroyWindow( "src" );
        cvDestroyWindow( "canny" );

        //释放图像
        cvReleaseImage( &img );
        cvReleaseImage( &cannyImg );

    }

}

void MainWindow::on_ChangeTheimage__currentIndexChanged(int index)
{
    if(index==0)
    {
        pp=pp.fromImage(SmoothOulineImage);
        ui->final_label->setPixmap(pp);
    }
    else if(index==1)
    {
        pp=pp.fromImage(grayImage);
        ui->final_label->setPixmap(pp);
    }
    else if(index==2)
    {
        pp=pp.fromImage(Timage);
        ui->final_label->setPixmap(pp);
    }
    else if(index==3)
    {
        pp=pp.fromImage(OulineImage);
        ui->final_label->setPixmap(pp);
    }
}
void MainWindow::DeleteOutlineNoise()
{
    qDebug()<<"this is delete funciton!";
    //delete the derection vector's noise
    int length=num_Derec.length();
    if(length==0)
    {
        QMessageBox::warning(NULL,"Warning","length is zero(deleteoutlinefunction)");
        exit(0);
    }
    const int Sample=8;//采样十个点
    const int Samplethresh=6;//高于6个点就去除
    int *Dr=new int [Sample];
    int *ss=new int [Sample];
    int samecount=0;//记录相同数
    QVector <int> pos;
    for(int i=0;i<length-8;i=i+4)
    {
        if(length-i<Sample)
        {
            qDebug()<<"out";
            break;
        }
        for(int j=0;j<8;j++)
        {
            Dr[j]=num_Derec[i+j].y();
            ss[j]=num_Derec[i+j].x();
            if(j>0)
            {
                if(Dr[j]==Dr[j-1])
                {
                    samecount++;
                }
                else
                {
                    if(j>1&&j<7)
                    {
                        pos.push_back(i+j);//位置放入pos
                        j++;
                    }
                }
            }
        }
        samecount=0;
    }
    QVector<int>::iterator end_unique=std::unique(pos.begin(),pos.end());
    pos.erase(end_unique,pos.end());


    for(int k=0;k<pos.length();k++)
    {
        int tempmid=(OrderdOutLine[pos[k]].x()+OrderdOutLine[pos[k]+2].x())/2;
        OrderdOutLine[pos[k]+1].setX(tempmid);
        tempmid=(OrderdOutLine[pos[k]].y()+OrderdOutLine[pos[k]+2].y())/2;
        OrderdOutLine[pos[k]+1].setY(tempmid);
        num_Derec[pos[k]].setY(num_Derec[pos[k]-1].y());
    }
    pos.clear();
    delete []Dr;
    Dr=NULL;
    delete []ss;
    ss=NULL;



    //增加一个五点三次平滑呢？
}
void MainWindow::ReOrderOutline(QVector <QVector2D> RO)
{
    qDebug()<<"reorder!";
    int length=RO.length();
    QVector <QVector2D> NewOrder;
    QVector2D Last;
    QVector<int>outbot;
    int *tempx=new int[length];
    int *tempy=new int[length];
    for(int i=0;i<length;i++)
    {
        tempx[i]=RO[i].x();
        tempy[i]=RO[i].y();
    }
    Last.setX(tempx[0]);
    Last.setY(tempy[0]);
    NewOrder.push_back(Last);
    tempx[0]=-10;
    tempy[0]=-10;
    outbot.push_back(0);
    qDebug()<<"to get new order!";

    for(int i=1;i<length;i++)
    {
        double *dis = new double[length];
        double minval=99999.0;
        int minspot=-5;
        for(int j=1;j<length;j++)
        {
            dis[j]=sqrt(pow(Last.x()-tempx[j],2)+pow(Last.y()-tempy[j],2));
            if(minval>dis[j]){
                minval=dis[j];
                minspot=j;
            }
        }

        Last.setX(tempx[minspot]);
        Last.setY(tempy[minspot]);
        NewOrder.push_back(Last);
        tempx[minspot]=-10;
        tempy[minspot]=-10;

        delete []dis;
        dis=NULL;

    }
    if(NewOrder.length()!=length)
    {

        QMessageBox::information(this,"notice!","NewOrder is not enough!");
        ErrorFunction();
    }
    OrderdOutLine=NewOrder;
    if(NewOrder.length()>0){
        Last=NewOrder.last();
        OrderdOutLine.push_back(Last);
    }
    else
    {
        QMessageBox::information(this,"notice!","NewOrder is empty!");
    }

    /*for(int i=0;i<length+1;i++)
    {
        qDebug()<<"Orderdoutline["<<i<<"]:        "<<OrderdOutLine[i];
    }*/
    delete[] tempx;
    delete[] tempy;
    tempx=NULL;
    tempy=NULL;
}
void MainWindow::ErrorFunction()
{
    QMessageBox::information(this,"notice","Some error happend ,and application will exit right now");
    exit(0);
}
void MainWindow::CurveFit(QVector<QVector2D> Curve)
{
    QTime CurveTimer;
    curvePoints.clear();
    ctrlPoints.clear();
    CurveTimer.start();
    if(Curve.length()==0)
    {
        ErrorFunction();
    }

    for(int i=0;i<Curve.length();i+=2)
    {
        QVector2D as=Curve[i];
        QPointF li;
        li.setX(as.x());
        li.setY(as.y());
        ctrlPoints.push_back(li);
    }

    for(double u=currentK; u<ctrlPoints.size(); u+=FitStep)
    {
        QPointF tmp(0,0);
        for(int i=0; i<ctrlPoints.size();i++)
        {
            QPointF t = ctrlPoints[i];//坐标给t

            t*=N(currentK,i,u);

            tmp+=t;
        }
        curvePoints.push_back(tmp);
    }

    int CurveCast=CurveTimer.elapsed();




    qDebug()<<"CurveFit time Cast:    "<<CurveCast;


}
void MainWindow::CharacteristicCalculate(QVector<int> CC)
{
    //CC 储存的是转折点在OrderdOutline中的序号，表示的是发生转向的点；
    //这个函数获取特征点
    //开始写的时间是2017.9.22
    //算法重新考虑中
    //替代Excursion函数和Slopecheck函数
    //此函数筛查这些转折点然后找出关键的特征点

    int length=CC.length();

    if(length==0)
    {
        ErrorFunction();
    }

    QVector<QVector2D> toslope;

    QVector<int> copyCC=CC;

    QVector<double> toslopereturn;

    QVector<int> SimpledBreak;

    int lengthcheck=-5;

    int outcount=0;

    while(lengthcheck!=copyCC.length())
    {
        lengthcheck=copyCC.length();

        length=copyCC.length();

        SimpledBreak.clear();

        toslope.clear();

        toslopereturn.clear();

        for(int i=0;i<length;i++)
        {
            toslope.push_back(OrderdOutLine[copyCC[i]]);//从外围的点提取出来（实际坐标）
        }

        toslopereturn= Slope(toslope);//计算全部点的斜率

        SimpledBreak=SimplifySlope(toslopereturn,copyCC);//此时得到斜率匹配后的转折点

        copyCC.clear();

        copyCC=SimpledBreak;

        SimpledBreak.clear();

        toslope.clear();

        toslopereturn.clear();

        if(copyCC.length()>Max_C_num)
        {
            QMessageBox::information(this,"notice",QString::number(copyCC.length()));
            exit(0);
        }

        outcount++;

        if(outcount>10)//最多进行十次迭代，然后退出
        {
            QMessageBox::information(this,"notice","Too much number of iterations");
            break;
        }
    }

    CharacteristicPoint.clear();

    QVector2D ToC;

    for(int i=0;i<copyCC.length();i++)
    {
        ToC.setX(OrderdOutLine[copyCC[i]].x());

        ToC.setY(OrderdOutLine[copyCC[i]].y());

        CharacteristicPoint.push_back(ToC);
    }

    CharacteristicPoint.push_back(CharacteristicPoint[0]);


    if(ui->disperse_checkBox->isChecked())
    {
        CharacteristicPoint .clear();

        for(int i=0;i<OrderdOutLine.length();i=i+disperse)
        {
            CharacteristicPoint.push_back(OrderdOutLine[i]);
        }
    }
    CharacteristicPoint.push_back(CharacteristicPoint[0]);
    /*
    //直线之间分断检查
    //先看前面有咩有bug

    QVector<double>EachDis;

    toslopereturn= Slope(toslope);//计算全部点的斜率
    EachDis=Distance(toslope);//计算两两点之间距离

    SimpledBreak=SimplifySlope(toslopereturn,CC);//此时得到斜率匹配后的转折点

*/

    if(CharacteristicPoint.length()<3)
    {
        QMessageBox::information(this,"notice","characteristicpoints is not enough!");
        CharacteristicPoint.clear();
        CharacteristicPoint=OrderdOutLine;
    }

    for(int i=0;i<CharacteristicPoint.length();i++)
    {
        SetCoordinate(CharacteristicPoint[i].x(),CharacteristicPoint[i].y(),0);
    }


    CreadOrders();

}
void MainWindow::DrawImage(QImage im,QVector<QVector2D> P,QColor Color)
{

}

void MainWindow::on_DisperseSlider_valueChanged(int value)
{
    disperse=2+value/4;
    ui->Shownumber_for_disperse->setText(QString::number(disperse));
}

void MainWindow::on_CheckCode_Button_clicked()
{
    if(Array.length()>0)
    {
        ui->Message_Label->clear();
        QString dis;
        for(int i=0;i<Array.length();i++)
        {
            dis.append(Array[i]);
        }

        CodeWindow *x=new CodeWindow(dis);
        x->show();

    }else
    {
        QMessageBox::warning(NULL,"warning","no code");
    }





}

void MainWindow::on_Hough_Button_clicked()
{


    using namespace cv;
    //方式一：全部使用opencv的函数

    Mat oriImage = imread("C:\\Users\\duke\\Desktop\\t2.png");  //工程目录下应该有一张名为1.jpg的素材图
    Mat Blured,Image2,lastImage;//临时变量和目标图的定义

    //【2】进行边缘检测和转化为灰度图

    cv::GaussianBlur(oriImage,Blured,Size(11,11),2);
    imshow("oriImage",oriImage);
    imshow("blured",Blured);
    Canny(oriImage, Image2, 50, 200, 3);//进行一此canny边缘检测
    cvtColor(Image2,lastImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图

    Mat newlast=oriImage;

    vector<Vec2f> cvlines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    vector<Vec4i> Pcvlines;//定义一个矢量结构lines用于存放得到的线段矢量集合

    HoughLines(Image2, cvlines, 1, CV_PI/180, 50, 0, 0 );
    HoughLinesP(Image2, Pcvlines, 1, CV_PI/180,50, 25, 5);


    for( size_t i = 0; i < cvlines.size(); i++ )
    {
        float rho = cvlines[i][0], theta = cvlines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( lastImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);
    }

    QVector<QVector4D>SLines;//stroe all lines （p1.x,p1.y;p2.x,p2.y）
    QVector4D Pis;
    for( size_t i = 0; i < Pcvlines.size(); i++ )
    {
        Vec4i l = Pcvlines[i];
        line( newlast, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 3, CV_AA);
        Pis.setX(l[0]);
        Pis.setY(l[1]);
        Pis.setZ(l[2]);
        Pis.setW(l[3]);
        SLines.push_back(Pis);
    }


    Output2File(SLines,"C:/Users/duke/Desktop/SLines.txt");

    Output2File(OrderdOutLine,"C:/Users/duke/Desktop/Orderdoutline.txt");

    for(size_t i=0;i<SLines.size();i++)
    {
        qDebug()<<SLines[i]<<"SLines is this";
    }

    size_t sizei=Pcvlines.size();

    qDebug()<<sizei<<"length  of Pcvlines";



    imshow("lastimage",lastImage);

    imshow("newlastimage",newlast);


    QImage tocv,todraw;
    todraw=tocv=spaceImage;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            tocv.setPixel(i,j,qRgb(0,0,0));
            todraw.setPixel(i,j,qRgb(255,255,255));
        }
    }
    for(int i=0;i<OrderdOutLine.length();i++){

        tocv.setPixel(OrderdOutLine[i].x(),OrderdOutLine[i].y(),qRgb(255,255,255));
        todraw.setPixel(OrderdOutLine[i].x(),OrderdOutLine[i].y(),qRgb(255,0,0));
    }





    Mat mat,graymat,drawmat;

    mat=QImage2cvMat(tocv);
    drawmat=QImage2cvMat(todraw);

    cvtColor(mat,graymat, CV_RGBA2GRAY);

    // std::cout<<graymat<<std::endl;



    imshow("origin", mat);

    imshow("graymat",graymat);

    vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合

    HoughLines(graymat, lines, 1, CV_PI/180, 50, 0, 0 );


    for(size_t i=0;i<lines.size();i++)
    {
        qDebug()<<lines[i][0]<<"   "<<lines[i][1];
    }
    //依次在图中绘制出每条线段
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];

        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 335*(-b));//here has problem the inverse transform is not currect!
        pt1.y = cvRound(y0 + 335*(a));
        pt2.x = cvRound(x0 - 335*(-b));
        pt2.y = cvRound(y0 - 335*(a));
        qDebug()<<"Pt1.x "<<pt1.x<<" pt1.y "<<pt1.y;
        qDebug()<<"Pt2.x "<<pt2.x<<" pt2.y "<<pt2.y;
        line( drawmat, pt1, pt2, Scalar(0,255,0),1,CV_AA);//(image,startpoint,endpoint,color,board,linetype,decemal)
    }




    imshow("after",drawmat);

    waitKey(0);

}

void MainWindow::on_replace_pushButton_clicked()
{
    QString Spot="G10 A=0 B=0 C=0 D=0 E=0 F=0\n";
    serial->write(Spot.toLatin1());
    ui->replace_pushButton->setEnabled(false);
    QEventLoop tsg;
    QTimer::singleShot(2000,&tsg,SLOT(quit()));
    tsg.exec();
    ui->replace_pushButton->setEnabled(true);
}

void MainWindow::on_ImageWatch_pushButton_clicked()
{
    AllImage.push_back(origin_image);
    AllImage.push_back(Timage);
    AllImage.push_back(grayImage);
    AllImage.push_back(OulineImage);
    AllImage.push_back(SmoothOulineImage);

    ImageWatch *IW=new ImageWatch(AllImage);

    IW->show();

}

void MainWindow::on_Outline_Button_clicked()
{
    QString fileadd=QFileDialog::getOpenFileName(this,"openfile",QDir::currentPath(),"*.xlsx");
    if(fileadd.isEmpty())
    {
        QMessageBox::information(this,"notice","not opened");
        return;
    }
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    work_books->dynamicCall("Open (const QString&)", QString(fileadd));
    QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_sheet = work_book->querySubObject("Sheets(int)", 1);
    QAxObject *used_range = work_sheet->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");
    row_start = used_range->property("Row").toInt();  //获取起始行
    column_start = used_range->property("Column").toInt();  //获取起始列
    row_count = rows->property("Count").toInt();  //获取行数
    column_count = columns->property("Count").toInt();  //获取列数
    QVector2D sc;
    for(int i=2;i<row_count+1;i++)
    {
        QAxObject *x = work_sheet->querySubObject("Cells(int,int)", i, 1);
        QVariant x_value = x->property("Value");  //获取单元格内容
        QAxObject *y = work_sheet->querySubObject("Cells(int,int)", i, 2);
        QVariant y_value = y->property("Value");  //获取单元格内容
        sc.setX(x_value.toDouble());
        sc.setY(y_value.toDouble());
        Outline_template.push_back(sc);

    }

    /*************************/
    work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
    excel.dynamicCall("Quit(void)");
    QMessageBox::information(NULL,"Read template","模板导入完毕");

}
