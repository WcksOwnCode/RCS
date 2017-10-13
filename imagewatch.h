#ifndef IMAGEWATCH_H
#define IMAGEWATCH_H

#include <QWidget>
#include<QVector>

#include<QPixmap>

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

private:
    Ui::ImageWatch *ui;
    QVector<QImage> Ima;
    QImage Image;
    QPixmap pixmap;
    int ImageCount=0;
};

#endif // IMAGEWATCH_H
