#include "videoplay.h"
#include "./ui_videoplay.h"
#include <QPixmap>
#include <QFileDialog>
#include <QSizePolicy>


class QTimer;
class QPdfPageSelector;
class QPdfView;
class QPdfDocument;
videoPlay::videoPlay(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::videoPlay)
    , m_cur_path(QDir::currentPath())
{
    ui->setupUi(this);

    QString win_ico = m_cur_path + "/res/aw5xs-wmg8w-001.ico";

    set_window_icon(win_ico);
    set_label_style(0,0,0);
    set_label_scale(true);

    QPdfPageSelector *m_pageSelector;

    connect(&m_ffmpeg, &ffmpeg::sig_send_one_frame, this, &videoPlay::solt_get_one_frame);
    connect(&m_media_stream, &media_stream::sig_send_stream_url, this, &videoPlay::solt_get_stream_url);
}

videoPlay::~videoPlay()
{
    m_ffmpeg.stop_play();
    delete ui;
}

// 设置app 图标
void videoPlay::set_window_icon(QString path)
{
    this->setWindowIcon(QIcon(path));
}

// 设置label 样式表(背景色)
void videoPlay::set_label_style(int r, int g, int b)
{
//    ui->label->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    QString str = QString("QLabel{background-color:rgb(%1,%2,%3);}").arg(r).arg(g).arg(b);
//    qDebug("str: %s", str.toStdString().data());
    ui->label->setStyleSheet(str);
}

// 设置label 缩放
void videoPlay::set_label_scale(bool is_scale)
{
    ui->label->setScaledContents(is_scale);
}

// 更新图片
void videoPlay::solt_get_one_frame(QImage img)
{
//    qDebug("width: %d   height: %d", img.width(), img.height());
    ui->label->setPixmap(QPixmap::fromImage(img));
    this->resize(img.width(), img.height());
//    ui->label->resize(QSize(img.width(), img.height()));
}


void videoPlay::start()
{
    m_ffmpeg.get_version_info();
    m_ffmpeg.start_play();
}

// 槽信号: 打开文件
void videoPlay::on_action_F_triggered()
{
    QString path = m_cur_path + "/res";
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("open file"),
                                                     path,
                                                     tr("*.mp4 *.264"));
    if (file_name.isEmpty()) {
        qDebug("open file error");
        return;
    }

    m_ffmpeg.set_url(file_name.toStdString().data());
    this->start();
}

//槽信号: 打开网络串流
void videoPlay::on_action_N_triggered()
{
    m_media_stream.show();
}

void videoPlay::solt_get_stream_url(std::string url)
{
    if (url.empty()) {
        qDebug("输入url错误");
        return;
    }
    m_media_stream.close();
    m_ffmpeg.set_url(url.data());
    this->start();
}

//槽信号: 打开readme文档
void videoPlay::on_action_readme_triggered()
{

}

//槽信号: 打开版本文档
void videoPlay::on_action_version_triggered()
{
//    m_helpform.show();
}

