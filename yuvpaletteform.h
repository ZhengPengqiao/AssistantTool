#ifndef YUVPALETTEFORM_H
#define YUVPALETTEFORM_H

#include <QWidget>

namespace Ui {
class YUVPaletteForm;
}

class YUVPaletteForm : public QWidget
{
    Q_OBJECT

public:
    explicit YUVPaletteForm(QWidget *parent = 0);
    ~YUVPaletteForm();

private:
    Ui::YUVPaletteForm *ui;
};

#endif // YUVPALETTEFORM_H
