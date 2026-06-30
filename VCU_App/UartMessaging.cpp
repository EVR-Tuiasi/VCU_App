#include <QSerialPort>
#include <QElapsedTimer>
#include <QDebug>
#include <Qdebug>

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "stdint.h"
#include "UartMessaging.h"
#include "CarData.h"


/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
extern MonitoredValues_t MonitoredValues;
static QSerialPort *serialPort = nullptr;
static QElapsedTimer timer;
static uint32_t timeSinceLastMessage = 0;
static ComPortSettings_t settings = {false, 921600, "COM4"};
static bool simulateSend = false;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
static void UartMessaging_ParseBuffer()
{
    //bucla while cat timp sunt minim 10 caractere disponibile (lungimea unui mesaj intreg):
        //verifica daca sunt 10 caractere disponibile pentru citit de pe portul COM conectat
        //apeleaza UartMessaging_CheckValidityOfBuffer pentru a verifica daca bufferul are header si CRC valid
        //daca bufferul e valid:
            //apeleaza UartMessaging_ExtractValuesFromValidatedBuffer pentru a extrage valorile din buffer si a le stoca in structurile globale
        //else daca bufferul nu are header valid:
            //scoate un singur caracter din buffer
        //else daca bufferul nu are crc valid:
            //scoate 10 caractere din buffer (lungimea unui mesaj intreg)
    //reia bucla

    while(serialPort->bytesAvailable()>=10){
        uint8_t buffer[10];
        serialPort->peek((char*)(buffer), 10);
        UartBufferValidity_t response = UartMessaging_CheckValidityOfBuffer(buffer);
        if(response==BufferValid){
            serialPort->read((char*)(buffer), 10);
            timeSinceLastMessage = 0;
            timer.restart();
            UartMessaging_ExtractValuesFromValidatedBuffer(buffer);
        }
        else{
            if(response==BufferHeaderInvalid){
                char tmp[1];
                serialPort->read(tmp, 1);
            }
            else{
                serialPort->read(reinterpret_cast<char*>(buffer), 10);
            }
        }
    }

}

static uint8_t calculateCRC(uint8_t buffer[10]) /* This was slightly modified but NOT tested so it NEEDS to be retested! Do not assume it works */
{
    uint8_t divisor = 0x8D, dividend;
    int i, j;

    dividend = ((uint8_t)buffer[0] << 8) | buffer[1];
    for (j = 15; j >= 8; j--)
        if ((dividend & (1 << j)) != 0)
            dividend ^= divisor << (j - 8);

    for (i = 2; i < 10; i++)
    {
        dividend = (dividend << 8) | buffer[i];

        for (j = 15; j >= 8; j--)
            if ((dividend & (1 << j)) != 0)
                dividend ^= divisor << (j - 8);
    }
    return (uint8_t)(dividend & 0xFF);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void UartMessaging_SetConnection(bool connected) {
    settings.shouldPortBeConnected = connected;
}
void UartMessaging_SetBaudRate(int BaudRate)
{
    settings.desiredBaudRate=BaudRate;
}

void UartMessaging_Update(void){
    if(!serialPort)
        serialPort = new QSerialPort();
    /*qDebug()<<"PORT COM:"<<serialPort->portName();
    qDebug()<<"Uart: val.shouldPortBeConencted:"<<settings.desiredBaudRate;
    qDebug()<<"Uart: val.shouldPortBeConencted:"<<settings.shouldPortBeConnected;*/
    if(settings.shouldPortBeConnected){
        if(serialPort->isOpen()){
            if(simulateSend){
                if(serialPort->isWritable())
                    sendTest();
            }
            if(serialPort->waitForReadyRead(0)){
                if(serialPort->bytesAvailable()>=10)
                    UartMessaging_ParseBuffer();
            }
            else{
                timeSinceLastMessage = timer.elapsed();
            }
        }
        else{
            serialPort->setPortName(settings.port);
            serialPort->setBaudRate(settings.desiredBaudRate);
            serialPort->setReadBufferSize(1000);
            serialPort->setWriteBufferSize(1000);
            serialPort->setDataBits(QSerialPort::Data8);
            serialPort->setParity(QSerialPort::NoParity);
            if(serialPort->open(QIODevice::ReadWrite)){
                //serialPort->setDataTerminalReady(true);
                serialPort->clear();
                timer.start();
                timeSinceLastMessage = 0;
            }
        }
    }
    else{
        if(serialPort->isOpen()){
            timer.invalidate();
            timeSinceLastMessage = 0;
            serialPort->close();
            if(serialPort){
                delete serialPort;
                serialPort = nullptr;
            }
        }
    }
    //daca ar trebui sa fie conectat:
        //verifica daca e conectat
        //daca e conectat:
            //verifica cati octeti sunt disponibili in buffer-ul portului COM
            //daca sunt minim 10 octeti disponibili:
                //apeleaza UartMessaging_ParseBuffer
            //else daca NU sunt minim 10 octeti disponibili pentru citire:
                //seteaza timpul de la ultimul mesaj cu valoarea temporizatorului
        //else daca NU e conectat:
            //seteaza portul si baudrate-ul
            //conecteaza portul COM
            //sterge bufferul sau intern
            //porneste temporizatorul
    //else daca NU ar trebui sa fie conectat:
        //verifica daca e conectat
        //daca e conectat:
            //deconecteaza portul COM
            //seteaza timpul de la ultimul mesaj pe 0
            //opreste temporizatorul

}

static UartBufferValidity_t UartMessaging_CheckValidityOfBuffer(uint8_t buffer[10]){ //this should check if the provided buffer has a valid CRC and header type.
    uint8_t readCRC = buffer[9];
    uint8_t readHeader = buffer[0];
    uint8_t calculatedCRC = calculateCRC(buffer);
    if(readCRC!=calculatedCRC)
        return BufferCrcInvalid;
    switch (readHeader) {
        case ID_UART_ACCELERATIE:
        case ID_UART_BATERIE:
        case ID_UART_BATERIE_TENSIUNI_CELULE:
        case ID_UART_BATERIE_TEMPERATURI_CELULE:
        case ID_UART_BATERIE_2:
        case ID_UART_BATERIE_CHARGER:
        case ID_UART_BORD:
        case ID_UART_FRANA:
        case ID_UART_INVERTOARE:
        case ID_UART_INVERTOR_STANGA:
        case ID_UART_INVERTOR_DREAPTA:
            return BufferValid;
        default:
            return BufferHeaderInvalid;
    }
}
static void UartMessaging_ExtractValuesFromValidatedBuffer(uint8_t buffer[10]){//this should parse the data from the buffer and update the global structure holding all values with the received ones.
    uint64_t buffer_merged;
        uint8_t id = buffer[0];
    uint8_t data[8];
    for(int i=1;i<=8;i++){
        data[i-1] = buffer[i];
    }
    buffer_merged = (((uint64_t)data[0]) << 56) + (((uint64_t)data[1]) << 48) + (((uint64_t)data[2]) << 40) + (((uint64_t)data[3]) << 32) + (((uint64_t)data[4]) << 24) + (((uint64_t)data[5]) << 16) + (((uint64_t)data[6]) << 8) + (uint64_t)data[7];

    //qDebug()<<id<<" "<<data[0]<<" "<<data[1]<<" "<<data[2]<<" "<<data[3]<<" "<<data[4]<<" "<<data[5]<<" "<<data[6]<<" "<<data[7];
    switch(id){
        case ID_UART_FRANA:
            //extragere date
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.BrakeSensor1Voltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.BrakeSensor2Voltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.BrakeSensor1TravelPercentage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.BrakeSensor2TravelPercentage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.PressureSensorBars);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Implausibility);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_OutOfRangeOutput);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToVcc);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor1_ShortToGnd);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_OutOfRangeOutput);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToVcc);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Brake_Sensor2_ShortToGnd);
			break;

        case ID_UART_ACCELERATIE:
            //extragere date
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1Voltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2Voltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor1TravelPercentage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.AcceleratorSensor2TravelPercentage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.PressureSensorVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Implausibility);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_OutOfRangeOutput);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToVcc);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor1_ShortToGnd);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_OutOfRangeOutput);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToVcc);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.PedalsMonitoredValues.Accel_Sensor2_ShortToGnd);
			break;

        case ID_UART_INVERTOR_STANGA:
            //extragere date
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftMotorTemperature);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftInverterTemperature);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftInverterThrottle);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftMotorSpeedKmh);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftInverterThrottleFeedback);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftInverterInputVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftMotorRpm);
			break;

        case ID_UART_INVERTOR_DREAPTA:
            //extragere date
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightMotorTemperature);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightInverterTemperature);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightInverterThrottle);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightMotorSpeedKmh);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightInverterThrottleFeedback);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightInverterInputVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightMotorRpm);
			break;

        case ID_UART_INVERTOARE:
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.IsCarRunning);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.IsCarInReverse);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.LeftInverterCurrent);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.InvertersMonitoredValues.RightInverterCurrent);
			break;

        case ID_UART_BATERIE:
            //extragere date
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.OverallCurrent);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.OverallVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.HighestCellTemperature);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.HighestCellVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.LowestCellVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.LowestCellTemperature);
			break;

        case ID_UART_BATERIE_TENSIUNI_CELULE:{
            uint8_t index = ((uint8_t)(data[0])) & (0x07);
            index = index * 5;
            UartMessaging_SetCellVoltageErrors(((data[0] & (1<<7)) >> 7), index + 0);
            UartMessaging_SetCellVoltageErrors(((data[0] & (1<<6)) >> 6), index + 1);
            UartMessaging_SetCellVoltageErrors(((data[0] & (1<<5)) >> 5), index + 2);
            UartMessaging_SetCellVoltageErrors(((data[0] & (1<<4)) >> 4), index + 3);
            UartMessaging_SetCellVoltageErrors(((data[0] & (1<<3)) >> 3), index + 4);
            UartMessaging_SetCellVoltage((((((uint16_t)data[1]) << 8) | data[2]) & (0x03FF)), index + 0);
            UartMessaging_SetCellVoltage(((((((uint16_t)data[3]) << 8) | data[4]) >> 6) & (0x03FF)), index + 1);
            UartMessaging_SetCellVoltage(((((((uint16_t)data[4]) << 8) | data[5]) >> 4) & (0x03FF)), index + 2);
            UartMessaging_SetCellVoltage(((((((uint16_t)data[5]) << 8) | data[6]) >> 2) & (0x03FF)), index + 3);
            UartMessaging_SetCellVoltage((((((uint16_t)data[6]) << 8) | data[7]) & (0x03FF)), index + 4);
            break;
        }

        case ID_UART_BATERIE_TEMPERATURI_CELULE:{
            uint16_t index = (uint16_t)((data[0] << 2) | (data[1] >> 6)) & (0x001F);
            index = index * 5;
            UartMessaging_SetCellTemperatureErrors(((data[0] & (1<<7)) >> 7), index + 0);
            UartMessaging_SetCellTemperatureErrors(((data[0] & (1<<6)) >> 6), index + 1);
            UartMessaging_SetCellTemperatureErrors(((data[0] & (1<<5)) >> 5), index + 2);
            UartMessaging_SetCellTemperatureErrors(((data[0] & (1<<4)) >> 4), index + 3);
            UartMessaging_SetCellTemperatureErrors(((data[0] & (1<<3)) >> 3), index + 4);
            UartMessaging_SetCellTemperature((((((uint16_t)data[1]) << 8) | data[2]) & (0x03FF)), index + 0);
            UartMessaging_SetCellTemperature(((((((uint16_t)data[3]) << 8) | data[4]) >> 6) & (0x03FF)), index + 1);
            UartMessaging_SetCellTemperature(((((((uint16_t)data[4]) << 8) | data[5]) >> 4) & (0x03FF)), index + 2);
            UartMessaging_SetCellTemperature(((((((uint16_t)data[5]) << 8) | data[6]) >> 2) & (0x03FF)), index + 3);
            UartMessaging_SetCellTemperature((((((uint16_t)data[6]) << 8) | data[7]) & (0x03FF)), index + 4);
            break;
        }

        case ID_UART_BATERIE_2:
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.MedianCellTemperature);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.MedianCellVoltage);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.ShuntError);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.TransceiverError);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.Bms0Error);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.Bms1Error);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.ThermistorsError);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.AmsError);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.DesiredChargingCurrent);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.DesiredChargingVoltage);
			break;

        case ID_UART_BATERIE_CHARGER:
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.ReportedChargingCurrent);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.ReportedChargingVolts);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.TsacMonitoredValues.ChargerCommand);
			break;

        case ID_UART_BORD:
            //extragere date
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.DashboardMonitoredValues.ActivationButtonPressed);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.DashboardMonitoredValues.CarReverseCommandPressed);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.DashboardMonitoredValues.IsDisplayWorking);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.DashboardMonitoredValues.IsSegmentsDriverWorking);
			break;
        case ID_UART_COMUNICATII:
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.CommunicationsMonitoredValues.IsDashboardVCUSimulated);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.CommunicationsMonitoredValues.IsInvertersVCUSimulated);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.CommunicationsMonitoredValues.IsPedalsVCUSimulated);
			WriteUartDataAtAddress(buffer_merged, &MonitoredValues.CommunicationsMonitoredValues.IsTsacVCUSimulated);
			break;
    }
}

void sendTest(){
    while(serialPort->isWritable()){
        //qDebug()<<serialPort->write("a\n", 2)<<'\n';
        //qDebug()<<serialPort->waitForBytesWritten(10)<<'\n';
    }
}

#ifdef __cplusplus
}
#endif
