#include "codewindow.h"
#include "ui_codewindow.h"

CodeWindow::CodeWindow(QString co,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodeWindow)
{
    ui->setupUi(this);
    code=co;
    ShowCode();

}


CodeWindow::~CodeWindow()
{
    delete ui;
}
 void CodeWindow:: ShowCode()
 {
    ui->Code_textBrowser->setText(code);

 }
