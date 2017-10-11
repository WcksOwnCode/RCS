#ifndef CODEWINDOW_H
#define CODEWINDOW_H

#include <QDialog>

namespace Ui {
class CodeWindow;
}

class CodeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CodeWindow(QString co,QWidget *parent = 0);

    ~CodeWindow();
    QString code;
    void ShowCode();

private:
    Ui::CodeWindow *ui;
};

#endif // CODEWINDOW_H
