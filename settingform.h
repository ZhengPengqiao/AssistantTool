#ifndef SETTINGFORM_H
#define SETTINGFORM_H

#include <QWidget>
#include <QString>

namespace Ui {
class SettingForm;
}

class SettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingForm(QWidget *parent = nullptr);
    ~SettingForm();
    static SettingForm *instances();
    QString version;

private:
    Ui::SettingForm *ui;
};

#endif // SETTINGFORM_H
