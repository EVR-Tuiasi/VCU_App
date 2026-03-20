#include "mainwindow.h"
#include "UartMessaging.h"

#include <QApplication>
#include <QThread>

static const bool simulateUart = false; // --- IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ---
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
    static uint32_t AcceleratorSensor1Voltage = 0U, AcceleratorSensor1TravelPercentage = 0U, HighestCellTemperature = 0U;

    //STEP 1: incrementing the values
    AcceleratorSensor1Voltage += 10U;//increment by whatever value desired
    AcceleratorSensor1TravelPercentage += 1U;
    HighestCellTemperature += 1U;

    //STEP 2: checking if the values are valid
    if(AcceleratorSensor1Voltage > 16383U)
    {//bounds checking so we do not set values outside the allowed ones
        AcceleratorSensor1Voltage -= 16384U;
    }
    if(AcceleratorSensor1TravelPercentage > 100U)
    {
        AcceleratorSensor1TravelPercentage -= 100U;
    }
    if(HighestCellTemperature > 1023U)
    {
        HighestCellTemperature -= 1023U;
    }

    //STEP 3: setting the new values
    CarData_SetValue(PEDALS_AcceleratorSensor1Voltage, AcceleratorSensor1Voltage);
    CarData_SetValue(PEDALS_AcceleratorSensor1TravelPercentage, AcceleratorSensor1TravelPercentage);
    CarData_SetValue(TSAC_HighestCellTemperature, HighestCellTemperature);

}

