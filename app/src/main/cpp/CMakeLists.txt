cmake_minimum_required(VERSION 3.10.2)

project("JYCVideoPlayer")
set(libname jyc-player)

include_directories(
    ${CMAKE_SOURCE_DIR}/ffmpeg/include
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/player/decoder
)

file(GLOB src-files
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/player/*.cpp
    ${CMAKE_SOURCE_DIR}/player/decoder/*.cpp
    )

add_library(
        ${libname}
        SHARED
        ${src-files}
)

set(third-party-libs
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        swresample
        swscale)

set(native-libs
        android
        EGL
        GLESv3
        OpenSLES
        log
        m
        z)


set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/ffmpeg/libs/${ANDROID_ABI})
add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec.so)
add_library(avdevice
        SHARED
        IMPORTED)
set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavdevice.so)
add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter.so)
add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavformat.so)
add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil.so)
add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample.so)
add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale.so)

target_link_libraries(
        ${libname}
        ${log-lib}
        ${third-party-libs}
        ${native-libs}
)