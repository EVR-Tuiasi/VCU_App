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
    setSignalCount(3);
    signalDataBuffers[0].color = Qt::green;
    signalDataBuffers[1].color = Qt::yellow;
    signalDataBuffers[2].color = Qt::red;
}
void Graph::addSample(int index, double Value) {
    if (index >= signalDataBuffers.size())
        return;
    signalDataBuffers[index].buffer.append(Value);
    if (signalDataBuffers[index].buffer.size() > width())
        signalDataBuffers[index].buffer.removeFirst();
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
void Graph::setSignalCount(int count)
{
    signalDataBuffers.resize(count);
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
    for (int signalIndex = 0; signalIndex < signalDataBuffers.size(); ++signalIndex)
    {
        if (signalDataBuffers[signalIndex].buffer.size() < 2)
            continue;

        QPen pen(signalDataBuffers[signalIndex].color);
        painter.setPen(pen);

    QPainterPath path;
    path.moveTo(0, valueToY(signalDataBuffers[signalIndex].buffer[0]));
    for (int x = 1; x < signalDataBuffers[signalIndex].buffer.size(); ++x) {
        double y = valueToY(signalDataBuffers[signalIndex].buffer[x]);
        path.lineTo(x, y);
    }
    painter.drawPath(path);
    }
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
