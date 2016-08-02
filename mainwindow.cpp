#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pauseButton->setEnabled(false);

    m_format.setChannelCount(2);
    m_format.setSampleRate(44100);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);

    m_audioOutput = new QAudioOutput(m_format);
    m_outDev = m_audioOutput->start();

    m_noiseSupperssion.init();
}

MainWindow::~MainWindow()
{
    delete ui;
    m_audioOutput->disconnect();
}

void MainWindow::outputData(QByteArray data)
{
    m_outDev->write(data);

    m_outfile->write(data);
}

void MainWindow::processing()
{
    static int i = 0;
    int pcmSize = 44100 * 16 * 2 / (8 * 100);
    QByteArray inBuffer;
    QByteArray outBuffer;
    inBuffer = m_infile->read(pcmSize);

    //noise supperssion
    outBuffer = m_noiseSupperssion.process(inBuffer);

    //output
    outputData(outBuffer);

    i += pcmSize;
    if (i >= m_infile->size())
        m_timer->stop();
}

void MainWindow::on_startButton_clicked()
{
    ui->pauseButton->setEnabled(true);
    bool flag = true;
    if(flag) {
        m_infile = new QFile("test_44K.pcm");
        if (!m_infile->open(QIODevice::ReadOnly))
            return;

        m_outfile = new QFile("test_44K_ns.pcm");
        m_outfile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Unbuffered);
        m_timer = new QTimer;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(processing()));
        m_timer->start(10);
        ui->startButton->setEnabled(false);
        flag = false;
    }
}

void MainWindow::on_pauseButton_clicked()
{
    static bool flag = true;
    if (flag) {
        m_timer->stop();
        flag = false;
    }
    else {
        m_timer->start(10);
        flag = true;
    }
}
