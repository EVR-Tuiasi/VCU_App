#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTimer>
#include<QDebug>

#include "CarData.h"

static MainWindow* window;

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
    ui->setupUi(this);
    timer = new QTimer();
    timer->callOnTimeout(MainWindow_Update);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(20);
    timer->start();
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
    static const char textFault[]="color:'#ff4444'>Fault";
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
    readValue = CarData_ReadValue(TSAC_HighestCellTemperature);
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
    readValue = CarData_ReadValue(TSAC_MedianCellTemperature);
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
    readValue = CarData_ReadValue(TSAC_LowestCellTemperature);
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
    readValue = CarData_ReadValue(TSAC_HighestCellVoltage);
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
    readValue = CarData_ReadValue(TSAC_MedianCellVoltage);
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
    readValue = CarData_ReadValue(TSAC_LowestCellVoltage);
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
    readValue = CarData_ReadValue(TSAC_OverallVoltage);
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
    readValue = CarData_ReadValue(TSAC_OverallCurrent);
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
    readValue = (CarData_ReadValue(PEDALS_AcceleratorSensor1Voltage) * 5000U) / 16383U;//this translates the value from interval 0-16383 to interval 0-5000
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;//needed so the "string" can be terminated
    window->ui->Accel_Volt_S1_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_AcceleratorSensor2Voltage
    readValue = (CarData_ReadValue(PEDALS_AcceleratorSensor2Voltage) * 5000U) / 16383U;//this translates the value from interval 0-16383 to interval 0-5000
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    window->ui->Accel_Volt_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_AcceleratorSensor1TravelPercentage
    readValue = CarData_ReadValue(PEDALS_AcceleratorSensor1TravelPercentage);
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
    readValue = CarData_ReadValue(PEDALS_AcceleratorSensor2TravelPercentage);
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
    readValue = (CarData_ReadValue(PEDALS_BrakeSensor1Voltage)* 5000U) / 16383U;
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    window->ui->Brake_Volt_S1_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_BrakeSensor2Voltage
    readValue = (CarData_ReadValue(PEDALS_BrakeSensor2Voltage)* 5000U) / 16383U; /* 14 bits, 0-16383, 0 to 5.00 Volts  +10U */
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    window->ui->Brake_Volt_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_BrakeSensor1TravelPercentage
    readValue = CarData_ReadValue(PEDALS_BrakeSensor1TravelPercentage);
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
    readValue = CarData_ReadValue(PEDALS_BrakeSensor2TravelPercentage);
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
    readValue = CarData_ReadValue(PEDALS_PressureSensorVoltage);
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
    readValue = CarData_ReadValue(PEDALS_PressureSensorBars);
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
    readValue = CarData_ReadValue(INVERTERS_LeftInverterTemperature);
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
    readValue = CarData_ReadValue(INVERTERS_RightInverterTemperature);
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
    readValue = CarData_ReadValue(INVERTERS_LeftMotorTemperature);
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
    readValue = CarData_ReadValue(INVERTERS_RightMotorTemperature);
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
    readValue = CarData_ReadValue(INVERTERS_LeftInverterInputVoltage);
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
    readValue = CarData_ReadValue(INVERTERS_RightInverterInputVoltage);
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
    readValue = CarData_ReadValue(INVERTERS_LeftInverterCurrent);
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
    readValue = CarData_ReadValue(INVERTERS_RightInverterCurrent);
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
    readValue = CarData_ReadValue(INVERTERS_LeftMotorRpm);
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
    readValue = CarData_ReadValue(INVERTERS_RightMotorRpm);
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
    readValue = CarData_ReadValue(INVERTERS_LeftMotorSpeedKmh);
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
    readValue = CarData_ReadValue(INVERTERS_RightMotorSpeedKmh);
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
    readValue = CarData_ReadValue(INVERTERS_LeftInverterThrottle)*2;
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
    readValue = CarData_ReadValue(INVERTERS_RightInverterSentThrottle);
    Value=readValue*2;
    index = 0U;
    char_array[index++] = '0' + (Value/100U)%10U;
    char_array[index++] = '.';
    char_array[index++] = '0' + (Value/10U)%10U;
    char_array[index++] = '0' + Value%10U;
    char_array[index++] = ' ';
    char_array[index++] = 'V';
    char_array[index++] = 0;
    window->ui->Inverter_Right_Throttle_lineEdit->setText((const QString)QString(char_array));

    //INVERTERS_LeftInverterThrottleFeedback
    readValue = CarData_ReadValue(INVERTERS_LeftInverterThrottleFeedback)*2;
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
    readValue = CarData_ReadValue(INVERTERS_RightInverterThrottleFeedback)*2;
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
    readValue = CarData_ReadValue(INVERTERS_IsCarRunning);
    if(readValue == 0)
    {
        window->ui->General_Car_Status_Qlabel->setStyleSheet("background-color: red;");
        window->ui->General_Car_Status_Qlabel->setText("STOPPED");
    }
    else
    {
        readValue = CarData_ReadValue(INVERTERS_IsCarRunning);
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
    readValue = CarData_ReadValue(DASHBOARD_ActivationButtonPressed);
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
    readValue = CarData_ReadValue(DASHBOARD_CarReverseCommandPressed);
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
    readValue = CarData_ReadValue(TSAC_IsAmsSafe);
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
    readValue = CarData_ReadValue(DASHBOARD_IsDisplayWorking);
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
    readValue = CarData_ReadValue(DASHBOARD_IsSegmentsDriverWorking);
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
    pedalsErrors[0] = false;//(bool)CarData_ReadValue(PEDALS_Accel_Sensor1_ShortToGnd);//momentan primesc 0 == false == safe
    pedalsErrors[1] = false;//(bool)CarData_ReadValue(PEDALS_Accel_Sensor1_ShortToVcc);
    pedalsErrors[2] = true;//(bool)CarData_ReadValue(PEDALS_Accel_Sensor1_OutOfRangeOutput);
    pedalsErrors[3] = false;//(bool)CarData_ReadValue(PEDALS_Accel_Sensor2_ShortToGnd);
    pedalsErrors[4] = true;//(bool)CarData_ReadValue(PEDALS_Accel_Sensor2_ShortToVcc);
    pedalsErrors[5] = true;//(bool)CarData_ReadValue(PEDALS_Accel_Sensor2_OutOfRangeOutput);
    pedalsErrors[6] = true;//(bool)CarData_ReadValue(PEDALS_Accel_Implausibility);

    pedalsErrors[7] = (bool)CarData_ReadValue(PEDALS_Brake_Sensor1_ShortToGnd);
    pedalsErrors[8] = true;//(bool)CarData_ReadValue(PEDALS_Brake_Sensor1_ShortToVcc);
    pedalsErrors[9] = true;//(bool)CarData_ReadValue(PEDALS_Brake_Sensor1_OutOfRangeOutput);
    pedalsErrors[10] = true;//(bool)CarData_ReadValue(PEDALS_Brake_Sensor2_ShortToGnd);
    pedalsErrors[11] = true;//(bool)CarData_ReadValue(PEDALS_Brake_Sensor2_ShortToVcc);
    pedalsErrors[12] = true;//(bool)CarData_ReadValue(PEDALS_Brake_Sensor2_OutOfRangeOutput);
    pedalsErrors[13] = true;//(bool)CarData_ReadValue(PEDALS_Brake_Implausibility);

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
    BmsErrors[0] = (bool)CarData_ReadValue(TSAC_IsShuntWorking);//momentan primesc 0 == false == safe
    BmsErrors[1] = false;//(bool)CarData_ReadValue(TSAC_IsBms0Working);
    BmsErrors[2] = true;//(bool)CarData_ReadValue(TSAC_IsBms1Working);
    BmsErrors[3] = true;//(bool)CarData_ReadValue(TSAC_IsTransceiverWorking);
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
    readValue = CarData_ReadValue(TSAC_HighestCellTemperature);
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
    readValue = CarData_ReadValue(TSAC_MedianCellTemperature);
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
    readValue = CarData_ReadValue(TSAC_LowestCellTemperature);
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
    readValue = CarData_ReadValue(TSAC_HighestCellVoltage);
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
    readValue = CarData_ReadValue(TSAC_MedianCellVoltage);
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
    readValue = CarData_ReadValue(TSAC_LowestCellVoltage);
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
    readValue = CarData_ReadValue(TSAC_OverallVoltage);
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
    readValue = CarData_ReadValue(TSAC_OverallCurrent);
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
    readValue = CarData_ReadValue(TSAC_IsAmsSafe);
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
    readValue = CarData_ReadValue(TSAC_IsShuntWorking);
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
    readValue = CarData_ReadValue(TSAC_IsTransceiverWorking);
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
    readValue = CarData_ReadValue(TSAC_IsBms0Working);
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
    readValue = CarData_ReadValue(TSAC_IsBms1Working);
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
    //TSAC_CellVoltages_Table
    window->ui->Cell_Volt_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    window->ui->Cell_Volt_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ParametersTab_Update(MainWindow* window)
{
    //TODO
}

void DebugTab_Update(MainWindow* window)
{
    //TODO
}

