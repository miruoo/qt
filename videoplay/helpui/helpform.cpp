#include "helpform.h"
#include "ui_helpform.h"
//#include <qpdf>

helpform::helpform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::helpform)
{
    ui->setupUi(this);
}

helpform::~helpform()
{
    delete ui;
}


void helpform::show(QString str)
{
//    qpdf
}
