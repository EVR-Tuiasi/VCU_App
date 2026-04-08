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
    MainWindow_Show(); //Make the user interface visible on the screen
    dataAcquisitionThread->start(); //Start the data acquisition thread
    a.exec(); //this function is the main app's internal loop. This returns when the user closes the app.
    shouldDataAcquisitionThreadRun = false; //Signal the data acquisition thread that it should stop.
    while(dataAcquisitionThread->isRunning()); //Wait for the data acquisition thread to stop.
    return 0;
}

void SimulatedData_Update(void)
{//this is called by the data acquisition thread. This should create new data and save it in the global structures with the CarData_SetValue function
    //PEDALS
    static uint16_t AcceleratorSensor1Voltage = 0U, AcceleratorSensor2Voltage = 0U, BrakeSensor1Voltage = 0U, BrakeSensor2Voltage = 0U;
    static uint8_t  AcceleratorSensor1TravelPercentage = 0U, AcceleratorSensor2TravelPercentage = 0U, BrakeSensor1TravelPercentage = 0U, BrakeSensor2TravelPercentage = 0U;
    static uint16_t PressureSensorVoltage = 0U;
    static uint8_t PressureSensorBars = 0U;
    //TSAC
    static uint16_t MedianCellTemperature = 0U, HighestCellTemperature = 0U, LowestCellTemperature = 0U, MedianCellVoltage = 0U, HighestCellVoltage = 0U, LowestCellVoltage = 0U, OverallVoltage = 0U, OverallCurrent = 0U;
    static uint8_t LeftInverterTemperature = 40U, LeftMotorTemperature = 30U, RightInverterTemperature = 40U, RightMotorTemperature = 30U;
    //INVERTERS
    static uint16_t LeftInverterInputVoltage = 0U, LeftInverterCurrent = 0U, RightInverterInputVoltage = 0U, RightInverterCurrent = 0U, LeftMotorRpm = 0U, RightMotorRpm = 0U;
    static uint8_t LeftMotorSpeedKmh = 0U, LeftInverterThrottle = 0U, LeftInverterThrottleFeedback = 0U, RightMotorSpeedKmh = 0U, RightInverterSentThrottle = 0U, RightInverterThrottleFeedback = 0U;


    static bool AmsError, ImdError, TransceiverError, ShuntError, Bms0Error, Bms1Error;

    //STEP 1: incrementing the values
    AcceleratorSensor1Voltage += 10U;//increment by whatever value desired
    AcceleratorSensor2Voltage += 10U;
    AcceleratorSensor1TravelPercentage += 1U;
    AcceleratorSensor2TravelPercentage += 1U;
    BrakeSensor1Voltage += 10U;
    BrakeSensor2Voltage += 10U;
    BrakeSensor1TravelPercentage += 1U;
    BrakeSensor2TravelPercentage += 1U;
    PressureSensorVoltage += 1U;
    PressureSensorBars += 1U;

    MedianCellTemperature += 1U;
    HighestCellTemperature += 1U;
    LowestCellTemperature += 1U;
    MedianCellVoltage += 1U;
    HighestCellVoltage += 1U;
    LowestCellVoltage += 1U;
    OverallVoltage += 1U;
    OverallCurrent += 1U;

    LeftInverterTemperature += 1U;
    LeftMotorTemperature += 1U;
    LeftInverterInputVoltage += 1U;
    LeftInverterCurrent += 1U;
    RightInverterTemperature += 1U;
    RightMotorTemperature += 1U;
    RightInverterInputVoltage += 1U;
    RightInverterCurrent += 1U;
    LeftMotorRpm += 1U;
    RightMotorRpm += 1U;
    LeftMotorSpeedKmh += 1U;
    LeftInverterThrottle += 1U;
    LeftInverterThrottleFeedback += 1U;
    RightMotorSpeedKmh += 1U;
    RightInverterSentThrottle += 1U;
    RightInverterThrottleFeedback += 1U;

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
    if(LeftInverterTemperature > 255U)
    {
        LeftInverterTemperature -= 256U;
    }
    if(LeftMotorTemperature > 255U)
    {
        LeftMotorTemperature -= 256U;
    }
    if(RightInverterTemperature > 255U)
    {
        RightInverterTemperature -= 256U;
    }
    if(RightMotorTemperature > 255U)
    {
        RightMotorTemperature -= 256U;
    }
    if(LeftInverterInputVoltage > 1800U)
    {
        LeftInverterInputVoltage -= 1801U;
    }
    if(LeftInverterCurrent > 4000U)
    {
        LeftInverterCurrent -= 4001U;
    }
    if(RightInverterInputVoltage > 1800U)
    {
        RightInverterInputVoltage -= 1801U;
    }
    if(RightInverterCurrent > 4000U)
    {
        RightInverterCurrent -= 4001U;
    }
    if(LeftMotorRpm > 6000U)
    {
        LeftMotorRpm -= 6001U;
    }
    if(RightMotorRpm > 6000U)
    {
        RightMotorRpm -= 6001U;
    }
    if(LeftMotorSpeedKmh > 255U)
    {
        LeftMotorSpeedKmh -= 256U;
    }
    if(RightMotorSpeedKmh > 255U)
    {
        RightMotorSpeedKmh -= 256U;
    }
    if(LeftInverterThrottle > 255U)
    {
        LeftInverterThrottle -= 256U;
    }
    if(RightInverterSentThrottle > 255U)
    {
        RightInverterSentThrottle -= 256U;
    }
    if(LeftInverterThrottleFeedback > 255U)
    {
        LeftInverterThrottleFeedback -= 256U;
    }
    if(RightInverterThrottleFeedback > 255U)
    {
        RightInverterThrottleFeedback -= 256U;
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

    CarData_SetValue(INVERTERS_LeftInverterTemperature, LeftInverterTemperature);
    CarData_SetValue(INVERTERS_LeftMotorTemperature, LeftMotorTemperature);
    CarData_SetValue(INVERTERS_RightInverterTemperature, RightInverterTemperature);
    CarData_SetValue(INVERTERS_RightMotorTemperature, RightMotorTemperature);
    CarData_SetValue(INVERTERS_LeftInverterInputVoltage, LeftInverterInputVoltage);
    CarData_SetValue(INVERTERS_LeftInverterCurrent, LeftInverterCurrent);
    CarData_SetValue(INVERTERS_RightInverterInputVoltage, RightInverterInputVoltage);
    CarData_SetValue(INVERTERS_RightInverterCurrent, RightInverterCurrent);
    CarData_SetValue(INVERTERS_LeftMotorRpm, LeftMotorRpm);
    CarData_SetValue(INVERTERS_RightMotorRpm, RightMotorRpm);
    CarData_SetValue(INVERTERS_LeftMotorSpeedKmh, LeftMotorSpeedKmh);
    CarData_SetValue(INVERTERS_LeftInverterThrottle, LeftInverterThrottle);
    CarData_SetValue(INVERTERS_LeftInverterThrottleFeedback, LeftInverterThrottleFeedback);
    CarData_SetValue(INVERTERS_RightMotorSpeedKmh, RightMotorSpeedKmh);
    CarData_SetValue(INVERTERS_RightInverterSentThrottle, RightInverterSentThrottle);
    CarData_SetValue(INVERTERS_RightInverterThrottleFeedback, RightInverterThrottleFeedback);

    CarData_SetValue(TSAC_MedianCellTemperature, MedianCellTemperature);
    CarData_SetValue(TSAC_HighestCellTemperature, HighestCellTemperature);
    CarData_SetValue(TSAC_LowestCellTemperature, LowestCellTemperature);
    CarData_SetValue(TSAC_MedianCellVoltage, MedianCellVoltage);
    CarData_SetValue(TSAC_HighestCellVoltage, HighestCellVoltage);
    CarData_SetValue(TSAC_LowestCellVoltage, LowestCellVoltage);
    CarData_SetValue(TSAC_OverallVoltage, OverallVoltage);
    CarData_SetValue(TSAC_OverallCurrent, OverallCurrent);

    CarData_SetValue(TSAC_IsAmsSafe, AmsError);
    CarData_SetValue(TSAC_IsImdSafe, ImdError);
    CarData_SetValue(TSAC_IsTransceiverWorking, TransceiverError);
    CarData_SetValue(TSAC_IsShuntWorking, ShuntError);
    CarData_SetValue(TSAC_IsBms0Working, Bms0Error);
    CarData_SetValue(TSAC_IsBms1Working, Bms1Error);


}

