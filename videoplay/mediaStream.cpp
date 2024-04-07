#include "mediaStream.h"
#include "ui_mediaStream.h"
#include <QPushButton>

media_stream::media_stream(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::media_stream)
{
    ui->setupUi(this);
    set_window_icon("./res/aw5xs-wmg8w-001.ico");
    set_window_title("打开媒体");
    set_line_edit_placeholder("请输入网络URL");
}

media_stream::~media_stream()
{
    delete ui;
}

// 设置app 图标
void media_stream::set_window_icon(std::string path)
{
    this->setWindowIcon(QIcon(path.data()));
}

// 设置app title
void media_stream::set_window_title(std::string path)
{
    this->setWindowTitle(path.data());
}

void media_stream::set_line_edit_placeholder(std::string arg)
{
    ui->lineEdit->setPlaceholderText(arg.data());
}


void media_stream::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button == static_cast<QAbstractButton *>(ui->buttonBox->button(QDialogButtonBox::Ok))) {
        QString url = ui->lineEdit->text();
        sig_send_stream_url(url.toStdString());
    } else {
        this->close();
    }
}

