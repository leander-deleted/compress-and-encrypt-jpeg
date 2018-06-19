#include "widget.h"
#include "ui_widget.h"
#include "simpleBMP.h"

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
    /*
    QString i_fileName = QFileDialog::getOpenFileName(this,"选择文件",
                                                    "C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex",
                                                 "Images (*.bmp *.jpeg)");
    qDebug()<<"open file name"<<i_fileName;
    fileName = i_fileName.toStdString();
    */
    //C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\2.bmp
    fileName_t = "C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\2.bmp";
    compressImg();
}


void Widget::compressImg(){
    if(img.LoadImage(fileName_t)){
        qDebug()<<"load success";
        //img.cp.show(img.imgData,2);
        qDebug()<<"width is :"<<img.bmpInfoHeaderData.biWidth;
        //bool convertToBlock(vector<uint8_t> data, int width,int height);
        img.cp.convertToBlock(img.y_ele_img,img.bmpInfoHeaderData.biWidth,img.bmpInfoHeaderData.biHeight);
        qDebug()<<"convert sucess";
        //img.cp.show();
        img.cp.dctTransform();
        qDebug()<<"dct tranform success";
        //img.cp.show();
        img.cp.quantization();
        //qDebug()<<"quantization sucess";
        //img.cp.show();
    }
    else{
        qDebug()<<"load file error";
    }
    if(img.SaveImage("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\savedImage.bmp")){
        qDebug()<<"save success";
    }
}

//compress
void Widget::on_pushButton_2_clicked()
{
    compressImg();
}
