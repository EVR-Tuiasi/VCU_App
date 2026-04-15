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


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
TsacMonitoredValues_t TsacMonitoredvalues;
PedalsMonitoredValues_t PedalsMonitoredValues;
InvertersMonitoredValues_t InvertersMonitoredValues;
DashboardMonitoredValues_t DashboardMonitoredValues;

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

void CarData_SetValue(MonitoredValue_t DesiredValueType, uint32_t Value){
    switch(DesiredValueType){
    //TSAC
    case TSAC_MedianCellTemperature:
        TsacMonitoredvalues.MedianCellTemperature = Value;
        break;
    case TSAC_HighestCellTemperature:
        TsacMonitoredvalues.HighestCellTemperature = Value;
        break;
    case TSAC_LowestCellTemperature:
        TsacMonitoredvalues.LowestCellTemperature = Value;
        break;
    case TSAC_MedianCellVoltage:
        TsacMonitoredvalues.MedianCellVoltage = Value;
        break;
    case TSAC_HighestCellVoltage:
        TsacMonitoredvalues.HighestCellVoltage = Value;
        break;
    case TSAC_LowestCellVoltage:
        TsacMonitoredvalues.LowestCellVoltage = Value;
        break;
    case TSAC_OverallVoltage:
        TsacMonitoredvalues.OverallVoltage = Value;
        break;
    case TSAC_OverallCurrent:
        if(Value>8095)
            TsacMonitoredvalues.OverallCurrent = 0;
        else
            TsacMonitoredvalues.OverallCurrent = Value;
        break;
    case TSAC_IsAmsSafe:
        TsacMonitoredvalues.AmsError = Value;
        break;
    case TSAC_IsTransceiverWorking:
        TsacMonitoredvalues.TransceiverError = Value;
        break;
    case TSAC_IsShuntWorking:
        TsacMonitoredvalues.ShuntError = Value;
        break;
    case TSAC_IsBms0Working:
        TsacMonitoredvalues.Bms0Error = Value;
        break;
    case TSAC_IsBms1Working:
        TsacMonitoredvalues.Bms1Error = Value;
        break;
    //PEDALS
    case PEDALS_AcceleratorSensor1Voltage:
        PedalsMonitoredValues.AcceleratorSensor1Voltage = Value;
        break;
    case PEDALS_AcceleratorSensor2Voltage:
        PedalsMonitoredValues.AcceleratorSensor2Voltage = Value;
        break;
    case PEDALS_AcceleratorSensor1TravelPercentage:
        if(Value>100)
            PedalsMonitoredValues.AcceleratorSensor1TravelPercentage = 0;
        else
            PedalsMonitoredValues.AcceleratorSensor1TravelPercentage = Value;
        break;
    case PEDALS_AcceleratorSensor2TravelPercentage:
        if(Value>100)
            PedalsMonitoredValues.AcceleratorSensor2TravelPercentage = 0;
        else
            PedalsMonitoredValues.AcceleratorSensor2TravelPercentage = Value;
        break;
    case PEDALS_BrakeSensor1Voltage:
        PedalsMonitoredValues.BrakeSensor1Voltage = Value;
        break;
    case PEDALS_BrakeSensor2Voltage:
        PedalsMonitoredValues.BrakeSensor2Voltage = Value;
        break;
    case PEDALS_BrakeSensor1TravelPercentage:
        if(Value>100)
            PedalsMonitoredValues.BrakeSensor1TravelPercentage = 0;
        else
            PedalsMonitoredValues.BrakeSensor1TravelPercentage = Value;
        break;
    case PEDALS_BrakeSensor2TravelPercentage:
        if(Value>100)
            PedalsMonitoredValues.BrakeSensor2TravelPercentage = 0;
        else
            PedalsMonitoredValues.BrakeSensor2TravelPercentage = Value;
        break;
    case PEDALS_PressureSensorVoltage:
        if(Value>500)
            PedalsMonitoredValues.PressureSensorVoltage = 0;
        else
            PedalsMonitoredValues.PressureSensorVoltage = Value;
        break;
    case PEDALS_PressureSensorBars:
        PedalsMonitoredValues.PressureSensorBars = Value;
        break;
    case PEDALS_Accel_Sensor1_ShortToGnd:
        PedalsMonitoredValues.Accel_Sensor1_ShortToGnd = Value;
        break;
    case PEDALS_Accel_Sensor1_ShortToVcc:
        PedalsMonitoredValues.Accel_Sensor1_ShortToVcc = Value;
        break;
    case PEDALS_Accel_Sensor1_OutOfRangeOutput:
        PedalsMonitoredValues.Accel_Sensor1_OutOfRangeOutput = Value;
        break;
    case PEDALS_Accel_Sensor2_ShortToGnd:
        PedalsMonitoredValues.Accel_Sensor2_ShortToGnd = Value;
        break;
    case PEDALS_Accel_Sensor2_ShortToVcc:
        PedalsMonitoredValues.Accel_Sensor2_ShortToVcc = Value;
        break;
    case PEDALS_Accel_Sensor2_OutOfRangeOutput:
        PedalsMonitoredValues.Accel_Sensor2_OutOfRangeOutput = Value;
        break;
    case PEDALS_Accel_Implausibility:
        PedalsMonitoredValues.Accel_Implausibility = Value;
        break;
    case PEDALS_Brake_Sensor1_ShortToGnd:
        PedalsMonitoredValues.Brake_Sensor1_ShortToGnd = Value;
        break;
    case PEDALS_Brake_Sensor1_ShortToVcc:
        PedalsMonitoredValues.Brake_Sensor1_ShortToVcc = Value;
        break;
    case PEDALS_Brake_Sensor1_OutOfRangeOutput:
        PedalsMonitoredValues.Brake_Sensor1_OutOfRangeOutput = Value;
        break;
    case PEDALS_Brake_Sensor2_ShortToGnd:
        PedalsMonitoredValues.Brake_Sensor2_ShortToGnd = Value;
        break;
    case PEDALS_Brake_Sensor2_ShortToVcc:
        PedalsMonitoredValues.Brake_Sensor2_ShortToVcc = Value;
        break;
    case PEDALS_Brake_Sensor2_OutOfRangeOutput:
        PedalsMonitoredValues.Brake_Sensor2_OutOfRangeOutput = Value;
        break;
    case PEDALS_Brake_Implausibility:
        PedalsMonitoredValues.Brake_Implausibility = Value;
        break;
    //INVERTERS
    case INVERTERS_LeftInverterTemperature:
        InvertersMonitoredValues.LeftInverterTemperature = Value;
        break;
    case INVERTERS_LeftMotorTemperature:
        InvertersMonitoredValues.LeftMotorTemperature = Value;
        break;
    case INVERTERS_LeftInverterInputVoltage:
        if(Value>1800)
            InvertersMonitoredValues.LeftInverterInputVoltage = 0;
        else
            InvertersMonitoredValues.LeftInverterInputVoltage = Value;
        break;
    case INVERTERS_LeftInverterCurrent:
        if(Value>4000)
            InvertersMonitoredValues.LeftInverterCurrent = 0;
        else
            InvertersMonitoredValues.LeftInverterCurrent = Value;
        break;
    case INVERTERS_LeftMotorRpm:
        if(Value>6000)
            InvertersMonitoredValues.LeftMotorRpm = 0;
        else
            InvertersMonitoredValues.LeftMotorRpm = Value;
        break;
    case INVERTERS_LeftMotorSpeedKmh:
        InvertersMonitoredValues.LeftMotorSpeedKmh = Value;
        break;
    case INVERTERS_LeftInverterThrottle:
        if(Value>250)
            InvertersMonitoredValues.LeftInverterThrottle = 0;
        else
            InvertersMonitoredValues.LeftInverterThrottle = Value;
        break;
    case INVERTERS_LeftInverterThrottleFeedback:
        if(Value>250)
            InvertersMonitoredValues.LeftInverterThrottleFeedback = 0;
        else
            InvertersMonitoredValues.LeftInverterThrottleFeedback = Value;
        break;
    case INVERTERS_RightInverterTemperature:
        InvertersMonitoredValues.RightInverterTemperature = Value;
        break;
    case INVERTERS_RightMotorTemperature:
        InvertersMonitoredValues.RightMotorTemperature = Value;
        break;
    case INVERTERS_RightInverterInputVoltage:
        if(Value>1800)
            InvertersMonitoredValues.RightInverterInputVoltage = 0;
        else
            InvertersMonitoredValues.RightInverterInputVoltage = Value;
        break;
    case INVERTERS_RightInverterCurrent:
        if(Value>4000)
            InvertersMonitoredValues.RightInverterCurrent = 0;
        else
            InvertersMonitoredValues.RightInverterCurrent = Value;
        break;
    case INVERTERS_RightMotorRpm:
        if(Value>6000)
            InvertersMonitoredValues.RightMotorRpm = 0;
        else
            InvertersMonitoredValues.RightMotorRpm = Value;
        break;
    case INVERTERS_RightMotorSpeedKmh:
        InvertersMonitoredValues.RightMotorSpeedKmh = Value;
        break;
    case INVERTERS_RightInverterSentThrottle:
        if(Value>250)
            InvertersMonitoredValues.RightInverterSentThrottle = 0;
        else
            InvertersMonitoredValues.RightInverterSentThrottle = Value;
        break;
    case INVERTERS_RightInverterThrottleFeedback:
        if(Value>250)
            InvertersMonitoredValues.RightInverterThrottleFeedback = 0;
        else
            InvertersMonitoredValues.RightInverterThrottleFeedback = Value;
        break;
    case INVERTERS_IsCarInReverse:
        InvertersMonitoredValues.IsCarInReverse = Value;
        break;
    case INVERTERS_IsCarRunning:
        InvertersMonitoredValues.IsCarRunning = Value;
        break;
    //DASHBOARD
    case DASHBOARD_ActivationButtonPressed:
        DashboardMonitoredValues.ActivationButtonPressed = Value;
        break;
    case DASHBOARD_CarReverseCommandPressed:
        DashboardMonitoredValues.CarReverseCommandPressed = Value;
        break;
    case DASHBOARD_IsDisplayWorking:
        DashboardMonitoredValues.IsDisplayWorking = Value;
        break;
    case DASHBOARD_IsSegmentsDriverWorking:
        DashboardMonitoredValues.IsSegmentsDriverWorking = Value;
        break;
    }
}

uint32_t CarData_ReadValue(MonitoredValue_t DesiredValueType){
    switch(DesiredValueType){
    case TSAC_MedianCellTemperature:
        return TsacMonitoredvalues.MedianCellTemperature;
    case TSAC_HighestCellTemperature:
        return TsacMonitoredvalues.HighestCellTemperature;
    case TSAC_LowestCellTemperature:
        return TsacMonitoredvalues.LowestCellTemperature;
    case TSAC_MedianCellVoltage:
        return TsacMonitoredvalues.MedianCellVoltage;
    case TSAC_HighestCellVoltage:
        return TsacMonitoredvalues.HighestCellVoltage;
    case TSAC_LowestCellVoltage:
        return TsacMonitoredvalues.LowestCellVoltage;
    case TSAC_OverallVoltage:
        return TsacMonitoredvalues.OverallVoltage;
    case TSAC_OverallCurrent:
        return TsacMonitoredvalues.OverallCurrent;
    case TSAC_IsAmsSafe:
        return TsacMonitoredvalues.AmsError;
    case TSAC_IsTransceiverWorking:
        return TsacMonitoredvalues.TransceiverError;
    case TSAC_IsShuntWorking:
        return TsacMonitoredvalues.ShuntError;
    case TSAC_IsBms0Working:
        return TsacMonitoredvalues.Bms0Error;
    case TSAC_IsBms1Working:
        return TsacMonitoredvalues.Bms1Error;
    case PEDALS_AcceleratorSensor1Voltage:
        return PedalsMonitoredValues.AcceleratorSensor1Voltage;
    case PEDALS_AcceleratorSensor2Voltage:
        return PedalsMonitoredValues.AcceleratorSensor2Voltage;
    case PEDALS_AcceleratorSensor1TravelPercentage:
        return PedalsMonitoredValues.AcceleratorSensor1TravelPercentage;
    case PEDALS_AcceleratorSensor2TravelPercentage:
        return PedalsMonitoredValues.AcceleratorSensor2TravelPercentage;
    case PEDALS_BrakeSensor1Voltage:
        return PedalsMonitoredValues.BrakeSensor1Voltage;
    case PEDALS_BrakeSensor2Voltage:
        return PedalsMonitoredValues.BrakeSensor2Voltage;
    case PEDALS_BrakeSensor1TravelPercentage:
        return PedalsMonitoredValues.BrakeSensor1TravelPercentage;
    case PEDALS_BrakeSensor2TravelPercentage:
        return PedalsMonitoredValues.BrakeSensor2TravelPercentage;
    case PEDALS_PressureSensorVoltage:
        return PedalsMonitoredValues.PressureSensorVoltage;
    case PEDALS_PressureSensorBars:
        return PedalsMonitoredValues.PressureSensorBars;
    case PEDALS_Accel_Sensor1_ShortToGnd:
        return PedalsMonitoredValues.Accel_Sensor1_ShortToGnd;
    case PEDALS_Accel_Sensor1_ShortToVcc:
        return PedalsMonitoredValues.Accel_Sensor1_ShortToVcc;
    case PEDALS_Accel_Sensor1_OutOfRangeOutput:
        return PedalsMonitoredValues.Accel_Sensor1_OutOfRangeOutput;
    case PEDALS_Accel_Sensor2_ShortToGnd:
        return PedalsMonitoredValues.Accel_Sensor2_ShortToGnd;
    case PEDALS_Accel_Sensor2_ShortToVcc:
        return PedalsMonitoredValues.Accel_Sensor2_ShortToVcc;
    case PEDALS_Accel_Sensor2_OutOfRangeOutput:
        return PedalsMonitoredValues.Accel_Sensor2_OutOfRangeOutput;
    case PEDALS_Accel_Implausibility:
        return PedalsMonitoredValues.Accel_Implausibility;
    case PEDALS_Brake_Sensor1_ShortToGnd:
        return PedalsMonitoredValues.Brake_Sensor1_ShortToGnd;
    case PEDALS_Brake_Sensor1_ShortToVcc:
        return PedalsMonitoredValues.Brake_Sensor1_ShortToVcc;
    case PEDALS_Brake_Sensor1_OutOfRangeOutput:
        return PedalsMonitoredValues.Brake_Sensor1_OutOfRangeOutput;
    case PEDALS_Brake_Sensor2_ShortToGnd:
        return PedalsMonitoredValues.Brake_Sensor2_ShortToGnd;
    case PEDALS_Brake_Sensor2_ShortToVcc:
        return PedalsMonitoredValues.Brake_Sensor2_ShortToVcc;
    case PEDALS_Brake_Sensor2_OutOfRangeOutput:
        return PedalsMonitoredValues.Brake_Sensor2_OutOfRangeOutput;
    case PEDALS_Brake_Implausibility:
        return PedalsMonitoredValues.Brake_Implausibility;
    case INVERTERS_LeftInverterTemperature:
        return InvertersMonitoredValues.LeftInverterTemperature;
    case INVERTERS_LeftMotorTemperature:
        return InvertersMonitoredValues.LeftMotorTemperature;
    case INVERTERS_LeftInverterInputVoltage:
        return InvertersMonitoredValues.LeftInverterInputVoltage;
    case INVERTERS_LeftInverterCurrent:
        return InvertersMonitoredValues.LeftInverterCurrent;
    case INVERTERS_LeftMotorRpm:
        return InvertersMonitoredValues.LeftMotorRpm;
    case INVERTERS_LeftMotorSpeedKmh:
        return InvertersMonitoredValues.LeftMotorSpeedKmh;
    case INVERTERS_LeftInverterThrottle:
        return InvertersMonitoredValues.LeftInverterThrottle;
    case INVERTERS_LeftInverterThrottleFeedback:
        return InvertersMonitoredValues.LeftInverterThrottleFeedback;
    case INVERTERS_RightInverterTemperature:
        return InvertersMonitoredValues.RightInverterTemperature;
    case INVERTERS_RightMotorTemperature:
        return InvertersMonitoredValues.RightMotorTemperature;
    case INVERTERS_RightInverterInputVoltage:
        return InvertersMonitoredValues.RightInverterInputVoltage;
    case INVERTERS_RightInverterCurrent:
        return InvertersMonitoredValues.RightInverterCurrent;
    case INVERTERS_RightMotorRpm:
        return InvertersMonitoredValues.RightMotorRpm;
    case INVERTERS_RightMotorSpeedKmh:
        return InvertersMonitoredValues.RightMotorSpeedKmh;
    case INVERTERS_RightInverterSentThrottle:
        return InvertersMonitoredValues.RightInverterSentThrottle;
    case INVERTERS_RightInverterThrottleFeedback:
        return InvertersMonitoredValues.RightInverterThrottleFeedback;
    case INVERTERS_IsCarInReverse:
        return InvertersMonitoredValues.IsCarInReverse;
    case INVERTERS_IsCarRunning:
        return InvertersMonitoredValues.IsCarRunning;
    case DASHBOARD_ActivationButtonPressed:
        return DashboardMonitoredValues.ActivationButtonPressed;
    case DASHBOARD_CarReverseCommandPressed:
        return DashboardMonitoredValues.CarReverseCommandPressed;
    case DASHBOARD_IsDisplayWorking:
        return DashboardMonitoredValues.IsDisplayWorking;
    case DASHBOARD_IsSegmentsDriverWorking:
        return DashboardMonitoredValues.IsSegmentsDriverWorking;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
