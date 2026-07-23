#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>
#include <QDebug>
#include <QPainter>

#include <QQmlComponent>
#include <QQmlEngine>

#include "CarData.h"
#include "UartMessaging.h"
#include <QSerialPortInfo>

static MainWindow* window;
static const uint32_t BaudRate_array[11]={9600, 19200, 28800, 38400, 57600, 76800, 115200, 230400, 460800, 576000, 921600};
static const int TEMP_ROWS = 16, VOLT_ROWS = 3;
static const int TEMP_COLS = 8, VOLT_COLS = 8;

void MainWindow_Create(){
    window = new MainWindow();
}

void MainWindow_Show(void){
    window->show();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow constructor start";
    ui->setupUi(this);

    setupSliderBindings();

    timer = new QTimer();
    timer->callOnTimeout(MainWindow_Update);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(20);
    timer->start();
    //COM PORT
    const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    ui->Com_Port->clear();
    if (ports.isEmpty())
    {
        ui->Com_Port->setEnabled(false);
    }
    else
    {
        ui->Com_Port->setEnabled(true);
        for (int index = 0; index < ports.size(); ++index)
        {
            ui->Com_Port->addItem(ports[index].portName());
        }
    }

    //BAUDRATE and CONNECTBUTTON
    ui->comboBox_BaudRate->setCurrentIndex(10);
    ui->connectButton->setCheckable(true);
    ui->connectButton->setText("Connect");

    //CELL_TEMP and CELL_VOLT
    ui->Cell_Temp_tableWidget->setRowCount(TEMP_ROWS);
    ui->Cell_Temp_tableWidget->setColumnCount(TEMP_COLS);
    for (int row_index = 0; row_index < TEMP_ROWS; row_index++)
        for (int col_index = 0; col_index < TEMP_COLS; col_index++)
            ui->Cell_Temp_tableWidget->setItem(row_index, col_index, new QTableWidgetItem("--"));

    ui->Cell_Volt_tableWidget->setRowCount(VOLT_ROWS);
    ui->Cell_Volt_tableWidget->setColumnCount(VOLT_COLS);
    for (int row_index = 0; row_index < VOLT_ROWS; row_index++)
        for (int col_index = 0; col_index < VOLT_COLS; col_index++)
            ui->Cell_Volt_tableWidget->setItem(row_index, col_index, new QTableWidgetItem("--"));

    //Graphs
    setupGraph();
/*   gridPixmap = QPixmap(800,300);
    wavePixmap = QPixmap(800,300);
    finalPixmap = QPixmap(800,300);

    gridPixmap.fill(Qt::black);
    wavePixmap.fill(Qt::transparent);
    QPainter grid(&gridPixmap);

    ui->oscilloscopeLabel->setPixmap(finalPixmap);*/
}
/*void MainWindow::on_ComPort_currentIndexChanged(int ComPort_index)
{
    if (ComPort_index >= 0)
        UartMessaging_SetPortName(ui->Com_Port->currentText());
    // qDebug()<<"Main: val.shouldPortBeConencted:"<<BaudRate_array[BaudRate_index];
}*/

void MainWindow::on_comboBox_BaudRate_currentIndexChanged(int BaudRate_index)
{
    UartMessaging_SetBaudRate(BaudRate_array[BaudRate_index]);
   // qDebug()<<"Main: val.shouldPortBeConencted:"<<BaudRate_array[BaudRate_index];
}

void MainWindow::on_connectButton_toggled(bool connected)
{
    UartMessaging_SetConnection(connected);
    if(connected)
    {
        ui->connectButton->setText("Disconnect");
    }
    else
    {
        ui->connectButton->setText("Connect");
    }
}

void MainWindow::on_checkBox_Charging_toggled(bool checked)
{
    if (checked == 1) {
        ui->checkBox_Charging->setText("Stop Charging");
        WriteUartDataAtAddress(1, &MonitoredValues.TsacMonitoredValues.ChargerCommand);/*1 means charger should be charging, 0 means charger should NOT be charging */
        //WriteUartDataAtAddress(ui->label->value(), &MonitoredValues.TsacMonitoredValues.DesiredChargingCurrent);
        //WriteUartDataAtAddress(ui->label->value(), &MonitoredValues.TsacMonitoredValues.DesiredChargingVoltage);

    } else {
        ui->checkBox_Charging->setText("Start Charging");
        WriteUartDataAtAddress(0, &MonitoredValues.TsacMonitoredValues.ChargerCommand);
        WriteUartDataAtAddress(0, &MonitoredValues.TsacMonitoredValues.DesiredChargingCurrent);
        WriteUartDataAtAddress(0, &MonitoredValues.TsacMonitoredValues.DesiredChargingVoltage);

    }
}

void MainWindow::setupGraph()
{
    ui->quickWidget_TSAC_Charging->setResizeMode(QQuickWidget::SizeRootObjectToView);

    static const char *qml = R"QML(
import QtQuick
import QtGraphs


    GraphsView {
        anchors.fill: parent
        anchors.margins: 0

axisX: ValueAxis {
    min: 0
    max: 30
    tickInterval: 2
    subTickCount: 1
}

        axisY: ValueAxis {
            min: 0
            max: 120
            tickInterval: 20
labelDecimals: 0
        }
}

)QML";

    auto *component = new QQmlComponent(
        ui->quickWidget_TSAC_Charging->engine(), this);

    component->setData(QByteArray(qml), QUrl());

    QObject *root = component->create();

    if (!root) {
        qDebug() << "Root object was not created!";
        return;
    }

    ui->quickWidget_TSAC_Charging->setContent(QUrl(), component, root);
}

void MainWindow::setupSliderBindings()
{
    SliderLink_vect = {
        { ui->Slider_Charging_maxVolt,  ui->LineEdit_Charging_maxVolt,   10.0, 1 },
        { ui->Slider_Charging_maxCurrent,  ui->LineEdit_Charging_maxCurrent,   10.0, 1 },
        { ui->Slider_test,  ui->lineEdit_test,   100.0, 2 },
    };

    for (int bindingIndex=0; bindingIndex<SliderLink_vect.size();bindingIndex++)
    {
        SliderLink currentBinding = SliderLink_vect.at(bindingIndex);
        connect(currentBinding.slider, &QSlider::valueChanged, this, &MainWindow::onAnySlider_ValueChanged);
        connect(currentBinding.lineEdit, &QLineEdit::editingFinished, this, &MainWindow::onAnyLineEdit_editingFinished);
    }
}

void MainWindow::onAnySlider_ValueChanged(int value)
{
    QObject *senderObject = sender();
    for (int bindingIndex=0; bindingIndex<SliderLink_vect.size();bindingIndex++)
    {
        SliderLink currentBinding = SliderLink_vect.at(bindingIndex);
        if (currentBinding.slider == senderObject)
        {
            double realValue = value / currentBinding.divisor;
            currentBinding.lineEdit->setText(QString::number(realValue, 'f', currentBinding.decimals));
        }
    }
}

void MainWindow::onAnyLineEdit_editingFinished()
{
    QObject *senderObject = sender();
    for (int bindingIndex = 0; bindingIndex < SliderLink_vect.size(); bindingIndex++)
    {
        SliderLink currentBinding = SliderLink_vect.at(bindingIndex);
        if (currentBinding.lineEdit == senderObject)
        {
            QString enteredText = currentBinding.lineEdit->text();
            bool conversionSucceeded = false;
            double realValue = enteredText.toDouble(&conversionSucceeded);

            if (!conversionSucceeded)
                return;

            int sliderValue = realValue * currentBinding.divisor;
            sliderValue = qBound(currentBinding.slider->minimum(), sliderValue, currentBinding.slider->maximum());

            currentBinding.slider->setValue(sliderValue);

            double normalizedValue = sliderValue / currentBinding.divisor;
            currentBinding.lineEdit->setText(QString::number(normalizedValue, 'f', currentBinding.decimals));
            return;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


static void GeneralTab_Update(MainWindow* window);
static void TsacTab_Update(MainWindow* window);
static void ParametersTab_Update(MainWindow* window);
static void DebugTab_Update(MainWindow* window);

void MainWindow_Update(void)
{//this is called by a timer from the main thread. This should update the values of every visible element from the currently selected tab.
    switch(window->ui->tabWidget->currentIndex())
    {//check which tab is currently active to not waste time refreshing user interface vavlues from inactive tabs
    case 0://GENERAL tab is active
        GeneralTab_Update(window);
        break;
    case 1://TSAC tab is active
        TsacTab_Update(window);
        break;
    case 2://PARAMETERS tab is active
        ParametersTab_Update(window);
        break;
    case 3://DEBUG tab is active
        DebugTab_Update(window);
        break;
    default:
        break;
    }
}

void GeneralTab_Update(MainWindow* window)
{
    static const char textFault[] = "color:'#ff4444'>Fault", textSafe[] = "color:'#44cc77'>OK    ";
    static const uint8_t pedalsErrorsLength = 14U, BmsErrorsLength = 4U;
    static const uint16_t tooltipTextIndexes_Pedals[pedalsErrorsLength] = {54, 134, 220, 300, 380, 466, 542, 622, 702, 788, 868, 948, 1034, 1110 } ,
                            tooltipTextIndexes_Bms[BmsErrorsLength] = {35, 95, 155, 222};
    bool fault = false;
    bool pedalsErrors[pedalsErrorsLength]={0} ,
        BmsErrors[BmsErrorsLength]={0};
    static QString tooltip_Pedals="<table>"
        "<tr><td>Accel_Sensor1_ShortToGnd</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Accel_Sensor1_ShortToVcc</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Accel_Sensor1_OutOfRangeOutput</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Accel_Sensor2_ShortToGnd</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Accel_Sensor2_ShortToVcc</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Accel_Sensor2_OutOfRangeOutput</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Accel_Implausibility</td><td style='color:#44cc77'>OK     </td></tr>"

        "<tr><td>Brake_Sensor1_ShortToGnd</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Brake_Sensor1_ShortToVcc</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Brake_Sensor1_OutOfRangeOutput</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Brake_Sensor2_ShortToGnd</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Brake_Sensor2_ShortToVcc</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Brake_Sensor2_OutOfRangeOutput</td><td style='color:#44cc77'>OK     </td></tr>"
        "<tr><td>Brake_Implausibility</td><td style='color:#44cc77'>OK     </td></tr>"
                "</table>";

    static QString tooltip_Bms="<table>"
                            "<tr><td>Shunt</td><td style='color:#44cc77'>OK     </td></tr>"
                            "<tr><td>BMS0</td><td style='color:#44cc77'>OK     </td></tr>"
                            "<tr><td>BMS1</td><td style='color:#44cc77'>OK     </td></tr>"
                            "<tr><td>Transceiver</td><td style='color:#44cc77'>OK     </td></tr>"
                            "</table>";

    uint32_t readValue, Value;
    char char_array[20], index = 0U;
    bool IsNegative;

    //TSAC_HighestCellTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.HighestCellTemperature);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->General_Tsac_Temp_Highest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_MedianCellTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.MedianCellTemperature);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->General_Tsac_Temp_Median_lineEdit->setText((const QString)QString(char_array));

    //TSAC_LowestCellTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.LowestCellTemperature);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->General_Tsac_Temp_Lowest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_HighestCellVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.HighestCellVoltage);
    index = 0U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->General_Tsac_Volt_Highest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_MedianCellVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.MedianCellVoltage);
    index = 0U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->General_Tsac_Volt_Median_lineEdit->setText((const QString)QString(char_array));

    //TSAC_LowestCellVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.LowestCellVoltage);
    index = 0U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;

    window->ui->General_Tsac_Volt_Lowest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_OverallVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.OverallVoltage);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->General_Tsac_V_Median_lineEdit->setText((const QString)QString(char_array));

    //TSAC_OverallCurrent
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.OverallCurrent);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'A';
    char_array[index++] = 0;
    window->ui->General_Tsac_Current_Median_lineEdit->setText((const QString)QString(char_array));


    //PEDALS_AcceleratorSensor1Voltage
    readValue = (ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1Voltage) * 5000U / 16383U);//this translates the value from interval 0-16383 to interval 0-5000
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;//needed so the "string" can be terminated
    window->ui->Accel_Volt_S1_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_AcceleratorSensor2Voltage
    readValue = (ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2Voltage) * 5000U / 16383U);//this translates the value from interval 0-16383 to interval 0-5000
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    window->ui->Accel_Volt_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_AcceleratorSensor1TravelPercentage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1TravelPercentage);
    index = 0U;
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/100U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = '%';
    char_array[index++] = 0;//needed so the "string" can be terminated
    window->ui->Accel_Travel_S1_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_AcceleratorSensor2TravelPercentage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2TravelPercentage);
    index = 0U;
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/100U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = '%';
    char_array[index++] = 0;
    window->ui->Accel_Travel_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_BrakeSensor1Voltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.BrakeSensor1Voltage) * 5000U / 16383U;

    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    window->ui->Brake_Volt_S1_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_BrakeSensor2Voltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.BrakeSensor2Voltage) * 5000U / 16383U;/* 14 bits, 0-16383, 0 to 5.00 Volts  +10U */

    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    window->ui->Brake_Volt_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_BrakeSensor1TravelPercentage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.BrakeSensor1TravelPercentage);
    index = 0U;
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/100U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = '%';
    char_array[index++] = 0;
    window->ui->Brake_Travel_S1_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_BrakeSensor2TravelPercentage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.BrakeSensor2TravelPercentage);
    index = 0U;
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/100U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = '%';
    char_array[index++] = 0;
    window->ui->Brake_Travel_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_PressureSensorVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.PressureSensorVoltage);
    index = 0U;
    char_array[index++] = '0' + readValue/100U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Pressure_Volt_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_PressureSensorBars
    readValue = ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.PressureSensorBars);
    index = 0U;
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/100U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'b';
    char_array[index++] = 'a';
    char_array[index++] = 'r';
    char_array[index++] = 0;
    window->ui->Pressure_Bars_lineEdit->setText((const QString)QString(char_array));


    //INVERTERS_LeftInverterTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftInverterTemperature);
    index = 0U;
    IsNegative = false;
    if(readValue < 40U)
    {
        IsNegative = true;
        Value = 40U - readValue;
    }
    else
    {
        Value = readValue - 40U;
    }
    if(IsNegative==true)
    {
        char_array[index++] = '-';
    }
    if(Value >= 100U)
    {
        char_array[index++] = '0' + Value/100U;
    }
    char_array[index++] = '0' + (Value/10U)%10U;
    char_array[index++] = '0' + Value%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Temp_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightInverterTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightInverterTemperature);
    index = 0U;
    IsNegative = false;
    if(readValue < 40U)
    {
        IsNegative = true;
        Value = 40U - readValue;
    }
    else
    {
        Value = readValue - 40U;
    }
    if(IsNegative==true)
    {
        char_array[index++] = '-';
    }
    if(Value >= 100U)
    {
        char_array[index++] = '0' + Value/100U;
    }
    char_array[index++] = '0' + (Value/10U)%10U;
    char_array[index++] = '0' + Value%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Temp_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftMotorTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftMotorTemperature);
    index = 0U;
    IsNegative = false;
    if(readValue < 30U)
    {
        IsNegative = true;
        Value = 30U - readValue;
    }
    else
    {
        Value = readValue - 30U;
    }
    if(IsNegative==true)
    {
        char_array[index++] = '-';
    }
    if(Value >= 100U)
    {
        char_array[index++] = '0' + Value/100U;
    }
    char_array[index++] = '0' + (Value/10U)%10U;
    char_array[index++] = '0' + Value%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Motor_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightMotorTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightMotorTemperature);
    index = 0U;
    IsNegative = false;
    if(readValue < 30U)
    {
        IsNegative = true;
        Value = 30U - readValue;
    }
    else
    {
        Value = readValue - 30U;
    }
    if(IsNegative==true)
    {
        char_array[index++] = '-';
    }
    if(Value >= 100U)
    {
        char_array[index++] = '0' + Value/100U;
    }
    char_array[index++] = '0' + (Value/10U)%10U;
    char_array[index++] = '0' + Value%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Motor_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftInverterInputVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftInverterInputVoltage);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Left_InV_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightInverterInputVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightInverterInputVoltage);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Right_InV_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftInverterCurrent
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftInverterCurrent);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'A';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Current_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightInverterCurrent
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightInverterCurrent);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'A';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Current_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftMotorRpm
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftMotorRpm);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'R';
    char_array[index++] = 'P';
    char_array[index++] = 'M';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Motor_Rpm_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightMotorRpm
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightMotorRpm);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'R';
    char_array[index++] = 'P';
    char_array[index++] = 'M';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Motor_Rpm_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftMotorSpeedKmh
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftMotorSpeedKmh);
    index = 0U;
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    if(readValue>=10)
    {
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'K';
    char_array[index++] = 'm';
    char_array[index++] = '/';
    char_array[index++] = 'h';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Motor_Speed_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightMotorSpeedKmh
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightMotorSpeedKmh);
    index = 0U;
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    if(readValue>=10)
    {
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'K';
    char_array[index++] = 'm';
    char_array[index++] = '/';
    char_array[index++] = 'h';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Motor_Speed_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftInverterThrottle
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftInverterThrottle)*2;
    index = 0U;
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Throttle_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightInverterSentThrottle
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightInverterThrottle)*2;
    index = 0U;
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Throttle_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftInverterThrottleFeedback
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.LeftInverterThrottleFeedback)*2;
    index = 0U;
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Left_Feedback_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_RightInverterThrottleFeedback
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.RightInverterThrottleFeedback)*2;
    index = 0U;
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Feedback_lineEdit->setText((const QString)QString(char_array));

//General tab status indicators:
    //INVERTERS_IsCarRunning
    readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.IsCarRunning);
    if(readValue == 0)
    {
        window->ui->General_Car_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_Car_Status_Qlabel->setText("STOPPED");
    }
    else
    {
        readValue = ReadUartDataFromAddress(&MonitoredValues.InvertersMonitoredValues.IsCarRunning);
        if(readValue == 0)
        {
            window->ui->General_Car_Status_Qlabel->setStyleSheet("background-color: green;");
            window->ui->General_Car_Status_Qlabel->setText("FORWARD");
        }
        else
        {
            window->ui->General_Car_Status_Qlabel->setStyleSheet("background-color: green;");
            window->ui->General_Car_Status_Qlabel->setText("REVERSE");
        }
    }
    //DASHBOARD_ActivationButtonPressed
    readValue = ReadUartDataFromAddress(&MonitoredValues.DashboardMonitoredValues.ActivationButtonPressed);
    if(readValue == 1)
    {
        window->ui->General_Activation_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->General_Activation_Status_Qlabel->setText("ACTIVATED");
    }
    else
    {
        window->ui->General_Activation_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_Activation_Status_Qlabel->setText("INACTIVE");
    }
    //DASHBOARD_CarReverseCommandPressed
    readValue = ReadUartDataFromAddress(&MonitoredValues.DashboardMonitoredValues.CarReverseCommandPressed);
    if(readValue == 1)
    {
        window->ui->General_Reverse_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->General_Reverse_Status_Qlabel->setText("ACTIVATED");
    }
    else
    {
        window->ui->General_Reverse_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_Reverse_Status_Qlabel->setText("INACTIVE");
    }
    //TSAC_IsAmsSafe
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.AmsError);
    if(readValue == 0)
    {
        window->ui->General_Ams_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->General_Ams_Status_Qlabel->setText("SAFE");
    }
    else
    {
        window->ui->General_Ams_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_Ams_Status_Qlabel->setText("UNSAFE");
    }

    //DASHBOARD_IsDisplayWorking
    readValue = ReadUartDataFromAddress(&MonitoredValues.DashboardMonitoredValues.IsDisplayWorking);
    if(readValue == 0)
    {
        window->ui->General_Display_HStatus_Qlabel->setStyleSheet("background-color: green;");
        window->ui->General_Display_HStatus_Qlabel->setText("ACTIVATED");
    }
    else
    {
        window->ui->General_Display_HStatus_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_Display_HStatus_Qlabel->setText("INACTIVE");
    }
    //DASHBOARD_IsSegmentsDriverWorking
    readValue = ReadUartDataFromAddress(&MonitoredValues.DashboardMonitoredValues.IsSegmentsDriverWorking);
    if(readValue == 0)
    {
        window->ui->General_7seg_HStatus_Qlabel->setStyleSheet("background-color: green;");
        window->ui->General_7seg_HStatus_Qlabel->setText("ACTIVATED");
    }
    else
    {
        window->ui->General_7seg_HStatus_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_7seg_HStatus_Qlabel->setText("INACTIVED");
    }

    //Pedals_Sensors
    pedalsErrors[0] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToGnd);
    pedalsErrors[1] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToVcc);
    pedalsErrors[2] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_OutOfRangeOutput);
    pedalsErrors[3] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToGnd);
    pedalsErrors[4] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToVcc);
    pedalsErrors[5] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_OutOfRangeOutput);
    pedalsErrors[6] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Accel_Implausibility);

    pedalsErrors[7] =  (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToGnd);
    pedalsErrors[8] =  (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToVcc);
    pedalsErrors[9] =  (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_OutOfRangeOutput);
    pedalsErrors[10] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToGnd);
    pedalsErrors[11] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToVcc);
    pedalsErrors[12] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_OutOfRangeOutput);
    pedalsErrors[13] = (bool)ReadUartDataFromAddress(&MonitoredValues.PedalsMonitoredValues.Brake_Implausibility);

    for (uint16_t indexEroare = 0; indexEroare < pedalsErrorsLength; indexEroare++)
    {
        if(pedalsErrors[indexEroare])
        {
            fault = true;
            for(uint16_t indexText = 0;indexText < strlen(textFault); indexText++)
            {
                tooltip_Pedals[tooltipTextIndexes_Pedals[indexEroare] + indexText] = textFault[indexText];
            }
        }
        else
        {
            for(uint16_t indexText = 0;indexText < strlen(textSafe); indexText++)
            {
                tooltip_Pedals[tooltipTextIndexes_Pedals[indexEroare] + indexText] = textSafe[indexText];
            }
        }
    }
    if (!fault) {
        window->ui->General_Pedals_HStatus_Qlabel->setText("SAFE");
        window->ui->General_Pedals_HStatus_Qlabel->setStyleSheet("background-color: green;");
    }
    else {
        window->ui->General_Pedals_HStatus_Qlabel->setText("FAULT");
        window->ui->General_Pedals_HStatus_Qlabel->setStyleSheet("QLabel { background-color: red; }");
    }
    window->ui->General_Pedals_HStatus_Qlabel->setToolTip(tooltip_Pedals);

    //General_IsBmsWorking
    BmsErrors[0] = (bool)ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.ShuntError);
    BmsErrors[1] = (bool)ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.Bms0Error);
    BmsErrors[2] = (bool)ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.Bms1Error);
    BmsErrors[3] = (bool)ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.TransceiverError);
    for (uint16_t indexEroare = 0; indexEroare < BmsErrorsLength; indexEroare++)
    {
        if(BmsErrors[indexEroare])
        {
            fault = true;
            for(uint16_t indexText = 0;indexText < strlen(textFault); indexText++)
            {
                tooltip_Bms[tooltipTextIndexes_Bms[indexEroare] + indexText] = textFault[indexText];
            }
        }
        else
        {
            for(uint16_t indexText = 0;indexText < strlen(textSafe); indexText++)
            {
                tooltip_Bms[tooltipTextIndexes_Bms[indexEroare] + indexText] = textSafe[indexText];
            }
        }
    }
    if (!fault) {
        window->ui->General_Bms_HStatus_Qlabel->setText("SAFE");
        window->ui->General_Bms_HStatus_Qlabel->setStyleSheet("background-color: green;");
    }
    else {
        window->ui->General_Bms_HStatus_Qlabel->setText("FAULT");
        window->ui->General_Bms_HStatus_Qlabel->setStyleSheet("QLabel { background-color: red; }");
    }
    window->ui->General_Bms_HStatus_Qlabel->setToolTip(tooltip_Bms);
}

void TsacTab_Update(MainWindow* window)
{
    uint32_t readValue;
    char char_array[20], index = 0U;

    //TSAC_HighestCellTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.HighestCellTemperature);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;//needed so the "string" can be terminated
    window->ui->Tsac_Temp_Highest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_MedianCellTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.MedianCellTemperature);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->Tsac_Temp_Median_lineEdit->setText((const QString)QString(char_array));

    //TSAC_LowestCellTemperature
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.LowestCellTemperature);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'C';
    char_array[index++] = 0;
    window->ui->Tsac_Temp_Lowest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_HighestCellVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.HighestCellVoltage);
    index = 0U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Tsac_Volt_Highest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_MedianCellVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.MedianCellVoltage);
    index = 0U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Tsac_Volt_Median_lineEdit->setText((const QString)QString(char_array));

    //TSAC_LowestCellVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.LowestCellVoltage);
    index = 0U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    char_array[index++] = '0' + (readValue/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Tsac_Volt_Lowest_lineEdit->setText((const QString)QString(char_array));

    //TSAC_OverallVoltage
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.OverallVoltage);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Tsac_V_Median_lineEdit->setText((const QString)QString(char_array));

    //TSAC_OverallCurrent
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.OverallCurrent);
    index = 0U;
    if(readValue >= 1000U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
    char_array[index++] = '0' + (readValue/10U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'A';
    char_array[index++] = 0;
    window->ui->Tsac_Current_Median_lineEdit->setText((const QString)QString(char_array));


    //TSAC_IsAmsSafe
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.AmsError);
    if(readValue == 0)
    {
        window->ui->TSAC_Ams_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->TSAC_Ams_Status_Qlabel->setText("SAFE");
    }
    else
    {
        window->ui->TSAC_Ams_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->TSAC_Ams_Status_Qlabel->setText("UNSAFE");
    }

    //TSAC_IsShuntWorking
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.ShuntError);
    if(readValue == 0)
    {
        window->ui->TSAC_Shunt_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->TSAC_Shunt_Status_Qlabel->setText("Ok");
    }
    else
    {
        window->ui->TSAC_Shunt_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->TSAC_Shunt_Status_Qlabel->setText("Error");
    }

    //TSAC_IsTransceiverWorking
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.TransceiverError);
    if(readValue == 0)
    {
        window->ui->TSAC_Transceiver_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->TSAC_Transceiver_Status_Qlabel->setText("Ok");
    }
    else
    {
        window->ui->TSAC_Transceiver_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->TSAC_Transceiver_Status_Qlabel->setText("Error");
    }

    //TSAC_IsBms0Working
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.Bms0Error);
    if(readValue == 0)
    {
        window->ui->TSAC_BMS0_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->TSAC_BMS0_Status_Qlabel->setText("Ok");
    }
    else
    {
        window->ui->TSAC_BMS0_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->TSAC_BMS0_Status_Qlabel->setText("Error");
    }

    //TSAC_IsBms1Working
    readValue = ReadUartDataFromAddress(&MonitoredValues.TsacMonitoredValues.Bms1Error);
    if(readValue == 0)
    {
        window->ui->TSAC_BMS1_Status_Qlabel->setStyleSheet("background-color: green;");
        window->ui->TSAC_BMS1_Status_Qlabel->setText("Ok");
    }
    else
    {
        window->ui->TSAC_BMS1_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->TSAC_BMS1_Status_Qlabel->setText("Error");
    }

    //TSAC_CellTemperatures_Table
    window->ui->Cell_Temp_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    window->ui->Cell_Temp_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int row_index = 0; row_index < TEMP_ROWS; row_index++)
    {
        for (int col_index = 0; col_index < TEMP_COLS; col_index++)
        {
            int idx = row_index * TEMP_COLS + col_index;
            readValue = UartMessaging_ReadCellTemperature(idx);

            index = 0U;
            if(readValue >= 1000U)
            {
                char_array[index++] = '0' + readValue/1000U;
            }
            if(readValue >= 100U){
                char_array[index++] = '0' + (readValue/100U)%10U;
            }
            char_array[index++] = '0' + (readValue/10U)%10U;
            char_array[index++] = '.';
            char_array[index++] = '0' + readValue%10U;
            char_array[index++] = ' ';
            char_array[index++] = 'C';
            char_array[index++] = 0;
            window->ui->Cell_Temp_tableWidget->item(row_index, col_index)->setText(QString(char_array));
        }
    }
    //TSAC_CellVoltages_Table
    window->ui->Cell_Volt_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    window->ui->Cell_Volt_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int row_index = 0; row_index < VOLT_ROWS; row_index++)
    {
        for (int col_index = 0; col_index < VOLT_COLS; col_index++)
        {
            int idx = row_index * VOLT_COLS + col_index;
            readValue = UartMessaging_ReadCellTemperatureErrors(idx);
            if(readValue == true)
            {
                window->ui->Cell_Volt_tableWidget->item(row_index, col_index)->setBackground(QColor("#ff4444"));
                window->ui->Cell_Volt_tableWidget->item(row_index, col_index)->setText(QString("Error"));
            }
            else
            {
                window->ui->Cell_Volt_tableWidget->item(row_index, col_index)->setBackground(Qt::transparent);
                readValue = UartMessaging_ReadCellTemperature(idx);
                index = 0U;
                if(readValue >= 1000U)
                {
                    char_array[index++] = '0' + (readValue/1000U);
                }
                char_array[index++] = '0' + (readValue/100U)%10U;
                char_array[index++] = '.';
                char_array[index++] = '0' + (readValue/10U)%10U;
                char_array[index++] = '0' + readValue%10U;
                char_array[index++] = ' ';
                char_array[index++] = 'V';
                char_array[index++] = 0;
                window->ui->Cell_Volt_tableWidget->item(row_index, col_index)->setText(QString(char_array));
            }

        }
    }

}

void ParametersTab_Update(MainWindow* window)
{
    //TODO
}

void DebugTab_Update(MainWindow* window)
{
    //TODO
}



