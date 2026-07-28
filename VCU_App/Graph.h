#pragma once
#include <QWidget>
#include <QPaintEvent>

#include <QVector>

class Graph : public QWidget {
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);
    void setRange_Oy(double minVal, double maxVal);
//    void setMaxSamples_Ox(int count);
    void addSample(double Value);


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> signalDataBuffer;
    double valueToY(double val);
    double minGraph = -1.0;
    double maxGraph = 1.0;



    /*
    void generateDataTest();
    QTimer* timer;
    void onTimerTimeout();
    double phase=0;
    double frequency = 0.05;*/


};
