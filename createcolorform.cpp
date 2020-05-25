#include "createcolorform.h"
#include "ui_createcolorform.h"
#include "mainwindow.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageReader>

CreateColorForm::CreateColorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateColorForm)
{
    ui->setupUi(this);
    width = ui->spinBox_width->text().toInt();
    height = ui->spinBox_height->text().toInt();
    frameCount = ui->spinBox_frameNum->text().toInt();
    frameSize = width*height*2;
    comboBox_Format_IndexChange(ui->comboBox_Format->currentText());
    frameBuff = NULL;

    fileName = QDir::currentPath()+"/1"+getExternalFileFormat(ui->comboBox_Format->currentText());
    ui->lineEdit_folderName->setText(QDir::currentPath());
    ui->lineEdit_fileName->setText("1"+getExternalFileFormat(ui->comboBox_Format->currentText()));
}

CreateColorForm::~CreateColorForm()
{
    delete ui;
}

void CreateColorForm::createYuvFile()
{
    ui->textEdit_showMess->clear();
    ui->statusBar->setText("CreateColorForm: createYuvFile");
    ui->textEdit_showMess->append(QString("frameSize:")+QString::number(frameSize,10));
    ui->textEdit_showMess->append(QString("width:")+QString::number(width,10));
    ui->textEdit_showMess->append(QString("height:")+QString::number(height,10));
    ui->textEdit_showMess->append(QString("fileName:")+fileName);
    ui->textEdit_showMess->append(QString("frameCount:")+QString::number(frameCount,10));
    ui->textEdit_showMess->append(QString("fmt:")+fmtToQString(fmt));
    createColor();
    ui->statusBar->setText("CreateColorForm: createYuvFile Ok");
}


void CreateColorForm::frame_render(int frameCount)
{
    int colorIndex = 0;
    int frame = 0;
    int ret;
    char *disY = NULL;
    char *disUV = NULL;
    int tw = 0;
    while(frame < frameCount)
    {
        qDebug() << "frame_render frame:" << frame;
        switch( fmt )
        {
            case FMT_NV12:
                disY = frameBuff;
                disUV = frameBuff+width*height;
                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < width; j++)
                    {
                        disY[i*width+j] = (j+colorIndex)%255;
                    }
                }

                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < width/4; j++)
                    {
                        disUV[(i*width/4+j)*2+0] = 90;
                        disUV[(i*width/4+j)*2+1] = 90;
                    }
                }
                colorIndex++;
            break;
            case FMT_YUYV:
                tw = width/2;
                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < width; j++)
                    {
                        frameBuff[(i*width+j)*2] = (j+colorIndex)%255;
                    }
                }

                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < tw; j++)
                    {
                        frameBuff[(i*tw+j)*4+1] = 90;
                        frameBuff[(i*tw+j)*4+3] = 90;
                    }
                }
                colorIndex++;
            break;
            case FMT_UYVY:
                tw = width/2;
                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < width; j++)
                    {
                        frameBuff[(i*width+j)*2+1] = (j+colorIndex)%255;
                    }
                }
                for(int i = 0; i < height; i++)
                {
                    for(int j = 0; j < tw; j++)
                    {
                        frameBuff[(i*tw+j)*4+0] = 90;
                        frameBuff[(i*tw+j)*4+2] = 90;
                    }
                }
                colorIndex++;
            break;
        }
        //frameBuff
        ret = write(fb, frameBuff, frameSize);
        qDebug() << "frame_render write ret = " << ret;
        frame++;
    }
}


void CreateColorForm::comboBox_Format_IndexChange(QString str)
{
    if( ui->comboBox_Format->currentText() == "FMT_NV12" )
    {
        fmt = FMT_NV12;
    }
    else if( ui->comboBox_Format->currentText() == "FMT_UYVY" )
    {
        fmt = FMT_UYVY;
    }
    else if( ui->comboBox_Format->currentText() == "FMT_YUYV" )
    {
        fmt = FMT_YUYV;
    }
    else
    {
        fmt = FMT_YUYV;
        ui->comboBox_Format->setCurrentText("FMT_YUYV");
    }

    ui->lineEdit_fileName->setText("1"+getExternalFileFormat(ui->comboBox_Format->currentText()));
}


QString CreateColorForm::getExternalFileFormat(QString fmt)
{
    QString str;
    if( fmt == "FMT_NV12" )
    {
        str = ".nv12";
    }
    else if( fmt == "FMT_UYVY" )
    {
        str = ".uyvy";
    }
    else if( fmt == "FMT_YUYV" )
    {
        str = ".yuyv";
    }
    else
    {
        str = ".yuyv";
    }
    return str;
}

/**
 * @brief CreateColorForm::fmtToQString
 *  将fmt 转换成 字符串
 * @param fmt : 格式的数字
 * @return 格式字符串
 */
QString CreateColorForm::fmtToQString(int fmt)
{
    QString ret;
    switch (fmt) {
        case FMT_NV12:
            ret = "FMT_NV12";
        break;
        case FMT_UYVY:
             ret = "FMT_UYVY";
        break;
        case FMT_YUYV:
             ret = "FMT_YUYV";
        break;
        default:
             ret = "No Support Fmt";
    }
    return ret;
}


/**
 * @brief CreateColorForm::createColor
 * 创建颜色文件
 * @return 0:Ok -1:Err
 */
int CreateColorForm::createColor()
{
    frameBuff = (char*)malloc(frameSize);
    if( frameBuff == NULL )
    {
        printf("frameBuff == NULL , malloc Err\n");
    }

    qDebug() << "fileName:" << fileName;
    fb = ::open(fileName.toStdString().c_str(), O_CREAT|O_RDWR, 0777);
    if( fb < 0 )
    {
        qDebug() << "fileName:" << fileName << "open err";
    }
    else
    {
        qDebug() << "fileName:" << fileName << "open ok";
    }

    frame_render(frameCount);

    ::close(fb);
    qDebug() << "fileName:" << fileName << "close ok";
    free(frameBuff);
    qDebug() << "createColor run end ok\n";
    return 0;
}


void CreateColorForm::spinBox_frameNum_OnClick(int val)
{
    frameCount = val;
    frameSize = width*height*2;
}


void CreateColorForm::spinBox_width_OnClick(int val)
{
    width = val;
    frameSize = width*height*2;
}


void CreateColorForm::spinBox_height_OnClick(int val)
{
    height = val;
    frameSize = width*height*2;
}


void CreateColorForm::pushButton_fileSelect_OnClick()
{
    ui->statusBar->setText("openFile: 正在选择文件");

    ui->lineEdit_folderName->setText(QFileDialog::getExistingDirectory(this, "choose folder", "/"));
    ui->lineEdit_fileName->setText("1"+getExternalFileFormat(ui->comboBox_Format->currentText()));
    fileName = ui->lineEdit_folderName->text()+"/"+ui->lineEdit_fileName->text();
    ui->statusBar->setText("openFile: "+fileName);
}


void CreateColorForm::lineEdit_fileName_OnChange(QString str)
{
    fileName = ui->lineEdit_folderName->text()+"/"+ui->lineEdit_fileName->text();
}
