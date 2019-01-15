/*
 * menus.h
 *
 * Define types and values for menu operation.
 *
 * Created: 10/15/2014
 *  Author: chris.parker
 */ 

#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

//#include "system.h"
#include "display.h"


/*
*	Defines
*******************************************************************************/
#define MENU_NUM_LINES		DISPLAY_LINES_PER_SCREEN
#define MENU_FIRST_LINE		DISPLAY_LINE_1
#define MENU_LAST_LINE		(DISPLAY_LINES_PER_SCREEN - 1)
#define MENU_NO_ITEM		255

//menu function flags
#define MENU_FLAG_RESET			(1u << 0)
#define MENU_FLAG_REDRAW		(1u << 1)

//how often to update value in fast update mode
//#define MENU_FAST_UPDATE_RATE_MS	500
#define MENU_FAST_UPDATE_RATE_MS	350

//maximum number of items allowed for each menu
// !! ensure that all menu lists are smaller than this !!
#define MENU_ITEM_LIST_MAX_SIZE		32

/*
*	Menu Lists
*******************************************************************************/
//menu ids
//- use these to access particular menus
typedef enum
{
	//level 1 menu item
	MENU_ID_TOP,
	//level 2 menu items
	MENU_ID_CONFIG,
	MENU_ID_OPTIONS,
	MENU_ID_SERVICE,
	MENU_ID_HOURS,
	MENU_ID_FAULTS,
	MENU_ID_KEYS,
	MENU_ID_SYSTEM,
	//level 3 menu items
	//service menu items
	MENU_ID_E2_STATUS,
	MENU_ID_E3_STATUS,
	MENU_ID_E4_STATUS,
	MENU_ID_OUTPUT_TEST,
	MENU_ID_PANEL_TEST,
	//faults menu items
	MENU_ID_ACTIVE_FAULTS,
	MENU_ID_FAULT_HISTORY,
	//keys menu items
	MENU_ID_READ_KEY,
	MENU_ID_KEY_LIST,
	//system menu items
	MENU_ID_INSTALL_FIRMWARE,
	MENU_ID_IMPACT_LOG,
	//
	MENU_NUM_MENUS

} menu_id_t;

//menu item lists
//------------------------------------------------
//top menu items
typedef enum
{
	MENU_TOP_ITEM_HOURS,
	MENU_TOP_ITEM_FAULTS,
	MENU_TOP_ITEM_KEYS,
	MENU_TOP_ITEM_SERVICE,
	MENU_TOP_ITEM_OPTIONS,
	MENU_TOP_ITEM_CONFIG,
	MENU_TOP_ITEM_SYSTEM,
	//
	MENU_NUM_TOP_ITEMS

} menu_top_item_t;

//configuration menu items
typedef enum
{
	MENU_CONFIG_ITEM_BRAND,
	MENU_CONFIG_ITEM_DECK_TYPE,
	MENU_CONFIG_ITEM_SIDEBROOM_INSTALLED,
	MENU_CONFIG_ITEM_ECOFLEX_INSTALLED,
	MENU_CONFIG_ITEM_OPTION_PUMP,
	MENU_CONFIG_ITEM_BATTERY_TYPE,
	MENU_CONFIG_ITEM_CHARGER_INSTALLED,
	MENU_CONFIG_ITEM_VACUUM2_INSTALLED,
	MENU_CONFIG_ITEM_BACKUP_ALARM_INSTALLED,
	MENU_CONFIG_ITEM_HEADLIGHT_INSTALLED,
	MENU_CONFIG_ITEM_TRACKCLEAN_INSTALLED,
	//
	MENU_NUM_CONFIG_ITEMS

} menu_config_item_t;

//options menu items
typedef enum
{
	MENU_OPTIONS_ITEM_LANGUAGE,
	MENU_OPTIONS_ITEM_FLOOR_TYPE,
	MENU_OPTIONS_ITEM_SCRUB_MODE_START,
	MENU_OPTIONS_ITEM_SCRUB_MODE_MAX,
	MENU_OPTIONS_ITEM_SOLUTION_MODE,
	MENU_OPTIONS_ITEM_SOLUTION_IN_REVERSE,
	MENU_OPTIONS_ITEM_LOCK_DETERGENT,
	MENU_OPTIONS_ITEM_HEADLIGHT,
	MENU_OPTIONS_ITEM_NEUTRAL_DELAY,
	MENU_OPTIONS_ITEM_VAC_OFF_DELAY,
	MENU_OPTIONS_ITEM_BOP_TIME,
	MENU_OPTIONS_ITEM_FWD_SPEED_MAX,
	MENU_OPTIONS_ITEM_LOCK_SPEED_LIMIT,
	MENU_OPTIONS_ITEM_INACTIVITY_TIMEOUT,
	MENU_OPTIONS_ITEM_IMPACT_DETECT,
	MENU_OPTIONS_ITEM_IMPACT_LEVEL,
	//
	MENU_NUM_OPTIONS_ITEMS

} menu_options_item_t;

//service menu items
typedef enum
{
	MENU_SERVICE_ITEM_E2_STATUS,		//main controller
	MENU_SERVICE_ITEM_E3_STATUS,		//c1355 power module
	MENU_SERVICE_ITEM_E4_STATUS,		//c1232 drive controller
	MENU_SERVICE_ITEM_OUTPUT_TEST,		//test individual outputs
	MENU_SERVICE_ITEM_PANEL_TEST,		//test membrane buttons, oled and leds
	//
	MENU_NUM_SERVICE_ITEMS

} menu_service_item_t;

//service - E2 (main controller) status menu items
typedef enum
{
	MENU_E2_ITEM_VBATT,				//J1-1 - B+ voltage
	MENU_E2_ITEM_MAIN_RELAY,		//J1-3 - KSI relay (K1 coil)
	MENU_E2_ITEM_SOLUTION_VLV,		//J1-4 - L1 solution solenoid
	MENU_E2_ITEM_HEADLIGHT,			//J1-9 - LP1,2 headlights
	MENU_E2_ITEM_ESTOP_SW,			//J1-10 - estop on/off
	MENU_E2_ITEM_OBC,				//J1-11 - obc interlock on/off
	MENU_E2_ITEM_SEAT_SW,			//J1-12 - seat switch on/off
	MENU_E2_ITEM_DUSTGUARD_VLV,		//J1-13 - L2 dust guard solenoid
	MENU_E2_ITEM_BACKUP_ALARM,		//J1-14 - H2 backup alarm
	MENU_E2_ITEM_KSI,				//J1-20 - KSI relay (K1 contacts) on/off
	MENU_E2_ITEM_HORN,				//J1-22 - H1 horn
	MENU_E2_ITEM_RTF_SW,			//J2-8 - recovery tank full switch on/off
	MENU_E2_ITEM_SWEEP_SW,			//J2-9 - side sweep switch on/off
	MENU_E2_ITEM_SOL_LVL_1,			//J2-10 - solution tank level sensor 1 on/off
	MENU_E2_ITEM_SOL_LVL_2,			//J2-13 - solution tank level sensor 2 on/off
	MENU_E2_ITEM_SOL_LVL_3,			//J2-14 - solution tank level sensor 3 on/off
	MENU_E2_ITEM_SOL_LVL_4,			//J2-15 - solution tank level sensor 4 on/off
	MENU_E2_ITEM_HORN_SW,			//J2-16 - horn paddle switch on/off
	MENU_E2_ITEM_BOP_SW,			//J2-17 - bop paddle switch on/off
	MENU_E2_ITEM_SOL_SW,			//J2-18 - solution paddle switch on/off
	MENU_E2_ITEM_DET_PUMPS,			//J1-16/17 - M8, J1-18/19 - M9, detergent pumps 1 & 2
	MENU_E2_ITEM_ACCEL_X,			//accelerometer X
	MENU_E2_ITEM_ACCEL_Y,			//accelerometer Y
	MENU_E2_ITEM_ACCEL_Z,			//accelerometer Z
	//
	MENU_NUM_E2_ITEMS

} menu_e2_item_t;

//service - E3 (Curtis 1355 power module) status menu items
typedef enum
{
	MENU_E3_ITEM_VBATT,
	MENU_E3_ITEM_TEMPERATURE,
	MENU_E3_ITEM_BRUSH1_PWM_REQ,		//M1 - brush motor 1 (right/rear)
	MENU_E3_ITEM_BRUSH1_PWM_OUT,
	MENU_E3_ITEM_BRUSH1_AMPS,
	MENU_E3_ITEM_BRUSH2_PWM_REQ,		//M2 - brush motor 2 (left/front)
	MENU_E3_ITEM_BRUSH2_PWM_OUT,
	MENU_E3_ITEM_BRUSH2_AMPS,
	MENU_E3_ITEM_BROOM_PWM_REQ,			//M3 - broom motor
	MENU_E3_ITEM_BROOM_PWM_OUT,
	MENU_E3_ITEM_BROOM_AMPS,
	MENU_E3_ITEM_VAC1_PWM_REQ,			//M4 - vacuum motor 1
	MENU_E3_ITEM_VAC1_PWM_OUT,
	MENU_E3_ITEM_VAC1_AMPS,
	MENU_E3_ITEM_VAC2_PWM_REQ,			//M5 - vacuum motor 2
	MENU_E3_ITEM_VAC2_PWM_OUT,
	MENU_E3_ITEM_VAC2_AMPS,
	MENU_E3_ITEM_SQG_PWM_REQ,			//A1 - squeegee
	MENU_E3_ITEM_SQG_PWM_OUT,
	MENU_E3_ITEM_SQG_AMPS,
	MENU_E3_ITEM_DECK_PWM_REQ,			//A2 - deck
	MENU_E3_ITEM_DECK_PWM_OUT,
	MENU_E3_ITEM_DECK_AMPS,
	MENU_E3_ITEM_OPTPUMP_PWM_REQ,		//A3 - options pump
	MENU_E3_ITEM_OPTPUMP_PWM_OUT,
	MENU_E3_ITEM_OPTPUMP_AMPS,
	//
	MENU_NUM_E3_ITEMS
	
} menu_e3_item_t;

//service - E4 (Curtis 1232e drive controller) status menu items
typedef enum
{
	MENU_E4_ITEM_VEHICLE_SPEED,			//vehicle speed
	MENU_E4_ITEM_THROTTLE_CMD,			//throttle command
	MENU_E4_ITEM_THROTTLE_POT,			//throttle pot voltage
	MENU_E4_ITEM_MOTOR_CURRENT,			//drive motor rms current
	MENU_E4_ITEM_MOTOR_RPM,				//drive motor rpm
	MENU_E4_ITEM_ENCODER_A,				//encoder phase A
	MENU_E4_ITEM_ENCODER_B,				//encoder phase B
	MENU_E4_ITEM_INTERLOCK_SWITCH,		//interlock (seat) switch input
	MENU_E4_ITEM_ESTOP,					//e-stop input
	MENU_E4_ITEM_MOTOR_TEMP,			//motor temperature
	MENU_E4_ITEM_MOTOR_TEMP_CUTBACK,	//motor temperature cutback %
	MENU_E4_ITEM_CONTROL_TEMP,			//controller temperature
	MENU_E4_ITEM_CONTROL_TEMP_CUTBACK,	//controller temperature cutback %
	//
	MENU_NUM_E4_ITEMS
	
} menu_e4_item_t;

//sevice - output test items
typedef enum
{
	//main board outputs
	MENU_OUTPUT_ITEM_SOLUTION_VLV,		//solution solenoid
	MENU_OUTPUT_ITEM_DUSTGUARD_VLV,		//dustguard solenoid (optional)
	MENU_OUTPUT_ITEM_HORN,				//horn
	MENU_OUTPUT_ITEM_BACKUP_ALARM,		//backup alarm (optional)
	MENU_OUTPUT_ITEM_HEADLIGHT,			//headlights (optional)
	MENU_OUTPUT_ITEM_DET_PUMPS,			//detergent pumps 1 & 2
	//curtis 1355 power module outputs
	MENU_OUTPUT_ITEM_BRUSH1,			//brush 1 motor
	MENU_OUTPUT_ITEM_BRUSH2,			//brush 2 motor
	MENU_OUTPUT_ITEM_BROOM,				//broom motor (optional)
	MENU_OUTPUT_ITEM_VAC1,				//vacuum 1 motor
	MENU_OUTPUT_ITEM_VAC2,				//vacuum 2 motor (optional)
	MENU_OUTPUT_ITEM_SQG,				//squeegee actuator
	MENU_OUTPUT_ITEM_DECK,				//deck actuator
	MENU_OUTPUT_ITEM_OPTPUMP,			//options pump (dustguard/spraywash, optional)
	//
	MENU_NUM_OUTPUT_ITEMS

} menu_output_item_t;

//hours menu items
typedef enum
{
	MENU_HOURS_ITEM_ON_TIME,		//system on time
	MENU_HOURS_ITEM_DRIVE_TIME,		//vehicle drive time
	MENU_HOURS_ITEM_SCRUB_TIME,		//scrub on time
	MENU_HOURS_ITEM_RECOVERY_TIME,	//recovery on time
	//
	MENU_NUM_HOURS_ITEMS
	
} menu_hours_item_t;

//faults menu items
typedef enum
{
	MENU_FAULTS_ITEM_ACTIVE_FAULTS,
	MENU_FAULTS_ITEM_FAULT_HISTORY,
	MENU_FAULTS_ITEM_CLEAR_HISTORY,
	//
	MENU_NUM_FAULTS_ITEMS

} menu_faults_item_t;

//keys menu items
typedef enum
{
	MENU_KEYS_ITEM_READ_KEY,
	MENU_KEYS_ITEM_KEY_LIST,
	//
	MENU_NUM_KEYS_ITEMS

} menu_keys_item_t;

//system menu items
typedef enum
{
	MENU_SYSTEM_ITEM_FIRMWARE,
	MENU_SYSTEM_ITEM_SERIAL_NUM,
	MENU_SYSTEM_ITEM_INSTALL_FIRMWARE,
	MENU_SYSTEM_ITEM_IMPACT_LOG,
	//
	MENU_NUM_SYSTEM_ITEMS

} menu_system_item_t;


/*
*	Types
*******************************************************************************/
//menu page item data
typedef struct
{
	uint32_t menuId;
	uint32_t current;
	uint32_t top;
	uint32_t numItems;
	uint32_t items[MENU_ITEM_LIST_MAX_SIZE];
	bool     drawScreen;

} menu_page_t;

//menu page levels
typedef enum
{
	MENU_PAGE_1,
	MENU_PAGE_2,
	MENU_PAGE_3,
	//
	MENU_NUM_PAGES

} menu_page_id_t;

//edit modes
typedef enum
{
	MENU_EDIT_NO_CHANGE,
	MENU_EDIT_INCREASE,
	MENU_EDIT_DECREASE,

} menu_edit_val_t;

/*
*	Globals
*******************************************************************************/
//global buffers for use by all menus (only 1 menu can be shown at a time so
//there should be no conflict)
extern char g_menuLabelBfr[32];
extern char g_menuValueBfr[32];

/*
*	Prototypes
*******************************************************************************/
//run-time function/state-machine
void Menu_Run (void);

//current menu access
void Menu_SetMenu (menu_id_t menuId, uint32_t startingItem);
menu_id_t Menu_GetMenu (void);
uint32_t Menu_GetCurrentItem (void);
//bool Menu_IsActive (void);
void Menu_Reset (bool clearPages);

//menu screen/navigation functions
//void Menu_LoadMenu (menu_id_t menuId, uint32_t firstItem);
bool Menu_DownBtnPressedOrHeld (void);
bool Menu_UpBtnPressedOrHeld (void);
void Menu_GetNextItemDown (menu_page_t *page);
void Menu_GetNextItemUp (menu_page_t *page);
void Menu_ScrollListDown (menu_page_t *page);
void Menu_ScrollListUp (menu_page_t *page);

//menu button/edit functions
//menu_edit_val_t Menu_CheckForValueChange (void);
void Menu_EditUint8 (uint8_t *pVal, uint8_t step, uint8_t min, uint8_t max, menu_edit_val_t mode);
void Menu_EditUint16 (uint16_t *pVal, uint16_t step, uint16_t min, uint16_t max, menu_edit_val_t mode);
void Menu_EditUint32 (uint32_t *pVal, uint32_t step, uint32_t min, uint32_t max, menu_edit_val_t mode);
void Menu_EditInt32 (int32_t *pVal, int32_t step, int32_t min, int32_t max, menu_edit_val_t mode);

//menu list navigation
//- these were created after and are separate from the menu page/screen functions above

//menu function handlers
//--------------------------------------
void TopMenu_Load (menu_page_t *page, uint32_t startingItem);
void TopMenu_Run (menu_page_t *page);
void ConfigMenu_Load (menu_page_t *page, uint32_t startingItem);
void ConfigMenu_Run (menu_page_t *page);
void OptionsMenu_Load (menu_page_t *page, uint32_t startingItem);
void OptionsMenu_Run (menu_page_t *page);
void ServiceMenu_Load (menu_page_t *page, uint32_t startingItem);
void ServiceMenu_Run (menu_page_t *page);
void HoursMenu_Load (menu_page_t *page, uint32_t startingItem);
void HoursMenu_Run (menu_page_t *page);
void FaultsMenu_Load (menu_page_t *page, uint32_t startingItem);
void FaultsMenu_Run (menu_page_t *page);
void KeysMenu_Load (menu_page_t *page, uint32_t startingItem);
void KeysMenu_Run (menu_page_t *page);
void SystemMenu_Load (menu_page_t *page, uint32_t startingItem);
void SystemMenu_Run (menu_page_t *page);
//service menu sub-menus
void ServiceMenu_LoadE2Status (menu_page_t *page, uint32_t startingItem);
void ServiceMenu_RunE2Status (menu_page_t *page);
void ServiceMenu_LoadE3Status (menu_page_t *page, uint32_t startingItem);
void ServiceMenu_RunE3Status (menu_page_t *page);
void ServiceMenu_LoadE4Status (menu_page_t *page, uint32_t startingItem);
void ServiceMenu_RunE4Status (menu_page_t *page);
void ServiceMenu_LoadOutputTest (menu_page_t *page, uint32_t startingItem);
void ServiceMenu_RunOutputTest (menu_page_t *page);
void ServiceMenu_LoadPanelTest (menu_page_t *page, uint32_t startingItem);
void ServiceMenu_RunPanelTest (menu_page_t *page);
//faults menu sub-menus
void FaultsMenu_LoadActiveFaults (menu_page_t *page, uint32_t startingItem);
void FaultsMenu_RunActiveFaults (menu_page_t *page);
void FaultsMenu_LoadFaultHistory (menu_page_t *page, uint32_t startingItem);
void FaultsMenu_RunFaultHistory (menu_page_t *page);
//keys menu sub-menus
void KeysMenu_LoadReadKey (menu_page_t *page, uint32_t startingItem);
void KeysMenu_RunReadKey (menu_page_t *page);
void KeysMenu_LoadKeyList (menu_page_t *page, uint32_t startingItem);
void KeysMenu_RunKeyList (menu_page_t *page);
//system menu sub-menus
void SystemMenu_LoadInstallFirmware (menu_page_t *page, uint32_t startingItem);
void SystemMenu_RunInstallFirmware (menu_page_t *page);
void SystemMenu_LoadImpactLog (menu_page_t *page, uint32_t startingItem);
void SystemMenu_RunImpactLog (menu_page_t *page);


#endif /* MENUS_H_INCLUDED */