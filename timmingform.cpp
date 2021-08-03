#include "timmingform.h"
#include "ui_timmingform.h"
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
#include <QMessageBox>

TimmingForm::TimmingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimmingForm)
{
    ui->setupUi(this);

    //ps
    param_pclk=1;
    param_vt=1;
    param_vBlank=1;
    param_hActive=1;
    param_vb=1;
    param_vf=1;
    param_vs=1;
    param_ht=1;
    param_hBlank=1;
    param_hActive=1;
    param_hb=1;
    param_hf=1;
    param_hs=1;

    //ps
    timming_pclk=1;
    timming_vt=1;
    timming_vBlank=1;
    timming_hActive=1;
    timming_vb=1;
    timming_vf=1;
    timming_vs=1;
    timming_ht=1;
    timming_hBlank=1;
    timming_hActive=1;
    timming_hb=1;
    timming_hf=1;
    timming_hs=1;

    //ps
    time_pclk=1;
    time_T1 = 1;
    time_T2 = 1;
    time_T3 = 1;
    time_T4 = 1;
    time_T5 = 1;
    time_T6 = 1;
    time_T7 = 1;
    time_T8 = 1;
    time_T9 = 1;
    time_T10= 1;
    time_T11= 1;
    time_T12= 1;

    loadFileToCombo();
}

TimmingForm::~TimmingForm()
{
    delete ui;
}


/**
 * @brief pushButton_SaveParam_OnClicked
 * 将列表中的参数保存到文件中
 */
void TimmingForm::pushButton_SaveParam_OnClicked()
{
    saveComboToFile();
}


void TimmingForm::loadFileToCombo()
{
    update_lineEdit_TimeData();
    update_lineEdit_ParamData();
    update_lineEdit_TimmingData();
    QString itemName;
    struct lcd_param *lcd;
    QString fileName = "./timming.txt";
    ui->statusBar->setText("提示信息:loadFileToCombo");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        lcd = new struct lcd_param;
        stream >> itemName >> lcd->name;
        stream >> itemName >> lcd->param_pclk;
        stream >> itemName >> lcd->param_vt;
        stream >> itemName >> lcd->param_vBlank;
        stream >> itemName >> lcd->param_vActive;
        stream >> itemName >> lcd->param_vb;
        stream >> itemName >> lcd->param_vf;
        stream >> itemName >> lcd->param_vs;
        stream >> itemName >> lcd->param_ht;
        stream >> itemName >> lcd->param_hBlank;
        stream >> itemName >> lcd->param_hActive;
        stream >> itemName >> lcd->param_hb;
        stream >> itemName >> lcd->param_hf;
        stream >> itemName >> lcd->param_hs;
        stream >> itemName >> lcd->param_hs_hps;
        stream >> itemName >> lcd->param_vs_fps;
        stream >> itemName >> lcd->param_t5_hbp;
        stream >> itemName >> lcd->param_t5_hspw;
        stream >> itemName >> lcd->param_t5_vbp;
        stream >> itemName >> lcd->param_t5_vspw;

        if( lcd->param_pclk != 0 && lcd->param_vt != 0 && lcd->param_ht != 0 )
        {
            list.append(lcd);
            ui->comboBox->addItem(lcd->name);
        }
    }
    file.close();

}


void TimmingForm::saveComboToFile()
{
    struct lcd_param *lcd;
    ui->statusBar->setText("提示信息:saveComboToFile");
    if( ui->comboBox->count() <= 0 )
    {
        ui->statusBar->setText("Combo is empty, not save param");
        return;
    }

    QString fileName = "./timming.txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    qDebug() << "comboBox->count()=" << ui->comboBox->count() << "  list.count()=" <<  list.count();
    QTextStream stream(&file);
    for( int i = 0; i < ui->comboBox->count() && i < list.count(); i++ )
    {

        lcd = list.at(i);
        stream << "param_name " << lcd->name << "\n";
        stream << "param_pclk " << lcd->param_pclk << "\n";
        stream << "param_vt " << lcd->param_vt << "\n";
        stream << "param_vBlank " << lcd->param_vBlank << "\n";
        stream << "param_vActive " << lcd->param_vActive << "\n";
        stream << "param_vb " << lcd->param_vb << "\n";
        stream << "param_vf " << lcd->param_vf << "\n";
        stream << "param_vs " << lcd->param_vs << "\n";
        stream << "param_ht " << lcd->param_ht << "\n";
        stream << "param_hBlank " << lcd->param_hBlank << "\n";
        stream << "param_hActive " << lcd->param_hActive << "\n";
        stream << "param_hb " << lcd->param_hb << "\n";
        stream << "param_hf " << lcd->param_hf << "\n";
        stream << "param_hs " << lcd->param_hs << "\n";
        stream << "param_hs_hps " << lcd->param_hs_hps << "\n";
        stream << "param_vs_fps " << lcd->param_vs_fps << "\n";
        stream << "param_t5_hbp " << lcd->param_t5_hbp << "\n";
        stream << "param_t5_hspw " << lcd->param_t5_hspw << "\n";
        stream << "param_t5_vbp " << lcd->param_t5_vbp << "\n";
        stream << "param_t5_vspw " << lcd->param_t5_vspw << "\n";
    }
    file.close();
}

void TimmingForm::showParam()
{

    qDebug() << "timming_pclk:" << timming_pclk;
    qDebug() << "timming_vb:" << timming_vb;
    qDebug() << "timming_vf:" << timming_vf;
    qDebug() << "timming_vs:" << timming_vs;
    qDebug() << "timming_vt:" << timming_vt;
    qDebug() << "timming_vBlank:" << timming_vBlank;
    qDebug() << "timming_vActive:" << timming_vActive;
    qDebug() << "timming_hb:" << timming_hb;
    qDebug() << "timming_hf:" << timming_hf;
    qDebug() << "timming_hs:" << timming_hs;
    qDebug() << "timming_ht:" << timming_ht;
    qDebug() << "timming_hBlank:" << timming_hBlank;
    qDebug() << "timming_hActive:" << timming_hActive;


    qDebug() << "time_pclk:" << time_pclk;
    qDebug() << "time_T1:" << time_T1;
    qDebug() << "time_T2:" << time_T2;
    qDebug() << "time_T3:" << time_T3;
    qDebug() << "time_T4:" << time_T4;
    qDebug() << "time_T5:" << time_T5;
    qDebug() << "time_T6:" << time_T6;
    qDebug() << "time_T7:" << time_T7;
    qDebug() << "time_T8:" << time_T8;
    qDebug() << "time_T9:" << time_T9;
    qDebug() << "time_T10:" << time_T10;
    qDebug() << "time_T11:" << time_T11;
    qDebug() << "time_T12:" << time_T12;


    qDebug() << "param_pclk:" << param_pclk;
    qDebug() << "param_hb:" << param_hb;
    qDebug() << "param_hf:" << param_hf;
    qDebug() << "param_hs:" << param_hs;
    qDebug() << "param_ht:" << param_ht;
    qDebug() << "param_hBlank:" << param_hBlank;
    qDebug() << "param_hActive:" << param_hActive;
    qDebug() << "param_vb:" << param_vb;
    qDebug() << "param_vf:" << param_vf;
    qDebug() << "param_vs:" << param_vs;
    qDebug() << "param_vt:" << param_vt;
    qDebug() << "param_vActive:" << param_vActive;
    qDebug() << "param_vBlank:" << param_vBlank;
    qDebug() << "param_hs_hps:" << param_hs_hps;
    qDebug() << "param_vs_fps:" << param_vs_fps;

}


void TimmingForm::pushButton_Comm_CurrectItemChange_OnClicked(int i)
{
    struct lcd_param *lcd = list.at(i);

    param_pclk = lcd->param_pclk;
    param_vt = lcd->param_vt;
    param_vBlank = lcd->param_vBlank;
    param_vActive = lcd->param_vActive;
    param_vb = lcd->param_vb;
    param_vf = lcd->param_vf;
    param_vs = lcd->param_vs;
    param_ht = lcd->param_ht;
    param_hBlank = lcd->param_hBlank;
    param_hActive = lcd->param_hActive;
    param_hb = lcd->param_hb;
    param_hf = lcd->param_hf;
    param_hs = lcd->param_hs;
    param_hs_hps = lcd->param_hs_hps;
    param_vs_fps = lcd->param_vs_fps;
    param_t5_hbp = lcd->param_t5_hbp;
    param_t5_hspw = lcd->param_t5_hspw;
    param_t5_vbp = lcd->param_t5_vbp;
    param_t5_vspw = lcd->param_t5_vspw;

    update_lineEdit_ParamData();
}

void TimmingForm::pushButton_AddComm_OnClicked()
{
    struct lcd_param *lcd = new struct lcd_param;
    lcd->name = QString::number(list.count()) + "_" + QString::number(param_vActive) +"x" + QString::number(param_hActive) + "@" +QString::number(param_vs_fps);
    lcd->param_pclk = param_pclk;
    lcd->param_vt = param_vt;
    lcd->param_vBlank = param_vBlank;
    lcd->param_vActive = param_vActive;
    lcd->param_vb = param_vb;
    lcd->param_vf = param_vf;
    lcd->param_vs = param_vs;
    lcd->param_ht = param_ht;
    lcd->param_hBlank = param_hBlank;
    lcd->param_hActive = param_hActive;
    lcd->param_hb = param_hb;
    lcd->param_hf = param_hf;
    lcd->param_hs = param_hs;
    lcd->param_hs_hps = param_hs_hps;
    lcd->param_vs_fps = param_vs_fps;
    lcd->param_t5_hbp = param_t5_hbp;
    lcd->param_t5_hspw = param_t5_hspw;
    lcd->param_t5_vbp = param_t5_vbp;
    lcd->param_t5_vspw = param_t5_vspw;

    list.append(lcd);

    ui->comboBox->addItem(lcd->name);
}

void TimmingForm::pushButton_RemoveComm_OnClicked()
{
    list.removeAt(ui->comboBox->currentIndex());
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}


/**
 * @brief pushButton_Timming_OnClicked
 */
void TimmingForm::pushButton_Timming_OnClicked()
{
    int clk_time = 0; // 每个clk时长

    qDebug() << "pushButton_Timming_OnClicked";
    timming_pclk = ui->lineEdit_Timming_PCLK->text().toDouble();
    timming_vt = getZoomIn(ui->lineEdit_Timming_VT->text(), 9).toDouble();
    timming_vBlank = getZoomIn(ui->lineEdit_Timming_VBlank->text(), 9).toDouble();
    timming_vb = getZoomIn(ui->lineEdit_Timming_VB->text(), 9).toDouble();
    timming_vf = getZoomIn(ui->lineEdit_Timming_VF->text(), 9).toDouble();
    timming_vs = getZoomIn(ui->lineEdit_Timming_VS->text(), 9).toDouble();
    timming_hBlank = getZoomIn(ui->lineEdit_Timming_HBlank->text(), 9).toDouble();
    timming_ht = getZoomIn(ui->lineEdit_Timming_HT->text(), 9).toDouble();
    timming_hb = getZoomIn(ui->lineEdit_Timming_HB->text(), 9).toDouble();
    timming_hf = getZoomIn(ui->lineEdit_Timming_HF->text(), 9).toDouble();
    timming_hs = getZoomIn(ui->lineEdit_Timming_HS->text(), 9).toDouble();
    timming_hActive = timming_ht - timming_hb - timming_hf - timming_hs;
    timming_vActive = timming_vt - timming_vb - timming_vf - timming_vs;
    clk_time = 1000000000000/timming_pclk;


    // 计算时间轴
    time_T1 = 0;
    time_T2 = timming_vb;
    time_T3 = timming_vActive + time_T2;
    time_T4 = timming_vf + time_T3;
    time_T5 = timming_vs + time_T4;

    time_T6 = 0;
    time_T7 = timming_hb + time_T6;
    time_T8 = timming_hActive + time_T7;
    time_T9 = timming_hf + time_T8;
    time_T10 = timming_hs + time_T9;

    time_T11 = 0;
    time_T12 = clk_time*10;
    time_pclk = timming_pclk;

    // 计算参数
    param_pclk = (double)1000000000000/((time_T12-time_T11)/10);
    param_hb = (double)timming_hb/clk_time;
    param_hf = (double)timming_hf/clk_time;
    param_hs = (double)timming_hs/clk_time;
    param_ht = (double)timming_ht/clk_time;
    param_hBlank = (double)timming_hBlank/clk_time;
    param_hActive = (double)timming_hActive/clk_time;

    param_vb = (double)timming_vb/timming_ht;
    param_vf = (double)timming_vf/timming_ht;
    param_vs = (double)timming_vs/timming_ht;
    param_vt = (double)timming_vt/timming_ht;
    param_vActive = (double)timming_vActive/timming_ht;
    param_vBlank = (double)timming_vBlank/timming_ht;
    param_hs_hps = (double)1000000000000/timming_ht;
    param_vs_fps = (double)1000000000000/timming_vt;
    param_t5_hbp = param_hb + param_hs;
    param_t5_hspw = param_hs;
    param_t5_vbp = param_vb + param_vs;
    param_t5_vspw = param_vs;

    update_lineEdit_ParamData();
    update_lineEdit_TimeData();

    ui->statusBar->setText("提示信息:pushButton Timming OnClicked");
}

/**
 * @brief pushButton_Param_OnClicked
 *
 */
void TimmingForm::pushButton_Param_OnClicked()
{
    int clk_time = 0; // 每个clk时长

    qDebug() << "pushButton_Param_OnClicked";
    param_pclk=ui->lineEdit_Param_PCLK->text().toDouble();
    param_vBlank=getZoomIn(ui->lineEdit_Param_VBlank->text(), 9).toDouble();
    param_vActive=getZoomIn(ui->lineEdit_Param_VActive->text(), 9).toDouble();
    param_vt=getZoomIn(ui->lineEdit_Param_VT->text(), 9).toDouble();
    param_vb=getZoomIn(ui->lineEdit_Param_VB->text(), 9).toDouble();
    param_vf=getZoomIn(ui->lineEdit_Param_VF->text(), 9).toDouble();
    param_vs=getZoomIn(ui->lineEdit_Param_VS->text(), 9).toDouble();
    param_hActive=getZoomIn(ui->lineEdit_Param_HActive->text(), 9).toDouble();
    param_hBlank=getZoomIn(ui->lineEdit_Param_HBlank->text(), 9).toDouble();
    param_ht=getZoomIn(ui->lineEdit_Param_HT->text(), 9).toDouble();
    param_hb=getZoomIn(ui->lineEdit_Param_HB->text(), 9).toDouble();
    param_hf=getZoomIn(ui->lineEdit_Param_HF->text(), 9).toDouble();
    param_hs=getZoomIn(ui->lineEdit_Param_HS->text(), 9).toDouble();


    clk_time = (double)1000000000000/param_pclk;

    // 计算时间轴
    time_T1 = 0;
    time_T2 = (param_vb/1000000000*param_ht/1000000000)*clk_time;
    time_T3 = param_vActive/1000000000*param_ht/1000000000*clk_time + time_T2;
    time_T4 = param_vf/1000000000*param_ht/1000000000*clk_time + time_T3;
    time_T5 = param_vs/1000000000*param_ht/1000000000*clk_time + time_T4;

    time_T6 = 0;
    time_T7 = param_hb/1000000000*clk_time + time_T6;
    time_T8 = param_hActive/1000000000*clk_time + time_T7;
    time_T9 = param_hf/1000000000*clk_time + time_T8;
    time_T10 = param_hs/1000000000*clk_time + time_T9;

    time_T11 = 0;
    time_T12 = clk_time*10;
    time_pclk = param_pclk;


    // 计算时序
    timming_pclk = 1000000000000/((time_T12-time_T11)/10);
    timming_vb = time_T2-time_T1;
    timming_vf = time_T4-time_T3;
    timming_vs = time_T5-time_T4;
    timming_vt = time_T5-time_T1;
    timming_vBlank = timming_vb+timming_vf+timming_vs;
    timming_vActive = timming_vt-timming_vBlank;
    timming_hb = time_T7-time_T6;
    timming_hf = time_T9-time_T8;
    timming_hs = time_T10-time_T9;
    timming_ht = time_T10-time_T6;
    timming_hBlank = timming_hb+timming_hf+timming_hs;
    timming_hActive = timming_ht-timming_hBlank;



    update_lineEdit_TimeData();
    update_lineEdit_TimmingData();

    ui->statusBar->setText("提示信息:pushButton Param OnClicked");
}


/**
 * @brief pushButton_Time_OnClicked
 *
 */
void TimmingForm::pushButton_Time_OnClicked()
{
    qDebug() << "pushButton_Time_OnClicked";
    time_T1=getZoomIn(ui->lineEdit_Time_T1->text(), 9).toDouble();
    time_T2=getZoomIn(ui->lineEdit_Time_T2->text(), 9).toDouble();
    time_T3=getZoomIn(ui->lineEdit_Time_T3->text(), 9).toDouble();
    time_T4=getZoomIn(ui->lineEdit_Time_T4->text(), 9).toDouble();
    time_T5=getZoomIn(ui->lineEdit_Time_T5->text(), 9).toDouble();
    time_T6=getZoomIn(ui->lineEdit_Time_T6->text(), 9).toDouble();
    time_T7=getZoomIn(ui->lineEdit_Time_T7->text(), 9).toDouble();
    time_T8=getZoomIn(ui->lineEdit_Time_T8->text(), 9).toDouble();
    time_T9=getZoomIn(ui->lineEdit_Time_T9->text(), 9).toDouble();
    time_T10=getZoomIn(ui->lineEdit_Time_T10->text(), 9).toDouble();
    time_T11=getZoomIn(ui->lineEdit_Time_T11->text(), 9).toDouble();
    time_T12=getZoomIn(ui->lineEdit_Time_T12->text(), 9).toDouble();
    qDebug() << "time_T2-time_T1:" << time_T2-time_T1;

    time_pclk = 1000000000000/((time_T12-time_T11)/10);

    // 计算时序
    timming_pclk = 1000000000000/((time_T12-time_T11)/10);
    timming_vb = time_T2-time_T1;
    timming_vf = time_T4-time_T3;
    timming_vs = time_T5-time_T4;
    timming_vt = time_T5-time_T1;
    timming_vBlank = timming_vb+timming_vf+timming_vs;
    timming_vActive = timming_vt-timming_vBlank;
    timming_hb = time_T7-time_T6;
    timming_hf = time_T9-time_T8;
    timming_hs = time_T10-time_T9;
    timming_ht = time_T10-time_T6;
    timming_hBlank = timming_hb+timming_hf+timming_hs;
    timming_hActive = timming_ht-timming_hBlank;


    // 计算参数
    double clk_time = (double)(time_T12-time_T11)/10;
    param_pclk = (double)1000000000000/((time_T12-time_T11)/10);
    param_hb = (double)timming_hb/clk_time;
    param_hf = (double)timming_hf/clk_time;
    param_hs = (double)timming_hs/clk_time;
    param_ht = (double)timming_ht/clk_time;
    param_hBlank = (double)timming_hBlank/clk_time;
    param_hActive = (double)timming_hActive/clk_time;

    param_vb = (double)timming_vb/timming_ht;
    param_vf = (double)timming_vf/timming_ht;
    param_vs = (double)timming_vs/timming_ht;
    param_vt = (double)timming_vt/timming_ht;
    param_vActive = (double)timming_vActive/timming_ht;
    param_vBlank = (double)timming_vBlank/timming_ht;
    param_hs_hps = (double)1000000000000/timming_ht;
    param_vs_fps = (double)1000000000000/timming_vt;
    param_t5_hbp = param_hb + param_hs;
    param_t5_hspw = param_hs;
    param_t5_vbp = param_vb + param_vs;
    param_t5_vspw = param_vs;


    ui->statusBar->setText("提示信息："+QString::number((time_T12-time_T11),10) + "  pclk" + QString::number(time_pclk,10));

    update_lineEdit_ParamData();
    update_lineEdit_TimmingData();

    ui->statusBar->setText("提示信息:pushButton Time OnClicked");
}

/**
 * @brief TimmingForm::update_lineEdit_ParamData
 * 使用数值，更新UI Param的显示
 */
void TimmingForm::update_lineEdit_ParamData()
{
    ui->lineEdit_Param_PCLK->setText( QString::number(param_pclk,10,0));
    ui->lineEdit_Param_VActive->setText(QString::number(param_vActive,10,3));
    ui->lineEdit_Param_VBlank->setText(QString::number(param_vBlank,10,3));
    ui->lineEdit_Param_VT->setText(QString::number(param_vt,10,3));
    ui->lineEdit_Param_VB->setText(QString::number(param_vb,10,3));
    ui->lineEdit_Param_VF->setText(QString::number(param_vf,10,3));
    ui->lineEdit_Param_VS->setText(QString::number(param_vs,10,3));
    ui->lineEdit_Param_HActive->setText(QString::number(param_hActive,10,3));
    ui->lineEdit_Param_HBlank->setText(QString::number(param_hBlank,10,3));
    ui->lineEdit_Param_HT->setText(QString::number(param_ht,10,3));
    ui->lineEdit_Param_HB->setText(QString::number(param_hb,10,3));
    ui->lineEdit_Param_HF->setText(QString::number(param_hf,10,3));
    ui->lineEdit_Param_HS->setText(QString::number(param_hs,10,3));
    ui->lineEdit_Param_HS_HPS->setText(QString::number(param_hs_hps,10,3));
    ui->lineEdit_Param_VS_FPS->setText(QString::number(param_vs_fps,10,3));
    ui->lineEdit_Param_t5_hbp->setText(QString::number(param_t5_hbp,10,3));
    ui->lineEdit_Param_t5_hspw->setText(QString::number(param_t5_hspw,10,3));
    ui->lineEdit_Param_t5_vbp->setText(QString::number(param_t5_vbp,10,3));
    ui->lineEdit_Param_t5_vspw->setText(QString::number(param_t5_vspw,10,3));
}

/**
 * @brief TimmingForm::update_lineEdit_TimmingData
 * 使用数值，更新UI Timming的显示
 */
void TimmingForm::update_lineEdit_TimmingData()
{

    ui->lineEdit_Timming_PCLK->setText( QString::number(timming_pclk,10) );
    ui->lineEdit_Timming_VBlank->setText( getZoomOut(QString::number(timming_vBlank,10),9) );
    ui->lineEdit_Timming_VActive->setText( getZoomOut(QString::number(timming_vActive,10),9) );
    ui->lineEdit_Timming_VT->setText(  getZoomOut(QString::number(timming_vt,10),9) );
    ui->lineEdit_Timming_VB->setText(  getZoomOut(QString::number(timming_vb,10),9) );
    ui->lineEdit_Timming_VF->setText(  getZoomOut(QString::number(timming_vf,10),9) );
    ui->lineEdit_Timming_VS->setText(  getZoomOut(QString::number(timming_vs,10),9) );
    ui->lineEdit_Timming_HBlank->setText( getZoomOut(QString::number(timming_hBlank,10),9) );
    ui->lineEdit_Timming_HActive->setText( getZoomOut(QString::number(timming_hActive,10),9) );
    ui->lineEdit_Timming_HT->setText(  getZoomOut(QString::number(timming_ht,10),9) );
    ui->lineEdit_Timming_HB->setText(  getZoomOut(QString::number(timming_hb,10),9) );
    ui->lineEdit_Timming_HF->setText(  getZoomOut(QString::number(timming_hf,10),9) );
    ui->lineEdit_Timming_HS->setText(  getZoomOut(QString::number(timming_hs,10),9) );
}


/**
 * @brief TimmingForm::update_lineEdit_TimeData
 * 使用数值，更新UI Frame的显示
 */
void TimmingForm::update_lineEdit_TimeData()
{
    ui->lineEdit_Time_T1->setText( getZoomOut(QString::number(time_T1,10),9) );
    ui->lineEdit_Time_T2->setText( getZoomOut(QString::number(time_T2,10),9) );
    ui->lineEdit_Time_T3->setText( getZoomOut(QString::number(time_T3,10),9) );
    ui->lineEdit_Time_T4->setText( getZoomOut(QString::number(time_T4,10),9) );
    ui->lineEdit_Time_T5->setText( getZoomOut(QString::number(time_T5,10),9) );
    ui->lineEdit_Time_T6->setText( getZoomOut(QString::number(time_T6,10),9) );
    ui->lineEdit_Time_T7->setText( getZoomOut(QString::number(time_T7,10),9) );
    ui->lineEdit_Time_T8->setText( getZoomOut(QString::number(time_T8,10),9) );
    ui->lineEdit_Time_T9->setText( getZoomOut(QString::number(time_T9,10),9) );
    ui->lineEdit_Time_T10->setText( getZoomOut(QString::number(time_T10,10),9) );
    ui->lineEdit_Time_T11->setText( getZoomOut(QString::number(time_T11,10),9) );
    ui->lineEdit_Time_T12->setText( getZoomOut(QString::number(time_T12,10),9) );
}


//获取放大后的字符串
QString TimmingForm::getZoomIn(QString str,int n)
{
    QString zoomStr;
    int index =  str.indexOf(".");
    if(index < 0)
    {
        index=0;
        zoomStr = str;
    }
    else
    {
        zoomStr+= str.left(index);
        index = str.length()-index-1;
        zoomStr += str.right(index);
    }
    for(int i=0; i < (n - index); i++)
        zoomStr+="0";
    return  zoomStr;
};
//获取缩小后的字符串
QString TimmingForm::getZoomOut(QString str,int n)
{
    if(str.isEmpty())
        return "";
    QString zoomStr = str;
    if(str.left(1) == "-")
    {
        zoomStr.remove(0,1);
    }
    if(n >0)
    {
        if(zoomStr.length() <= n)
        {
            int len = (n +1 - zoomStr.length() );
            for(int i =0 ; i < len;i++)
                zoomStr = "0"+zoomStr;
        }
        if(zoomStr.length() > n)
        {
            int index = zoomStr.length()-n;
            zoomStr.insert(index,".");
            for(int i =0 ;i < zoomStr.length(); i++)
            {
                index = zoomStr.length()-1- i;
                if(index < 0)
                    break;
                if(zoomStr.mid(index,1) == "0")
                {
                    zoomStr.remove(index,1);
                    i--;
                }
                else if(zoomStr.mid(index,1) == ".")
                {
                    zoomStr.remove(index,1);
                    break;
                }
                else
                {
                    break;
                }
            }
        }
    }
    if(str.left(1) == "-")
    {
        zoomStr.insert(0,"-");
    }
    return  zoomStr;
};

