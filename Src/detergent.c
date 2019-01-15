/******************************************************************************
* detergent.c
*
* Detergent system control.
*
* Copyright (c) 2014 Nilfisk-Advance, Inc. All rights reserved.
*
* Version History
*------------------------------------------------------------------------------
* 0.01, 05Mar2014, CParker
*	- file created
*
******************************************************************************/


//#include "rtos.h"

//#include "microcontroller.h"
//#include "parameters.h"
//#include "outputs.h"
//#include "solution.h"
//#include "leds.h"
//#include "debug.h"

#include "stdint.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "system.h"
#include "detergent.h"
/*
*	Defines
**********************************************************/
//task update time in ms (how often to run the task)
//- ensure that time value is >= portTICK_RATE_MS
//#define DET_UPDATE_RATE_MS		portTICK_RATE_MS
#define DET_UPDATE_RATE_MS				5
#define DET_INTERVAL_UPDATE_RATE_MS		500


//detergent pump pwm
//-----------------------------------------------
#define DET_PUMP_EXTEND_TIME_MS			80
#define DET_PUMP_RETRACT_TIME_MS		50
#define DET_PUMP_MIN_DWELL_TIME_MS		(DET_PUMP_EXTEND_TIME_MS - DET_PUMP_RETRACT_TIME_MS)


//macro for converting interval time to dwell time
//#define DET_PUMP_DWELL_TIME(interval)		(((interval) * 2) + DET_PUMP_EXTEND_TIME - DET_PUMP_RETRACT_TIME)
//#define DET_PUMP_DWELL_TIME(interval)		(((interval) << 1) + DET_PUMP_EXTEND_TIME - DET_PUMP_RETRACT_TIME)

//define ratio information
typedef struct
{
	int32_t    ratio;
	const char *ratioStr;
	const char *percentStr;

} det_ratio_info_t;
static const det_ratio_info_t mk_ratioInfo[DET_NUM_RATIOS] =
{
	[DET_RATIO_300] = { 300		,"300:1"	,"0.3%" },
	[DET_RATIO_256] = { 256		,"256:1"	,"0.4%" },
	[DET_RATIO_200] = { 200		,"200:1"	,"0.5%" },
	[DET_RATIO_150] = { 150		,"150:1"	,"0.7%" },
	[DET_RATIO_128] = { 128		,"128:1"	,"0.8%" },
	[DET_RATIO_100] = { 100		,"100:1"	,"1.0%" },
	[DET_RATIO_64]  = { 64		,"64:1"		,"1.5%" },
	[DET_RATIO_50]  = { 50		,"50:1"		,"2.0%" },
	[DET_RATIO_32]  = { 32		,"32:1"		,"3.0%" },
	[DET_RATIO_26]  = { 26		,"26:1"		,"3.8%" },
};

#if (0)
//look-up table for detergent flow pwm interval times
//-----------------------------------------------
//- the interval time will depend on the current detergent ratio and solution
//	flow rate settings
//- each interval should be multiple of DET_UPDATE_RATE_MS
//-----------------------------------------------
static const uint32_t mk_intervalTime[DET_NUM_RATIOS][SOL_NUM_FLOW_LEVELS] =
{//	Solution Level:		low		,med	,high	,flood	
	[DET_RATIO_300] = { 1600	,1200	,900	,590 },
	[DET_RATIO_256] = { 1300	,940	,730	,500 },
	[DET_RATIO_200] = { 1050	,820	,500	,400 },
	[DET_RATIO_150] = { 960		,750	,400	,320 },
	[DET_RATIO_128] = { 500		,350	,250	,200 },
	[DET_RATIO_100] = { 380		,300	,210	,160 },
/*	[DET_RATIO_64]  = { 342		,252	,170	,120 },	*/
	[DET_RATIO_64]  = { 340		,250	,170	,120 },
	[DET_RATIO_50]  = { 190		,130	,75		,45	 },
/*	[DET_RATIO_32]  = { 133		,70		,40		,20	 },	*/
	[DET_RATIO_32]  = { 135		,70		,40		,20	 },
/*	[DET_RATIO_26]  = { 62		,20		,0		,0	 },	*/
	[DET_RATIO_26]  = { 60		,20		,0		,0	 },
};
#endif

//detergent pump states
typedef enum
{
	DET_PWM_STATE_OFF,
	DET_PWM_STATE_DWELL_2,
	DET_PWM_STATE_EXT_1,
	DET_PWM_STATE_DWELL_1,
	DET_PWM_STATE_EXT_2,

} det_pwm_state_t;

//detergent pump object
typedef struct
{
	det_pwm_state_t		pwmState;
	uint32_t			timer;
	uint32_t			timeout;
	det_output_state_t	outputState;
	uint32_t			faultCounts;

} det_pump_t;


/*
*	Variables
**********************************************************/


static det_level_t m_level;
static det_ratio_t m_ratio[DET_NUM_LEVELS];

static det_pump_t m_pump[DET_NUM_PUMPS];

static struct
{
	bool isEnabled;
	bool isOn;
	bool forceOn;
	bool retract1;
	bool retracting1;
	bool retract2;
	bool retracting2;

} m_flags;

/*
*	Private Function Prototypes
**********************************************************/
static void m_StopPumps (void);


/******************************************************************************
* Det_Init
*	Perform required initialization
*
* Returns:	0 = success
*			negative value = error
******************************************************************************/
int32_t Det_Init (void)
{

	
	m_StopPumps();
	
	m_flags.isEnabled = false;
	m_flags.isOn = false;


	//success
	return (0);
}

/******************************************************************************
* Det_Task
*	Task to implement the detergent flow control state-machine.
*	This task will be started when vTaskStartScheduler is run.
*
*	Note: this task needs to run quickly (~200-1000Hz) in order to get the
*	desired pump on/off time resolution (1-5ms).
*
* parameters:	not used
******************************************************************************/
void Det_Task (void *parameters)
{

}

/******************************************************************************
* Det_Cmd
*	Send detergent task command.
******************************************************************************/
void Det_Cmd (det_cmd_t cmd)
{

}

/******************************************************************************
* Det_SetLevel
*	Set the desired detergent ratio level.
******************************************************************************/
void Det_SetLevel (det_level_t level)
{
	m_level = level;
}

/******************************************************************************
* Det_GetLevel
*	Return the current detergent ratio level.
******************************************************************************/
inline det_level_t Det_GetLevel (void)
{
	return (m_level);
}

/******************************************************************************
* Det_SetRatio
*	Set the desired detergent ratio for a particular level.
******************************************************************************/
void Det_SetRatio (det_level_t level, det_ratio_t ratio)
{


	m_ratio[level] = ratio;

}

/******************************************************************************
* Det_GetRatio
*	Return current detergent ratio for a particular level.
******************************************************************************/
det_ratio_t Det_GetRatio (det_level_t level)
{

	return (m_ratio[level]);
}

/******************************************************************************
* Det_IsEnabled
*	Return detergent enabled/disabled status.
******************************************************************************/
inline bool Det_IsEnabled (void)
{
	return (m_flags.isEnabled);
}

/******************************************************************************
* Det_IsOn
*	Return detergent flow control on/off status.
******************************************************************************/
inline bool Det_IsOn (void)
{
	return (m_flags.isOn);
}

/******************************************************************************
* Det_SetOutput
*	Set the designated pump driver outputs to the desired state.
******************************************************************************/
void Det_SetOutput (det_pump_id_t id, det_output_state_t state)
{

	
	m_pump[id].outputState = state;
}

/******************************************************************************
* Det_GetOutput
*	Get the current output state of the designated pump driver.
******************************************************************************/
inline det_output_state_t Det_GetOutput (det_pump_id_t id)
{
	return (m_pump[id].outputState);
}

/******************************************************************************
* Det_GetRatioStr
*	Get detergent ratio in string form ('300:1') or in percent form ('0.3%').
******************************************************************************/
const char* Det_GetRatioStr (det_ratio_t ratio)
{
	return ((ratio < DET_NUM_RATIOS) ? mk_ratioInfo[ratio].ratioStr : NULL);
}

/******************************************************************************
* Det_GetPercentStr
*	Get detergent ratio in percent form ('0.3%').
******************************************************************************/
const char* Det_GetPercentStr (det_ratio_t ratio)
{
	return ((ratio < DET_NUM_RATIOS) ? mk_ratioInfo[ratio].percentStr : NULL);
}

/******************************************************************************
* m_StopPumps
*	Private: Helper function to stop both detergent pumps.
******************************************************************************/
static void m_StopPumps (void)
{
	Det_SetOutput(DET_PUMP_1, DET_OUTPUT_OFF);
	Det_SetOutput(DET_PUMP_2, DET_OUTPUT_OFF);

	m_flags.isOn = false;
	m_flags.forceOn = false;
	m_flags.retract1 = false;
	m_flags.retracting1 = false;
	m_flags.retract2 = false;
	m_flags.retracting2 = false;
//	m_pwmState = DET_PWM_STATE_OFF;
}


