#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtQuickWidgets/QQuickWidget>

#include <QSlider>
#include <QLineEdit>
#include <QPixmap>

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
private:
    struct SliderLink {
        QSlider*   slider;
        QLineEdit* lineEdit;
        double     divisor;
        int        decimals;
    };
    QVector<SliderLink> SliderLink_vect;
    void setupSliderBindings();
/*
    QPixmap gridPixmap;
    QPixmap wavePixmap;
    QPixmap finalPixmap;
    int lastY;
*/
private slots:
    void on_connectButton_toggled(bool connected);
    void on_comboBox_BaudRate_currentIndexChanged(int index);
    //void on_ComPort_currentIndexChanged(int index);
    void on_checkBox_Charging_toggled(bool checked);
    void setupGraph();
    void onAnySlider_ValueChanged(int value);
    void onAnyLineEdit_editingFinished();
//    void updateGraph();
};

void MainWindow_Create(void);

void MainWindow_Show(void);

void MainWindow_Update(void);

void SimulatedData_Update(void);

#endif // MAINWINDOW_H
