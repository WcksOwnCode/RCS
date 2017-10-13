#include "imagewatch.h"
#include "ui_imagewatch.h"
#include<QImage>
#include<QMessageBox>
ImageWatch::ImageWatch(QVector<QImage> xx, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWatch)
{
    ui->setupUi(this);
    Ima=xx;

    QString name="Image";
    if(xx.length()!=0){
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
    pixmap=QPixmap::fromImage(Ima[index]);
    ui->Image_Label->setPixmap(pixmap);
}

void ImageWatch::on_size_horizontalSlider_valueChanged(int value)
{
    ui->graphicsView->setScene();
}
