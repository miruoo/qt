#include "ffmpeg.h"
#include <QDebug>


using namespace std;


ffmpeg::ffmpeg()
    : m_is_running(false)
{

}

ffmpeg::~ffmpeg()
{
    this->quit();
}


string ffmpeg::get_version_info()
{
    return av_version_info();
}

void ffmpeg::set_url(std::string path)
{
    m_url = path;
}

void ffmpeg::start_play()
{
//    m_url = "rtsp://admin:admin123@192.168.198.125:554/cam/realmonitor?channel=1&amp;subtype=0&amp;unicast=true&amp;proto=Onvif";
//    m_url = "D:/qtpro/videoplay/res/admiral.264";
    if (isRunning()) {
        stop_play();
        QThread::sleep(4);
    }
    m_is_running = true;
    this->start();
}

void ffmpeg::stop_play()
{
    if (isRunning()) {
        m_is_running = false;
        this->quit();
        this->wait();
    }
}

void ffmpeg::run()
{
    AVGuard guard;
    memset(&guard, 0, sizeof(guard));

    // 解复用
    qDebug("av_read_frame url = %s", m_url.data());
    guard.avformat_ctx = avformat_alloc_context();
    if(avformat_open_input(&guard.avformat_ctx, m_url.data(), nullptr, nullptr) != 0) {
        qDebug("avformat_open_input error");
        return;
    }

    if (avformat_find_stream_info(guard.avformat_ctx, nullptr) < 0) {
        qDebug("avformat_find_stream_info error");
        return;
    }

    int video_index = av_find_best_stream(guard.avformat_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_index < 0) {
        qDebug("find viedo stream error");
        return;
    }

    guard.avcodec = avcodec_find_decoder(guard.avformat_ctx->streams[video_index]->codecpar->codec_id);
    if (guard.avcodec == nullptr) {
        qDebug("avcodec_find_decoder error");
        return;
    }

    guard.avcodec_ctx = avcodec_alloc_context3(guard.avcodec);
    avcodec_parameters_to_context(guard.avcodec_ctx, guard.avformat_ctx->streams[video_index]->codecpar);
    if (avcodec_open2(guard.avcodec_ctx, guard.avcodec, nullptr) < 0) {
        qDebug("avcodec_open2 error");
        return;
    }


    guard.avpkt = av_packet_alloc();
    if (guard.avpkt == nullptr) {
        qDebug("avpkt error");
    }
    guard.avframe = av_frame_alloc();
    if (guard.avpkt == nullptr) {
        qDebug("avframe error");
    }
    guard.avframe_rgb = av_frame_alloc();
    if (guard.avframe_rgb == nullptr) {
        qDebug("avframe_rgb error");
    }

    int size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, guard.avcodec_ctx->width, guard.avcodec_ctx->height, 1);
    guard.outBuf = static_cast<uint8_t *>(av_malloc(static_cast<size_t>(size)));
    if (av_image_fill_arrays(guard.avframe_rgb->data, guard.avframe_rgb->linesize, guard.outBuf, AV_PIX_FMT_RGB24, guard.avcodec_ctx->width, guard.avcodec_ctx->height, 1) < 0) {
        qDebug("av_image_fill_arrays error");
        return;
    }

    guard.sws_ctx = sws_getContext(guard.avcodec_ctx->width, guard.avcodec_ctx->height, guard.avcodec_ctx->pix_fmt, guard.avcodec_ctx->width, guard.avcodec_ctx->height,\
                             AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr, nullptr);

    while (m_is_running) {
        if (av_read_frame(guard.avformat_ctx, guard.avpkt) == 0) {
            qDebug("stream index = %d", guard.avpkt->stream_index);
            if (guard.avpkt->stream_index == video_index) {
                int ret = avcodec_send_packet(guard.avcodec_ctx, guard.avpkt);
                if (ret != 0) {
                    qDebug("avcodec_send_packet error");
                    return;
                }
                ret = avcodec_receive_frame(guard.avcodec_ctx, guard.avframe);
                if (ret != 0) {
                    qDebug("avcodec_receive_frame error, ret = %d", ret);
                    return;
                }
                sws_scale(guard.sws_ctx, static_cast<const uint8_t* const*>(guard.avframe->data), guard.avframe->linesize, 0, \
                                guard.avcodec_ctx->height, guard.avframe_rgb->data, guard.avframe_rgb->linesize);

//                qDebug("avcodec_receive_frame ok, width = %d,  height = %d", guard.avcodec_ctx->width, guard.avcodec_ctx->height);
                QImage img((uchar*)guard.avframe_rgb->data[0], guard.avcodec_ctx->width, guard.avcodec_ctx->height, QImage::Format_RGB888);
                emit sig_send_one_frame(img);
            }
            av_packet_unref(guard.avpkt);
        } else {
            qDebug("av_read_frame error");
            return;
        }
        QThread::msleep(40);
    }

    return;
}
