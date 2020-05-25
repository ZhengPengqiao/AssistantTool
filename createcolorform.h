#ifndef CREATECOLORFORM_H
#define CREATECOLORFORM_H

#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QImageWriter>

namespace Ui {
class CreateColorForm;
}

class CreateColorForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreateColorForm(QWidget *parent = nullptr);
    ~CreateColorForm();
    void frame_render(int frameCount);
    QString fmtToQString(int fmt);
    int createColor();
    QString getExternalFileFormat(QString fmt);

    enum{
        FMT_NV12,
        FMT_UYVY,
        FMT_YUYV,
    };

    int frameSize;
    int width;
    int height;
    QString fileName;
    int frameCount;
    int fmt;
    char *frameBuff;
    int fb;

public slots:
    void createYuvFile();
    void comboBox_Format_IndexChange(QString str);
    void spinBox_frameNum_OnClick(int val);
    void spinBox_width_OnClick(int val);
    void spinBox_height_OnClick(int val);
    void pushButton_fileSelect_OnClick();
    void lineEdit_fileName_OnChange(QString);

private:
    Ui::CreateColorForm *ui;
};

#endif // CREATECOLORFORM_H
