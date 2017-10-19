#ifndef IMAGEWATCH_H
#define IMAGEWATCH_H

#include <QWidget>
#include<QVector>
#include <opencv2/opencv.hpp>
#include<QPixmap>
#include"funcitons.h"
namespace Ui {
class ImageWatch;
}

class ImageWatch : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWatch(QVector<QImage> xx,QWidget *parent = 0);
    ~ImageWatch();

private slots:
    void on_image_comboBox_currentIndexChanged(int index);

    void on_size_horizontalSlider_valueChanged(int value);

    void on_size_horizontalSlider_sliderReleased();

private:
    Ui::ImageWatch *ui;

    QVector<QImage> Ima;

    cv::Mat Ima_Mat;

    QImage Image;

    int m_CurrentIndex;

    double m_CurrentScale;

    QPixmap pixmap;

    int ImageCount=0;
};

#endif // IMAGEWATCH_H
