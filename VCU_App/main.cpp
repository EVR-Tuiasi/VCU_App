#include "mainwindow.h"
#include "UartMessaging.h"

#include <QApplication>
#include <QThread>
static const bool simulateUart = true; // --- IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ---
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
        //QThread::msleep(10); //10ms of sleep should get us around 100 updates per second for the data. Change value if needed
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
    static uint64_t cnt = 0;
    static uint8_t CellVoltageIndex=0U, ThermistorTemperatureIndex=0U;
    static uint16_t CellVoltage[CELLS_NUM]={0}, ThermistorTemperature[THERMISTOR_NUM]={0};
    static bool CellVoltageErrors[CELLS_NUM]={false}, ThermistorTemperatureErrors[THERMISTOR_NUM]={false};

    WriteUartDataAtAddress(((cnt+60)*2)%1024, &MonitoredValues.TsacMonitoredValues.MedianCellTemperature);
    WriteUartDataAtAddress(((cnt+120)*2)%1024, &MonitoredValues.TsacMonitoredValues.HighestCellTemperature);
    WriteUartDataAtAddress((cnt*2)%1024, &MonitoredValues.TsacMonitoredValues.LowestCellTemperature);
    WriteUartDataAtAddress(((cnt+60)*2)%1024, &MonitoredValues.TsacMonitoredValues.MedianCellVoltage);
    WriteUartDataAtAddress(((cnt+120)*2)%1024, &MonitoredValues.TsacMonitoredValues.HighestCellVoltage);
    WriteUartDataAtAddress((cnt*2)%1024, &MonitoredValues.TsacMonitoredValues.LowestCellVoltage);
    WriteUartDataAtAddress((cnt*2)%2048, &MonitoredValues.TsacMonitoredValues.OverallVoltage);
    WriteUartDataAtAddress(((cnt+60)*8)%8096, &MonitoredValues.TsacMonitoredValues.OverallCurrent);

    WriteUartDataAtAddress(cnt&1, &MonitoredValues.TsacMonitoredValues.AmsError);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.TsacMonitoredValues.TransceiverError);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.TsacMonitoredValues.ShuntError);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.TsacMonitoredValues.Bms0Error);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.TsacMonitoredValues.Bms1Error);

    WriteUartDataAtAddress((cnt*30)%16384, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1Voltage);
    WriteUartDataAtAddress(((cnt+60)*30)%16384, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2Voltage);
    WriteUartDataAtAddress((cnt/4)%101, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1TravelPercentage);
    WriteUartDataAtAddress(((cnt+60)/4)%101, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2TravelPercentage);
    WriteUartDataAtAddress((cnt*30)%16384, &MonitoredValues.PedalsMonitoredValues.BrakeSensor1Voltage);
    WriteUartDataAtAddress(((cnt+60)*30)%16384, &MonitoredValues.PedalsMonitoredValues.BrakeSensor2Voltage);
    WriteUartDataAtAddress((cnt/4)%101, &MonitoredValues.PedalsMonitoredValues.BrakeSensor1TravelPercentage);
    WriteUartDataAtAddress(((cnt+60)/4)%101, &MonitoredValues.PedalsMonitoredValues.BrakeSensor2TravelPercentage);
    WriteUartDataAtAddress((cnt*3/2)%501, &MonitoredValues.PedalsMonitoredValues.PressureSensorVoltage);
    WriteUartDataAtAddress(cnt%256, &MonitoredValues.PedalsMonitoredValues.PressureSensorBars);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToGnd);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToVcc);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_OutOfRangeOutput);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToGnd);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToVcc);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_OutOfRangeOutput);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Accel_Implausibility);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToGnd);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToVcc);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_OutOfRangeOutput);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToGnd);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToVcc);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_OutOfRangeOutput);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.PedalsMonitoredValues.Brake_Implausibility);

    WriteUartDataAtAddress(cnt%256, &MonitoredValues.InvertersMonitoredValues.LeftInverterTemperature);
    WriteUartDataAtAddress((cnt+60)%256, &MonitoredValues.InvertersMonitoredValues.LeftMotorTemperature);
    WriteUartDataAtAddress(cnt%1801, &MonitoredValues.InvertersMonitoredValues.LeftInverterInputVoltage);
    WriteUartDataAtAddress(cnt%401, &MonitoredValues.InvertersMonitoredValues.LeftInverterCurrent);
    WriteUartDataAtAddress(cnt%6000, &MonitoredValues.InvertersMonitoredValues.LeftMotorRpm);
    WriteUartDataAtAddress(cnt%256, &MonitoredValues.InvertersMonitoredValues.LeftMotorSpeedKmh);
    WriteUartDataAtAddress(cnt%251, &MonitoredValues.InvertersMonitoredValues.LeftInverterThrottle);
    WriteUartDataAtAddress((cnt+60)%251, &MonitoredValues.InvertersMonitoredValues.LeftInverterThrottleFeedback);
    WriteUartDataAtAddress(cnt%256, &MonitoredValues.InvertersMonitoredValues.RightInverterTemperature);
    WriteUartDataAtAddress((cnt+60)%256, &MonitoredValues.InvertersMonitoredValues.RightMotorTemperature);
    WriteUartDataAtAddress(cnt%1801, &MonitoredValues.InvertersMonitoredValues.RightInverterInputVoltage);
    WriteUartDataAtAddress(cnt%401, &MonitoredValues.InvertersMonitoredValues.RightInverterCurrent);
    WriteUartDataAtAddress(cnt%6000, &MonitoredValues.InvertersMonitoredValues.RightMotorRpm);
    WriteUartDataAtAddress(cnt%256, &MonitoredValues.InvertersMonitoredValues.RightMotorSpeedKmh);
    WriteUartDataAtAddress(cnt%251, &MonitoredValues.InvertersMonitoredValues.RightInverterThrottle);
    WriteUartDataAtAddress((cnt+60)%251, &MonitoredValues.InvertersMonitoredValues.RightInverterThrottleFeedback);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.InvertersMonitoredValues.IsCarInReverse);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.InvertersMonitoredValues.IsCarRunning);

    WriteUartDataAtAddress(cnt&1, &MonitoredValues.DashboardMonitoredValues.ActivationButtonPressed);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.DashboardMonitoredValues.CarReverseCommandPressed);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.DashboardMonitoredValues.IsDisplayWorking);
    WriteUartDataAtAddress(cnt&1, &MonitoredValues.DashboardMonitoredValues.IsSegmentsDriverWorking);
	cnt++;

    for (int i = 0; i < CELLS_NUM; i++)
    {
        // STEP 1: increment
        CellVoltage[i] += 1U;

        // STEP 2: bounds check
        if (CellVoltage[i] > 1023U)  // max 10.23V
            CellVoltage[i] = 0U;

        // STEP 3: simulează eroare când tensiunea e în afara ferestrei normale
        CellVoltageErrors[i] = (((CellVoltage[i] < 250U) ? 1 : 0) || ((CellVoltage[i] > 420U) ? 1 : 0));//bool
        // STEP 4: set values
        UartMessaging_SetCellVoltage(CellVoltage[i], i);
        UartMessaging_SetCellVoltageErrors(CellVoltageErrors[i], i);

    }
    for (int i = 0; i < THERMISTOR_NUM; i++)
    {
        // STEP 1:
        ThermistorTemperature[i] += 2U;
         // STEP 2:
        if (ThermistorTemperature[i] > 1023U)  // max 102.3
            ThermistorTemperature[i] = 0U;
        // STEP 3:
        ThermistorTemperatureErrors[i] = (ThermistorTemperature[i] > 600U);//bool >60grade
        // STEP 4: set values
        UartMessaging_SetCellTemperature(ThermistorTemperature[i], i);
        UartMessaging_SetCellTemperatureErrors(ThermistorTemperatureErrors[i], i);
    }
}

