#pragma once
#include <QWidget>
#include <QPaintEvent>

class Graph : public QWidget {
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);

protected:
    // Override the paintEvent
    void paintEvent(QPaintEvent *event) override;
};