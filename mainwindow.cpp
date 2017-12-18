
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"funcitons.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"codewindow.h"
#include"worldvalues.h"
#include<windows.h>
#include<iostream>
#include <fstream>
#include "capturethread.h"
extern int samplingcount=0;
extern  int samplingGap=0;
extern  int LineCount=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    WhoseTime.start();
    ui->setupUi(this);
    ui->BaudRate->setCurrentIndex(0);
    //关闭发送按钮的使能
    ui->SendMesg_Button->setEnabled(false);
    ui->AutoSend_Button->setEnabled(false);
    ui->Creat_code->setEnabled(false);
    ui->SpeedDirection_comboBox->setEnabled(false);
    ui->BoltSpeed_spinBox->setEnabled(false);

    ui->Xplus_Button->setEnabled(false);
    ui->Yplus_Button->setEnabled(false);
    ui->Zplus_Button->setEnabled(false);
    ui->Aplus_Button->setEnabled(false);
    ui->Bplus_Button->setEnabled(false);
    ui->Cplus_Button->setEnabled(false);
    ui->Xmini_Button->setEnabled(false);
    ui->Ymini_Button->setEnabled(false);
    ui->Zmini_Button->setEnabled(false);
    ui->Amini_Button->setEnabled(false);
    ui->Bmini_Button->setEnabled(false);
    ui->Cmini_Button->setEnabled(false);



    cam     = NULL;
    timer   = new QTimer(this);
    imag    = new QImage();
    WTimer  = new QTimer(this);
    GetpicTimer= new QTimer(this);
    orispot.push_back("\n");
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

    ImageInitialize();
    qDebug()<<tr("Initialize Success!");

    int initial=WhoseTime.elapsed();

    qDebug()<<"program initialize time used: "<<initial;
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
    QString buf;
    buf = serial->readAll();
    qDebug()<<"buff         "<<buf;

    if(m_bReadState){

        QString StrC;
        StrC.append(Array[m_iSendCount]);
        QString StrCheck=StrC.left(2);

        QString tocheck;
        tocheck.prepend(buf.left(2));

        if(tocheck==StrCheck)
        {
            emit Receveid();
            m_bStrCompare=true;
        }
    }
    if(m_bWorldCheck){

        QString sss;
        sss=buf;


        if(Currentcont==0)

        {
            CurrentReturn.clear();
            CurrentReturn=buf;

        }
        else if(Currentcont==1)
        {
            /// sss+=tr(buf);
            QString StrC;
            StrC.append("G97");
            CurrentReturn+=buf;
            qDebug()<<"case currentcont=1 and currentreturn is "<<CurrentReturn;
            QString toFindXYZABC;
            int sx,sy,sz,sa,sb,sc;
            int tempcount=-1;
            qDebug()<<CurrentReturn.length()<<"CurrentReturn length";
            if(m_bWorldCheck&&Currentcont==1)
            {
                QString Xc="X";
                QString Yc="Y";
                QString Zc="Z";
                QString Ac="A";
                QString Bc="B";
                QString Cc="C";
                for(int i=0;i<CurrentReturn.length();i++)
                {
                    QString K=CurrentReturn.mid(i,1);
                    qDebug()<<K;
                    if(K==Xc){
                        //qDebug()<<"enter x";
                        tempcount++;

                        sx=i;
                    }
                    if(K==Yc)
                    {
                        //qDebug()<<"enter y";
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
                    if(K==Bc)
                    {
                        tempcount++;

                        sb=i;
                    }
                    if(K==Cc)
                    {
                        tempcount++;

                        sc=i;
                    }

                }

                toFindXYZABC=CurrentReturn.mid(sx+2,sy-sx-3) ;
                m_dXbase=toFindXYZABC.toDouble();


                toFindXYZABC=CurrentReturn.mid(sy+2,sz-sy-3) ;
                m_dYbase=toFindXYZABC.toDouble();


                toFindXYZABC=CurrentReturn.mid(sz+2,sa-sz-3) ;
                m_dZbase=toFindXYZABC.toDouble();


                CurrentAngel.clear();
                toFindXYZABC=CurrentReturn.mid(sa+2,sb-sa-3) ;
                CurrentAngel.push_back(toFindXYZABC.toFloat());


                toFindXYZABC=CurrentReturn.mid(sb+2,sc-sb-3) ;
                CurrentAngel.push_back(toFindXYZABC.toFloat());

                toFindXYZABC=CurrentReturn.mid(sc+2,sss.length()-sc-2) ;
                CurrentAngel.push_back(toFindXYZABC.toFloat());

                ui->Xbase_spin->setValue(m_dXbase);
                ui->Ybase_spin->setValue(m_dYbase);
                ui->Zbase_spin->setValue(m_dZbase);

                CurrentSpot.setX(m_dXbase);
                CurrentSpot.setY(m_dYbase);
                CurrentSpot.setZ(m_dZbase);

                qDebug()<<"Current angel: "<<CurrentAngel;
                qDebug()<<"Current spot: "<<CurrentSpot;

                m_bWorldCheck=false;
                m_bCurrentGoted=true;
                Currentcont=0;
                if(m_bCurrentGoted){
                    ui->Xplus_Button->setEnabled(true);
                    ui->Yplus_Button->setEnabled(true);
                    ui->Zplus_Button->setEnabled(true);
                    ui->Aplus_Button->setEnabled(true);
                    ui->Bplus_Button->setEnabled(true);
                    ui->Cplus_Button->setEnabled(true);
                    ui->Xmini_Button->setEnabled(true);
                    ui->Ymini_Button->setEnabled(true);
                    ui->Zmini_Button->setEnabled(true);
                    ui->Amini_Button->setEnabled(true);
                    ui->Bmini_Button->setEnabled(true);
                    ui->Cmini_Button->setEnabled(true);
                }

            }
        }
        Currentcont++;
        qDebug()<<"currentcont:  "<<Currentcont;
    }


    if(m_bOrisend)
    {//说明是回归原点的设置发送
        ui->AutoSend_Button->setEnabled(true);
        ui->GetTheWorldCoordinate_button->setEnabled(true);
        ui->SendMesg_Button->setEnabled(true);
        ui->openCamera->setEnabled(true);
        ui->ReadPicture->setEnabled(true);
        ui->Read_txt_Button->setEnabled(true);
        ui->CameraView_Button->setEnabled(true);
        ui->Canny_button->setEnabled(true);

        ui->ChangeTheimage_->setEnabled(true);
        ui->replace_pushButton->setEnabled(true);
        ui->DistortionCalibration_button->setEnabled(true);
        ui->CheckCode_Button->setEnabled(true);
        ui->ImageWatch_pushButton->setEnabled(true);

        m_bOrisend=false;
    }

    if(m_bFromMinitrim)
    {
        m_bFromMinitrim=false;
        on_GetTheWorldCoordinate_button_clicked();
        ui->AutoSend_Button->setEnabled(true);
        ui->GetTheWorldCoordinate_button->setEnabled(true);
        ui->SendMesg_Button->setEnabled(true);
        ui->openCamera->setEnabled(true);
        ui->ReadPicture->setEnabled(true);
        ui->Read_txt_Button->setEnabled(true);
        ui->CameraView_Button->setEnabled(true);
        ui->Canny_button->setEnabled(true);

        ui->ChangeTheimage_->setEnabled(true);
        ui->replace_pushButton->setEnabled(true);
        ui->DistortionCalibration_button->setEnabled(true);
        ui->CheckCode_Button->setEnabled(true);
        ui->ImageWatch_pushButton->setEnabled(true);
    }

    if(!buf.isEmpty())
    {

        QString str = ui->Message_Label->text();
        //   str+=tr(buf);
        str=buf;
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString Tstr = time.toString("yyyy-MM-dd hh:mm:ss.zzz ");
        Tstr.append(": \n");
        Tstr+=str;
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
        qDebug()<<where_value;
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

    // qDebug()<<Todisplay;

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
    //qDebug()<<Array;
    QString filename=QFileDialog::getSaveFileName(this,"savefile",QDir::currentPath());
    //qDebug()<<filename;
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
    if(VCcam.isOpened())
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(EmptyFunction()));
        timer->stop();

        GetpicTimer->stop();
        VCcam.release();
    }
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
void MainWindow::ImageInitialize()
{
    qDebug()<<"Image Initializing...";
    spaceImage=QImage(width,height,QImage::Format_ARGB32);
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            spaceImage.setPixel(x,y,qRgb(255,255,255));
        }
    }

    origin_image=spaceImage;//原图

    Timage=spaceImage;//二色图

    grayImage=spaceImage;//灰度图



    OulineImage=spaceImage;

    SmoothOulineImage=spaceImage;

    GridImage=spaceImage;

    xx=spaceImage;



    qDebug()<<"Image Initialize done!";

}
QImage MainWindow::ImageDrawer(QImage Img, QVector<QVector2D> Array, QColor col, int Broad)
{
    int Imgwidth=Img.width();
    int Imgheight=Img.height();

    QImage toreturn=Img.copy();

    foreach (QVector2D kk,Array)
    {

        if(kk.x()-Broad>=0&&kk.y()-Broad>=0&&kk.x()+Broad<Imgwidth&&kk.y()+Broad<Imgheight)
        {
            int i=kk.x();
            int j=kk.y();

            for(int x=i-Broad;x<i+Broad;x++)
            {
                for(int y=j-Broad;y<j+Broad;y++)
                {
                    toreturn.setPixel(x,y,qRgb(col.red(),col.green(),col.blue()));
                }

            }

        }


    }
    return toreturn;



}
QImage MainWindow::ImageDrawer(QImage Img, int Broad)
{
    int Imgwidth=Img.width();
    int Imgheight=Img.height();
    QImage toreturn=Img;
    QColor color;
    QColor back=Img.pixel(0,0);


    for(int i=0;i<Imgwidth;i++)
    {
        for(int j=0;j<Imgheight;j++)
        {
            color=Img.pixel(i,j);

            if(color!=back)
            {
                //  qDebug()<<"Img pixel  :"<<color<<"     back  "<<back;
                for(int x=i-Broad;x<i+Broad;x++)
                {
                    for(int y=j-Broad;y<j+Broad;y++)
                    {
                        if(i-Broad>=0&&j-Broad>=0&&i+Broad<Imgwidth&&j+Broad<Imgheight)
                            toreturn.setPixel(x,y,qRgb(color.red(),color.green(),color.blue()));
                    }

                }
            }

        }
    }
    return toreturn;
}

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
    // qDebug()<<"all:  "<<all;
    //qDebug()<<"VD  "<<V4Domain;
    ConnectDomains();

    int elapsed=DCtimer.elapsed();
    qDebug()<<"DomainFunction used time:"<<elapsed;
}

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
    qDebug()<<"spotpoint: "<<spotpoint;
    QVector<int>Dflag;
    for(int i=0;i<OrderStatistic.length();i++)
    {
        if(OrderArea[i]>spotpoint){
            Dflag.push_back(order[i]);
        }

    }
    qDebug()<<"DFlag: "<<Dflag;

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
    qDebug()<<"maxflag: "<<maxflag;
    for(int qq=0;qq<V4Domain.length();qq++)
    {
        if(V4Domain[qq].y()==maxflag)
        {
            V4Domain_max.push_back(V4Domain[qq]);
        }

    }
    // qDebug()<<"max: "<<V4Domain_max;
}

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
    OnlyOutLine.clear();
    OnlyOutLine_count.clear();
    /* qDebug()<<Ix.width();
   qDebug()<<Ix.height();
   qDebug()<<rx.width();
   qDebug()<<rx.height();
    exit(0);*/


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
                }
            }

        }

    }
    int firstela=GOLtimer.elapsed();
    qDebug()<<"first step time used "<<firstela;
    //qDebug()<<"Outline:  "<<OutLine.length();
    OutLine_copy=OutLine;//copy the all xy to use

    int *outlineX=new int[OutLine.length()];
    int *outlineY=new int[OutLine.length()];//由于容器类操作速度太慢，尝试使用数组操作
    for(int i=0;i<OutLine.length();i++)
    {
        outlineX[i]=OutLine[i].x();
        outlineY[i]=OutLine[i].y();
    }

    //qDebug()<<"outline_copy lenth: "<<OutLine_copy.length();
    if(m_bOnlyOutline){
        qDebug()<<"Entered it!";
        int stx=-5;
        int sty=-5;
        for(int x = 1; x<width3-1; x++)
        {
            for(int y = 1; y<height3-1; y++)
            {
                oldColor3 = QColor(Ix.pixel(x,y));

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
                //qDebug()<<"there: "<<outflag;
                //qDebug()<<allcounting;
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
            //qDebug()<<OnlyOutLine_count;
        }
        int secondst=GOLtimer.elapsed()-timeF;
        qDebug()<<"second step time used:"<<secondst;
        //上色

        if(m_bOnlyOutline)
        {
            qDebug()<<"draw new color!";
            if(OnlyOutLine.length()!=0){
                for(int i=0;i<OnlyOutLine.length();i++)
                {
                    rx.setPixel(OnlyOutLine[i].x(),OnlyOutLine[i].y(),qRgb(253,0,0));
                }
            }
        }
        qDebug()<<"go to here";

        //  Ix.setPixel(SumX/pixcount,SumY/pixcount,qRgb(255,0,0));

        int alltime=GOLtimer.elapsed();

        qDebug()<<"Get outline time used:"<<alltime;

        delete []outlineX;

        delete []outlineY;


    }

    OnlyOutLine=Unique_2D(OnlyOutLine);
    return rx;
}

void MainWindow::SmoothOutline()
{
    QTime SOtimer;

    SOtimer.start();

    Unique_2D(OnlyOutLine);
    ReOrderOutline(OnlyOutLine);//重新排序边界

    //  CurveFit(OrderdOutLine);//进行B样条的曲线拟合，这个数据暂时没用


    //计算全部边界的两点间方向//9.22尝试

    num_Derec=DerectionCalFunc(OrderdOutLine);


    //去除一些噪点
    //DeleteOutlineNoise();


    //得到转折点

    BreakPoints=GetBreakPoints(num_Derec,OrderdOutLine);

    //获取特征点

    //  CharacteristicCalculate(BreakPoints);//此处未完待续

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
QImage MainWindow::DeleteOutRectangel(QImage input)
{
    QImage toreturn=input;

    int Imwidth=input.width();
    int Imheight=input.height();

    const int DelBoard=2;

    for(int i=0;i<Imwidth;i++)
    {
        for(int j=0;j<Imheight;j++)
        {
            if(i<DelBoard||j<DelBoard||i>Imwidth-DelBoard||j>Imheight-DelBoard)
            {
                toreturn.setPixel(i,j,qRgb(255,255,255));
            }
        }
    }


    return toreturn;


}

void MainWindow::ReadPngButton()
{
    ui->CameraView_Button->setEnabled(false);
    ui->openCamera->setEnabled(false);
    ClearVector();
    ImageDisplayFunciton(ui->Origin_Label,spaceImage,400,300);
    ImageDisplayFunciton(ui->final_label,spaceImage,400,300);
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
    origin_image=DeleteOutRectangel(origin_image);
    Timage=origin_image;//二色图
    grayImage=origin_image;//灰度图
    ImageDisplayFunciton(ui->Origin_Label,origin_image,400,300);
    ToGray();
    ToTwoColor();
    //jisuan liantongyu
    DomainCalcu(Timage);//then we get V4Domain
    /**************************************************************/
    //过滤
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

    for(int i=0;i<V4Domain_max.length();i++)
    {

        for(int j=V4Domain_max[i].z();j<V4Domain_max[i].w();j++)
        {
            int x=V4Domain_max[i].x();
            int y=j;

            OulineImage.setPixel(x,y,qRgb(0,0,0));

        }
    }

    QImage maxDomain=OulineImage;

    OulineImage=GetOutLine(OulineImage);//get the outline of the max domain
    if(ui->isOutlineoffset->isChecked())
    {
        //表示需要偏移
        int offsetValue=ui->OffsetValueInput->text().toInt();
        Orioutline=OnlyOutLine;
        QVector<QVector2D>insideroutline= OutlineErosion(maxDomain,OnlyOutLine,V4Domain_max,offsetValue);
        OulineImage=GetOutLine(ErosionImage);//get the outline of the max domain
    }

    SmoothOutline();
    SmoothOulineImage=spaceImage;

    int offsetValue=ui->OffsetValueInput->text().toInt();

    if(!ui->Erosion_checkBox->isChecked()&&ui->isOutlineoffset->isChecked()){
        QVector<QVector2D>insideroutline=CurveOffset(OrderdOutLine,ImportantKey,curvePoints,offsetValue);
        OulineImage=GetOutLine(ErosionImage);//get the outline of the max domain
    }

    for(int i=0;i<OrderdOutLine.length();i++){
        SmoothOulineImage.setPixel(OrderdOutLine[i].x(),OrderdOutLine[i].y(),qRgb(255,0,0));
    }

    OulineImage_b=ImageDrawer( SmoothOulineImage,3);//边界加粗
    ui->CameraView_Button->setEnabled(true);
    ui->openCamera->setEnabled(true);
    if(!ui->disperse_checkBox->isChecked())
    {
        QVector<QVector2D>HoughPoints;//每一条直线组成各一个2D向量
        HoughPoints=HoughTransform(OulineImage,OrderdOutLine.length()/30,minmumLine);
        Output2File(HoughPoints,"F:/output/HoughPoints.txt");
        QVector<int>HoughPoints_int=TransSequence2D_ToInt(OrderdOutLine,HoughPoints);
        QVector<QVector2D>OrderedSline;
        QVector<int>testorder;

        if(HoughPoints.length()!=0){//如果有直线
            testorder=PointReorder_Rint(HoughPoints,OrderdOutLine,minmumDcres);

            OrderedSline=PointReorder(HoughPoints,OrderdOutLine,minmumDcres);

            HoughPoints=OrderedSline;
            HoughPoints_int=TransSequence2D_ToInt(OrderdOutLine,HoughPoints);
        }
        else
        {
            OrderedSline=HoughPoints;
        }

        LineCount=OrderedSline.length();

        Output2File(OrderedSline,"F:/output/OrderedSline.txt");
        Output2File(OrderdOutLine,"F:/output/Orderdoutline.txt");
        qDebug()<<" Hough points "<<HoughPoints_int<<"  "<<HoughPoints;;
        int All_Points_cout=OrderdOutLine.length();//获取总点数


        if( OrderedSline.length()>=4){

            qDebug()<<"+++***   There are more than 2 strait line   ***+++";
            HoughPoints=OrderedSline;
            HoughPoints_int=TransSequence2D_ToInt(OrderdOutLine,HoughPoints);

            QVector<int>m_Int_Line;

            m_Int_Line=LineMerge(testorder,OrderedSline,OrderdOutLine,
                                 BreakPoints,ui->CurvedBisector_checkBox->isChecked(),ImportantKey,minmumDcres);

            CharacteristicPoint.clear();
            CharacteristicPoint   =TransSequenceTo2D(OrderdOutLine,m_Int_Line);
        }
        else if( OrderedSline.length()==2)

        { //一条直线的情况
            //直线数量不足，说明全部是曲线，直接进行曲线检测
            HoughPoints=OrderedSline;
            HoughPoints_int=TransSequence2D_ToInt(OrderdOutLine,HoughPoints);
            qDebug()<<"+++***   Only one strait line is here    ***+++";
            int Checkflag=abs(HoughPoints_int[0]-HoughPoints_int[1]);//可能是直线点数
            //如果直线包含原点，这个值就会大于全部点数的一半
            int Checklength=All_Points_cout*0.35;
            if(Checkflag>=minmumDcres*2)
            {//直线太短就不要了

                if(Checkflag<Checklength)
                {
                    //this situation is that  origin point is not in this line；
                    // so curve contain origin points
                    qDebug()<<"Only one line and *******curve****** contain the origin point";
                    QVector<int> CurvePoints_int;
                    if(HoughPoints_int[0]-HoughPoints_int[1]>0)
                    {
                        //在0的点数据大，说明是在原点前面
                        for(int n=HoughPoints_int[0];n<All_Points_cout;n++)
                        {
                            CurvePoints_int.push_back(n);
                        }
                        for(int n=0;n<=HoughPoints_int[1];n++)
                        {
                            CurvePoints_int.push_back(n);
                        }
                    }

                    else
                    {
                        for(int n=HoughPoints_int[1];n<All_Points_cout;n++)
                        {
                            CurvePoints_int.push_back(n);
                        }
                        for(int n=0;n<=HoughPoints_int[0];n++)
                        {
                            CurvePoints_int.push_back(n);
                        }
                    }

                    //QVector<int > DispersedP=CheckPointInline(CurvePoints_int,OrderdOutLine,BreakPoints,minmumDcres);
                    QVector<int >DispersedP=FindKeypoints(CurvePoints_int,OrderdOutLine,minmumDcres,ImportantKey);
                    CharacteristicPoint.clear();

                    CharacteristicPoint.push_back(OrderedSline[0]);
                    CharacteristicPoint= TransSequenceTo2D(OrderdOutLine,DispersedP);//生成关键点坐标

                    CharacteristicPoint.push_back(OrderedSline[1]);


                }
                else
                {
                    //直线包含原点
                    qDebug()<<"Only one line and **********Line******* contain the origin point";
                    QVector<int> CurvePoints_int;

                    if(HoughPoints_int[0]>HoughPoints_int[1])
                    {
                        for(int n=HoughPoints_int[1];n<=HoughPoints_int[0];n++)
                        {
                            CurvePoints_int.push_back(n);
                        }
                    }
                    else{
                        for(int n=HoughPoints_int[0];n<=HoughPoints_int[1];n++)
                        {
                            CurvePoints_int.push_back(n);
                        }
                    }

                    // QVector<int > DispersedP=CheckPointInline(CurvePoints_int,OrderdOutLine,BreakPoints,minmumDcres);
                    QVector<int >DispersedP=FindKeypoints(CurvePoints_int,OrderdOutLine,minmumDcres,ImportantKey);
                    CharacteristicPoint.clear();

                    CharacteristicPoint.push_back(OrderedSline[0]);
                    CharacteristicPoint=  TransSequenceTo2D(OrderdOutLine,DispersedP);//生成关键点坐标
                    CharacteristicPoint.push_back(OrderedSline[1]);



                }
            }
            else
            {

                HoughPoints.clear();
                HoughPoints_int.clear();
                goto T;
            }
        }
        else if( OrderedSline.length()<1)//全是曲线
        {
T:
            QVector<int> CurvePoints_int;

            for(int n=0;n<=All_Points_cout-1;n++)
            {
                CurvePoints_int.push_back(n);
            }
            QVector<int > DispersedP=FindKeypoints(CurvePoints_int,OrderdOutLine,minmumDcres,ImportantKey);

            CharacteristicPoint.clear();
            CharacteristicPoint=  TransSequenceTo2D(OrderdOutLine,DispersedP);//生成关键点坐标
        }

        Output2File(CharacteristicPoint,"F:/output/CharacteristicPoint.txt");

    }
    else
    {//全部等距离散
        CharacteristicPoint.clear();
        for(int nn=0;nn<OrderdOutLine.length();nn+=disperse)
        {
            CharacteristicPoint.push_back(OrderdOutLine[nn]);
        }
        CharacteristicPoint.push_back(OrderdOutLine[0]);
    }

    OulineImage_b=ImageDrawer( OulineImage_b,Orioutline,QColor(185,120,125),2);

    // Mat toshow =QImage2cvMat(OulineImage_b);
    //  imshow("boundary",toshow);

    OulineImage_b=ImageDrawer( OulineImage_b,CharacteristicPoint,QColor(0,255,0),7);

    ImageDisplayFunciton(ui->final_label,OulineImage_b,400,300);

    //创建关键点坐标代码

    for(int i=0;i<CharacteristicPoint.length();i++)
    {
        SetCoordinate(CharacteristicPoint[i].x(),CharacteristicPoint[i].y(),0);
    }

    CreatReport("");
    CreadOrders();

    QimageSave(OulineImage_b,"outlineb");
    QimageSave(origin_image,"Originimage");
    m_bReadState=true;


    //CreadOrders();
    int elapsed=WhoseTime.elapsed()-readstart;
    ui->time_label->setText(QString::number(elapsed)+" ms");

}
void MainWindow::on_openCamera_clicked()
{
    if(ui->openCamera->text()=="自动视觉")
    {

        ui->ReadPicture->setEnabled(false);
        ui->openCamera->setText("关闭相机");


        VCcam=cv::VideoCapture(0);

        VCcam.set(CV_CAP_PROP_FRAME_WIDTH,camerawidth);
        VCcam.set(CV_CAP_PROP_FRAME_HEIGHT,cameraheight);



        timer->start(80);

        GetpicTimer->start(4000);
        QObject::connect(GetpicTimer, SIGNAL(timeout()), this, SLOT(TakingPhoto()));
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));
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
        QObject::connect(GetpicTimer, SIGNAL(timeout()), this, SLOT(EmptySlots()));
        //cvReleaseCapture(&cam);
        VCcam.release();
    }

}
Mat MainWindow::CenterClipping(Mat inputarray)
{
    int rows=inputarray.rows/2;
    int cols=inputarray.cols/2;
    qDebug()<<"to clip the mat to size of  "<<rows<<"   "<<cols;
    Mat Toreturn,midarray;
    midarray=inputarray.clone();
    Toreturn=midarray.rowRange(rows-height/2,rows+height/2);
    Toreturn=Toreturn.colRange(cols-width/2,cols+width/2);
    return Toreturn;
}

void MainWindow::readFarme()
{
    if(VCcam.isOpened()){
        cv::Mat vcframe;
        VCcam>>vcframe;
        if(!frame)
        {
            exit(0);
        }
        if(m_bCalibration)
        {
            Mat newframe=CenterClipping(vcframe);
            vcframe.release();
            vcframe=newframe.clone();
            newframe.release();


            QImage image=cvMat2QImage(vcframe);

            image=image.scaled(width,height);

            if(m_bFrompreView){
                for(int i=0;i<width;i++)
                {
                    for(int j=0;j<height;j++)
                    {
                        if(i<5||i>width-5||(i<width/2+3)&&(i>width/2-3)||j<5||j>height-5||(j<height/2+3)&&(j>height/2-3))
                            image.setPixel(i,j,qRgb(255,255,0));
                    }

                }
            }
            //ui->Origin_Label->setPixmap(QPixmap::fromImage(image));
            ImageDisplayFunciton(ui->Origin_Label,image,400,300);



        }
        else{

            QImage image=cvMat2QImage(vcframe);

            image=image.scaled(width,height);

            if(m_bFrompreView){
                for(int i=0;i<width;i++)
                {
                    for(int j=0;j<height;j++)
                    {
                        if(i<5||i>width-5||(i<width/2+3)&&(i>width/2-3)||j<5||j>height-5||(j<height/2+3)&&(j>height/2-3))
                            image.setPixel(i,j,qRgb(255,255,0));
                    }

                }
            }
            //ui->Origin_Label->setPixmap(QPixmap::fromImage(image));
            ImageDisplayFunciton(ui->Origin_Label,image,400,300);
        }

    }

}
void MainWindow::EmptyFunction()
{
    qDebug()<<"empty function";
}
void MainWindow::TakingPhoto()
{
    // frame = cvQueryFrame(cam);
    //   QImage image2 = QImage((const uchar*)frame->imageData, frame->width, frame->height, QImage::Format_RGB888).rgbSwapped();
    if(VCcam.isOpened()){
        if(m_bCalibration)
        {
            cv:: Mat vcframe;
            VCcam>>vcframe;

            Mat mid=CenterClipping(vcframe);


            QImage image2 =cvMat2QImage(mid);
            origin_image=image2.scaled(width,height);
            AutoRun();
        }
        else
        {
            cv:: Mat vcframe;
            VCcam>>vcframe;
            QImage image2 =cvMat2QImage(vcframe);
            origin_image=image2.scaled(width,height);
            AutoRun();
        }
    }
}
void MainWindow::AutoRun()
{
    qDebug()<<"Auto run ";

    AutoRunTimer.restart();
    ui->progressBar->setRange(0,1000);

    ui->progressBar->setValue(0);

    ui->progressBar->setValue(3);

    ImageDisplayFunciton(ui->Origin_Label,spaceImage,400,300);

    ImageDisplayFunciton(ui->final_label,spaceImage,400,300);
    ClearVector();
    ui->progressBar->setValue(9);
    origin_image=DeleteOutRectangel(origin_image);

    // spaceImage=origin_image;


    Timage=origin_image;//二色图

    grayImage=origin_image;//灰度图
    ui->progressBar->setValue(18);
    ImageDisplayFunciton(ui->Origin_Label,origin_image,400,300);

    ToGray();

    ToTwoColor();
    ui->progressBar->setValue(40);
    //jisuan liantongyu

    DomainCalcu(Timage);//then we get V4Domain

    /* QImage DomainTest=spaceImage;

    for(int i=0;i<V4Domain.length();i++)
    {

        for(int j=V4Domain[i].z();j<V4Domain[i].w();j++)
        {
            int x=V4Domain[i].x();
            int y=j;
            DomainTest.setPixel(x,y,qRgb(0,0,0));
        }
    }*/

    /**************************************************************/
    //过滤
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
    ui->progressBar->setValue(60);

    /******************************************************/
    //画出最大连通域的外轮廓
    /************************************************************/
    OulineImage=spaceImage;

    for(int i=0;i<V4Domain_max.length();i++)
    {

        for(int j=V4Domain_max[i].z();j<V4Domain_max[i].w();j++)
        {
            int x=V4Domain_max[i].x();
            int y=j;

            OulineImage.setPixel(x,y,qRgb(0,0,0));

        }
    }

    ui->progressBar->setValue(100);

    QImage maxDomain=OulineImage;

    OulineImage=GetOutLine(OulineImage);//get the outline of the max domain

    if(ui->isOutlineoffset->isChecked())
    {
        //表示需要偏移
        int offsetValue=ui->OffsetValueInput->text().toInt();

        QVector<QVector2D>insideroutline= OutlineErosion(maxDomain,OnlyOutLine,V4Domain_max,offsetValue);

        OulineImage=GetOutLine(ErosionImage);//get the outline of the max domain
    }

    ui->progressBar->setValue(150);
    SmoothOutline();
    ui->progressBar->setValue(220);
    SmoothOulineImage=spaceImage;

    for(int i=0;i<OrderdOutLine.length();i++){
        SmoothOulineImage.setPixel(OrderdOutLine[i].x(),OrderdOutLine[i].y(),qRgb(255,0,0));
    }


    OulineImage_b=ImageDrawer( SmoothOulineImage,3);//边界加粗

    ui->progressBar->setValue(250);





    ui->CameraView_Button->setEnabled(true);

    ui->openCamera->setEnabled(true);

    QVector<QVector2D>HoughPoints;//每一条直线组成各一个2D向量

    HoughPoints=HoughTransform(OulineImage,OrderdOutLine.length()/30,minmumLine);

    Output2File(HoughPoints,"F:/output/HoughPoints.txt");

    int All_Points_cout=OrderdOutLine.length();//获取总点数


    ui->progressBar->setValue(300);
    if(HoughPoints.length()>=2){

        qDebug()<<"+++***   There are more than 2 strait line   ***+++";



        QVector<int>testorder;

        QVector<QVector2D>test2D;

        testorder=PointReorder_Rint(HoughPoints,OrderdOutLine,minmumDcres);



        OrderedSline=PointReorder(HoughPoints,OrderdOutLine,minmumDcres);

        ui->progressBar->setValue(360);

        Output2File(OrderedSline,"F:/output/OrderedSline.txt");

        Output2File(OrderdOutLine,"F:/output/Orderdoutline.txt");

        ui->progressBar->setValue(370);

        QVector<int>m_Int_Line;



        m_Int_Line=LineMerge(testorder,OrderedSline,OrderdOutLine,
                             BreakPoints,ui->CurvedBisector_checkBox->isChecked(),ImportantKey,minmumDcres);




        CharacteristicPoint.clear();

        CharacteristicPoint   =TransSequenceTo2D(OrderdOutLine,m_Int_Line);




    }
    else if(HoughPoints.length()==1)

    {//一条直线的情况
        //直线数量不足，说明全部是曲线，直接进行曲线检测
        qDebug()<<"+++***   Only one strait line is here    ***+++";


        int Checkflag=abs(HoughPoints[0].x()-HoughPoints[0].y());

        int Checklength=HoughPoints[0].x()+ All_Points_cout-HoughPoints[0].y();

        if(Checkflag<Checklength)
        {//this situation is that  origin point is not in this line；
            // so curve contain origin points
            qDebug()<<"Only one line and *******curve****** contain the origin point";
            QVector<int> CurvePoints_int;

            for(int n=HoughPoints[0].y();n<All_Points_cout;n++)
            {
                CurvePoints_int.push_back(n);
            }
            for(int n=0;n<=HoughPoints[0].x();n++)
            {
                CurvePoints_int.push_back(n);
            }



            QVector<int > DispersedP=CheckPointInline(CurvePoints_int,OrderdOutLine,BreakPoints,minmumDcres);
            CharacteristicPoint.clear();

            CharacteristicPoint=  TransSequenceTo2D(OrderdOutLine,DispersedP);//生成关键点坐标
            CharacteristicPoint.push_back(CharacteristicPoint[0]);


        }
        else
        {//直线包含原点
            qDebug()<<"Only one line and **********Line******* contain the origin point";
            QVector<int> CurvePoints_int;

            for(int n=HoughPoints[0].x();n<=HoughPoints[0].y();n++)
            {
                CurvePoints_int.push_back(n);
            }



            QVector<int > DispersedP=CheckPointInline(CurvePoints_int,OrderdOutLine,BreakPoints,minmumDcres);
            CharacteristicPoint.clear();

            CharacteristicPoint=  TransSequenceTo2D(OrderdOutLine,DispersedP);//生成关键点坐标
            CharacteristicPoint.push_back(CharacteristicPoint[0]);

        }

    }


    else
    {
        qDebug()<<"+++***               there is no strait line                    ***+++";


        QVector<int> CurvePoints_int;

        for(int n=0;n<=All_Points_cout;n++)
        {
            CurvePoints_int.push_back(n);
        }

        QVector<int > DispersedP=CheckPointInline(CurvePoints_int,OrderdOutLine,BreakPoints,minmumDcres);
        CharacteristicPoint.clear();

        CharacteristicPoint=  TransSequenceTo2D(OrderdOutLine,DispersedP);//生成关键点坐标
        CharacteristicPoint.push_back(CharacteristicPoint[0]);

    }

    ui->progressBar->setValue(750);
    Output2File(CharacteristicPoint,"F:/output/CharacteristicPoint.txt");


    OulineImage_b=ImageDrawer( OulineImage_b,CharacteristicPoint,QColor(0,255,0),7);
    ui->progressBar->setValue(800);

    ImageDisplayFunciton(ui->final_label,OulineImage_b,400,300);

    //创建关键点坐标代码

    for(int i=0;i<CharacteristicPoint.length();i++)
    {
        SetCoordinate(CharacteristicPoint[i].x(),CharacteristicPoint[i].y(),0);
    }

    ui->progressBar->setValue(900);
    CreadOrders();
    if(m_bSerialIsOpen)
    {//如果串口是打开的，即表示机器人是连接的

    }
    ui->progressBar->setValue(1000);






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
    ctrlPoints.clear();
    curvePoints.clear();
    Outline_template.clear();
    Orioutline.clear();
    OrderedSline.clear();
    samplingcount=0;
    samplingGap=0;
    LineCount=0;
    CoorCount=0;
}
void MainWindow::CameraPreView()
{
    if(ui->CameraView_Button->text()=="相机预览")
    {
        using namespace cv;
        qDebug()<<"open camera!";
        m_bFrompreView=true;

        ui->openCamera->setEnabled(false);

        connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));
        //  cam = cvCreateCameraCapture(0);
        VCcam=cv::VideoCapture(0);
        VCcam.set(CV_CAP_PROP_FRAME_WIDTH,camerawidth);
        VCcam.set(CV_CAP_PROP_FRAME_HEIGHT,cameraheight);

        timer->start(100);

        ui->CameraView_Button->setText("关闭相机");

    }else
    {
        m_bFrompreView=false;
        ui->CameraView_Button->setText("相机预览");
        timer->stop();
        connect(timer, SIGNAL(timeout()), this, SLOT(EmptyFunction()));
        //cvReleaseCapture(&cam);
        VCcam.release();
        ui->openCamera->setEnabled(true);
        // ui->Origin_Label->setPixmap(QPixmap::fromImage(spaceImage));

        ImageDisplayFunciton(ui->Origin_Label,spaceImage,400,300);


    }

}
void MainWindow::on_GetTheWorldCoordinate_button_clicked()
{
    m_bSerialIsOpen=true;
    if(m_bSerialIsOpen){

        QString sendstr;
        sendstr.append("G97");
        sendstr.append("\n");
        serial->write(sendstr.toLatin1());
        m_bWorldCheck=true;
    }
    else
    {
        QMessageBox::warning(this,"warning","SerialPort is not open!");
    }

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
        // pp=pp.fromImage(SmoothOulineImage);
        //ui->final_label->setPixmap(pp);
        ImageDisplayFunciton(ui->final_label,OulineImage_b,400,300);


    }
    else if(index==1)
    {
        // pp=pp.fromImage(grayImage);
        // ui->final_label->setPixmap(pp);
        ImageDisplayFunciton(ui->final_label,grayImage,400,300);


    }
    else if(index==2)
    {
        //pp=pp.fromImage(Timage);
        // ui->final_label->setPixmap(pp);
        ImageDisplayFunciton(ui->final_label,Timage,400,300);
    }
    else if(index==3)
    {
        ///pp=pp.fromImage(OulineImage);
        //ui->final_label->setPixmap(pp);
        ImageDisplayFunciton(ui->final_label,OulineImage_b,400,300);
    }
}
void MainWindow::DeleteOutlineNoise()
{
    qDebug()<<"this is delete funciton!";
    //delete the derection vector's noise
    QTime DOtime;
    DOtime.start();
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
    qDebug()<<"delete outline noise time "<<QString::number(DOtime.elapsed());

}
QVector<QVector2D>MainWindow:: ReOrderOutline_8Neighboor(QVector <QVector2D> inputarray,QImage inputIMG)
{
    QVector<QVector2D> Toreturn;
    Output2File(inputarray,"F:/output/outline");
    foreach (QVector2D P, inputarray) //边界上异色
    {
        inputIMG.setPixel(P.x(),P.y(),qRgb(125,200,235));
    }
    Toreturn.push_back(inputarray[0]);
    inputIMG.setPixel(inputarray[0].x(),inputarray[0].y(),qRgb(0,0,0));
    QVector2D LastP(-5,-5);
    QVector2D P=inputarray[0];
    int conntt=0;
    for(int i=1;i<inputarray.length();i++)
    {

        if(Toreturn.length()>=2)
        {
            LastP=Toreturn[Toreturn.length()-2];
        }

        QVector2D NewP;
        if(inputIMG.pixel(P.x()-1,P.y()+1)==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x()-1,P.y()+1)>1)
        {
            //左上角
            NewP.setX(P.x()-1);
            NewP.setY(P.y()+1);
            P=NewP;
            Toreturn.push_back(NewP);
            inputIMG.setPixel(P.x()-1,P.y()+1,qRgb(0,0,0));
            continue;

        }
        else if(inputIMG.pixel(P.x(),P.y()+1)==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x(),P.y()+1)>1)
        {
            //正上方
            NewP.setX(P.x());
            NewP.setY(P.y()+1);
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x(),P.y()+1,qRgb(0,0,0));
            continue;
        }
        else if(inputIMG.pixel(P.x()+1,P.y()+1)==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x()+1,P.y()+1)>1)
        {
            //右上角
            NewP.setX(P.x()+1);
            NewP.setY(P.y()+1);
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x()+1,P.y()+1,qRgb(0,0,0));
            continue;
        }
        else if(inputIMG.pixel(P.x()+1,P.y())==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x()+1,P.y())>1)
        {
            //正右边
            NewP.setX(P.x()-1);
            NewP.setY(P.y());
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x()+1,P.y(),qRgb(0,0,0));
            continue;
        }
        else if(inputIMG.pixel(P.x()+1,P.y()-1)==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x()+1,P.y()-1)>1)
        {
            //右下角
            NewP.setX(P.x()+1);
            NewP.setY(P.y()-1);
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x()+1,P.y()-1,qRgb(0,0,0));
            continue;
        }
        else if(inputIMG.pixel(P.x(),P.y()-1)==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x(),P.y()-1)>1)
        {
            //正下方
            NewP.setX(P.x());
            NewP.setY(P.y()-1);
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x(),P.y()-1,qRgb(0,0,0));
            continue;
        }
        else if(inputIMG.pixel(P.x()-1,P.y()-1)==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x()-1,P.y()-1)>1)
        {
            //左下角
            NewP.setX(P.x()-1);
            NewP.setY(P.y()-1);
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x()-1,P.y()-1,qRgb(0,0,0));
            continue;
        }else if(inputIMG.pixel(P.x()-1,P.y())==qRgb(125,200,235)&&DisCalFuc(LastP.x(),LastP.y(),P.x()-1,P.y())>1)
        {
            //正左边
            NewP.setX(P.x()-1);
            NewP.setY(P.y());
            Toreturn.push_back(NewP);
            P=NewP;
            inputIMG.setPixel(P.x()-1,P.y(),qRgb(0,0,0));
            continue;
        }
        else{
            QMessageBox::information(NULL,"infor","impossible");
            qDebug()<<"dis   "<<DisCalFuc(LastP.x(),LastP.y(),P.x(),P.y())<<"    spot   P "<< P<<"  LastP"<<LastP;
            qDebug()<<"toreturn  "<<Toreturn;
            exit(0);
        }
    }

    qDebug()<<"length of the input:"<<inputarray.length();
    qDebug()<<"length of the output:"<<Toreturn.length();
    return Toreturn;

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

    QVector2D Cantchoose;

    for(int i=0;i<length;i++)
    {
        tempx[i]=RO[i].x();
        tempy[i]=RO[i].y();
    }
    Last.setX(tempx[0]);
    Last.setY(tempy[0]);
    NewOrder.push_back(Last);
    Cantchoose=Last;
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
            // if(abs(tempx[j]-Cantchoose.x())>1&&abs(tempy[j]-Cantchoose.y())>1)
            //{
            dis[j]=sqrt(pow(Last.x()-tempx[j],2)+pow(Last.y()-tempy[j],2));
            if(minval>dis[j]){
                minval=dis[j];
                minspot=j;
            }
            //  }
        }
        if(minval<2){
            Last.setX(tempx[minspot]);
            Last.setY(tempy[minspot]);
            Cantchoose=Last;
            NewOrder.push_back(Last);
        }
        tempx[minspot]=-10;
        tempy[minspot]=-10;

        delete []dis;
        dis=NULL;

    }
    /* if(NewOrder.length()!=length)
    {
        QMessageBox::information(this,"notice!","NewOrder is not enough!");
        ErrorFunction();
    }*/
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
QVector<QVector2D> MainWindow ::OutlineErosion(QImage inputImg,QVector<QVector2D> OutlinePoints,
                                               QVector <QVector4D> V4area,int distances)
{
    /*
     * inputImg is the input image which the max domain on it
     * OutlinePoints is the outline of the domain and it already sorted
     * V4area is the all points of the domain
     *distances is the value about how far the distance is to be move for the outline,minis means move into inside
     * return the new ordered outline in 2D type
 */

    qDebug()<<"==================================>enter the function OutlineErosion!";
    QTime ErosionTimer;
    ErosionTimer.start();
    QVector<QVector2D>Toreturn;

    if(ui->Erosion_checkBox->isChecked()&&ui->isOutlineoffset->isChecked())//两个都勾选的时候则用我的方法
    {
        QVector<QVector2D>MidData;
        QVector<QVector2D> area;
        QVector2D temp2DD;

        int Xstart,Xend,Ystart,Yend;//找到一个更小的矩形框来框住连通域，这样检索范围会小一些

        //to get all  the area points to 2D type
        for(int i=0;i<V4area.length();i++)
        {

            for(int j=V4area[i].z();j<V4area[i].w();j++)
            {
                temp2DD.setX(V4area[i].x());
                temp2DD.setY(j);
                area.push_back(temp2DD);

            }
        }
        Xstart=Xend=area[0].x();
        Ystart=Yend=area[0].y();

        foreach (QVector2D xy, area)
        {
            if(xy.x()<Xstart)
            {
                Xstart=xy.x();
            }
            if(xy.x()>Xend)
            {
                Xend=xy.x();
            }
            if(xy.y()<Ystart)
            {
                Ystart=xy.y();
            }
            if(xy.y()>Yend)
            {
                Yend=xy.y();
            }
        }
        Xstart=Xstart-2;
        Ystart=Ystart-2;
        Xend=Xend+2;
        Yend=Yend+2;//扩大一点方框

        foreach (QVector2D ks, OutlinePoints)
        {
            inputImg.setPixel(ks.x(),ks.y(),qRgb(255,255,255));
        }


        QVector<int>newhere;

        if(distances<0){
            //first to check four neiborhood
            bool lastround=false;
            for(int i=1;i<=-distances;i++){
                MidData.clear();
                for(int x=Xstart;x<Xend;x++)
                {
                    for(int y=Ystart;y<Yend;y++)
                    {
                        QColor inputcolor;
                        inputcolor=inputImg.pixel(x,y);
                        if(inputcolor.red()==255&&inputcolor.blue()==255&&inputcolor.green()==255)
                        {
                            inputcolor=inputImg.pixel(x+1,y);
                            if(inputcolor.red()==0)
                            {
                                temp2DD.setX(x+1);
                                temp2DD.setY(y);
                                MidData.push_back(temp2DD);
                                continue;
                            }
                            inputcolor=inputImg.pixel(x-1,y);
                            if(inputcolor.red()==0)
                            {
                                temp2DD.setX(x-1);
                                temp2DD.setY(y);
                                MidData.push_back(temp2DD);
                                continue;
                            }
                            inputcolor=inputImg.pixel(x,y+1);
                            if(inputcolor.red()==0)
                            {
                                temp2DD.setX(x);
                                temp2DD.setY(y+1);
                                MidData.push_back(temp2DD);
                                continue;
                            }
                            inputcolor=inputImg.pixel(x,y-1);
                            if(inputcolor.red()==0)
                            {
                                temp2DD.setX(x);
                                temp2DD.setY(y-1);
                                MidData.push_back(temp2DD);
                                continue;
                            }
                        }

                    }
                }


                for(int t=0;t<MidData.length();t++)
                {
                    inputImg.setPixel(MidData[t].x(),MidData[t].y(),qRgb(255,255,255));
                }


                /*if(i==-distances)
            {
                lastround=true;
            }
            foreach (QVector2D kk, OutlinePoints)
            {
                if(inputImg.pixel( kk.x()+1,kk.y())==qRgb(0,0,0))
                {
                    temp2DD.setX(kk.x()+1);
                    temp2DD.setY(kk.y());
                    MidData.push_back(temp2DD);
                    if(!lastround)
                    {
                        inputImg.setPixel(temp2DD.x(),temp2DD.y(),qRgb(255,255,255));
                    }
                }
                else if(inputImg.pixel( kk.x(),kk.y()+1)==qRgb(0,0,0))
                {
                    temp2DD.setX(kk.x());
                    temp2DD.setY(kk.y()+1);
                    MidData.push_back(temp2DD);
                    if(!lastround)
                    {
                        inputImg.setPixel(temp2DD.x(),temp2DD.y(),qRgb(255,255,255));
                    }
                }
                else if(inputImg.pixel( kk.x()-1,kk.y())==qRgb(0,0,0))
                {
                    temp2DD.setX(kk.x()-1);
                    temp2DD.setY(kk.y());
                    MidData.push_back(temp2DD);
                    if(!lastround)
                    {
                        inputImg.setPixel(temp2DD.x(),temp2DD.y(),qRgb(255,255,255));
                    }
                }
                else if(inputImg.pixel( kk.x(),kk.y()-1)==qRgb(0,0,0))
                {
                    temp2DD.setX(kk.x());
                    temp2DD.setY(kk.y()-1);
                    MidData.push_back(temp2DD);
                    if(!lastround)
                    {
                        inputImg.setPixel(temp2DD.x(),temp2DD.y(),qRgb(255,255,255));
                    }
                }


            }
            OutlinePoints.clear();
             OutlinePoints=MidData;
             MidData.clear();
*/
            }

        }
        else if(distances>0)
        {//向外扩张
            Xstart=Xstart-distances-2;
            Xend=Xend+distances+2;
            Ystart=Ystart-distances-2;
            Yend=Yend+distances+2;
            int count=0;


            for(int i=1;i<=distances;i++){
                MidData.clear();

                for(int x=Xstart;x<Xend;x++)
                {
                    for(int y=Ystart;y<Yend;y++)
                    {
                        count=0;
                        QVector4D type(0,0,0,0);//(左，上，右，下),有值则是1，没值是0
                        QColor inputcolor;
                        inputcolor=inputImg.pixel(x,y);
                        if(inputcolor.red()==0&&inputcolor.blue()==0&&inputcolor.green()==0)
                        {
                            inputcolor=inputImg.pixel(x+1,y);
                            if(inputcolor.red()>200&&inputcolor.blue()>200&&inputcolor.green()>200)
                            {
                                temp2DD.setX(x+1);
                                temp2DD.setY(y);
                                MidData.push_back(temp2DD);
                                count++;
                                type.setZ(1);
                            }
                            inputcolor=inputImg.pixel(x-1,y);
                            if(inputcolor.red()>200&&inputcolor.blue()>200&&inputcolor.green()>200)
                            {
                                temp2DD.setX(x-1);
                                temp2DD.setY(y);
                                MidData.push_back(temp2DD);
                                count++;
                                type.setX(1);

                            }
                            inputcolor=inputImg.pixel(x,y+1);
                            if(inputcolor.red()>200&&inputcolor.blue()>200&&inputcolor.green()>200)
                            {
                                temp2DD.setX(x);
                                temp2DD.setY(y+1);
                                MidData.push_back(temp2DD);
                                count++;
                                type.setY(1);

                            }
                            inputcolor=inputImg.pixel(x,y-1);
                            if(inputcolor.red()>200&&inputcolor.blue()>200&&inputcolor.green()>200)
                            {
                                temp2DD.setX(x);
                                temp2DD.setY(y-1);
                                MidData.push_back(temp2DD);
                                count++;
                                type.setW(1);

                            }

                            if(count>=2)
                            {
                                if(type.x()==1&&type.w()==1)
                                {
                                    temp2DD.setX(x-1);
                                    temp2DD.setY(y-1);
                                    MidData.push_back(temp2DD);
                                }else if(type.x()==1&&type.y()==1)
                                {
                                    temp2DD.setX(x-1);
                                    temp2DD.setY(y+1);
                                    MidData.push_back(temp2DD);
                                }
                                else if(type.y()==1&&type.z()==1)
                                {
                                    temp2DD.setX(x+1);
                                    temp2DD.setY(y+1);
                                    MidData.push_back(temp2DD);
                                }
                                else if(type.z()==1&&type.w()==1)
                                {
                                    temp2DD.setX(x+1);
                                    temp2DD.setY(y-1);
                                    MidData.push_back(temp2DD);
                                }
                                else{

                                }
                            }


                        }

                    }
                }


                for(int t=0;t<MidData.length();t++)
                {
                    inputImg.setPixel(MidData[t].x(),MidData[t].y(),qRgb(0,0,0));
                }
            }
        }
        else
        {
            //return OutlinePoints;
        }

        Toreturn=MidData;

        Toreturn=Unique_2D(Toreturn);

        Output2File(Toreturn,"F:/output/insidertest.txt");



        qDebug()<<"returned from outline Erosion function;";


        ErosionImage=inputImg;


    }
    else if(ui->isOutlineoffset->isChecked()&&!ui->Erosion_checkBox->isChecked())
    {//形态学方法试试

        Mat inputmat=QImage2cvMat(inputImg);

        Mat erosion_dst, dilation_dst;

        int erosion_elem = 0;
        int erosion_size =abs(distances);
        int dilation_elem = 0;
        int dilation_size = abs(distances);



        if(ui->OffsetValueInput->text().toInt()>0)
        {//向外，腐蚀方法
            int erosion_type;
            if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }//矩形
            else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }//椭圆和圆形
            else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }//十字形

            Mat element = getStructuringElement( erosion_type,
                                                 Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                                 Point( erosion_size, erosion_size ) );
            /// 腐蚀操作
            erode( inputmat, erosion_dst, element );
            ErosionImage=cvMat2QImage(erosion_dst);
            imshow( "Erosion Demo", erosion_dst );
            imshow("ori",inputmat);
        }
        else if(ui->OffsetValueInput->text().toInt()<0)//向内，膨胀方法
        {
            int dilation_type;
            if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
            else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
            else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

            Mat element = getStructuringElement( dilation_type,
                                                 Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                 Point( dilation_size, dilation_size ) );
            ///膨胀操作
            dilate( inputmat, dilation_dst, element );
            imshow( "Dilation Demo", dilation_dst );
            imshow("ori",inputmat);
            ErosionImage=cvMat2QImage(dilation_dst);
        }

        //显示原图


        //尺寸调整
        // Mat dstImage1,dstImage2;
        //   cv::resize(inputmat,dstImage1,Size(inputmat.cols/2,inputmat.rows/2),0,0,INTER_LINEAR);
        //    cv::resize(inputmat,dstImage2,Size(inputmat.cols*2,inputmat.rows*2),0,0,INTER_LINEAR);
        //   imshow("dst1",dstImage1);
        //imshow("dst2",dstImage2);
    }

    int Erotimerus=ErosionTimer.elapsed();
    qDebug()<<"EROSION FUNCTION TIME US ::"<<Erotimerus;



    //waitKey(0);

    return Toreturn;

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

    Output2File(curvePoints,"F:/output/curvefit.txt");



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


    //here problem
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


    Output2File(SLines,"F:/output/SLines.txt");

    Output2File(OrderdOutLine,"F:/output/Orderdoutline.txt");

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

/*void MainWindow::on_Outline_Button_clicked()
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


    work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
    excel.dynamicCall("Quit(void)");
    QMessageBox::information(NULL,"Read template","模板导入完毕");

}*/
void MainWindow::ImageDisplayFunciton(QLabel *outputlabel, QImage inputImg, int Iwidth,
                                      int Iheight)
{
    /*QLabel *outputlabel 需要显示的目标标签框
    QImage inputImg  输入的Image
    int width  显示宽度 默认400
    int height  显示高度 默认300


            */
    QImage Todis;

    QPixmap TP;




    Todis=inputImg.scaled(Iwidth,Iheight);

    TP=TP.fromImage(Todis);

    outputlabel->clear();

    outputlabel->setPixmap(TP);

}
QVector<QVector2D>MainWindow::CurveOffset(QVector<QVector2D>input,QVector<int>KeyP, QVector<QPointF>curvefit,int Offset=0)
{
    //input 是输入的排序好的外轮廓，xy形势
    //Offset是偏移距离，负数表示向内偏移，正数表示向外偏移
    QVector<QVector2D>KeyP_2D=TransSequenceTo2D(input,KeyP);
    QVector2D StartP;
    QVector2D EndP;
    for(int i=0;i<KeyP_2D.length()-1;i++)
    {
        StartP=KeyP_2D[i];
        EndP=KeyP_2D[i+1];
        double slope=SingelSlopeCalculate(EndP,StartP);




    }

    int In_Length=input.length();

    const int GroupP_count=5;//多少个点为一组进行考察

    int Dynamic_count=0;

    QVector<QVector2D>Toreturn;

    if(!ui->Erosion_checkBox->isChecked())//没有加入腐蚀才用这个方式
    {

    }

    return Toreturn;
}


void MainWindow::on_ChangeTheimage__currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_DistortionCalibration_button_clicked()
{
    using namespace cv;
    using namespace std;

    bool b_Interrupt=false;

    enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };
    /************************************************************************
               从摄像机中读取多幅图像,从中提取出角点，然后对角点进行亚像素精确化
        *************************************************************************/
    int image_count=  10;                    /****    图像数量     ****/
    Mat frame;
    Mat Tocheck;
    Size image_size;                         /****     图像的尺寸      ****/
    Size board_size = Size(9,6);            /****    定标板上每行、列的角点数       ****/
    vector<Point2f> corners;                  /****    缓存每幅图像上检测到的角点       ****/
    vector<vector<Point2f>>  corners_Seq;    /****  保存检测到的所有角点       ****/
    ofstream fout("F:/output/calibration_result.txt");  /**    保存定标结果的文件     **/
    int mode = DETECTION;
    VideoCapture cap(0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH,camerawidth);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,cameraheight);

    if(!cap.isOpened()){
        std::cout<<"Open Camera Failed !";
        exit(-1);
    }
    namedWindow("Calibration");

    std::cout<<"Press 'g'on your keyboard to start capturing images!,and ESC to quit!"<<endl;

    int count = 0,n=0;
    stringstream tempname;
    string filename;
    int key;
    string msg;
    int baseLine;
    Size textSize;
    while(n < image_count )
    {
        frame.setTo(0);
        cap>>frame;
        if(mode == DETECTION)
        {
            key = 0xff & waitKey(30);
            if( (key & 255) == 27 )
            {
                destroyWindow("Calibration");
                cap.release();
                b_Interrupt=true;
                break;
            }
            if( cap.isOpened() && key == 'g' )
            {
                mode = CAPTURING;
            }
        }

        if(mode == CAPTURING)
        {
            key = 0xff & waitKey(30);
            if( (key & 255) == 32 )
            {
                image_size = frame.size();
                /* 提取角点 */
                Mat imageGray;
                cvtColor(frame, imageGray , CV_RGB2GRAY);
                bool patternfound = findChessboardCorners(frame, board_size, corners,CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK );
                if (patternfound)
                {
                    n++;
                    tempname<<"F:/output/";
                    tempname<<n;
                    tempname>>filename;
                    filename+=".jpg";
                    /* 亚像素精确化 */
                    cornerSubPix(imageGray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
                    count += corners.size();
                    corners_Seq.push_back(corners);

                    imwrite(filename,frame);

                    if(n==3)
                    {
                        Tocheck=frame;
                    }

                    tempname.clear();
                    filename.clear();
                }
                else
                {
                    std::cout<<"Detect Failed.\n";
                }
            }
        }
        msg = mode == CAPTURING ? "100/100/s" : mode == CALIBRATED ? "Calibrated" : "Press 'g' on your keyboard to startand ESC to quit!";
        baseLine = 0;
        textSize = getTextSize(msg, 1, 1, 1, &baseLine);
        Point textOrigin(frame.cols - 2*textSize.width - 10, frame.rows - 2*baseLine - 10);

        if( mode == CAPTURING )
        {
            msg = format( "%d/%d",n,image_count);
        }

        putText( frame, msg, textOrigin, 1, 1,mode != CALIBRATED ? Scalar(0,0,255) : Scalar(0,255,0));

        imshow("Calibration",frame);
        key = 0xff & waitKey(1);
        if( (key & 255) == 27 )
        {
            destroyWindow("Calibration");
            b_Interrupt=true;
            cap.release();
            break;
        }
    }
    if(b_Interrupt){
        std::cout<<"Interrupted\n";
    }else{
        std::cout<<"角点提取完成！\n";
    }
    /************************************************************************
                   摄像机定标
            *************************************************************************/
    if(!b_Interrupt){
        std::cout<<"Start to calibration"<<endl;

        Size square_size = Size(19,19);                                      /**** 实际测量得到的定标板上每个棋盘格的大小   ****/

        vector<vector<Point3f>>  object_Points;                                      /****  保存定标板上角点的三维坐标   ****/

        Mat image_points = Mat(1, count , CV_32FC2, Scalar::all(0));          /*****   保存提取的所有角点   *****/
        vector<int>  point_counts;                                          /*****    每幅图像中角点的数量    ****/

        vector<Mat> rotation_vectors;                                      /* 每幅图像的旋转向量 */
        vector<Mat> translation_vectors;                                  /* 每幅图像的平移向量 */

        /* 初始化定标板上角点的三维坐标 */
        for (int t=0;t<image_count;t++)
        {
            vector<Point3f> tempPointSet;
            for (int i=0;i<board_size.height;i++)
            {
                for (int j=0;j<board_size.width;j++)
                {
                    /* 假设定标板放在世界坐标系中z=0的平面上 */
                    Point3f tempPoint;
                    tempPoint.x = i*square_size.width;
                    tempPoint.y = j*square_size.height;
                    tempPoint.z = 0;
                    tempPointSet.push_back(tempPoint);
                }
            }
            object_Points.push_back(tempPointSet);
        }

        /* 初始化每幅图像中的角点数，这里我们假设每幅图像中都可以看到完整的定标板 */

        for (int i=0; i< image_count; i++)
        {
            point_counts.push_back(board_size.width*board_size.height);
        }

        /* 开始定标 */

        calibrateCamera(object_Points, corners_Seq, image_size,  intrinsic_matrix  ,distortion_coeffs, rotation_vectors, translation_vectors);
        std::cout<<"标定完成！\n";

        /************************************************************************
                   对定标结果进行评价
            *************************************************************************/
        std::cout<<" start evaluate............."<<endl;
        double total_err = 0.0;                   /* 所有图像的平均误差的总和 */
        double err = 0.0;                        /* 每幅图像的平均误差 */
        vector<Point2f>  image_points2;             /****   保存重新计算得到的投影点    ****/

        std::cout<<"每幅图像的定标误差："<<endl;
        fout<<"每幅图像的定标误差："<<endl<<endl;
        for (int i=0;  i<image_count;  i++)
        {
            vector<Point3f> tempPointSet = object_Points[i];
            /****    通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点     ****/
            projectPoints(tempPointSet, rotation_vectors[i], translation_vectors[i], intrinsic_matrix, distortion_coeffs, image_points2);
            /* 计算新的投影点和旧的投影点之间的误差*/
            vector<Point2f> tempImagePoint = corners_Seq[i];
            Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
            Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
            for (int j = 0 ; j < tempImagePoint.size(); j++)
            {
                image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
                tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
            }
            err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
            total_err += err/=  point_counts[i];
            std::cout<<"the  "<<i+1<<" image 's average Deviation :"<<err<<"  pixel"<<endl;
            fout<<"the"<<i+1<<"image 's average Deviation:"<<err<<"  pixel"<<endl;
        }
        std::cout<<"Average deviation in all :"<<total_err/image_count<<"pixel"<<endl;
        fout<<"Average deviation in all :"<<total_err/image_count<<"pixel"<<endl<<endl;
        std::cout<<"Evaluate Done!"<<endl;

        /************************************************************************
                   Save the Results
            *************************************************************************/
        std::cout<<"save the results.........."<<endl;
        Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */

        fout<<"Camera parameter matrix:  "<<endl;
        fout<<intrinsic_matrix<<endl<<endl;
        fout<<"distortion factor：\n";
        fout<<distortion_coeffs<<endl<<endl<<endl;
        for (int i=0; i<image_count; i++)
        {
            fout<<"the "<<i+1<<"image's rotate vector: "<<endl;
            fout<<rotation_vectors[i]<<endl;

            /* 将旋转向量转换为相对应的旋转矩阵 */
            Rodrigues(rotation_vectors[i],rotation_matrix);
            fout<<"the "<<i+1<<"image's rotate matrix"<<endl;
            fout<<rotation_matrix<<endl;
            fout<<"the "<<i+1<<"image's move vector "<<endl;
            fout<<translation_vectors[i]<<endl<<endl;
        }
        std::cout<<"Save is done"<<endl;
        m_bCalibration=true;
        fout<<endl;
        destroyWindow("Calibration");
        cap.release();


        /*******************************************/
        Mat newimage;

        Mat R = Mat::eye(3,3,CV_32F);
        initUndistortRectifyMap(intrinsic_matrix,distortion_coeffs,intrinsic_matrix ,R,image_size,CV_32FC1,xmat,ymat);
        remap(Tocheck,newimage,xmat, ymat, INTER_LINEAR);

        Mat clipcheck=CenterClipping(Tocheck);


        //计算像素和实际尺寸的关系
        float corMinX,corMinY;
        float corMindis=11120.11;
        float PixtoM[10];
        int tempcont=0;
        int pictucont=0;

        foreach (vector<Point2f>ff, corners_Seq) {

            foreach (Point2f xy2f, ff)
            {
                if(tempcont==0)
                {
                    corMinX=xy2f.x;
                    corMinY=xy2f.y;

                }
                else
                {
                    float disp=TwoPointdis(corMinX,corMinY,xy2f.x,xy2f.y);
                    if(corMindis>disp)
                    {
                        corMindis=disp;

                    }
                }
                tempcont++;

            }
            PixtoM[pictucont]=corMindis;
            pictucont++;
            tempcont=0;
        }
        float sumf;
        std::cout<<"pixto m"<<PixtoM;
        for(int kkk=0;kkk<10;kkk++)
        {
            float aver=PixtoM[kkk];
            sumf+=aver;
        }
        sumf=sumf/corners_Seq.size();
        qDebug()<<"sumf   "<<sumf;
        PixeltoMeter=19/sumf;
        qDebug()<<"pixel size is"<<PixeltoMeter;
        ui->Pixeltomm_Label->setText(QString::number(PixeltoMeter));

        imshow("origin image",Tocheck);
        imshow("correctec image",newimage);
        imshow("cliped",clipcheck);

        /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
        /************************************************************************
                   显示定标结果
            *************************************************************************/
        // 	Mat mapx = Mat(image_size,CV_32FC1);
        // 	Mat mapy = Mat(image_size,CV_32FC1);
        // 	Mat R = Mat::eye(3,3,CV_32F);
        // 	std::cout<<"save the corrected image"<<endl;
        // 	string imageFileName;
        // 	std::stringstream StrStm;
        // 	for (int i = 0 ; i != image_count ; i++)
        // 	{
        // 		std::cout<<"Frame #"<<i+1<<"..."<<endl;
        // 		Mat newCameraMatrix = Mat(3,3,CV_32FC1,Scalar::all(0));
        // 		initUndistortRectifyMap(intrinsic_matrix,distortion_coeffs,R,intrinsic_matrix,image_size,CV_32FC1,mapx,mapy);
        // 		StrStm.clear();
        // 		imageFileName.clear();
        // 		StrStm<<i+1;
        // 		StrStm>>imageFileName;
        // 		imageFileName += ".jpg";
        // 		Mat t = imread(imageFileName);
        // 		Mat newimage = t.clone();
        // 		cv::remap(t,newimage,mapx, mapy, INTER_LINEAR);
        // 		StrStm.clear();
        // 		imageFileName.clear();
        // 		StrStm<<i+1;
        // 		StrStm>>imageFileName;
        // 		imageFileName += "_d.jpg";
        // 		imwrite(imageFileName,newimage);
        // 	}
        // 	std::cout<<"save is done"<<endl;
    }
}

void MainWindow::on_GoToOriginSpot_button_clicked()
{
    if(m_bSerialIsOpen)
    {


        serial->write(orispot.toLatin1());
        ui->AutoSend_Button->setEnabled(false);
        ui->GetTheWorldCoordinate_button->setEnabled(false);
        ui->SendMesg_Button->setEnabled(false);
        ui->openCamera->setEnabled(false);
        ui->ReadPicture->setEnabled(false);
        ui->Read_txt_Button->setEnabled(false);
        ui->CameraView_Button->setEnabled(false);
        ui->Canny_button->setEnabled(false);

        ui->ChangeTheimage_->setEnabled(false);
        ui->replace_pushButton->setEnabled(false);
        ui->DistortionCalibration_button->setEnabled(false);
        ui->CheckCode_Button->setEnabled(false);
        ui->ImageWatch_pushButton->setEnabled(false);
        m_bOrisend=true;

    }
    else
    {
        QMessageBox::warning(NULL,"warning","Serial port is not open , please check!");
    }
}

void MainWindow::on_Xplus_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        double NextValue=CurrentSpot.x()+ui->XYZ_spinBox->text().toDouble();
        QString sendCode="G00 X="+QString::number(NextValue)+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);


        CurrentSpot.setX(NextValue);
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Yplus_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        double NextValue=CurrentSpot.y()+ui->XYZ_spinBox->text().toDouble();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(NextValue)+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Zplus_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        double NextValue=CurrentSpot.z()+ui->XYZ_spinBox->text().toDouble();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(NextValue)+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Xmini_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        double NextValue=CurrentSpot.x()-ui->XYZ_spinBox->text().toDouble();
        QString sendCode="G00 X="+QString::number(NextValue)+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Ymini_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        double NextValue=CurrentSpot.y()-ui->XYZ_spinBox->text().toDouble();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(NextValue)+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Zmini_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        double NextValue=CurrentSpot.z()-ui->XYZ_spinBox->text().toDouble();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(NextValue)+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Aplus_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        float NextValue=CurrentAngel[0]+ui->ABC_doubleSpinBox->text().toFloat();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(NextValue)+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Bplus_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        float NextValue=CurrentAngel[1]+ui->ABC_doubleSpinBox->text().toFloat();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(NextValue)+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Cplus_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        float NextValue=CurrentAngel[2]+ui->ABC_doubleSpinBox->text().toFloat();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(NextValue);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}

void MainWindow::on_Amini_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        float NextValue=CurrentAngel[0]-ui->ABC_doubleSpinBox->text().toFloat();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(NextValue)+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}
void MainWindow::on_Bmini_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        float NextValue=CurrentAngel[1]-ui->ABC_doubleSpinBox->text().toFloat();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(NextValue)+" C="+QString::number(CurrentAngel[2]);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}
void MainWindow::on_Cmini_Button_clicked()
{
    if(m_bCurrentGoted&&m_bSerialIsOpen)//确保已经有现在的坐标和串口已经打开
    {
        float NextValue=CurrentAngel[2]-ui->ABC_doubleSpinBox->text().toFloat();
        QString sendCode="G00 X="+QString::number(CurrentSpot.x())+" Y="+QString::number(CurrentSpot.y())+" Z="+
                QString::number(CurrentSpot.z())+" A="+QString::number(CurrentAngel[0])+" B="+
                QString::number(CurrentAngel[1])+" C="+QString::number(NextValue);

        sendCode.append("\n");
        qDebug()<<sendCode;
        // serial->write(ui->SendEditor->toPlainText().toLatin1());
        serial->write(sendCode.toLatin1());
        m_bFromMinitrim=true;

        ui->Xplus_Button->setEnabled(false);
        ui->Yplus_Button->setEnabled(false);
        ui->Zplus_Button->setEnabled(false);
        ui->Aplus_Button->setEnabled(false);
        ui->Bplus_Button->setEnabled(false);
        ui->Cplus_Button->setEnabled(false);
        ui->Xmini_Button->setEnabled(false);
        ui->Ymini_Button->setEnabled(false);
        ui->Zmini_Button->setEnabled(false);
        ui->Amini_Button->setEnabled(false);
        ui->Bmini_Button->setEnabled(false);
        ui->Cmini_Button->setEnabled(false);

    }
}
void MainWindow::CreatReport(QString add)
{
    if(add.isEmpty())
    {
        //   QMessageBox::information(NULL,"warning","no out put file address!");
        add="F:/output/ReportFile_RCS/Report.txt";
    }
    QFile *outflie=new QFile;
    outflie->setFileName(add);
    bool ok=outflie->open(QIODevice::Text|QIODevice::WriteOnly);//加入QIODevice：：Text可以换行
    if(ok)
    {
        int count=0;
        QTextStream out(outflie);

        out<<"Robot Control System Report File";
        out<<"\n";
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        out<<"\n";
        current_date.push_front("生成文件时间     ");
        out<<current_date;
        out<<"\n";
        QString outstr;
        outstr.push_back("文件位置     ");
        outstr.push_back(readfileadd);

        out<<outstr;
        out<<"\n";
        outstr.clear();
        out<<"离散距离    "+QString::number(minmumDcres);
        out<<"\n";
        out<<"外轮廓总点数   "+QString::number(OrderdOutLine.length());
        out<<"\n";
        out<<"直线数量   "+QString::number(LineCount);
        out<<"\n";
        out<<"特征点数量   "+QString::number(CharacteristicPoint.length());
        out<<"\n";
        out<<"采样点数   "+QString::number(samplingcount);
        out<<"\n";
        out<<"采样间距   "+QString::number(samplingGap);
        QImage tosaveimage=ImageDrawer(origin_image,CharacteristicPoint,QColor(255,0,0),6);
        QimageSave(OulineImage_b,"ReportFile_RCS/outlineb");
        QimageSave(origin_image,"ReportFile_RCS/Originimage");
        QimageSave(origin_image,"ReportFile_RCS/Originimage_withC");
        outflie->close();
        delete outflie;
    }

}

void MainWindow::on_Sample_Pic_clicked()
{
    int tolerance=20;//色度变化公差
    if(VCcam.isOpened()&&ui->CameraView_Button->text()=="关闭相机"&&BackMat.cols<50)
    {

        cv:: Mat cvframe;

        VCcam>>cvframe;

        Mat mid=CenterClipping(cvframe);

        BackMat=mid.clone();
        qDebug()<<"done";
        VCcam.release();
        CameraPreView();
    }
    else if(VCcam.isOpened()&&ui->CameraView_Button->text()=="关闭相机"
            &&BackMat.cols>50&&TosetcoorMat.cols<50)
    {
        cv:: Mat cvframe;

        VCcam>>cvframe;

        Mat mid=CenterClipping(cvframe);

        TosetcoorMat=mid.clone();
        qDebug()<<"done";
        VCcam.release();
        Mat temp;
        cvtColor(BackMat,temp,CV_RGB2GRAY);
        BackMat.release();
        BackMat=temp.clone();
        temp.release();

        cvtColor(TosetcoorMat,temp,CV_RGB2GRAY);
        TosetcoorMat.release();
        TosetcoorMat=temp.clone();
        temp.release();
        QVector<QVector2D>blackPoint;
        for(int i=0;i<BackMat.cols;i++)
        {
            for(int j=0;j<BackMat.cols;j++)
            {
                if(abs(BackMat.at<Vec3b>(i, j)[0]-TosetcoorMat.at<Vec3b>(i, j)[0])>tolerance)
                {
                    blackPoint.push_back(QVector2D(i,j));
                }
            }

        }
        //分析blackpoint数据



        CameraPreView();
    }
}
