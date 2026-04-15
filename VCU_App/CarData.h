#ifndef CAR_DATA_H
#define CAR_DATA_H

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

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
#define CELLS_NUM 400
#define THERMISTOR_NUM 100

typedef struct
{
    /* General values */
    uint16_t MedianCellTemperature;                     /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    uint16_t HighestCellTemperature;                    /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    uint16_t LowestCellTemperature;                     /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    uint16_t MedianCellVoltage;                         /* 10 bits, 0-1023, 0 to 10.23 Volts, 0.01 Volts per bit */
    uint16_t HighestCellVoltage;                        /* 10 bits, 0-1023, 0 to 10.23 Volts, 0.01 Volts per bit */
    uint16_t LowestCellVoltage;                         /* 10 bits, 0-1023, 0 to 10.23 Volts, 0.01 Volts per bit */
    uint16_t OverallVoltage;                            /* 11 bits, 0-2047, 0 to 204.7 Volts, 0.1 Volts per bit */
    uint16_t OverallCurrent;                            /* 13 bits, 0-8095, 0 to 809.5 Amps, 0.1 Amps per bit */
    /* Cell voltages and temperatures*/
    uint32_t CellVoltage[CELLS_NUM];                    /* TODO */
    uint16_t ThermistorTemperature[THERMISTOR_NUM];     /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    /* Status and errors */
    bool AmsError;                                      /* 1 bit, 0 means safe, 1 means errors */
    bool TransceiverError;                              /* 1 bit, 0 means safe, 1 means errors */
    bool ShuntError;                                    /* 1 bit, 0 means safe, 1 means errors */
    bool Bms0Error;                                     /* 1 bit, 0 means safe, 1 means errors */
    bool Bms1Error;                                     /* 1 bit, 0 means safe, 1 means errors */
}TsacMonitoredValues_t;

typedef struct
{
    /* Accelerator */
    uint16_t AcceleratorSensor1Voltage;                 /* 14 bits, 0-16383, 0 to 5.00 Volts */
    uint16_t AcceleratorSensor2Voltage;                 /* 14 bits, 0-16383, 0 to 5.00 Volts */
    uint8_t AcceleratorSensor1TravelPercentage;         /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    uint8_t AcceleratorSensor2TravelPercentage;         /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    /* Brake */
    uint16_t BrakeSensor1Voltage;                       /* 14 bits, 0-16383, 0 to 5.00 Volts */
    uint16_t BrakeSensor2Voltage;                       /* 14 bits, 0-16383, 0 to 5.00 Volts */
    uint8_t BrakeSensor1TravelPercentage;               /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    uint8_t BrakeSensor2TravelPercentage;               /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    /* Pressure */
    uint16_t PressureSensorVoltage;                     /* 9 bits, 0-500, 0 to 5.00 Volts, 0.1 Volts per bit */
    uint8_t PressureSensorBars;                         /* 8 bits, 0-255, 0 to 255 Bars, 1 Bar per bit */
    /* Status and errors */
    bool Accel_Sensor1_ShortToGnd;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Accel_Sensor1_ShortToVcc;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Accel_Sensor1_OutOfRangeOutput;                /* 1 bit, 0 means safe, 1 means errors */
    bool Accel_Sensor2_ShortToGnd;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Accel_Sensor2_ShortToVcc;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Accel_Sensor2_OutOfRangeOutput;                /* 1 bit, 0 means safe, 1 means errors */
    bool Accel_Implausibility;                          /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Sensor1_ShortToGnd;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Sensor1_ShortToVcc;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Sensor1_OutOfRangeOutput;                /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Sensor2_ShortToGnd;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Sensor2_ShortToVcc;                      /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Sensor2_OutOfRangeOutput;                /* 1 bit, 0 means safe, 1 means errors */
    bool Brake_Implausibility;                          /* 1 bit, 0 means safe, 1 means errors */  
}PedalsMonitoredValues_t;

typedef struct
{
    /* Left inverter and motor */
    uint8_t LeftInverterTemperature;                    /* 8 bits, 0-255, -40 to 215 degrees C, 1 degree C per bit */
    uint8_t LeftMotorTemperature;                       /* 8 bits, 0-255, -30 to 225 degrees C, 1 degree C per bit */
    uint16_t LeftInverterInputVoltage;                  /* 11 bits, 0-1800, 0 to 180.0 Volts, 0.1 Volts per bit */
    uint16_t LeftInverterCurrent;                       /* 12 bits, 0-4000, 0 to 400.0 Amps, 0.1 Amps per bit */
    uint16_t LeftMotorRpm;                              /* 13 bits, 0-6000, 0 to 6000 RPM, 1 RPM per bit */
    uint8_t LeftMotorSpeedKmh;                          /* 8 bits, 0-255, 0 to 255 Km/h, 1 Km/h per bit */
    uint8_t LeftInverterThrottle;                       /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    uint8_t LeftInverterThrottleFeedback;               /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    /* Right inverter and motor */
    uint8_t RightInverterTemperature;                   /* 8 bits, 0-255, -40 to 215 degrees C, 1 degree C per bit */
    uint8_t RightMotorTemperature;                      /* 8 bits, 0-255, -30 to 225 degrees C, 1 degree C per bit */
    uint16_t RightInverterInputVoltage;                 /* 11 bits, 0-1800, 0 to 180.0 Volts, 0.1 Volts per bit */
    uint16_t RightInverterCurrent;                      /* 12 bits, 0-4000, 0 to 400.0 Amps, 0.1 Amps per bit */
    uint16_t RightMotorRpm;                             /* 13 bits, 0-6000, 0 to 6000 RPM, 1 RPM per bit */
    uint8_t RightMotorSpeedKmh;                         /* 8 bits, 0-255, 0 to 255 Km/h, 1 Km/h per bit */
    uint8_t RightInverterSentThrottle;                  /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    uint8_t RightInverterThrottleFeedback;              /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    /* Status and errors */
    bool IsCarInReverse;                                /* 1 bit, 0 means car is in FORWARD, 1 means car is in REVERSE */
    bool IsCarRunning;                                  /* 1 bit, 0 means car is RUNNING (Responding to accelerator pedal), 1 means car is IDLE (NOT responding to accelerator pedal)*/
}InvertersMonitoredValues_t;

typedef struct
{
    bool ActivationButtonPressed;                       /* 1 bit, 0 means button is NOT ACTIVATED, 1 means button is ACTIVATED */
    bool CarReverseCommandPressed;                      /* 1 bit, 0 means reverse command is NOT ACTIVATED, 1 means reverse command is ACTIVATED */
    bool IsDisplayWorking;                              /* 1 bit, 0 means display is WORKING, 1 means display is NOT WORKING */
    bool IsSegmentsDriverWorking;                       /* 1 bit, 0 means segments driver is WORKING, 1 means segments driver is NOT WORKING */
}DashboardMonitoredValues_t;

typedef enum{
    /* TSAC */
    TSAC_MedianCellTemperature,                         /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    TSAC_HighestCellTemperature,                        /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    TSAC_LowestCellTemperature,                         /* 10 bits, 0-1023, 0 to 102.3 degrees C, 0.1 degrees C per bit */
    TSAC_MedianCellVoltage,                             /* 10 bits, 0-1023, 0 to 10.23 Volts, 0.01 Volts per bit */
    TSAC_HighestCellVoltage,                            /* 10 bits, 0-1023, 0 to 10.23 Volts, 0.01 Volts per bit */
    TSAC_LowestCellVoltage,                             /* 10 bits, 0-1023, 0 to 10.23 Volts, 0.01 Volts per bit */
    TSAC_OverallVoltage,                                /* 11 bits, 0-2047, 0 to 204.7 Volts, 0.1 Volts per bit */
    TSAC_OverallCurrent,                                /* 13 bits, 0-8095, 0 to 809.5 Amps, 0.1 Amps per bit */
    TSAC_IsAmsSafe,                                     /* 1 bit, 0 means safe, 1 means errors */
    TSAC_IsTransceiverWorking,                          /* 1 bit, 0 means safe, 1 means errors */
    TSAC_IsShuntWorking,                                /* 1 bit, 0 means safe, 1 means errors */
    TSAC_IsBms0Working,                                 /* 1 bit, 0 means safe, 1 means errors */
    TSAC_IsBms1Working,                                 /* 1 bit, 0 means safe, 1 means errors */
    /* PEDALS */
    PEDALS_AcceleratorSensor1Voltage,                   /* 14 bits, 0-16383, 0 to 5.000 Volts */
    PEDALS_AcceleratorSensor2Voltage,                   /* 14 bits, 0-16383, 0 to 5.000 Volts */
    PEDALS_AcceleratorSensor1TravelPercentage,          /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    PEDALS_AcceleratorSensor2TravelPercentage,          /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    PEDALS_BrakeSensor1Voltage,                         /* 14 bits, 0-16383, 0 to 5.000 Volts */
    PEDALS_BrakeSensor2Voltage,                         /* 14 bits, 0-16383, 0 to 5.000 Volts */
    PEDALS_BrakeSensor1TravelPercentage,                /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    PEDALS_BrakeSensor2TravelPercentage,                /* 7 bits, 0-100, 0 to 100 Percent, 1 Percent per bit */
    PEDALS_PressureSensorVoltage,                       /* 9 bits, 0-500, 0 to 5.00 Volts, 0.1 Volts per bit */
    PEDALS_PressureSensorBars,                          /* 8 bits, 0-255, 0 to 255 Bars, 1 Bar per bit */
    PEDALS_Accel_Sensor1_ShortToGnd,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Accel_Sensor1_ShortToVcc,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Accel_Sensor1_OutOfRangeOutput,              /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Accel_Sensor2_ShortToGnd,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Accel_Sensor2_ShortToVcc,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Accel_Sensor2_OutOfRangeOutput,              /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Accel_Implausibility,                        /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Sensor1_ShortToGnd,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Sensor1_ShortToVcc,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Sensor1_OutOfRangeOutput,              /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Sensor2_ShortToGnd,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Sensor2_ShortToVcc,                    /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Sensor2_OutOfRangeOutput,              /* 1 bit, 0 means safe, 1 means errors */
    PEDALS_Brake_Implausibility,                        /* 1 bit, 0 means safe, 1 means errors */
    /* INVERTERS */
    INVERTERS_LeftInverterTemperature,                  /* 8 bits, 0-255, -40 to 215 degrees C, 1 degree C per bit */
    INVERTERS_LeftMotorTemperature,                     /* 8 bits, 0-255, -30 to 225 degrees C, 1 degree C per bit */
    INVERTERS_LeftInverterInputVoltage,                 /* 11 bits, 0-1800, 0 to 180.0 Volts, 0.1 Volts per bit */
    INVERTERS_LeftInverterCurrent,                      /* 12 bits, 0-4000, 0 to 400.0 Amps, 0.1 Amps per bit */
    INVERTERS_LeftMotorRpm,                             /* 13 bits, 0-6000, 0 to 6000 RPM, 1 RPM per bit */
    INVERTERS_LeftMotorSpeedKmh,                        /* 8 bits, 0-255, 0 to 255 Km/h, 1 Km/h per bit */
    INVERTERS_LeftInverterThrottle,                     /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    INVERTERS_LeftInverterThrottleFeedback,             /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    INVERTERS_RightInverterTemperature,                 /* 8 bits, 0-255, -40 to 215 degrees C, 1 degree C per bit */
    INVERTERS_RightMotorTemperature,                    /* 8 bits, 0-255, -30 to 225 degrees C, 1 degree C per bit */
    INVERTERS_RightInverterInputVoltage,                /* 11 bits, 0-1800, 0 to 180.0 Volts, 0.1 Volts per bit */
    INVERTERS_RightInverterCurrent,                     /* 12 bits, 0-4000, 0 to 400.0 Amps, 0.1 Amps per bit */
    INVERTERS_RightMotorRpm,                            /* 13 bits, 0-6000, 0 to 6000 RPM, 1 RPM per bit */
    INVERTERS_RightMotorSpeedKmh,                       /* 8 bits, 0-255, 0 to 255 Km/h, 1 Km/h per bit */
    INVERTERS_RightInverterSentThrottle,                /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    INVERTERS_RightInverterThrottleFeedback,            /* 8 bits, 0-250, 0 to 5.00 Volts, 0.02 Volts per bit */
    INVERTERS_IsCarInReverse,                           /* 1 bit, 0 means car is in FORWARD, 1 means car is in REVERSE */
    INVERTERS_IsCarRunning,                             /* 1 bit, 0 means car is RUNNING (Responding to accelerator pedal), 1 means car is IDLE (NOT responding to accelerator pedal)*/
    /* DASHBOARD */
    DASHBOARD_ActivationButtonPressed,                  /* 1 bit, 0 means button is NOT ACTIVATED, 1 means button is ACTIVATED */
    DASHBOARD_CarReverseCommandPressed,                 /* 1 bit, 0 means reverse command is NOT ACTIVATED, 1 means reverse command is ACTIVATED */
    DASHBOARD_IsDisplayWorking,                         /* 1 bit, 0 means display is WORKING, 1 means display is NOT WORKING */
    DASHBOARD_IsSegmentsDriverWorking                   /* 1 bit, 0 means segments driver is WORKING, 1 means segments driver is NOT WORKING */
}MonitoredValue_t;

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


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void CarData_SetValue(MonitoredValue_t DesiredValueType, uint32_t Value);
uint32_t CarData_ReadValue(MonitoredValue_t DesiredValueType);

#ifdef __cplusplus
}
#endif

#endif
