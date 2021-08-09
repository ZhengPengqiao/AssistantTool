#ifndef ASCANALYSEFORM_H
#define ASCANALYSEFORM_H

#include <QWidget>
#include <QStringList>
#include <QStandardItemModel>

namespace Ui {
class AscAnalyseForm;
}

class AscAnalyseForm : public QWidget
{
    Q_OBJECT

public:
    explicit AscAnalyseForm(QWidget *parent = 0);
    ~AscAnalyseForm();

    void setupPlot(QString name, int lineNum, QVector<double> times, QVector<double> values);

    void dealOffsetByAscVersion();
    int dataOffset;
    int idOffset;

public slots:
    void pushButton_UpdateFile_OnCliecked();
    void pushButton_UpdateSignal_OnCliecked();
    void pushButton_AddSignal_OnCliecked();
    void pushButton_DelSignal_OnCliecked();
    void wheelEvent(QWheelEvent *event);

private:
    Ui::AscAnalyseForm *ui;
    QStringList datalist;
    QStandardItemModel* model;
};

#endif // ASCANALYSEFORM_H
