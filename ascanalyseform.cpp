#include "ascanalyseform.h"
#include "ui_ascanalyseform.h"
#include "datautil.h"
#include <QStandardItemModel>

AscAnalyseForm::AscAnalyseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AscAnalyseForm)
{
    ui->setupUi(this);

    /* 创建数据模型 */
    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    QStringList labels = QObject::trUtf8("信号ID,信号开始位,信号位长度, 方向TxRx").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);
    ui->tableView->setModel(model);

    // 有效数据的偏移位置
    int dataOffset = ui->lineEdit_signalDataOffset->text().toInt();
    int idOffset = ui->lineEdit_signalIDOffset->text().toInt();
}

AscAnalyseForm::~AscAnalyseForm()
{
    delete ui;
}


void AscAnalyseForm::dealOffsetByAscVersion()
{
    if( datalist.count() <= 0 )
    {
        pushButton_UpdateFile_OnCliecked();
        if( datalist.count() <= 0 )
        {
            ui->statusBar->setText("File Data Size is Zero, Reread File");
            return;
        }
    }
    QString version = DataUtil::getAscVersion(datalist);

    if( ui->checkBox_OffsetByVersion->isChecked() )
    {
        if( version.contains("13.") )
        {
            dataOffset = 10;
            idOffset = 4;
            ui->lineEdit_signalDataOffset->setText(QString::number(dataOffset));
            ui->lineEdit_signalIDOffset->setText(QString::number(idOffset));
        }
        else if( version.contains("7.") )
        {
            dataOffset = 10;
            idOffset = 3;
            ui->lineEdit_signalDataOffset->setText(QString::number(dataOffset));
            ui->lineEdit_signalIDOffset->setText(QString::number(idOffset));
        }
        else
        {
            dataOffset = ui->lineEdit_signalDataOffset->text().toInt();
            idOffset = ui->lineEdit_signalIDOffset->text().toInt();
        }
    }
    else
    {
        dataOffset = ui->lineEdit_signalDataOffset->text().toInt();
        idOffset = ui->lineEdit_signalIDOffset->text().toInt();
    }

    ui->statusBar->setText("asc版本：" + version + " dataOffset:" + QString::number(dataOffset) + " idOffset:" + QString::number(idOffset));
}

void AscAnalyseForm::setupPlot(QString name, int lineNum, QVector<double> times, QVector<double> values)
{
    // The following plot setup is mostly taken from the plot demos:
    ui->plot->addGraph();
    ui->plot->graph()->setLineStyle(QCPGraph::lsStepLeft); // 线的类型
    ui->plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));    // 每个数据绘制一个示意点
    ui->plot->graph()->setPen(QPen(QColor(qrand()%100+100, qrand()%255, qrand()%255, 255))); // 线的颜色
    ui->plot->graph(lineNum)->setData(times, values);
    ui->plot->graph(lineNum)->setName(name);
    ui->plot->axisRect()->setupFullAxesBox(false);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    // 显示图例
    ui->plot->legend->setVisible(true);
    ui->plot->legend->setFont(QFont("Helvetica",9));
}


void AscAnalyseForm::pushButton_UpdateFile_OnCliecked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Asc"), ".", tr("Can Asc File(*.asc)"));
    ui->statusBar->setText("File Asc fileName :" + fileName);
    ui->lineEdit->setText(fileName);
    datalist.clear();
    datalist = DataUtil::readAscFile(ui->lineEdit->text());
    qDebug() << "file:" << ui->lineEdit->text() << "datalist.count():" << datalist.count();
}

void AscAnalyseForm::pushButton_UpdateSignal_OnCliecked()
{
    QVector<double> times;
    QVector<double> values;
    int count;
    double valMin = 1000000;
    double valMax = -1000000;

    double timMin = 1000000;
    double timMax = -1000000;

    if( datalist.count() <= 0 )
    {
        pushButton_UpdateFile_OnCliecked();
        if( datalist.count() <= 0 )
        {
            ui->statusBar->setText("File Data Size is Zero, Reread File");
            return;
        }
    }

    dealOffsetByAscVersion();

    for (int i = ui->plot->plotLayout()->elementCount() - 1; i > 0; i--)
    {
        ui->plot->plotLayout()->removeAt(i);
    }
    for (int i = ui->plot->plottableCount() - 1; i > 0; i--)
    {
        ui->plot->removePlottable(i);
    }
    for (int i = ui->plot->graphCount() - 1; i >= 0; i--)
    {
        ui->plot->removeGraph(i);
    }


    for ( int i = 0; i < model->rowCount(); i++)
    {

        int signalID = model->item(i,0)->text().toInt(nullptr, 16);
        int signalOffset = model->item(i,1)->text().toInt();
        int signalLen = model->item(i,2)->text().toInt();
        QString txrx = model->item(i,3)->text();
        count = DataUtil::readAscSignal(datalist, dataOffset, idOffset, signalID, signalOffset, signalLen, &times, &values, txrx.contains("Tx"), txrx.contains("Rx"));
        if( count > 0 )
        {
            QString sigName = QString::number(i)+" 0x"+QString::number(signalID,16)+" bit"+QString::number(signalOffset)+ "-" + QString::number(signalLen);
            setupPlot(sigName, i, times, values);

            foreach (double val, times)
            {
                if( timMin > val )
                {
                    timMin = val;
                }
                if( timMax < val )
                {
                    timMax = val;
                }
            }

            foreach (double val, values)
            {
                if( valMin > val )
                {
                    valMin = val;
                }
                if( valMax < val )
                {
                    valMax = val;
                }
            }
            qDebug() << "timMin=" << timMin << " timMax:" << timMax;
            qDebug() << "valMin=" << valMin << " valMax:" << valMax;
            ui->plot->xAxis->setRange(timMin-1, timMax+1);
            ui->plot->yAxis->setRange(valMin-1, valMax+1);
            ui->plot->replot();
        }
        else
        {
            ui->statusBar->setText("num:" + QString::number(i) + " data size is empty : " + QString::number(signalID, 16));
        }
    }
}

/**
* @brief AscAnalyseForm::pushButton_AddSignal_OnCliecked
* 将数据添加到tableView中
*/
void AscAnalyseForm::pushButton_AddSignal_OnCliecked()
{
    QString txrx;
    int i = model->rowCount();
    model->setItem(i, 0, new QStandardItem(ui->lineEdit_signalID->text()));
    model->setItem(i, 1, new QStandardItem(ui->lineEdit_signalOffset->text()));
    model->setItem(i, 2, new QStandardItem(ui->lineEdit_signalLen->text()));

    if( ui->checkBox_Rx->isChecked() )
    {
        txrx += "Rx";
    }

    if( ui->checkBox_Tx->isChecked() )
    {
        txrx += "Tx";
    }
    model->setItem(i, 3, new QStandardItem(txrx));
}

/**
 * @brief AscAnalyseForm::pushButton_DelSignal_OnCliecked
 * 删除选中的行数据
 */
void AscAnalyseForm::pushButton_DelSignal_OnCliecked()
{
    if( ui->tableView->currentIndex().row() >= 0 )
    {
        model->removeRow(ui->tableView->currentIndex().row());
    }
}

void AscAnalyseForm::wheelEvent(QWheelEvent *event)
{
    static int i=0;
    i++;

    if (ui->plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
         ui->plot->axisRect()->setRangeZoom(ui->plot->xAxis->orientation());
    }
    else if (ui->plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        ui->plot->axisRect()->setRangeZoom(ui->plot->yAxis->orientation());
    }
    else
    {
        ui->plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    }
}
