#include "Graph.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

Graph::Graph(QWidget *parent) : QWidget(parent) {
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
void Graph::setSignalRange(int index, double minVal, double maxVal)
{
    if (index >= signalDataBuffers.size())
        return;

    signalDataBuffers[index].minValue = minVal;
    signalDataBuffers[index].maxValue = maxVal;
}

void Graph::setSignalCount(int count)
{
    signalDataBuffers.resize(count);
}

double Graph::valueToY(double value, double minVal, double maxVal)
{
    double norm = (value - minVal) / (maxVal - minVal);
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
    path.moveTo(0, valueToY(signalDataBuffers[signalIndex].buffer[0],
                            signalDataBuffers[signalIndex].minValue,
                            signalDataBuffers[signalIndex].maxValue));
    for (int x = 1; x < signalDataBuffers[signalIndex].buffer.size(); ++x) {
        double y = valueToY(signalDataBuffers[signalIndex].buffer[x],
                            signalDataBuffers[signalIndex].minValue,
                            signalDataBuffers[signalIndex].maxValue);
        path.lineTo(x, y);
    }
    painter.drawPath(path);
    }
}


