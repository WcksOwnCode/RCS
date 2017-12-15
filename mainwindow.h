#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/*************************************/
//From APOtoCommunicate

#include <QFile>
#include <fstream>
#include <vector>
#include <QtMultimedia/QCamera>
#include <QtMultimediaWidgets/QCameraViewfinder>
#include <QtMultimedia/QCameraImageCapture>
#include <QFileDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <Qtimer>
#include <QIODevice>
#include <QDebug>
#include <QFile>
#include <QDialog>
#include <QMessageBox>
#include <iostream>
#include <string.h>
#include <QString>
#include <QAxObject>
#include <vector>
#include <QDateTime>
/*************************************/
#include <QPicture>
#include <QImage>
#include <QPixmap>
#include <QVector4D>
#include <QVector2D>
#include <QTime>
#include<QImageReader>
#include<time.h>
#include<ctime>
#include<QVector>
#include<QVector3D>
#include<QTime>
#include<cmath>
#include<QTimer>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include<QPixmap>
#include <opencv2/opencv.hpp>
#include <curvefit.h>
#include "funcitons.h"
#include "matrix_4.h"
#include"imagewatch.h"

using namespace std;
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetCoordinate(double x,double y,double z);

    bool RecCheck(QString str);

    QVector<double>Vd_slope;//用于slope函数返回

    void DomainCalcu(QImage x);

    void ConnectDomains();

    void ToGray();

    void ToTwoColor();

    void SmoothOutline();

    void ClearVector();

    void ImageCoor2RealCoor();

    void EmptyFunction();

    void TransferToWorldCoordinate();

    void DynamicEncoding(QVector <QVector2D> DE);

    void CharacteristicCalculate(QVector<int> CC);

    void DeleteOutlineNoise();

    void ReOrderOutline(QVector <QVector2D> RO);

    QVector<QVector2D> ReOrderOutline_8Neighboor(QVector <QVector2D> inputarray,QImage inputIMG);

    void ErrorFunction();

    void ImageDisplayFunciton(QLabel *outputlabel, QImage inputImg, int Iwidth=400,
                              int Iheight=300);

    void CurveFit(QVector <QVector2D> Curve);

    QImage ImageDrawer(QImage Img, int Broad=1);

    QImage ImageDrawer(QImage Img, QVector<QVector2D> Array, QColor col, int Broad=1);

    QVector<QVector2D>CurveOffset(QVector<QVector2D>input,QVector<QPointF> fitcurve,int Offset);

    QImage GaussianBlur(QImage GB);

    QImage GetOutLine(QImage II);

    QImage NoiseFilter(QImage x);

    void ImageInitialize();

    QVector<QVector2D>OutlineErosion(QImage inputImg,QVector<QVector2D> OutlinePoints,QVector <QVector4D> V4area,int distances);

    Mat CenterClipping(Mat inputarray);

    void CreatReport(QString add);

private:
    Ui::MainWindow *ui;

    QSerialPort *serial;//串口

    QString Todisplay;

    QString RootAddress="F:\output";

    QVector<QString>Array;//gcode向量

    QVector<QVector2D>OrderedSline;

    QString FullOrder;

    const int minmumDcres=30;//曲线离散后最短直线

    const int minmumLine=15;//霍夫变换最后参数
    int rows;

    int columns;

    int row_start;

    int column_start;

    int row_count;

    int column_count;

    int CoorCount;//记录有多少个坐标

    int* BinCode=new int[8*5000];

    int SpeedValue;//速度值

    int CodeLength[500];//记录命令转成ascii时候的每条长度

    int m_iSendCount;

    int m_iSendRepeatedlyCount;

    bool m_bOrisend=false;//去原点的发送函数

    bool m_bCalibration=false;//是否标定完成

    bool m_bReadState;

    bool m_bSerialIsOpen;//判断串口是否打开

    bool m_bStrCompare;

    bool m_bFirstsend;

    bool m_bWorldCheck=false;//获取现在坐标的bool

    bool m_bCurrentGoted=false;

    bool m_bOnlyStraitLine=true;//轮廓边界是否只看做直线

    bool m_bFromMinitrim=false;

    double Coordinate[3][5000];//储存坐标值x y z最多5000组

    double Discoor[15000];//展示坐标用

    double m_dXbase,m_dYbase,m_dZbase;

    double m_dCurrentX,m_dCurrentY,m_dCurrentZ;

    double m_dBoltSpeed;

    double m_dXspeed=0;

    double m_dYspeed=0;

    double m_dZspeed=0;

    double m_dPoint2PointTime=0.5;//500ms点到点移动

    char binarycode[8][500];//保存Ascii的代码转成二进制

    int count;

    int all;



    const int m_iTotwoValue=120;//转二值图的时候的阈值

    const int width=800;//设定图片长宽

    const int height=600;

    const int cameraheight=768;

    const int camerawidth=1024;

    int tempus=-1;

    int m_iSampleSizes=15;//每十五个点作为样本进行对比

    const int m_iLinePointCoint=30;

    bool m_bOnlyOutline=true;//确定是否只检测最外部轮廓

    bool m_bFrompreView=false;

    bool m_bCareraAutoRun=false;

    double DomainKeep;

    QVector4D tempvec;//临时4维向量

    QVector <QVector4D> V4Domain;

    QVector <QVector4D> V4Domain_new;

    QVector <QVector4D> V4Domain_max;//keep the max area domain

    QVector <QVector4D> V4Domain_main;//保留主要连通域

    QVector <QVector4D> V4Domain_white;//反色连通域

    QVector<QVector3D> LineDomain;//记录每行连通域的编号的起编号和止编号

    QVector<QVector3D> WhiteLineDomain;//记录白色每行连通域的编号的起编号和止编号

    QVector <QVector2D> OutLine;

    QVector <QVector2D> OutLine_copy;

    QVector <QVector2D> OnlyOutLine;

    QVector <QVector2D> Outline_template;

    QVector <QVector2D> OnlyOutLine_count;

    QVector <QVector2D> OrderdOutLine;

    QVector <QVector2D>Orioutline;//偏移如果选中，则这个保存原始边界

    QVector <QVector2D> CharacteristicPoint;

    QVector <QVector2D> Segmentboundary;

    QVector <QVector3D> GridPoints;

    QVector <int> BreakPoints;

    QVector <QVector3D> Derection;//(序号；X差；Y差)

    QVector <QVector2D> num_Derec;//(序号；X差；Y差)

    QVector <QVector2D> StraitLine;

    QVector <int> StraitLineCount;

    QVector<int> BreakPointDistance;

    QVector<QVector2D>Linetype;

    QVector3D CurrentSpot;

    QVector<float> CurrentAngel;

    QImage origin_image;//原图

    QImage Timage;//二色图

    QImage grayImage;//灰度图

    QImage spaceImage;//空白图

    QImage OulineImage;

    QImage ErosionImage;

    QImage OulineImage_b;

    QImage SmoothOulineImage;

    QImage GridImage;

    QImage xx;

    QTime WhoseTime;

    QTime DCtimer;

    QTime NFtimer;

    QTime AutoRunTimer;

    QPixmap pp;

    QColor tempcolor;

    QTimer  *timer;

    QTimer *WTimer;

    QTimer *GetpicTimer;

    QImage  *imag;

    CvCapture *cam;// 视频获取结构， 用来作为视频获取函数的一个参数

    cv::VideoCapture VCcam;

    IplImage  *frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像

    QString SpaceimageADD="E:/softwaresFiles/QT/RobotCotrolSystem/SpaceImage.png";

   QString orispot="G00 X=800 Y=0 Z=400 A=3.14 B=0 C=0";

    QString readfileadd;

    const int Max_C_num=500;//最大特征点数量，过多跳出

    const int currentK=3;

    int currentNode;

    const double FitStep=0.1;

    QVector<QPointF> ctrlPoints;  // 控制点

    QVector<QPointF> curvePoints; // 曲线上的点

    int disperse=2;

    QVector<QImage> AllImage;

    Mat intrinsic_matrix = Mat(3,3, CV_32FC1, Scalar::all(0));                /*****    摄像机内参数矩阵    ****/

    Mat distortion_coeffs = Mat(1,5, CV_32FC1, Scalar::all(0));            /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */

    Mat xmat,ymat;

    QString CurrentReturn;

    int Currentcont=0;

    float PixeltoMeter;

private slots:

    void readmycom();

    void SendMessgOut();

    void ReadtxtButton();

    void CreadOrders();

    void CreatGcodefile();

    void OpenSerialClicked();

    void on_ExitButton_clicked();

    void AutoSendClicked();

    void BoltSpeedCheckBox_checked();

    void TakingPhoto();

    void ReadPngButton();

    void AutoRun();

    void readFarme();

    void CameraPreView();

    void on_openCamera_clicked();

    void on_GetTheWorldCoordinate_button_clicked();

    void on_Canny_button_clicked();

    void on_ChangeTheimage__currentIndexChanged(int index);

    void on_DisperseSlider_valueChanged(int value);

    void on_CheckCode_Button_clicked();

    void on_Hough_Button_clicked();

    void on_replace_pushButton_clicked();

    void on_ImageWatch_pushButton_clicked();

    void on_ChangeTheimage__currentIndexChanged(const QString &arg1);

    QImage DeleteOutRectangel(QImage input);

    void on_DistortionCalibration_button_clicked();

    void on_GoToOriginSpot_button_clicked();

    void on_Xplus_Button_clicked();

    void on_Yplus_Button_clicked();

    void on_Zplus_Button_clicked();

    void on_Xmini_Button_clicked();

    void on_Ymini_Button_clicked();

    void on_Zmini_Button_clicked();

    void on_Aplus_Button_clicked();

    void on_Bplus_Button_clicked();

    void on_Cplus_Button_clicked();

    void on_Amini_Button_clicked();

    void on_Bmini_Button_clicked();

    void on_Cmini_Button_clicked();

signals:
    void Receveid();
    void MoveFinished();

};

#endif // MAINWINDOW_H
