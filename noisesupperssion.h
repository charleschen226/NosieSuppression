#ifndef NOISESUPPERSSION_H
#define NOISESUPPERSSION_H

#include <QByteArray>

#include "webrtc_ns/signal_processing_library.h"
#include "webrtc_ns/noise_suppression_x.h"
#include "webrtc_ns/noise_suppression.h"
#include "samplerate.h"

//_RESAMPLE_
class ReSample
{
public:
    ReSample(int inputSamplerate, int outputSamplerate);

public:
    void init();
    QByteArray process(const QByteArray &inputData);

private:
    int         m_inputSample;
    int         m_outputSample;

    SRC_STATE   *m_state;
    double      m_ratio;
};


//_NOISEPROCESS_
class NoiseProcess
{
public:
    void init();
    QByteArray process(QByteArray &in);

private:
    int m_filter_state_1[6];
    int m_filter_state_2[6];
    int m_synthesis_state_1[6];
    int m_synthesis_state_2[6];

    NsxHandle *m_nsx;
};

//_NOISESUPPERSSION_
class NoiseSupperssion
{
public:
    NoiseSupperssion();

public:
    void init();
    QByteArray process(QByteArray &in);

private:
    void channelSeparate(QByteArray &in, QByteArray &left, QByteArray &right);
    QByteArray channelMerge(QByteArray &left, QByteArray &right);

private:
    NoiseProcess *m_handle[2];

    ReSample     *m_44_32;
    ReSample     *m_32_44;

    QByteArray   m_dataCache;
    QByteArray   m_dataCache_32;
    QByteArray   m_dataCache_32_Processed;
    QByteArray   m_dataCache_44;
};

#endif // NOISESUPPERSSION_H
