#include "widget.h"
#include "ui_widget.h"
#include "simpleBMP.h"
#include "jpeg_encoder.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//select file
void Widget::on_pushButton_clicked()
{
    
    QString i_fileName = QFileDialog::getOpenFileName(this,"选择文件",
                                                    "C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex",
                                                 "Images (*.bmp *.jpeg)");
    //QString t = "test";
    fileName = i_fileName;
    //f = t.toStdString().c_str();
    //qDebug()<<"*f"<<f[0]<<f[1];
}

//compress
void Widget::on_pushButton_2_clicked()
{
    JpegEncoder encoder;
    //qDebug()<<"file name"<<fileName;
    //qDebug()<<"*f"<<f[0]<<f[1];
    QString i_outputf = ui->lineEdit->text();
    QString degree = ui->lineEdit_2->text();
    QString outputf = QString("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\")+i_outputf+QString(".jpeg");
    qDebug()<<"outputf: "<<outputf<<"degree"<<degree.toInt();
    if(encoder.readFromBMP(fileName.toStdString().c_str())){
        qDebug()<<"finish loading bmp";
    }
    else{
        qDebug()<<"fail to load";
    }
    if(encoder.encodeToJPG(outputf.toStdString().c_str(),degree.toInt())){
     qDebug()<<"finish compression";       
    }
    else{
        qDebug()<<"fail to compress";
    }
}
