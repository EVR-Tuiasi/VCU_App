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
}
#ifdef __cplusplus
}
#endif
