# 简易视频播放器(QT+ffmpeg)
1. name:简易视频播放器
2. describe:播放本地视频文件及网络流媒体

## 环境搭建
1. 下载ffmpeg windos官方编译好得库文件(百度网盘见) 或者下载源码编译。注：ffmpeg源码编译比较复杂。
2. 添加qt widget工程，选择cmake编译工具链(qmake在qt6中已弃用)，cmake添加ffmpeg库。
3. 引用库函数av_version_info, 输出：N-102781-g05f9b3a0a5。至此，环境搭建完成。

## ffmpeg简单介绍
FFmpeg是一套可以用来记录、转换数字音频、视频，并能将其转化为流的开源计算机程序。采用LGPL或GPL许可证。它提供了录制、转换以及流化音视频的完整解决方案。

### 1. ffmpeg组件介绍
1. libavutil核心工具库，包含用于简化编程的函数，包括随机数生成器、数据结构、数学例程、核心多媒体实用程序等。
2. libavcodec编解码库，包含用于音频/视频编解码器的解码器和编码器。
3. libavformat容器库，包含用于多媒体容器格式的解复用器和复用器，封装了各种容器和协议。
4. libavdevice设备录制播放，包含输入和输出设备，用于从许多常见的多媒体输入/输出软件框架（包括Video4Linux、Video4Linux2、VfW和ALSA）中获取和呈现这些设备。
5. libavfilter是一个包含媒体过滤器的库，提供了一个通用的音频、视频、字幕等滤镜处理框架。
6. libswscale是一个执行高度优化的图像缩放和颜色空间/像素格式转换操作的库。
7. libswresample是一个执行高度优化的音频重采样、重矩阵化和采样格式转换操作的库。
8. libpostproc用于后期效果处理。

### 2. FFMpeg播放视频文件的标准流程 [流程图](./ffmpeg_flow.png)

## qt
使用QLabel控件显示图片，QMenu作为控制按钮，ffmpeg解码视频为rgb数据传输给到QLabel显示。整个widget使用布局(Layout)，使得QLabel自适应widget窗口。注意：QLabel刷新率低，不适合高频率刷新的视频，可以换成QPainter。
### 控件
	1. QLabel	显示文本或图像的控件。它通常被用作标签或显示静态信息的区域。
	2. QPainter	用于进行绘图操作的类。
	3. QPushButton 常规按键。
	4. QMenu 	菜单栏。
	5. QFileDialog 选择文件对话框。
	6. QDialogButtonBox	用于显示一组标准窗口操作按钮的小部件，如“确定”、“取消”、“应用”按钮等。
### 信号槽
类似于事件队列，connect向队列中注册事件，并绑定id, emit触发该id,绑定至该id中的事件被触发。

### 打包发布
1. release编译，生成exe文件。
2. 生成的exe文件拷贝到空目录，Windows cmd在该目录下执行windeployqt .\xxx.exe，生成需要的dll文件。
3. 拷贝ffmpeg dll至该目录中即可正常运行。
4. windeployqt.exe是Qt自带的工具，用于创建应用程序发布包。 简单来说，这个工具可以自动地将某程序依赖的库、资源拷贝到其所在目录，防止程序在其他电脑上运行报找不到库的错误。