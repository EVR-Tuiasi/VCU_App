#pragma once
#include <QWidget>
#include <QPaintEvent>

#include <QVector>
#include <QColor>

struct Signal
{
    QVector<double> buffer;
    QColor color;
    double minValue=0.0, maxValue=1.0;
};

class Graph : public QWidget {
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);
    void addSample(int index, double Value);
    void  setSignalRange(int index, double minVal, double maxVal);
    void setSignalCount(int count);


protected:
    void paintEvent(QPaintEvent *event) override;

private:

    QVector<Signal> signalDataBuffers;

    double valueToY(double val,double minVal, double maxVal);



    /*
    void generateDataTest();
    QTimer* timer;
    void onTimerTimeout();
    double phase=0;
    double frequency = 0.05;*/


};
