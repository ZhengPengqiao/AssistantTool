#ifndef TIMMINGFORM_H
#define TIMMINGFORM_H

#include <QWidget>
#include <QList>

namespace Ui {
class TimmingForm;
}

struct lcd_param{
    QString name;
    double param_pclk;
    double param_vt;
    double param_vBlank;
    double param_vActive;
    double param_vb;
    double param_vf;
    double param_vs;
    double param_ht;
    double param_hBlank;
    double param_hActive;
    double param_hb;
    double param_hf;
    double param_hs;
    double param_hs_hps;
    double param_vs_fps;
    double param_t5_hbp;
    double param_t5_hspw;
    double param_t5_vbp;
    double param_t5_vspw;
};

class TimmingForm : public QWidget
{
    Q_OBJECT

public:
    explicit TimmingForm(QWidget *parent = 0);
    ~TimmingForm();


    void loadFileToCombo();
    void saveComboToFile();
    //获取放大后的字符串
    QString getZoomIn(QString str,int n);
    //获取缩小后的字符串
    QString getZoomOut(QString str,int n);
    //使用数值，更新UI Param的显示
    void update_lineEdit_ParamData();
    //使用数值，更新UI Timming的显示
    void update_lineEdit_TimmingData();
    //使用数值，更新UI Time的显示
    void update_lineEdit_TimeData();

    void showParam();
public slots:
    void pushButton_SaveParam_OnClicked();
    void pushButton_Timming_OnClicked();
    void pushButton_Param_OnClicked();
    void pushButton_Time_OnClicked();
    void pushButton_AddComm_OnClicked();
    void pushButton_Comm_CurrectItemChange_OnClicked(int i);
    void pushButton_RemoveComm_OnClicked();

private:


    // 参数
    double param_pclk;
    double param_vt;
    double param_vBlank;
    double param_vActive;
    double param_vb;
    double param_vf;
    double param_vs;
    double param_ht;
    double param_hBlank;
    double param_hActive;
    double param_hb;
    double param_hf;
    double param_hs;
    double param_hs_hps;
    double param_vs_fps;
    double param_t5_hbp;
    double param_t5_hspw;
    double param_t5_vbp;
    double param_t5_vspw;


    // 时序 ps, 1S=1000 000 000 000ns
    long long int timming_pclk;
    long long int timming_vt;
    long long int timming_vBlank;
    long long int timming_vActive;
    long long int timming_vb;
    long long int timming_vf;
    long long int timming_vs;
    long long int timming_ht;
    long long int timming_hBlank;
    long long int timming_hActive;
    long long int timming_hb;
    long long int timming_hf;
    long long int timming_hs;

    // 时间轴 ps, 1S=1000 000 000 000ns
    long long int time_pclk;
    long long int time_T1;
    long long int time_T2;
    long long int time_T3;
    long long int time_T4;
    long long int time_T5;
    long long int time_T6;
    long long int time_T7;
    long long int time_T8;
    long long int time_T9;
    long long int time_T10;
    long long int time_T11;
    long long int time_T12;

    QList<struct lcd_param*> list;

private:
    Ui::TimmingForm *ui;
};

#endif // TIMMINGFORM_H
