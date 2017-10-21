#include "imagewatch.h"
#include "ui_imagewatch.h"
#include<QImage>
#include<QMessageBox>
using namespace cv;
ImageWatch::ImageWatch(QVector<QImage> xx, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWatch)
{
    ui->setupUi(this);

    Ima=xx;  

    m_CurrentIndex=0;

    m_CurrentScale=10;
    if(Ima.length()!=0){

        ImageCount=Ima.length();

        for(int i=0;i<Ima.length();i++)
        {
             ui->image_comboBox->addItem(QWidget::tr("Image")+QString::number(i+1));
        }

        pixmap=QPixmap::fromImage(Ima[0]);

        ui->Image_Label->setPixmap(pixmap);

        ui->image_comboBox->setCurrentIndex(0);

    }



}

ImageWatch::~ImageWatch()
{
    delete ui;
}

void ImageWatch::on_image_comboBox_currentIndexChanged(int index)
{
//the window showd and then this function be called

    double scale=m_CurrentScale;

    Mat SrcImg=QImage2cvMat(Ima[index]);


    cv::Size ResImgSiz = Size(SrcImg.cols*scale, SrcImg.rows*scale);

    Mat ResImg = Mat(ResImgSiz, SrcImg.type());


    cv::resize(SrcImg, ResImg, ResImgSiz, CV_INTER_CUBIC);

    QImage xxx;

    xxx=cvMat2QImage(ResImg);

    pixmap=QPixmap::fromImage(xxx);

    ui->Image_Label->setPixmap(pixmap);

    m_CurrentIndex=index;
}

void ImageWatch::on_size_horizontalSlider_valueChanged(int value)
{

}

void ImageWatch::on_size_horizontalSlider_sliderReleased()
{

    double scale=(double)(ui->size_horizontalSlider->value())/10;

    qDebug()<<ui->size_horizontalSlider->value()<<"value of the slider!";

    qDebug()<<scale<<"value of the scale!";
    Mat SrcImg=QImage2cvMat(Ima[m_CurrentIndex]);


    cv::Size ResImgSiz = Size(SrcImg.cols*scale, SrcImg.rows*scale);

    Mat ResImg = Mat(ResImgSiz, SrcImg.type());


    cv::resize(SrcImg, ResImg, ResImgSiz, CV_INTER_CUBIC);

    QImage xxx;
    xxx=cvMat2QImage(ResImg);
    pixmap=QPixmap::fromImage(xxx);
    ui->Image_Label->setPixmap(pixmap);
    m_CurrentScale=scale;

}
