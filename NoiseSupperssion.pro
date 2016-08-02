#-------------------------------------------------
#
# Project created by QtCreator 2016-07-27T10:48:11
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoiseSupperssion
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    webrtc_ns/complex_bit_reverse.c \
    webrtc_ns/complex_fft.c \
    webrtc_ns/copy_set_operations.c \
    webrtc_ns/cross_correlation.c \
    webrtc_ns/division_operations.c \
    webrtc_ns/downsample_fast.c \
    webrtc_ns/energy.c \
    webrtc_ns/fft4g.c \
    webrtc_ns/get_scaling_square.c \
    webrtc_ns/min_max_operations.c \
    webrtc_ns/noise_suppression.c \
    webrtc_ns/noise_suppression_x.c \
    webrtc_ns/ns_core.c \
    webrtc_ns/nsx_core.c \
    webrtc_ns/nsx_core_c.c \
    webrtc_ns/nsx_core_neon_offsets.c \
    webrtc_ns/real_fft.c \
    webrtc_ns/resample.c \
    webrtc_ns/resample_48khz.c \
    webrtc_ns/resample_by_2.c \
    webrtc_ns/resample_by_2_internal.c \
    webrtc_ns/resample_by_2_mips.c \
    webrtc_ns/resample_fractional.c \
    webrtc_ns/ring_buffer.c \
    webrtc_ns/spl_init.c \
    webrtc_ns/spl_sqrt.c \
    webrtc_ns/spl_sqrt_floor.c \
    webrtc_ns/splitting_filter.c \
    webrtc_ns/vector_scaling_operations.c \
    noisesupperssion.cpp


HEADERS  += mainwindow.h \
    webrtc_ns/complex_fft_tables.h \
    webrtc_ns/cpu_features_wrapper.h \
    webrtc_ns/defines.h \
    webrtc_ns/fft4g.h \
    webrtc_ns/noise_suppression.h \
    webrtc_ns/noise_suppression_x.h \
    webrtc_ns/ns_core.h \
    webrtc_ns/nsx_core.h \
    webrtc_ns/nsx_defines.h \
    webrtc_ns/real_fft.h \
    webrtc_ns/resample_by_2_internal.h \
    webrtc_ns/ring_buffer.h \
    webrtc_ns/signal_processing_library.h \
    webrtc_ns/spl_inl.h \
    webrtc_ns/typedefs.h \
    webrtc_ns/windows_private.h \
    samplerate.h \
    config.h \
    noisesupperssion.h



FORMS    += mainwindow.ui
LIBS += "D:/workspace/NoiseSupperssion/libsamplerate-0.1.8/src/.libs/libsamplerate-0.dll"\

