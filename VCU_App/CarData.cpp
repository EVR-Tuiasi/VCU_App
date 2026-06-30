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
MonitoredValues_t MonitoredValues ={
		{/* START TsacMonitoredValues */
				{/* MedianCellTemperature */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						54U	/* shift */
				},
				{/* HighestCellTemperature */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						24U	/* shift */
				},
				{/* LowestCellTemperature */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						54U	/* shift */
				},
				{/* MedianCellVoltage */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						44U	/* shift */
				},
				{/* HighestCellVoltage */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						34U	/* shift */
				},
				{/* LowestCellVoltage */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						44U	/* shift */
				},
				{/* OverallVoltage */
						0,	/* value */
						0,	/* value */
						11,	/* nrOfBits */
						13U	/* shift */
				},
				{/* OverallCurrent */
						0,	/* value */
						0,	/* value */
						13U,/* nrOfBits */
						0U	/* shift */
				},
				{/* CellVoltage */
						{0}
				},
				{/* ThermistorTemperature */
						{0}	/* zero-initializing array */
				},
				{/* AmsError */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						38U	/* shift */
				},
				{/* TransceiverError */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						42U	/* shift */
				},
				{/* ShuntError */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						43U	/* shift */
				},
				{/* Bms0Error */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						41U	/* shift */
				},
				{/* Bms1Error */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						40U	/* shift */
				},
				{/* ThermistorsError */
						0,	/* value */
						0,	/* value */
						1U,/* nrOfBits */
						39U	/* shift */
				},
				{/* ChargerStatus */
						0,	/* value */
						0,	/* value */
						1U,/* nrOfBits */
						32U	/* shift */
				},
				{/* ReportedChargingCurrent */
						0,	/* value */
						0,	/* value */
						16U,/* nrOfBits */
						16U	/* shift */
				},
				{/* ReportedChargingVolts */
						0,	/* value */
						0,	/* value */
						16U,/* nrOfBits */
						0U	/* shift */
				},
				{/* ChargerCommand */
						0,	/* value */
						0,	/* value */
						1U, /* nrOfBits */
						63U	/* shift */
				},
				{/* DesiredChargingCurrent */
						0,	/* value */
						0,	/* value */
						9U, /* nrOfBits */
						10U	/* shift */
				},
				{/* DesiredChargingVoltage */
						0,	/* value */
						0,	/* value */
						10U,/* nrOfBits */
						0U	/* shift */
				}
		},/* END TsacMonitoredValues */
		{/* START PedalsMonitoredValues*/
				{/* AcceleratorSensor1Voltage */
						0,	/* value */
						0,	/* value */
						14U,/* nrOfBits */
						0U	/* shift */
				},
				{/* AcceleratorSensor2Voltage */
						0,	/* value */
						0,	/* value */
						14U,/* nrOfBits */
						14U	/* shift */
				},
				{/* AcceleratorSensor1TravelPercentage */
						0,	/* value */
						0,	/* value */
						7U,	/* nrOfBits */
						28U	/* shift */
				},
				{/* AcceleratorSensor2TravelPercentage */
						0,	/* value */
						0,	/* value */
						7U,	/* nrOfBits */
						35U	/* shift */
				},
				{/* BrakeSensor1Voltage */
						0,	/* value */
						0,	/* value */
						14U,/* nrOfBits */
						0U	/* shift */
				},
				{/* BrakeSensor2Voltage */
						0,	/* value */
						0,	/* value */
						14U,/* nrOfBits */
						14U	/* shift */
				},
				{/* BrakeSensor1TravelPercentage */
						0,	/* value */
						0,	/* value */
						7U,	/* nrOfBits */
						28U	/* shift */
				},
				{/* BrakeSensor2TravelPercentage */
						0,	/* value */
						0,	/* value */
						7U,	/* nrOfBits */
						35U	/* shift */
				},
				{/* PressureSensorVoltage */
						0,	/* value */
						0,	/* value */
						9U,	/* nrOfBits */
						42U	/* shift */
				},
				{/* PressureSensorBars */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						42U	/* shift */
				},
				{/* Accel_Sensor1_ShortToGnd */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						63U	/* shift */
				},
				{/* Accel_Sensor1_ShortToVcc */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						62U	/* shift */
				},
				{/* Accel_Sensor1_OutOfRangeOutput */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						61U	/* shift */
				},
				{/* Accel_Sensor2_ShortToGnd */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						60U	/* shift */
				},
				{/* Accel_Sensor2_ShortToVcc */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						59U	/* shift */
				},
				{/* Accel_Sensor2_OutOfRangeOutput */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						58U	/* shift */
				},
				{/* Accel_Implausibility */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						57U	/* shift */
				},
				{/* Brake_Sensor1_ShortToGnd */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						63U	/* shift */
				},
				{/* Brake_Sensor1_ShortToVcc */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						62U	/* shift */
				},
				{/* Brake_Sensor1_OutOfRangeOutput */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						61U	/* shift */
				},
				{/* Brake_Sensor2_ShortToGnd */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						60U	/* shift */
				},
				{/* Brake_Sensor2_ShortToVcc */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						59U	/* shift */
				},
				{/* Brake_Sensor2_OutOfRangeOutput */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						58U	/* shift */
				},
				{/* Brake_Implausibility */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						57U	/* shift */
				}
		},/* END PedalsMonitoredValues*/
		{/* START InvertersMonitoredValues */
				{/* LeftInverterTemperature */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						8U	/* shift */
				},
				{/* LeftMotorTemperature */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						0U	/* shift */
				},
				{/* LeftInverterInputVoltage */
						0,	/* value */
						0,	/* value */
						11U,/* nrOfBits */
						40U	/* shift */
				},
				{/* LeftInverterCurrent */
						0,	/* value */
						0,	/* value */
						12U,/* nrOfBits */
						0U	/* shift */
				},
				{/* LeftMotorRpm */
						0,	/* value */
						0,	/* value */
						13U,/* nrOfBits */
						51U	/* shift */
				},
				{/* LeftMotorSpeedKmh */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						24U	/* shift */
				},
				{/* LeftInverterThrottle */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						16U	/* shift */
				},
				{/* LeftInverterThrottleFeedback */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						32U	/* shift */
				},
				{/* RightInverterTemperature */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						8U	/* shift */
				},
				{/* RightMotorTemperature */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						0U	/* shift */
				},
				{/* RightInverterInputVoltage */
						0,	/* value */
						0,	/* value */
						11U,/* nrOfBits */
						40U	/* shift */
				},
				{/* RightInverterCurrent */
						0,	/* value */
						0,	/* value */
						12U,/* nrOfBits */
						12U	/* shift */
				},
				{/* RightMotorRpm */
						0,	/* value */
						0,	/* value */
						13U,/* nrOfBits */
						51U	/* shift */
				},
				{/* RightMotorSpeedKmh */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						24U	/* shift */
				},
				{/* RightInverterThrottle */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						16U	/* shift */
				},
				{/* RightInverterThrottleFeedback */
						0,	/* value */
						0,	/* value */
						8U,	/* nrOfBits */
						32U	/* shift */
				},
				{/* IsCarInReverse */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						62U	/* shift */
				},
				{/* IsCarRunning */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						63U	/* shift */
				}
		},/* END InvertersMonitoredValues */
		{/* START DashboardMonitoredValues */
				{/* ActivationButtonPressed */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						63U	/* shift */
				},
				{/* CarReverseCommandPressed */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						62U	/* shift */
				},
				{/* IsDisplayWorking */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						61U	/* shift */
				},
				{/* IsSegmentsDriverWorking */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						60U	/* shift */
				}
		},/* END DashboardMonitoredValues */
		{/* START CommunicationsMonitoredValues */
				{/* IsInvertersVCUSimulated */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						63U	/* shift */
				},
				{/* IsTsacVCUSimulated */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						62U	/* shift */
				},
				{/* IsDashboardVCUSimulated */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						61U	/* shift */
				},
				{/* IsPedalsVCUSimulated */
						0,	/* value */
						0,	/* value */
						1U,	/* nrOfBits */
						60U	/* shift */
				}
		}/* END CommunicationsMonitoredValues */
};

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

uint16_t UartMessaging_ReadCellVoltage(uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
	if(index < CELLS_NUM)
		return MonitoredValues.TsacMonitoredValues.CellVoltage[index].valueUart;
	else
		return 0;
}
bool UartMessaging_ReadCellVoltageErrors(uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
	if(index < CELLS_NUM)
		return MonitoredValues.TsacMonitoredValues.CellVoltage[index].errorUart;
	else
		return 0;
}
uint16_t UartMessaging_ReadCellTemperature(uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
    if(index < THERMISTOR_NUM)
		return MonitoredValues.TsacMonitoredValues.ThermistorTemperature[index].valueUart;
	else
		return 0;
}
bool UartMessaging_ReadCellTemperatureErrors(uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
    if(index < THERMISTOR_NUM)
		return MonitoredValues.TsacMonitoredValues.ThermistorTemperature[index].errorUart;
	else
		return 0;
}

void UartMessaging_SetCellVoltage(uint16_t Value, uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
	if(index < CELLS_NUM)
		MonitoredValues.TsacMonitoredValues.CellVoltage[index].valueUart = Value;
}
void UartMessaging_SetCellVoltageErrors(bool Value, uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
	if(index < CELLS_NUM)
		 MonitoredValues.TsacMonitoredValues.CellVoltage[index].errorUart = Value;
}
void UartMessaging_SetCellTemperature(uint16_t Value, uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
    if(index < THERMISTOR_NUM)
		 MonitoredValues.TsacMonitoredValues.ThermistorTemperature[index].valueUart = Value;
}
void UartMessaging_SetCellTemperatureErrors(bool Value, uint16_t index){
	//NU SCOATE IF-URILE: SUNT DE SIGURANTA
    if(index < THERMISTOR_NUM)
		MonitoredValues.TsacMonitoredValues.ThermistorTemperature[index].errorUart = Value;
}

#ifdef __cplusplus
}
#endif
