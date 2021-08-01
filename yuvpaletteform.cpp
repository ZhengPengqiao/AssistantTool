#include "yuvpaletteform.h"
#include "ui_yuvpaletteform.h"

YUVPaletteForm::YUVPaletteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::YUVPaletteForm)
{
    ui->setupUi(this);
}

YUVPaletteForm::~YUVPaletteForm()
{
    delete ui;
}
