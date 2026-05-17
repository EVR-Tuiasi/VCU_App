#ifndef UART_MESSAGING_H
#define UART_MESSAGING_H

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
#include "CarData.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

typedef enum{
    idUartInvertorStanga = 0x10,
    idUartInvertorDreapta = 0x11,
    idUartInvertoare = 0x12,
    idUartBord = 0x13,
    idUartAcceleratie = 0x30,
    idUartFrana = 0x31,
    idUartBaterie1 = 0x14,
    idUartBaterie2 = 0x15,
    idUartBaterie3 = 0x16,
    idUartBaterie4 = 0x17,
    idUartBaterie5 = 0x19,
    idUartComunicatii = 0x18,
}idUart_t;

typedef enum{
    BufferCrcInvalid,
    BufferHeaderInvalid,
    BufferValid
}UartBufferValidity_t;

typedef struct{
    bool shouldPortBeConnected;
    uint32_t desiredBaudRate;
    char port[6];
}ComPortSettings_t;
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


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
static UartBufferValidity_t UartMessaging_CheckValidityOfBuffer(uint8_t buffer[10]); //this should check if the provided buffer has a valid CRC and header type.
static void UartMessaging_ExtractValuesFromValidatedBuffer(uint8_t buffer[10]);//this should parse the data from the buffer and update the global structure holding all values with the received ones.
static uint8_t calculateCRC(uint8_t buffer[10]);
static void UartMessaging_ParseBuffer(void);
static void sendTest();

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

void UartMessaging_Update(void);    //the main uart function that is called inside the data acquisition thread

#ifdef __cplusplus
}
#endif

#endif
