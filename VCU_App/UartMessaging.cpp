#include <QSerialPort>
#include <QElapsedTimer>

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

static QSerialPort *serialPort = nullptr;
static QElapsedTimer timer;
static uint32_t timeSinceLastMessage = 0;
ComPortSettings_t settings = {true, 9600};

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
        serialPort->peek(reinterpret_cast<char*>(buffer), 10);
        UartBufferValidity_t response = UartMessaging_CheckValidityOfBuffer(buffer);
        if(response==BufferValid){
            serialPort->read(reinterpret_cast<char*>(buffer), 10);
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

void UartMessaging_Update(void)
{
    if(settings.shouldPortBeConnected){
        if(!serialPort){
            serialPort = new QSerialPort();
            serialPort->setPortName("COM9");
            serialPort->setBaudRate(settings.desiredBaudRate);
            if(serialPort->open(QIODevice::ReadOnly)){
                serialPort->clear();
                timer.start();
                timeSinceLastMessage = 0;
            }
        }
        else{
            if(serialPort->isOpen()){
                if(serialPort->bytesAvailable()>=10)
                    UartMessaging_ParseBuffer();
                else
                    timeSinceLastMessage = timer.elapsed();
            }
        }
    }
    else{
        if(serialPort){
            delete serialPort;
            serialPort = nullptr;
            timer.invalidate();
            timeSinceLastMessage = 0;
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
        case idUartAcceleratie:
        case idUartBaterie:
        case idUartBord:
        case idUartFrana:
        case idUartInvertoare:
        case idUartInvertorStanga:
        case idUartInvertorDreapta:
            return BufferValid;
        default:
            return BufferHeaderInvalid;
    }
}
static void UartMessaging_ExtractValuesFromValidatedBuffer(uint8_t buffer[10]){//this should parse the data from the buffer and update the global structure holding all values with the received ones.
    uint8_t id = buffer[0];
    uint8_t data[8];
    for(int i=1;i<=8;i++)
        data[i-1] = buffer[i];
    switch(id){
        case idUartFrana:
            //extragere date
            CarData_SetValue(PEDALS_BrakeSensor1Voltage, ((((uint16_t)data[6])<<8) | data[7]) & (0x3FFF));
            CarData_SetValue(PEDALS_BrakeSensor2Voltage, ((((((uint16_t)data[4])<<8) | data[5]) & (0x0FFF)) << 2) | (data[6]>>6));
            CarData_SetValue(PEDALS_BrakeSensor1TravelPercentage, (((uint8_t)(data[3]<<4)) | (data[4]>>4)) & (0x7F));
            CarData_SetValue(PEDALS_BrakeSensor2TravelPercentage, (((uint8_t)(data[2]<<5)) | (data[3]>>3)) & (0x7F));
            CarData_SetValue(PEDALS_PressureSensorBars, ((uint8_t)(data[1]<<6)) | (data[2]>>2));
            CarData_SetValue(PEDALS_Brake_Implausibility, (data[0] & (1<<1)) >> 1);
            CarData_SetValue(PEDALS_Brake_Sensor1_OutOfRangeOutput, (data[0] & (1<<5)) >> 5);
            CarData_SetValue(PEDALS_Brake_Sensor1_ShortToVcc, (data[0] & (1<<6)) >> 6);
            CarData_SetValue(PEDALS_Brake_Sensor1_ShortToGnd, (data[0] & (1<<7)) >> 7);
            CarData_SetValue(PEDALS_Brake_Sensor2_OutOfRangeOutput, (data[0] & (1<<2)) >> 2);
            CarData_SetValue(PEDALS_Brake_Sensor2_ShortToVcc, (data[0] & (1<<3)) >> 3);
            CarData_SetValue(PEDALS_Brake_Sensor2_ShortToGnd, (data[0] & (1<<4)) >> 4);
            break;

        case idUartAcceleratie:
            //extragere date
            CarData_SetValue(PEDALS_AcceleratorSensor1Voltage, ((((uint16_t)data[6])<<8) | data[7]) & (0x3FFF));
            CarData_SetValue(PEDALS_AcceleratorSensor2Voltage, ((((((uint16_t)data[4])<<8) | data[5]) & (0x0FFF)) << 2) | (data[6]>>6));
            CarData_SetValue(PEDALS_AcceleratorSensor1TravelPercentage, (((uint8_t)(data[3]<<4)) | (data[4]>>4)) & (0x7F));
            CarData_SetValue(PEDALS_AcceleratorSensor2TravelPercentage, (((uint8_t)(data[2]<<5)) | (data[3]>>3)) & (0x7F));
            CarData_SetValue(PEDALS_PressureSensorVoltage, ((((uint16_t)data[1]<<6)) | (data[2]>>2)) & (0x01FF));
            CarData_SetValue(PEDALS_Accel_Implausibility, (data[0] & (1<<1)) >> 1);
            CarData_SetValue(PEDALS_Accel_Sensor1_OutOfRangeOutput, (data[0] & (1<<5)) >> 5);
            CarData_SetValue(PEDALS_Accel_Sensor1_ShortToVcc, (data[0] & (1<<6)) >> 6);
            CarData_SetValue(PEDALS_Accel_Sensor1_ShortToGnd, (data[0] & (1<<7)) >> 7);
            CarData_SetValue(PEDALS_Accel_Sensor2_OutOfRangeOutput, (data[0] & (1<<2)) >> 2);
            CarData_SetValue(PEDALS_Accel_Sensor2_ShortToVcc, (data[0] & (1<<3)) >> 3);
            CarData_SetValue(PEDALS_Accel_Sensor2_ShortToGnd, (data[0] & (1<<4)) >>4);
            break;

        case idUartInvertorStanga:
            //extragere date
            CarData_SetValue(INVERTERS_LeftMotorTemperature, data[7]);
            CarData_SetValue(INVERTERS_LeftInverterTemperature, data[6]);
            CarData_SetValue(INVERTERS_LeftInverterThrottle, data[5]);
            CarData_SetValue(INVERTERS_LeftMotorSpeedKmh, data[4]);
            CarData_SetValue(INVERTERS_LeftInverterThrottleFeedback, data[3]);
            CarData_SetValue(INVERTERS_LeftInverterInputVoltage, ((((uint16_t)data[1])<<8) | data[2]) & (0x07FF));
            CarData_SetValue(INVERTERS_LeftMotorRpm, ((((uint16_t)data[0])<<8) | data[1]) >> 3);
            break;

        case idUartInvertorDreapta:
            //extragere date
            CarData_SetValue(INVERTERS_RightMotorTemperature, data[7]);
            CarData_SetValue(INVERTERS_RightInverterTemperature, data[6]);
            CarData_SetValue(INVERTERS_RightInverterSentThrottle, data[5]);
            CarData_SetValue(INVERTERS_RightMotorSpeedKmh, data[4]);
            CarData_SetValue(INVERTERS_RightInverterThrottleFeedback, data[3]);
            CarData_SetValue(INVERTERS_RightInverterInputVoltage, ((((uint16_t)data[1])<<8) | data[2]) & (0x7FF));
            CarData_SetValue(INVERTERS_RightMotorRpm, ((((uint16_t)data[0])<<8) | data[1]) >> 3);
            break;

        case idUartInvertoare:
            CarData_SetValue(INVERTERS_IsCarRunning, (data[0] & (1<<7)) >> 7);
            CarData_SetValue(INVERTERS_IsCarInReverse, (data[0] & (1<<6)) >> 6);
            CarData_SetValue(INVERTERS_LeftInverterCurrent, ((((uint16_t)data[6])<<8) | data[7]) & (0x0FFF));
            CarData_SetValue(INVERTERS_RightInverterCurrent, ((((uint16_t)data[5])<<8) | data[6]) >> 4);
            break;

        case idUartBaterie:
            //extragere date
            CarData_SetValue(TSAC_OverallCurrent, ((((uint16_t)data[6])<<8) | data[7]) & (0x1FFF));
            CarData_SetValue(TSAC_OverallVoltage, ((((uint16_t)data[5])<<8) | data[6]) >> 5);
            CarData_SetValue(TSAC_HighestCellTemperature, ((((uint16_t)data[3])<<8) | data[4]) & (0x03FF));
            CarData_SetValue(TSAC_HighestCellVoltage, (((((uint16_t)data[2])<<8) | data[3]) >> 2) & (0x03FF));
            //More To Come:)
            break;

        case idUartBord:
            //extragere date
            CarData_SetValue(DASHBOARD_ActivationButtonPressed, (data[0] & (1<<7)) >> 7);
            CarData_SetValue(DASHBOARD_CarReverseCommandPressed, (data[0] & (1<<6)) >> 6);
            CarData_SetValue(DASHBOARD_IsDisplayWorking, (data[0] & (1<<5)) >> 5);
            CarData_SetValue(DASHBOARD_IsSegmentsDriverWorking, (data[0] & (1<<4)) >> 4);
            break;
    }
}
#ifdef __cplusplus
}
#endif
