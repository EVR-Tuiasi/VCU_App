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
    uint32_t readValue;
    char char_array[20], index = 0U;
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
    char_array[6] = 0U;//needed so the "string" can be terminated
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
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
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
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
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
    char_array[0] = '0' + readValue/1000U;
    if(readValue >= 1000U){
        char_array[index++] = '0' + (readValue/1000U);
    }
    if(readValue >= 100U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
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

/*
    //PEDALS_PressureSensorVoltage
    readValue = (CarData_ReadValue(PEDALS_PressureSensorVoltage)* 5000U) / 500U; /* 9 bits, 0-500, 0 to 5.00 Volts, 0.1 Volts per bit
    char_array[0] = '0' + readValue/1000U;
    char_array[1] = '.';
    char_array[2] = '0' + (readValue/100U)%10U;
    char_array[3] = '0' +(readValue/10U)%10U;
    char_array[4] = '0' + readValue%10U;
    char_array[5] = 'V';
    char_array[6] = 0U;
    //window->ui->Brake_Volt_S2_lineEdit->setText((const QString)QString(char_array));

    //PEDALS_PressureSensorBars
    readValue = CarData_ReadValue(PEDALS_PressureSensorBars);
    index = 0U;
    if(readValue >= 255U)
    {
        char_array[index++] = '0' + readValue/255U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/10U)%10U;
    }
    char_array[index++] = '0' + readValue%10U;
    char_array[index++] = 'B';
    char_array[index++] = 0;
*/

}

void TsacTab_Update(MainWindow* window)
{
    uint32_t readValue;
    char char_array[20], index = 0U;
    //TSAC
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
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
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
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
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
    if(readValue >= 100U)
    {
        char_array[index++] = '0' + readValue/1000U;
    }
    if(readValue >= 10U){
        char_array[index++] = '0' + (readValue/100U)%10U;
    }
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

    //TODO rest
}

void ParametersTab_Update(MainWindow* window)
{
    //TODO
}

void DebugTab_Update(MainWindow* window)
{
    //TODO
}

