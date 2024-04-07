cmake_minimum_required(VERSION 3.5)

project(videoplay VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# include
include_directories(${PROJECT_SOURCE_DIR}/ffmpeg-N-102781-g05f9b3a0a5-win64-gpl-shared/include)

# lib
link_directories(${PROJECT_SOURCE_DIR}/ffmpeg-N-102781-g05f9b3a0a5-win64-gpl-shared/lib)
set(FFMPEG_LIB avcodec avdevice avfilter avformat avutil swscale postproc)

set(PROJECT_SOURCES
        main.cpp
        videoplay.cpp
        videoplay.h
        videoplay.ui

        ffmpeg.h
        ffmpeg.cpp

        mediaStream.h
        mediaStream.cpp
        mediaStream.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(videoplay
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        helpui/helpform.h helpui/helpform.cpp helpui/helpform.ui
    )
else()
    if(ANDROID)
        add_library(mediaStreamcppRED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(videoplay
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(videoplay PRIVATE ${FFMPEG_LIB} Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.videoplay)
endif()
set_target_properties(videoplay PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS videoplay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(videoplay)
endif()
