#ifndef HELPFORM_H
#define HELPFORM_H

#include <QWidget>

namespace Ui {
class helpform;
}

class helpform : public QWidget
{
    Q_OBJECT

public:
    explicit helpform(QWidget *parent = nullptr);
    ~helpform();

    void show(QString);

private:
    Ui::helpform *ui;

    QString m_file_name;
};

#endif // HELPFORM_H
