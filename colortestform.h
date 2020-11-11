#ifndef COLORTESTFORM_H
#define COLORTESTFORM_H

#include <QWidget>

namespace Ui {
class ColorTestForm;
}

class ColorTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit ColorTestForm(QWidget *parent = 0);
    ~ColorTestForm();
    int clearFrameBuff(char* indata, int x, int y, int w, int h, int r, int g, int b, int bps);
    int testColor(char * indata, int x, int y, int w, int h, int br, int bg, int bb, int bps);
    int checkerBoard(char *indata, int x, int y, int w, int h, int bps);
    int testSolidColor(char * indata, int x, int y, int w, int h, int br, int bg, int bb, int bps);
    int testDefinitionColor(char * indata, int x, int y, int w, int h, int br, int bg, int bb, int bps);
    int testUrandom(char *indata, int x, int y, int w, int h, int bps);
    QString toExternalFormat(QString fmt);
    int toBps(QString fmt);
    QImage::Format toQtFormat(QString fmt);
    int toImageFormat(QString str);
    int toPixelBytes(QString str);

public:
    int r;
    int g;
    int b;
    int a;
    int uc;
    int l_num;
    int xres;
    int yres;
    int dataSize;
    char *data;
    QString fileName;
    int fb;

public slots:
    void pushButton_updateShow_onClicked();
    void pushButton_savefile_onClicked();
    void pushButton_fileSelect_OnClick();
    void lineEdit_fileName_OnChange(QString str);
    void comboBox_Format_onChange(QString str);
    void comboBox_imageFormat_onClicked(QString str);

private:
    Ui::ColorTestForm *ui;
};

#endif // COLORTESTFORM_H
