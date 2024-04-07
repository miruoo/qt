#ifndef VIDEOPLAY_H
#define VIDEOPLAY_H

#include <QMainWindow>
#include <QImage>
#include "ffmpeg.h"
#include "mediaStream.h"
#include "helpui/helpform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class videoPlay; }
QT_END_NAMESPACE

class videoPlay : public QMainWindow
{
    Q_OBJECT

public:
    videoPlay(QWidget *parent = nullptr);
    ~videoPlay();


private slots:

    void solt_get_one_frame(QImage image);

    void on_action_F_triggered();

    void on_action_N_triggered();

    void solt_get_stream_url(std::string url);

    void on_action_readme_triggered();

    void on_action_version_triggered();

private:
    void set_window_icon(QString path);

    void set_label_style(int r = 0, int g = 0, int b = 0);

    void set_label_scale(bool is_scale = true);

    void start();

    Ui::videoPlay *ui;

    ffmpeg m_ffmpeg;

    media_stream m_media_stream;

    helpform m_helpform;

    QString m_cur_path;
};
#endif // VIDEOPLAY_H
