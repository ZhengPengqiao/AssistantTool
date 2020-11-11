#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include "colortestform.h"
#include "ui_colortestform.h"

ColorTestForm::ColorTestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorTestForm)
{
    ui->setupUi(this);

    r = ui->lineEdit_Red->text().toInt();
    g = ui->lineEdit_Green->text().toInt();
    b = ui->lineEdit_Blue->text().toInt();
    a = ui->lineEdit_alpha->text().toInt();
    uc = 0;
    l_num = 8;
    xres = ui->spinBox_width->value();
    yres = ui->spinBox_height->value();
    data = nullptr;
    fileName = ui->lineEdit_folderName->text()+"/"+ui->lineEdit_fileName->text()+ui->lineEdit_externName->text();
}

ColorTestForm::~ColorTestForm()
{
    delete ui;
}


/*

 * 函数名称 : clearFrameBuff
 * 函数介绍 : 使用指定颜色刷新FrameBuffer
 * 参数介绍 : data:FrameBuffer相关信息， x,y,w,h：清空的矩形， r,g,b:使用的颜色,
 *           bps:framebuffer的bps
 * 返回值   : -1:失败，  0：成功
 */
int ColorTestForm::clearFrameBuff(char *indata, int x, int y, int w, int h, int r, int g, int b, int bps)
{
    unsigned int rgb = 0;
    int drawW = 0;
    int drawH = 0;
    int fbw = xres;
    int fbh = yres;


    if ( x + w > fbw )
    {
        drawW = fbw-x;
    }
    else
    {
        drawW = w;
    }

    if ( y + h > fbh )
    {
        drawH = fbh-y;
    }
    else
    {
        drawH = h;
    }

    switch( bps )
    {
        case 32:
            rgb = (b<<24) | (g<<16) | (r<<8);
        break;
        case 16:
            rgb = (((unsigned(r) << 8) & 0xF800) |
                    ((unsigned(g) << 3) & 0x7E0) |
                    ((unsigned(b) >> 3)));
        break;
    }


    for( int i = 0; i < drawH; i++ )
    {
        for( int j = 0; j < drawW; j++ )
        {
            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4] = (rgb>>24)&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = (rgb>>16)&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = (rgb>>8)&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }
    return 0;
}



/*
 * 函数名称 : testColor
 * 函数介绍 : 测试颜色FrameBuffer - 渐变色
 * 参数介绍 : data:FrameBuffer相关信息， x,y,w,h：清空的矩形，  br,bg,bb:使用的颜色,
 *           bps:framebuffer的bps
 * 返回值   : -1:失败，  0：成功
 */
int ColorTestForm::testColor(char * indata, int x, int y, int w, int h, int br, int bg, int bb, int bps)
{
    unsigned int rgb = 0;
    int drawW = 0;
    int drawH = 0;
    int fbw = xres;
    int fbh = yres;
    int r = br, g = bg, b = bb;
    int rgFlag = 0;

    if ( x + w > fbw )
    {
        drawW = fbw-x;
    }
    else
    {
        drawW = w;
    }


    if ( y + h > fbh )
    {
        drawH = fbh-y;
    }
    else
    {
        drawH = h;
    }

    for( int i = 0; i < drawH; i++ )
    {
        for( int j = 0; j < drawW; j++ )
        {
            r = (unsigned int)(br + (float)i/drawH*255)%255;
            g = (unsigned int)(bg + (float)j/drawW*255)%255;
            b = bb % 255;
            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }
    return 0;
}


/*
 * 函数名称 : testUrandom
 * 函数介绍 : 测试颜色FrameBuffer - 随机数
 * 参数介绍 : fbInfo:FrameBuffer相关信息， x,y,w,h：清空的矩形
 *           bps:framebuffer的bps
 * 返回值   : -1:失败，  0：成功
 */
int ColorTestForm::testUrandom(char *indata, int x, int y, int w, int h, int bps)
{
    unsigned int rgb = 0;
    int drawW = 0;
    int drawH = 0;
    int fbw = xres;
    int fbh = yres;
    int rgFlag = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 0;

    if ( x + w > fbw )
    {
        drawW = fbw-x;
    }
    else
    {
        drawW = w;
    }

    if ( y + h > fbh )
    {
        drawH = fbh-y;
    }
    else
    {
        drawH = h;
    }

    for( int i = 0; i < drawH; i++ )
    {
        for( int j = 0; j < drawW; j++ )
        {
            r = rand()/255;
            g = rand()/255;
            b = rand()/255;
            a = rand()/255;
            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;

                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }
    return 0;
}


/*
 * 函数名称 : testSolidColor
 * 函数介绍 : 测试单颜色FrameBuffer - 单色渐变色
 * 参数介绍 : fbInfo:FrameBuffer相关信息， x,y,w,h：清空的矩形，  br,bg,bb:使用的颜色,
 *           bps:framebuffer的bps
 * 返回值   : -1:失败，  0：成功
 */
int ColorTestForm::testSolidColor(char * indata, int x, int y, int w, int h, int br, int bg, int bb, int bps)
{
    unsigned int rgb = 0;
    int drawW = 0;
    int drawH = 0;
    int fbw = xres;
    int fbh = yres;
    int r = br, g = bg, b = bb;
    int rgFlag = 0;

    if ( x + w > fbw )
    {
        drawW = fbw-x;
    }
    else
    {
        drawW = w;
    }

    if ( y + h > fbh )
    {
        drawH = fbh-y;
    }
    else
    {
        drawH = h;
    }

    for( int i = 0; i < drawH; i++ )
    {
        r = br;
        g = bg;
        b = bb;
        for( int j = 0; j < drawW; j++ )
        {
            if ( i <= drawH/3 )
            {
                r = (unsigned int)(br + j*255/drawW)%255;
            }
            else if ( drawH/3 && i <= drawH*2/3 )
            {
                g = (unsigned int)(bg + j*255/drawW)%255;
            }
            else if ( drawH*2/3 < i )
            {
                b = (unsigned int)(bb + j*255/drawW)%255;
            }

            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }
    return 0;
}


/*
 * 函数名称 : testDefinitionColor
 * 函数介绍 : 条纹清晰度侧视图FrameBuffer
 * 参数介绍 : fbInfo:FrameBuffer相关信息， x,y,w,h：清空的矩形，  br,bg,bb:使用的颜色,
 *           bps:framebuffer的bps
 * 返回值   : -1:失败，  0：成功
 */
int ColorTestForm::testDefinitionColor(char * indata, int x, int y, int w, int h, int br, int bg, int bb, int bps)
{
    unsigned int rgb = 0;
    int drawW = 0;
    int drawH = 0;
    int fbw = xres;
    int fbh = yres;
    int r = br, g = bg, b = bb;
    int rgFlag = 0;

    if ( x + w > fbw )
    {
        drawW = fbw-x;
    }
    else
    {
        drawW = w;
    }

    if ( y + h > fbh )
    {
        drawH = fbh-y;
    }
    else
    {
        drawH = h;
    }

    // Update 1/4
    for( int i = 0; i < drawH/4; i++ )
    {
        r = br;
        g = bg;
        b = bb;
        int jStep = 1;
        int reTime = 0;
        int jOld = 0;

        for( int j = 0; j < drawW; j++ )
        {
            if( j > jOld && j <= (jOld+jStep) )
            {
                r=g=b=255;
            }
            else if( j > (jOld+jStep) && j <= (jOld+2*jStep) )
            {
                r=g=b=0;
            }

            if( j == (jOld+2*jStep))
            {
                jOld = jOld+2*jStep;
                if( reTime++ > 4 )
                {
                    reTime = 0;
                    jStep++;
                }
            }


            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }

    // Update 1/4~2/4
    for( int i = drawH/4; i < drawH/4*2; i++ )
    {
        r = br;
        g = bg;
        b = bb;
        int jStep = 1;
        int reTime = 0;
        int jOld = drawW;

        for( int j = drawW; j > 0; j-- )
        {
            if( j < jOld && j >= (jOld-jStep) )
            {
                r=g=b=255;
            }
            else if( j < (jOld-jStep) && j >= (jOld-2*jStep) )
            {
                r=g=b=0;
            }

            if( j == (jOld-2*jStep))
            {
                jOld = jOld-2*jStep;
                if( reTime++ > 4 )
                {
                    reTime = 0;
                    jStep++;
                }
            }


            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }

    // Update 2/4 3/4
    for( int i = drawH/4*2; i < drawH/4*3; i++ )
    {
        r = br;
        g = bg;
        b = bb;
        int jStep = 1;
        int jOld = 0;
        int jFlag = 0;

        for( int j = 0; j < drawW; j++ )
        {
            if( j > jOld && j <= (jOld+jStep) )
            {
                r=g=b=255;
            }
            else if( j > (jOld+jStep) && j <= (jOld+2*jStep) )
            {
                r=g=b=0;
            }

            if( j == (jOld+2*jStep))
            {
                jOld = jOld+2*jStep;
                if( jStep == 5 )
                {
                    jFlag = 0;
                }
                else if( jStep == 1 )
                {
                    jFlag = 1;
                }

                if(jFlag)
                {
                    jStep++;
                }
                else
                {
                    jStep--;
                }
            }


            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }

    // Update 3/4~4/4
    for( int i = drawH/4*3; i < drawH; i++ )
    {
        r = br;
        g = bg;
        b = bb;

        int jStep = 1;
        int jOld = drawW;
        int jFlag = 0;

        for( int j = drawW; j > 0; j-- )
        {
            if( j < jOld && j >= (jOld-jStep) )
            {
                r=g=b=255;
            }
            else if( j < (jOld-jStep) && j >= (jOld-2*jStep) )
            {
                r=g=b=0;
            }

            if( j == (jOld-2*jStep))
            {
                jOld = jOld-2*jStep;
                if( jStep == 10 )
                {
                    jFlag = 0;
                }
                else if( jStep == 1 )
                {
                    jFlag = 1;
                }

                if(jFlag)
                {
                    jStep++;
                }
                else
                {
                    jStep--;
                }
            }

            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                    indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                    indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));
                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;
            }
        }
    }

    return 0;
}


/*
 * 函数名称 : checkerBoard
 * 函数介绍 : 测试颜色FrameBuffer - 棋盘格
 * 参数介绍 : data:FrameBuffer相关信息， x,y,w,h：清空的矩形
 *           bps:framebuffer的bps
 * 返回值   : -1:失败，  0：成功
 */
int ColorTestForm::checkerBoard(char *indata, int x, int y, int w, int h, int bps)
{
    unsigned int rgb = 0;
    int drawW = 0;
    int drawH = 0;
    int fbw = xres;
    int fbh = yres;
    int r, g, b;
    int rgFlag = 0;

    if ( x + w > fbw )
    {
        drawW = fbw-x;
    }
    else
    {
        drawW = w;
    }

    if ( y + h > fbh )
    {
        drawH = fbh-y;
    }
    else
    {
        drawH = h;
    }


    for( int i = 0; i < drawH; i++ )
    {
        for( int j = 0; j < drawW; j++ )
        {
            r = (i/(drawH/10))*70%255;
            g = (j/(drawW/10))*40%255;
            b = (r+g)%255;

            switch( bps )
            {
                case 32:
                    indata[fbw*4*(i+y)+(j+x)*4]   = b&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+1] = g&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+2] = r&0xFF;
                    indata[fbw*4*(i+y)+(j+x)*4+3] = a;
                break;
                case 24:
                indata[fbw*3*(i+y)+(j+x)*3]   = b&0xFF;
                indata[fbw*3*(i+y)+(j+x)*3+1] = g&0xFF;
                indata[fbw*3*(i+y)+(j+x)*3+2] = r&0xFF;
                break;
                case 16:
                    rgb = (((r << 8) & 0xF800) |
                        ((g << 3) & 0x7E0) |
                        ((b >> 3)));

                    indata[fbw*2*(i+y)+(j+x)*2] = (rgb)&0xFF;
                    indata[fbw*2*(i+y)+(j+x)*2+1] = (rgb>>8)&0xFF;
                break;

            }

        }

    }

    return 0;

}


/**
 * @brief pushButton_updateShow_onClicked
 * 按照配置信息，更新显示的内容
 */
void ColorTestForm::pushButton_updateShow_onClicked()
{
    xres = ui->spinBox_width->value();
    yres = ui->spinBox_height->value();
    dataSize = xres*yres*toPixelBytes(ui->comboBox_Format->currentText());
    fileName = ui->lineEdit_folderName->text()+"/"+ui->lineEdit_fileName->text()+ui->lineEdit_externName->text();
    uc = toImageFormat(ui->comboBox_imageFormat->currentText());

    if( data != nullptr )
    {
        free(data);
    }
    data = (char*)malloc(dataSize);

    ui->statusBar->setText("ColorTest uc:"+QString::number(uc)+ "  " + ui->comboBox_Format->currentText() + "  dataSize:" + QString::number(dataSize));

    if ( uc == 0 )
    {
        testColor(data, 0, 0, xres, yres, r, g, b, toBps(ui->comboBox_Format->currentText()));
    }
    else if(uc == 1)
    {
        float len = xres/8;
        clearFrameBuff(data, 0*len, 0, xres/8, yres,  0xff, 0xff, 0xff, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 1*len, 0, xres/8, yres,  0xb8, 0xc6, 0x00, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 2*len, 0, xres/8, yres,  0x00, 0xc2, 0xca, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 3*len, 0, xres/8, yres,  0x00, 0xc6, 0x00, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 4*len, 0, xres/8, yres,  0xc7, 0x00, 0xb9, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 5*len, 0, xres/8, yres,  0xc4, 0x00, 0x00, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 6*len, 0, xres/8, yres,  0x00, 0x00, 0xcb, toBps(ui->comboBox_Format->currentText()));
        clearFrameBuff(data, 7*len, 0, xres/8, yres,  0x00, 0x00, 0x00, toBps(ui->comboBox_Format->currentText()));
    }
    else if(uc == 2)
    {
        clearFrameBuff(data, 0, 0, xres, yres, ui->lineEdit_Red->text().toInt(), \
                       ui->lineEdit_Green->text().toInt(), ui->lineEdit_Blue->text().toInt(), \
                       toBps(ui->comboBox_Format->currentText()));
    }
    else if(uc == 3)
    {
        checkerBoard(data, 0, 0, xres, yres, toBps(ui->comboBox_Format->currentText()));
    }
    else if(uc == 4)
    {
        float len = xres/l_num;
        for( int i = 0; i < l_num; i++ )
        {
            if( i%2 == 0 )
            {
                clearFrameBuff(data, i*len, 0, xres/l_num, yres,  0xff, 0xff, 0xff, toBps(ui->comboBox_Format->currentText()));
            }
            else
            {
                clearFrameBuff(data, i*len, 0, xres/l_num, yres,  0x00, 0x00, 0x00, toBps(ui->comboBox_Format->currentText()));
            }
        }
    }
    else if(uc == 5)
    {
        testUrandom(data, 0, 0, xres, yres, toBps(ui->comboBox_Format->currentText()));
    }
    else if ( uc == 6 )
    {
        testSolidColor(data, 0, 0, xres, yres, r, g, b, toBps(ui->comboBox_Format->currentText()));
    }
    else if ( uc == 7 )
    {
        testDefinitionColor(data, 0, 0, xres, yres, r, g, b, toBps(ui->comboBox_Format->currentText()));
    }

    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGBA8888不同的摄像头用不同的格式。
    QImage image((const uchar*)data, xres, yres, toQtFormat(ui->comboBox_Format->currentText()));
    ui->label_image->setPixmap(QPixmap::fromImage(image));    //  将图片显示到label上
    ui->label_image->resize( ui->label_image->pixmap()->size());    //  将label控件resize到fame的尺寸
    ui->label_image->setGeometry(0,0,ui->label_image->sizeHint().width(),ui->label_image->sizeHint().height());

    ui->scrollAreaWidgetContents->resize(ui->label_image->sizeHint());

    qDebug() << "pushButton_updateShow_onClicked";
}

void ColorTestForm::pushButton_savefile_onClicked()
{
    int ret;

    //将文本框数据取出并按行排列
    fb = ::open(fileName.toStdString().c_str(), O_CREAT|O_RDWR, 0777);
    if( fb < 0 )
    {
        qDebug() << "fileName:" << fileName << "open err";
    }
    else
    {
        qDebug() << "fileName:" << fileName << "open ok";
    }


    ret = ::write(fb, data, dataSize);

    ::close(fb);
    QMessageBox::information(this, "Save LogForm", "Save " + fileName + " Ok! byte:" + QString::number(ret));
}


void ColorTestForm::pushButton_fileSelect_OnClick()
{
    ui->lineEdit_folderName->setText(QFileDialog::getExistingDirectory(this, "choose folder", "/"));
    fileName = ui->lineEdit_folderName->text()+"/"+ui->lineEdit_fileName->text()+ui->lineEdit_externName->text();
}


void ColorTestForm::lineEdit_fileName_OnChange(QString str)
{
    fileName = ui->lineEdit_folderName->text()+"/"+ui->lineEdit_fileName->text()+ui->lineEdit_externName->text();
}


void ColorTestForm::comboBox_Format_onChange(QString str)
{
    ui->lineEdit_externName->setText(toExternalFormat(str));
    qDebug() << "comboBox_Format_onChange " << str;
}


/**
 * @brief ColorTestForm::toExternalFormat
 * @param str : 界面选择的格式
 * @return 根据格式，返回文件的扩展名
 */
QString ColorTestForm::toExternalFormat(QString str)
{
    QString ext;
    if( str == "RGBA8888" )
    {
        ext = ".rgba8888";
    }
    else if( str == "RGB565" )
    {
        ext = ".rgb565";
    }
    else if( str == "RGB888" )
    {
        ext = ".rgb888";
    }
    else
    {
        ext = ".rgba8888";
    }
    return ext;
}


/**
 * @brief ColorTestForm::toBps
 * @param str : 界面选择的格式
 * @return 转换后的像素位数
 */
int ColorTestForm::toBps(QString str)
{
    int bps;
    if( str == "RGBA8888" )
    {
        bps = 32;
    }
    else if( str == "RGB565" )
    {
        bps = 16;
    }
    else if( str == "RGB888" )
    {
        bps = 24;
    }
    else
    {
        bps = 32;
    }
    return bps;
}


/**
 * @brief ColorTestForm::toPixelBytes
 * @param str : 界面选择的格式
 * @return 转换后的像素 byte数
 */
int ColorTestForm::toPixelBytes(QString str)
{
    int bs;
    if( str == "RGBA8888" )
    {
        bs = 4;
    }
    else if( str == "RGB565" )
    {
        bs = 2;
    }
    else if( str == "RGB888" )
    {
        bs = 3;
    }
    else
    {
        bs = 4;
    }
    return bs;
}


/**
 * @brief ColorTestForm::toQtFormat
 * @param str : 界面选择的格式
 * @return 转换成Qt的格式
 */
QImage::Format ColorTestForm::toQtFormat(QString str)
{
    QImage::Format qtfmt;
    if( str == "RGBA8888" )
    {
        qtfmt = QImage::Format_RGB32;
    }
    else if( str == "RGB565" )
    {
        qtfmt = QImage::Format_RGB16;
    }
    else if( str == "RGB888" )
    {
        qtfmt = QImage::Format_RGB888;
    }
    else
    {
        qtfmt = QImage::Format_RGBA8888;
    }

    return qtfmt;
}


/**
 * @brief ColorTestForm::toImageFormat
 * @param str : 界面选择的图形样式
 * @return 返回对应的图像样式ID
 */
int ColorTestForm::toImageFormat(QString str)
{
    ui->lineEdit_fileName->setText(str);
    int imagefmt;
    if( str == "Gradient" )
    {
        imagefmt = 0;
    }
    else if( str == "ColoredBar" )
    {
        imagefmt = 1;
    }
    else if( str == "Solid" )
    {
        imagefmt = 2;
    }
    else if( str == "Chessboard" )
    {
        imagefmt = 3;
    }
    else if( str == "BlackAndWhiteGrid" )
    {
        imagefmt = 4;
    }
    else if( str == "Urandom" )
    {
        imagefmt = 5;
    }
    else if( str == "SolidGradient" )
    {
        imagefmt = 6;
    }
    else if( str == "Definition" )
    {
        imagefmt = 7;
    }
    else
    {
        imagefmt = 0;
    }

    return imagefmt;
}


/**
 * @brief ColorTestForm::comboBox_imageFormat_onClicked
 * @param str : 用户选择的图像格式
 */
void ColorTestForm::comboBox_imageFormat_onClicked(QString str)
{
    if( str != "Solid" )
    {
        ui->lineEdit_Red->setDisabled(true);
        ui->lineEdit_Blue->setDisabled(true);
        ui->lineEdit_Green->setDisabled(true);
    }
    else
    {
        ui->lineEdit_Red->setDisabled(false);
        ui->lineEdit_Blue->setDisabled(false);
        ui->lineEdit_Green->setDisabled(false);
    }
}
