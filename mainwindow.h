#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QIODevice>
#include <QAudioOutput>

#include "noisesupperssion.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void noiseSupperssion(NsHandle *pNS_inst, char* pInBuffer, char *pOutBuffer, int nSample, int nMode, int len);
    void outputData(QByteArray data);
    void channelSeparate(QByteArray &in, QByteArray &left, QByteArray &right);
    QByteArray channelMerge(QByteArray &left, QByteArray &right);
    void donoise(char *pInBuffer, char *pOutBuffer, int nSample, int nMode, int len);

private slots:
    void processing();
    void on_startButton_clicked();
    void on_pauseButton_clicked();

private:
    Ui::MainWindow  *ui;

    QTimer              *m_timer;
    QFile               *m_infile;
    QFile               *m_outfile;
    QAudioFormat         m_format;
    QAudioOutput        *m_audioOutput;
    QIODevice           *m_outDev;

    NoiseSupperssion    m_noiseSupperssion;
};

#endif // MAINWINDOW_H
