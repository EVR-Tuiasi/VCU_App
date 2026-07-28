#include "Graph.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

Graph::Graph(QWidget *parent) : QWidget(parent) {
    //generateDataTest();
    /*timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Graph::onTimerTimeout);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(20);
    timer->start();*/
}
void Graph::addSample(double Value) {
    signalDataBuffer.append(Value);
    if (signalDataBuffer.size() > width())
        signalDataBuffer.removeFirst();
    update();
}
/*void Graph::onTimerTimeout()
{
    addSample(std::sin(phase));
    phase += frequency;
}*/
void Graph::setRange_Oy(double minVal, double maxVal) {
    minGraph = minVal;
    maxGraph = maxVal;
    update();
}
/*void Graph::setMaxSamples_Ox(int count) {
    maxSamples = count;
    while (m_samples.size() > m_maxSamples)
        m_samples.removeFirst();
    update();
}*/
double Graph::valueToY(double value)
{
    double norm = (value - minGraph) / (maxGraph - minGraph);
    if(norm < 0)
        norm = 0;

    if(norm > 1)
        norm = 1;
    return height() - norm * height();
}

void Graph::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);
    QPen pen(Qt::green);
    pen.setWidth(3);
    painter.setPen(pen);
    if (signalDataBuffer.size() < 2)
       return;

    QPainterPath path;
    path.moveTo(0, valueToY(signalDataBuffer[0]));
    for (int x = 1; x < signalDataBuffer.size(); ++x) {
        double y = valueToY(signalDataBuffer[x]);
        path.lineTo(x, y);
    }
    qDebug() << signalDataBuffer.first()
             << signalDataBuffer.last();
    painter.drawPath(path);
}


/*void Graph::generateDataTest()
{
    double w = 600;
    int h = 150;
    signalDataBuffer.resize(w);
    double midY = h / 2.0;
    double amplitude = h / 3.0;
    double frequency = 0.05;
    for (int x = 0; x < w; ++x) {
        signalDataBuffer[x] = midY - amplitude * std::sin(x * frequency);
    }
}*/
