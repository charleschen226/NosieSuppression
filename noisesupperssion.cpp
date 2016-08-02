#include "noisesupperssion.h"


enum {Mild, Medium, Aggressive};


//_RESAMPLE_
ReSample::ReSample(int inputSamplerate, int outputSamplerate)
    : m_inputSample(inputSamplerate)
    , m_outputSample(outputSamplerate)
    , m_state(NULL)
{

}

void ReSample::init()
{
    int err;
    m_state = src_new(SRC_SINC_BEST_QUALITY, 2, &err);
    if (!m_state)
        return;

    m_ratio = double(m_outputSample) / double(m_inputSample);
}

QByteArray ReSample::process(const QByteArray &inputData)
{
    int inFrame = inputData.size() / 4;
    int outFrame = int((double(inFrame) * m_ratio) + 0.5);

    static float *inData = new float[44100];
    static float *outData = new float[44100];
    static short *finalData = new short[44100];

    src_short_to_float_array((short*) (inputData.data()), inData, inFrame * 2);

    SRC_DATA src_data;
    src_data.end_of_input = 0 ;
    src_data.data_in = inData ;
    src_data.input_frames = inFrame;
    src_data.data_out = outData;
    src_data.output_frames = outFrame;
    src_data.src_ratio = m_ratio ;

    src_process (m_state, &src_data);

    src_float_to_short_array(outData, finalData, outFrame * 2);
    QByteArray outputData = QByteArray((char*)finalData, outFrame * 2 * 2);

    return outputData;
}


//_NOISEPROCESS_
void NoiseProcess::init()
{
    memset(m_filter_state_1, 0, sizeof(m_filter_state_1));
    memset(m_filter_state_2, 0, sizeof(m_filter_state_2));
    memset(m_synthesis_state_1, 0, sizeof(m_synthesis_state_1));
    memset(m_synthesis_state_2, 0, sizeof(m_synthesis_state_2));

    WebRtcNsx_Create(&m_nsx);
    WebRtcNsx_Init(m_nsx, 32000);
    WebRtcNsx_set_policy(m_nsx, Aggressive);
}

QByteArray NoiseProcess::process(QByteArray &in)
{
    static short shBufferIn[320];

    static short shInL[160];
    static short shInH[160];
    static short shOutL[160];
    static short shOutH[160];

    memcpy(shBufferIn, (char*)in.data(), 320 * sizeof(short));
    WebRtcSpl_AnalysisQMF(shBufferIn, 320, shInL, shInH, m_filter_state_1, m_filter_state_2);

    if (0 == WebRtcNsx_Process(m_nsx, shInL, shInH, shOutL, shOutH)) {
        short shBufferOut[320];
        WebRtcSpl_SynthesisQMF(shOutL, shOutH, 160, shBufferOut, m_synthesis_state_1, m_synthesis_state_2);
        return QByteArray((char*)shBufferOut, 320 * sizeof(short));
    }
}


//_NOISESUPPERSSION_
NoiseSupperssion::NoiseSupperssion()
{
    m_44_32 = new ReSample(44100, 32000);
    m_32_44 = new ReSample(32000, 44100);

    m_44_32->init();
    m_32_44->init();
}

void NoiseSupperssion::init()
{
    NoiseProcess *handle1 = new NoiseProcess;
    NoiseProcess *handle2 = new NoiseProcess;

    handle1->init();
    handle2->init();

    m_handle[0] = handle1;
    m_handle[1] = handle2;
}

QByteArray NoiseSupperssion::process(QByteArray &in)
{
    m_dataCache.append(in);
    if (true) {
        int size = m_dataCache.size();
        QByteArray inData = m_dataCache.mid(0, size);
        QByteArray data32 = m_44_32->process(inData);
        m_dataCache_32.append(data32);
        m_dataCache.remove(0, size);
    }

    while (m_dataCache_32.size() >= 1280) {
        QByteArray inData = m_dataCache_32.mid(0, 1280);
        QByteArray leftData, rightData;
        channelSeparate(inData, leftData, rightData);

        QByteArray left = m_handle[0]->process(leftData);
        QByteArray right = m_handle[1]->process(rightData);

        QByteArray mergeData = channelMerge(left, right);

        m_dataCache_32_Processed.append(mergeData);
        m_dataCache_32.remove(0, 1280);
    }

    if (true) {
        int size = m_dataCache_32_Processed.size();
        QByteArray inData = m_dataCache_32_Processed.mid(0, size);
        QByteArray data44 = m_32_44->process(inData);
        m_dataCache_32_Processed.remove(0, size);

        return data44;
    }
}

void NoiseSupperssion::channelSeparate(QByteArray &in, QByteArray &left, QByteArray &right)
{
    int count = in.size() / sizeof(short);
    int samplesPerChannel = count / 2;

    static short *leftBytes = new short[44100];
    static short *rightBytes = new short[44100];

    short *data = (short*)in.data();
    int pos = 0;

    for (int i = 0; i < count; i += 2) {
        leftBytes[pos] = data[i];
        rightBytes[pos] = data[i+1];
        ++pos;
    }

    left.append((char*)leftBytes, samplesPerChannel*sizeof(short));
    right.append((char*)rightBytes, samplesPerChannel*sizeof(short));
}

QByteArray NoiseSupperssion::channelMerge(QByteArray &left, QByteArray &right)
{
    short *ldata = (short*)left.data();
    short *rdata = (short*)right.data();

    QByteArray ret;
    int count = left.size() / 2;
    for (int i = 0; i < count; ++i) {
        short ld = ldata[i];
        short rd = rdata[i];
        ret.append((char*)&ld, 2);
        ret.append((char*)&rd, 2);
    }

    return ret;
}

