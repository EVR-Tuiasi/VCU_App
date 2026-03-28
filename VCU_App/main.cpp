#include "mainwindow.h"
#include "UartMessaging.h"

#include <QApplication>
#include <QThread>

static const bool simulateUart = true; // --- IMPORTANT ---
//TRUE: values will be generated randomly by the data acquisition thread to test the user interface.
//FALSE: value will be read from the COM port
static bool shouldDataAcquisitionThreadRun = true;

void SimulatedData_Update(void);//Prototype only. Function is implemented at the bottom of the file

void SimulatedUartThread(bool AppDoesNotBuildWithoutThis)
{
    while(shouldDataAcquisitionThreadRun)
    {
        SimulatedData_Update();//call the function that handles creating the simulated uart data
        QThread::msleep(10); //10ms of sleep should get us around 100 updates per second for the data. Change value if needed
    }
}

void UartThread(bool AppDoesNotBuildWithoutThis)
{
    while(shouldDataAcquisitionThreadRun)
    {
        UartMessaging_Update(); //call the function that handles reading the actual uart data from the configured COM port
        QThread::msleep(10); //10ms of sleep should get us around 100 updates per second for the data. Change value if needed
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread* dataAcquisitionThread;
    MainWindow_Create();
    if(simulateUart == true)
    {//creare thread nou simulare date
        dataAcquisitionThread = QThread::create(SimulatedUartThread, 0);
    }
    else
    {//creare thread nou uart
        dataAcquisitionThread = dataAcquisitionThread->create(UartThread, 0);
    }
    dataAcquisitionThread->start(); //Start the data acquisition thread
    MainWindow_Show(); //Make the user interface visible on the screen
    a.exec(); //this function is the main app's internal loop. This returns when the user closes the app.
    shouldDataAcquisitionThreadRun = false; //Signal the data acquisition thread that it should stop.
    while(dataAcquisitionThread->isRunning()); //Wait for the data acquisition thread to stop.
    return 0;
}


void SimulatedData_Update(void)
{//this is called by the data acquisition thread. This should create new data and save it in the global structures with the CarData_SetValue function
    static uint32_t AcceleratorSensor1Voltage = 0U, AcceleratorSensor2Voltage = 0U, AcceleratorSensor1TravelPercentage = 0U, AcceleratorSensor2TravelPercentage = 0U;
    static uint32_t BrakeSensor1Voltage = 0U, BrakeSensor2Voltage = 0U, BrakeSensor1TravelPercentage = 0U, BrakeSensor2TravelPercentage = 0U;
    static uint32_t PressureSensorVoltage = 0U, PressureSensorBars = 0U;
    static uint32_t MedianCellTemperature = 0U, HighestCellTemperature = 0U, LowestCellTemperature = 0U, MedianCellVoltage = 0U, HighestCellVoltage = 0U, LowestCellVoltage = 0U, OverallVoltage = 0U, OverallCurrent = 0U;



    //STEP 1: incrementing the values
    AcceleratorSensor1Voltage += 10U;//increment by whatever value desired
    AcceleratorSensor2Voltage += 10U;
    AcceleratorSensor1TravelPercentage += 1U;
    AcceleratorSensor2TravelPercentage += 1U;
    BrakeSensor1Voltage += 10U;
    BrakeSensor2Voltage += 10U;
    BrakeSensor1TravelPercentage += 1U;
    BrakeSensor2TravelPercentage += 1U;
    PressureSensorVoltage += 10U;
    PressureSensorBars += 1U;

    MedianCellTemperature += 1U;
    HighestCellTemperature += 1U;
    LowestCellTemperature += 1U;
    MedianCellVoltage += 1U;
    HighestCellVoltage += 1U;
    LowestCellVoltage += 1U;
    OverallVoltage += 1U;
    OverallCurrent += 1U;

    //STEP 2: checking if the values are valid
    if(AcceleratorSensor1Voltage > 16383U)
    {//bounds checking so we do not set values outside the allowed ones
        AcceleratorSensor1Voltage -= 16384U;
    }
    if(AcceleratorSensor2Voltage >16383U)
    {
        AcceleratorSensor2Voltage -= 16384U;
    }
    if(AcceleratorSensor1TravelPercentage > 100U)
    {
        AcceleratorSensor1TravelPercentage -= 100U;
    }
    if(AcceleratorSensor2TravelPercentage > 100U)
    {
        AcceleratorSensor2TravelPercentage -= 100U;
    }
    if(BrakeSensor1Voltage > 16383U)
    {
        BrakeSensor1Voltage -= 16384U;
    }
    if(BrakeSensor2Voltage >16383U) /* 14 bits, 0-16383, 0 to 5.00 Volts */
    {
        BrakeSensor2Voltage -= 16384U;
    }
    if(BrakeSensor1TravelPercentage > 100U)
    {
        BrakeSensor1TravelPercentage -= 100U;
    }
    if(BrakeSensor2TravelPercentage > 100U)
    {
        BrakeSensor2TravelPercentage -= 100U;
    }
    if(PressureSensorVoltage > 500U)
    {
        PressureSensorVoltage -= 501U;
    }
    if(PressureSensorBars > 255U)
    {
        PressureSensorBars -= 256U;
    }

    if(MedianCellTemperature > 1023U)
    {
        MedianCellTemperature -= 1024U;
    }
    if(HighestCellTemperature > 1023U)
    {
        HighestCellTemperature -= 1024U;
    }
    if(LowestCellTemperature > 1023U)
    {
        LowestCellTemperature -= 1024U;
    }
    if(MedianCellVoltage > 1023U)
    {
        MedianCellVoltage -= 1024U;
    }
    if(HighestCellVoltage > 1023U)
    {
        HighestCellVoltage -= 1024U;
    }
    if(LowestCellVoltage > 1023U)
    {
        LowestCellVoltage -= 1024U;
    }
    if(OverallVoltage > 2047U)
    {
        OverallVoltage -= 2048U;
    }
    if(OverallCurrent > 8095U)
    {
        OverallCurrent -= 8096U;
    }


    //STEP 3: setting the new values
    CarData_SetValue(PEDALS_AcceleratorSensor1Voltage, AcceleratorSensor1Voltage);
    CarData_SetValue(PEDALS_AcceleratorSensor2Voltage, AcceleratorSensor2Voltage);
    CarData_SetValue(PEDALS_AcceleratorSensor1TravelPercentage, AcceleratorSensor1TravelPercentage);
    CarData_SetValue(PEDALS_AcceleratorSensor2TravelPercentage, AcceleratorSensor2TravelPercentage);
    CarData_SetValue(PEDALS_BrakeSensor1Voltage, BrakeSensor1Voltage);
    CarData_SetValue(PEDALS_BrakeSensor2Voltage, BrakeSensor2Voltage);
    CarData_SetValue(PEDALS_BrakeSensor1TravelPercentage, BrakeSensor1TravelPercentage);
    CarData_SetValue(PEDALS_BrakeSensor2TravelPercentage, BrakeSensor2TravelPercentage);
    CarData_SetValue(PEDALS_PressureSensorVoltage, PressureSensorVoltage);
    CarData_SetValue(PEDALS_PressureSensorBars, PressureSensorBars);


    CarData_SetValue(TSAC_MedianCellTemperature, MedianCellTemperature);
    CarData_SetValue(TSAC_HighestCellTemperature, HighestCellTemperature);
    CarData_SetValue(TSAC_LowestCellTemperature, LowestCellTemperature);
    CarData_SetValue(TSAC_MedianCellVoltage, MedianCellVoltage);
    CarData_SetValue(TSAC_HighestCellVoltage, HighestCellVoltage);
    CarData_SetValue(TSAC_LowestCellVoltage, LowestCellVoltage);
    CarData_SetValue(TSAC_OverallVoltage, OverallVoltage);
    CarData_SetValue(TSAC_OverallCurrent, OverallCurrent);






}

