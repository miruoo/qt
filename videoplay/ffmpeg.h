#ifndef FFMPEG_H
#define FFMPEG_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
}

#include <string>
#include <QThread>
#include <QImage>

struct AVGuard
{
    AVFormatContext     *avformat_ctx;  // i/o上下文
    const AVCodec       *avcodec;       // 解码器
    AVCodecContext      *avcodec_ctx;   // 解码器上下文
    AVPacket            *avpkt;         // 存储接收的原始帧
    AVFrame             *avframe;       // 存储解码后的数据，如：YUV\RGB
    AVFrame             *avframe_rgb;   // ...
    struct SwsContext   *sws_ctx;       // 转换图像大小、格式
    uint8_t             *outBuf;        // 绑定到转换后avframe的data；这里即avframe_rgb->data

    AVGuard() {
        avformat_network_init();
    }
    ~AVGuard() {
        if (avformat_ctx) {
            avformat_close_input(&avformat_ctx);
            avformat_free_context(avformat_ctx);
            avformat_ctx = nullptr;
        }
        if (avcodec_ctx) {
            avcodec_close(avcodec_ctx);
            avcodec_free_context(&avcodec_ctx);
            avcodec_ctx = nullptr;
        }
        if (avpkt) {
            av_packet_unref(avpkt);
            av_packet_free(&avpkt);
            avpkt = nullptr;
        }
        if (avframe) {
            av_frame_unref(avframe);
            av_frame_free(&avframe);
            avframe = nullptr;
        }
        if (avframe_rgb) {
            av_frame_unref(avframe_rgb);
            av_frame_free(&avframe_rgb);
            avframe_rgb = nullptr;
        }
        if (sws_ctx) {
            sws_freeContext(sws_ctx);
            sws_ctx = nullptr;
        }
        if (outBuf) {
            av_free(outBuf);
            outBuf = nullptr;
        }
        avformat_network_deinit();
    }
};

class ffmpeg : public QThread
{
    Q_OBJECT
public:
    ffmpeg();
    ~ffmpeg();

    void start_play();

    void stop_play();

    std::string get_version_info();

    void set_url(std::string path);

protected:
    void run();

signals:
    void sig_send_one_frame(QImage);

private:
    std::string m_url;
    bool m_is_running;
};

#endif // FFMPEG_H
