#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class media_stream;
}

class media_stream : public QDialog
{
    Q_OBJECT

public:
    explicit media_stream(QWidget *parent = nullptr);
    ~media_stream();

private slots:

    void on_buttonBox_clicked(QAbstractButton *button);

signals:
    void sig_send_stream_url(std::string);

private:
    Ui::media_stream *ui;

    void set_window_icon(std::string arg);

    void set_window_title(std::string arg);

    void set_line_edit_placeholder(std::string arg);
};

#endif // MEDIASTREAM_H
