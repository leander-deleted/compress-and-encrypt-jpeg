/*
    //select file
    QPushButton *pushButton;
    //compress
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QFrame *line;
    QFrame *line_2;
    //output file name
    QLineEdit *lineEdit;
    //degree
    QLineEdit *lineEdit_2;
*/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QFileDialog>
#include <QDebug>
#include "simpleBMP.h"
#include <string>
#include "encryption.h"
#include <QMessageBox>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void compressImg();

private slots:
    void on_pushButton_clicked();

    //compress
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
private:
    Ui::Widget *ui;
    ClImgBMP img;
    const char * f;
    QString fileName;
    Encrpytor e;
};

#endif // WIDGET_H
