#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtQuickWidgets/QQuickWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    QTimer* timer;
private slots:
    void on_connectButton_toggled(bool connected);
    void on_comboBox_BaudRate_currentIndexChanged(int index);
};

void MainWindow_Create(void);

void MainWindow_Show(void);

void MainWindow_Update(void);

void SimulatedData_Update(void);

#endif // MAINWINDOW_H
