/*
 * detergent.h
 *
 * Created: 03/05/2014
 *  Author: chris.parker
 */ 

#ifndef DETERGENT_H_INCLUDED
#define DETERGENT_H_INCLUDED

//#include <asf.h>


/*
*	DEFINITIONS
**********************************************************/
//detergent pump ids
#define DET_NUM_PUMPS	2
typedef enum
{
	DET_PUMP_1,
	DET_PUMP_2,

} det_pump_id_t;

//detergent pump output states
typedef enum
{
	DET_OUTPUT_OFF,
	DET_OUTPUT_A_ON,
	DET_OUTPUT_B_ON,

} det_output_state_t;
//aliases for pump action
#define DET_OUTPUT_EXTEND_PUMP		DET_OUTPUT_A_ON
#define DET_OUTPUT_RETRACT_PUMP		DET_OUTPUT_B_ON

//define solution flow rates
typedef enum /*__attribute__ ((packed))*/
{
	DET_RATIO_300,		//lowest flow
	DET_RATIO_256,
	DET_RATIO_200,
	DET_RATIO_150,
	DET_RATIO_128,
	DET_RATIO_100,
	DET_RATIO_64,
	DET_RATIO_50,
	DET_RATIO_32,
	DET_RATIO_26,		//highest flow
	//
	DET_NUM_RATIOS
	
} det_ratio_t;

//detergent flow levels
typedef enum /*__attribute__ ((packed))*/
{
	DET_LEVEL_LOW,
	DET_LEVEL_HIGH,
	//
	DET_NUM_LEVELS
	
} det_level_t;

//detergent task commands
typedef enum
{
	DET_CMD_NONE,
	DET_CMD_DISABLE,
	DET_CMD_ENABLE,
	DET_CMD_FORCE_ON,

} det_cmd_t;

/*
*			PROTOTYPES
**********************************************************/
//initialization routine
int32_t Det_Init (void);
//detergent flow control task
void Det_Task (void *parameters);

//API
//-----------------------------------------------
//send detergent task command
void Det_Cmd (det_cmd_t cmd);
//detergent level access
void Det_SetLevel (det_level_t level);
det_level_t Det_GetLevel (void);
//detergent ratio access (per level)
void Det_SetRatio (det_level_t level, det_ratio_t ratio);
det_ratio_t Det_GetRatio (det_level_t level);
//get detergent system status
bool Det_IsEnabled (void);
bool Det_IsOn (void);

void Det_SetOutput (det_pump_id_t id, det_output_state_t state);
det_output_state_t Det_GetOutput (det_pump_id_t id);

//get detergent ratio in string form ('300:1') or in percent form ('0.3%')
const char* Det_GetRatioStr (det_ratio_t ratio);
const char* Det_GetPercentStr (det_ratio_t ratio);


#endif /* DETERGENT_H_INCLUDED */