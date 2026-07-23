#include "Graph.h"
#include <QPainter>

Graph::Graph(QWidget *parent) : QWidget(parent) {
    // Constructor logic if needed
}

void Graph::paintEvent(QPaintEvent *event) {
    // Optional: Call the base class paint event to draw default styles/backgrounds
    //QWidget::paintEvent(event);

    // Initialize the painter for this widget
    QPainter painter(this);

    // Your custom drawing logic here
    painter.setPen(Qt::blue);
    painter.drawText(rect(), Qt::AlignCenter, "Hello from Custom Paint!");
}
