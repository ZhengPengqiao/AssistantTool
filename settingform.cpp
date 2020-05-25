#include "settingform.h"
#include "ui_settingform.h"
#include <QDebug>

static SettingForm *inst = nullptr;

SettingForm::SettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    version = ui->label_version->text();
}

SettingForm::~SettingForm()
{
    delete ui;
}


SettingForm *SettingForm::instances()
{
    qDebug() << "getSetting";
    if( inst == nullptr )
    {
        qDebug() << "inst is null, and new SettingForm()";
        inst = new SettingForm();
    }
    return inst;
}
