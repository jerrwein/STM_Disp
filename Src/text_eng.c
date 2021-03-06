/******************************************************************************
* text_eng.c
*
* Text string list for english text.
*
* Copyright (c) 2013 Nilfisk-Advance, Inc. All rights reserved.
*
* Version History
*------------------------------------------------------------------------------
* 0.01, 25Sep2013, CParker
*	- file created
*
******************************************************************************/

#include "text.h"

/*
*	English text list
**********************************************************
* Text strings can be listed in one of three ways:
* 1) as string: "Idle Mode",
* 2) as char array: (char []){'I','d','l','e',' ','M','o','d','e','\0'},
* 3) as decimal array: (char []){73, 100, 108, 101, 32, 77, 111, 100, 101, 0},
*    (each value corresponds to index of desired character in font map table)
*
* Note: if using char or decimal array,
* - ensure to include a 'NULL' character (0, '\0') at end of array
* - you must cast the array to (char [])
**********************************************************/
const char* gk_textListEnglish[TEXT_LIST_SIZE] =
{
	[TEXT_ID_BLANK]					= "",
	[TEXT_ID_EXIT]					= "Exit",
	[TEXT_ID_CANCEL]				= "Cancel",
	[TEXT_ID_SELECT]				= "Select",
	[TEXT_ID_BACK]					= "Back",
	[TEXT_ID_EDIT]					= "Edit",
	[TEXT_ID_SAVE]					= "Save",
	[TEXT_ID_SCROLL]				= "Scroll",
	[TEXT_ID_PAGE]					= "Page",
	[TEXT_ID_CLEAR]					= "Clear",
	[TEXT_ID_DELETE]				= "Delete",
	[TEXT_ID_RESET]					= "Reset",
	[TEXT_ID_YES]					= "Yes",
	[TEXT_ID_NO]					= "No",
	[TEXT_ID_OFF]					= "Off",
	[TEXT_ID_ON]					= "On",
	[TEXT_ID_UP]					= "Up",
	[TEXT_ID_DOWN]					= "Down",
	[TEXT_ID_ADD]					= "Add",
	[TEXT_ID_SET]					= "Set",
	[TEXT_ID_MORE]					= "More",
	[TEXT_ID_LOG]					= "Log",
	[TEXT_ID_LOCKOUT]				= "Lockout",
	[TEXT_ID_LOW]					= "Low",
	[TEXT_ID_HIGH]					= "High",
	//
	[TEXT_ID_MENU]					= "Menu",
	[TEXT_ID_CONFIGURATION]			= "Configuration",
	[TEXT_ID_OPTIONS]				= "Options",
	[TEXT_ID_SERVICE]				= "Service",
	[TEXT_ID_HOURS]					= "Hours",
	[TEXT_ID_FAULTS]				= "Faults",
	[TEXT_ID_KEYS]					= "Keys",
	[TEXT_ID_SYSTEM]				= "System",
	//
	[TEXT_ID_BRAND]					= "Brand",
	[TEXT_ID_ADVANCE]				= "Advance",
	[TEXT_ID_NILFISK]				= "Nilfisk",
	//
	[TEXT_ID_DECK_TYPE]				= "Deck",
	[TEXT_ID_DISC_34]				= "34D/860D",
	[TEXT_ID_DISC_40]				= "40D/1050D",
	[TEXT_ID_CYL_36]				= "36C/910C",
	//
	[TEXT_ID_BATTERY_TYPE]			= "Battery",
	[TEXT_ID_WET]					= "Wet",
	[TEXT_ID_AGM]					= "AGM",
	//
	[TEXT_ID_OPT_PUMP]				= "Opt Pump",
	[TEXT_ID_NONE]					= "None",
	[TEXT_ID_DUSTGUARD]				= "Dust Guard",
	[TEXT_ID_SPRAYWASH]				= "Spray Wash",
	//
	[TEXT_ID_VACUUM]				= "Vacuum",
	[TEXT_ID_SINGLE]				= "Single",
	[TEXT_ID_DUAL]					= "Dual",
	[TEXT_ID_FULL]					= "Full",
	//
	[TEXT_ID_SIDE_SWEEP]			= "Side Sweep",
	[TEXT_ID_ECOFLEX]				= "Ecoflex",
	[TEXT_ID_CHARGER]				= "Charger",
	[TEXT_ID_BACKUP_ALARM]			= "Backup Alarm",
	[TEXT_ID_HEADLIGHT]				= "Headlight",
	[TEXT_ID_TRACKCLEAN]			= "TrackClean",
	//
	[TEXT_ID_LANGUAGE]				= "Language",
	[TEXT_ID_ENGLISH]				= "English",
	[TEXT_ID_FRENCH]				= "Fran�ais",
	[TEXT_ID_GERMAN]				= "Deutsch",
	[TEXT_ID_ITALIAN]				= "Italiano",
	[TEXT_ID_PORTUGUESE]			= "Portugu�s",
	[TEXT_ID_SPANISH]				= "Espa�ol",
	//
	[TEXT_ID_FLOOR]					= "Floor",
	[TEXT_ID_STANDARD]				= "Standard",
	[TEXT_ID_SMOOTH]				= "Smooth",
	//
	[TEXT_ID_SCRUB_START]			= "Scrub Startup",
	[TEXT_ID_LIGHT]					= "Light",
	[TEXT_ID_HEAVY]					= "Heavy",
	[TEXT_ID_EXTREME]				= "Extreme",
	[TEXT_ID_LAST]					= "Last Used",
	//
	[TEXT_ID_SCRUB_MAX]				= "Scrub Max",
	[TEXT_ID_NEUTRAL_DELAY]			= "Neutral Delay",
	[TEXT_ID_VAC_OFF_DELAY]			= "Vac Off Delay",
	[TEXT_ID_SOLUTION_IN_REVERSE]	= "Solution In Rev",
	[TEXT_ID_LOCK_DETERGENT]		= "Lock Detergent",
	[TEXT_ID_BOP_TIME]				= "Burst Of Power",
	[TEXT_ID_FWD_SPEED_MAX]			= "Fwd Speed Max",
	[TEXT_ID_LOCK_SPEED_LIMIT]		= "Lock Speed Limit",
	[TEXT_ID_INACTIVITY_TIMEOUT]	= "Inactive Time",
	[TEXT_ID_IMPACT_DETECT]			= "Impact Detect",
	[TEXT_ID_IMPACT_LEVEL]			= "Impact Level",
	//
	[TEXT_ID_SOLUTION]				= "Solution",
	[TEXT_ID_PROPORTIONAL]			= "Proportional",
	[TEXT_ID_FIXED]					= "Fixed",
	[TEXT_ID_UK]					= "UK",
	//
	[TEXT_ID_E2_MAIN_CONTROLLER]	= "E2 Main Controller",
	[TEXT_ID_E3_POWER_MODULE]		= "E3 Power Module",
	[TEXT_ID_E4_DRIVE_CONTROLLER]	= "E4 Drive Controller",
	[TEXT_ID_OUTPUT_TEST]			= "Output Test",
	[TEXT_ID_PANEL_TEST]			= "Panel Test",
	//
	[TEXT_ID_FRONT_BRUSH]			= "Front Brush",
	[TEXT_ID_REAR_BRUSH]			= "Rear Brush",
	[TEXT_ID_RIGHT_BRUSH]			= "Right Brush",
	[TEXT_ID_LEFT_BRUSH]			= "Left Brush",
	[TEXT_ID_SQUEEGEE]				= "Squeegee",
	[TEXT_ID_DECK]					= "Deck",
	[TEXT_ID_OPTION_PUMP]			= "Option Pump",
	[TEXT_ID_VOLTAGE]				= "Voltage",
	[TEXT_ID_CURRENT]				= "Current",
	[TEXT_ID_SPEED]					= "Speed",
	[TEXT_ID_THROTTLE]				= "Throttle",
	[TEXT_ID_TEMPERATURE]			= "Temperature",
	[TEXT_ID_TEMP_CUTBACK]			= "Temp Cutback",
	//
	[TEXT_ID_M1_CURRENT]			= "M1 Amps",
	[TEXT_ID_M1_PWM]				= "M1 PWM Req %",
	[TEXT_ID_M1_DUTY_CYCLE]			= "M1 PWM Out %",
	[TEXT_ID_M2_CURRENT]			= "M2 Amps",
	[TEXT_ID_M2_PWM]				= "M2 PWM Req %",
	[TEXT_ID_M2_DUTY_CYCLE]			= "M2 PWM Out %",
	[TEXT_ID_M3_CURRENT]			= "M3 Amps",
	[TEXT_ID_M3_PWM]				= "M3 PWM Req %",
	[TEXT_ID_M3_DUTY_CYCLE]			= "M3 PWM Out %",
	[TEXT_ID_M4_CURRENT]			= "M4 Amps",
	[TEXT_ID_M4_PWM]				= "M4 PWM Req %",
	[TEXT_ID_M4_DUTY_CYCLE]			= "M4 PWM Out %",
	[TEXT_ID_M5_CURRENT]			= "M5 Amps",
	[TEXT_ID_M5_PWM]				= "M5 PWM Req %",
	[TEXT_ID_M5_DUTY_CYCLE]			= "M5 PWM Out %",
	[TEXT_ID_M6_CURRENT]			= "M6 Amps",
	[TEXT_ID_M6_PWM]				= "M6 PWM Req %",
	[TEXT_ID_M6_DUTY_CYCLE]			= "M6 PWM Out %",
	[TEXT_ID_M7_CURRENT]			= "M7 Amps",
	[TEXT_ID_M7_PWM]				= "M7 PWM Req %",
	[TEXT_ID_M7_DUTY_CYCLE]			= "M7 PWM Out %",
	[TEXT_ID_M10_CURRENT]			= "M10 Amps",
	[TEXT_ID_M10_PWM]				= "M10 PWM Req %",
	[TEXT_ID_M10_DUTY_CYCLE]		= "M10 PWM Out %",
	//
	[TEXT_ID_ON_TIME]				= "On Time",
	[TEXT_ID_SCRUB_TIME]			= "Scrub Time",
	[TEXT_ID_RECOVERY_TIME]			= "Recovery Time",
	[TEXT_ID_DRIVE_TIME]			= "Drive Time",
	//
	[TEXT_ID_ACTIVE_FAULTS]			= "Active Faults",
	[TEXT_ID_FAULT_HISTORY]			= "Fault History",
	[TEXT_ID_CLEAR_HISTORY]			= "Clear History",
	//
	[TEXT_ID_READ_KEY]				= "Read Key",
	[TEXT_ID_KEY_LIST]				= "Key List",
	[TEXT_ID_ADD_KEY]				= "Add Key",
	[TEXT_ID_CLEAR_KEY_LIST]		= "Clear Key List",
	[TEXT_ID_DELETE_KEY]			= "Delete Key",
	[TEXT_ID_INVALID_KEY]			= "Invalid Key",
	[TEXT_ID_FAMILY]				= "Family",
	[TEXT_ID_TYPE]					= "Type",
	[TEXT_ID_USER]					= "User",
	[TEXT_ID_SUPERVISOR]			= "Supervisor",
	[TEXT_ID_TECHNICIAN]			= "Technician",
	[TEXT_ID_CALL]					= "Call",
	[TEXT_ID_MAKE_SUPERVISOR]		= "Make Supervisor",
	//
	[TEXT_ID_FIRMWARE]				= "Firmware",
	[TEXT_ID_SERIAL_NUM]			= "Serial Num",
	[TEXT_ID_INSTALL_FIRMWARE]		= "Install Firmware",
	[TEXT_ID_INSERT_USB]			= "Insert USB Drive",
	[TEXT_ID_FILE_NOT_FOUND]		= "File Not Found",
	[TEXT_ID_INVALID_FILE]			= "Invalid File",
	[TEXT_ID_FILE_ERROR]			= "File Error",
	[TEXT_ID_INSTALL]				= "Install",
	[TEXT_ID_NEW_VERSION]			= "New Version",
	[TEXT_ID_LOADING]				= "Loading",
	[TEXT_ID_RESET_TO_INSTALL]		= "Reset to Install",
	//
	[TEXT_ID_IMPACT_LOG]			= "Impact Log",
	[TEXT_ID_MAX]					= "Max",
	//
	//fault codes
	//----------------------------------
	//main fault codes
	[TEXT_ID_FAULT_1_001]		= "K1 Coil Open",
	[TEXT_ID_FAULT_1_002]		= "K1 Coil Short",
	[TEXT_ID_FAULT_1_003]		= "K1 Contact Weld",
	[TEXT_ID_FAULT_1_010]		= "CAN Bus 0",
	[TEXT_ID_FAULT_1_011]		= "CAN Bus 1",
	[TEXT_ID_FAULT_1_101]		= "L1 Short",
	[TEXT_ID_FAULT_1_102]		= "L2 Short",
	[TEXT_ID_FAULT_1_103]		= "H2 Short",
	[TEXT_ID_FAULT_1_104]		= "LP1,2 Short",
	[TEXT_ID_FAULT_1_105]		= "H1 Short",
	[TEXT_ID_FAULT_1_106]		= "M8 Short",
	[TEXT_ID_FAULT_1_107]		= "M9 Short",
	[TEXT_ID_FAULT_1_201]		= "Level Sensor",
	[TEXT_ID_FAULT_1_560]		= "EEPROM Configuration",
	[TEXT_ID_FAULT_1_561]		= "EEPROM Options",
	[TEXT_ID_FAULT_1_562]		= "EEPROM Sys Values",
	[TEXT_ID_FAULT_1_563]		= "EEPROM Fault Log",
	[TEXT_ID_FAULT_1_564]		= "EEPROM User Key List",
	[TEXT_ID_FAULT_1_565]		= "EEPROM Impact Log",
	[TEXT_ID_FAULT_1_566]		= "EEPROM Statistics",
	//curtis 1355 power module fault codes
	[TEXT_ID_FAULT_2_001]		= "E3 Timeout",
	[TEXT_ID_FAULT_2_011]		= "Precharge Fail",
	[TEXT_ID_FAULT_2_012]		= "K2 Overload",
	[TEXT_ID_FAULT_2_013]		= "K2 Contact Weld",
	[TEXT_ID_FAULT_2_014]		= "K2 Contact Open",
	[TEXT_ID_FAULT_2_015]		= "OverVolt Warning",
	[TEXT_ID_FAULT_2_016]		= "UnderVolt Warning",
	[TEXT_ID_FAULT_2_017]		= "OverVolt Cutoff",
	[TEXT_ID_FAULT_2_018]		= "UnderVolt Cutoff",
	[TEXT_ID_FAULT_2_021]		= "M1 Open",
	[TEXT_ID_FAULT_2_022]		= "M2 Open",
	[TEXT_ID_FAULT_2_023]		= "M3 Open",
	[TEXT_ID_FAULT_2_024]		= "M4 Open",
	[TEXT_ID_FAULT_2_025]		= "M5 Open",
	[TEXT_ID_FAULT_2_026]		= "M6 Open",
	[TEXT_ID_FAULT_2_027]		= "M7 Open",
	[TEXT_ID_FAULT_2_028]		= "M10 Open",
	[TEXT_ID_FAULT_2_031]		= "M1 Overload",
	[TEXT_ID_FAULT_2_032]		= "M2 Overload",
	[TEXT_ID_FAULT_2_033]		= "M3 Overload",
	[TEXT_ID_FAULT_2_034]		= "M4 Overload",
	[TEXT_ID_FAULT_2_035]		= "M5 Overload",
	[TEXT_ID_FAULT_2_036]		= "M6 Overload",
	[TEXT_ID_FAULT_2_037]		= "M7 Overload",
	[TEXT_ID_FAULT_2_038]		= "M10 Overload",
	[TEXT_ID_FAULT_2_041]		= "M1 Overcurrent",
	[TEXT_ID_FAULT_2_042]		= "M2 Overcurrent",
	[TEXT_ID_FAULT_2_043]		= "M3 Overcurrent",
	[TEXT_ID_FAULT_2_044]		= "M4 Overcurrent",
	[TEXT_ID_FAULT_2_045]		= "M5 Overcurrent",
	[TEXT_ID_FAULT_2_046]		= "M6 Overcurrent",
	[TEXT_ID_FAULT_2_047]		= "M7 Overcurrent",
	[TEXT_ID_FAULT_2_048]		= "M10 Overcurrent",
	[TEXT_ID_FAULT_2_051]		= "M1 Mosfet Short",
	[TEXT_ID_FAULT_2_052]		= "M2 Mosfet Short",
	[TEXT_ID_FAULT_2_053]		= "M3 Mosfet Short",
	[TEXT_ID_FAULT_2_054]		= "M4 Mosfet Short",
	[TEXT_ID_FAULT_2_055]		= "M5 Mosfet Short",
	[TEXT_ID_FAULT_2_056]		= "M6 Mosfet Short",
	[TEXT_ID_FAULT_2_057]		= "M7 Mosfet Short",
	[TEXT_ID_FAULT_2_058]		= "M10 Mosfet Short",
	[TEXT_ID_FAULT_2_061]		= "M1 Current Sensor",
	[TEXT_ID_FAULT_2_062]		= "M2 Current Sensor",
	[TEXT_ID_FAULT_2_063]		= "M3 Current Sensor",
	[TEXT_ID_FAULT_2_064]		= "M4 Current Sensor",
	[TEXT_ID_FAULT_2_065]		= "M5 Current Sensor",
	[TEXT_ID_FAULT_2_066]		= "OverTemp Cutoff",
	[TEXT_ID_FAULT_2_067]		= "UnderTemp Cutoff",
	[TEXT_ID_FAULT_2_071]		= "M1 Overload Trip",
	[TEXT_ID_FAULT_2_072]		= "M2 Overload Trip",
	[TEXT_ID_FAULT_2_073]		= "M3 Overload Trip",
	[TEXT_ID_FAULT_2_074]		= "M4 Overload Trip",
	[TEXT_ID_FAULT_2_075]		= "M5 Overload Trip",
	[TEXT_ID_FAULT_2_076]		= "M6 Stall",
	[TEXT_ID_FAULT_2_077]		= "M7 Stall",
	[TEXT_ID_FAULT_2_078]		= "M10 Stall",
	[TEXT_ID_FAULT_2_081]		= "EEPROM Fault",
	[TEXT_ID_FAULT_2_082]		= "PDO Timeout",
	[TEXT_ID_FAULT_2_083]		= "CAN Bus",
	[TEXT_ID_FAULT_2_084]		= "Actuator Timeout",
	[TEXT_ID_FAULT_2_086]		= "OverTemp Cutback",
	[TEXT_ID_FAULT_2_087]		= "UnderTemp Cutback",
	[TEXT_ID_FAULT_2_088]		= "K2 Coil Open",
	[TEXT_ID_FAULT_2_091]		= "M1 HW Fault",
	[TEXT_ID_FAULT_2_092]		= "M2 HW Fault",
	[TEXT_ID_FAULT_2_093]		= "M3 HW Fault",
	[TEXT_ID_FAULT_2_094]		= "M4 HW Fault",
	[TEXT_ID_FAULT_2_095]		= "M5 HW Fault",
	[TEXT_ID_FAULT_2_096]		= "Parameter Change",
	[TEXT_ID_FAULT_2_097]		= "M6 Current Sensor",
	[TEXT_ID_FAULT_2_098]		= "M7 Current Sensor",
	[TEXT_ID_FAULT_2_101]		= "M10 Current Sensor",
	[TEXT_ID_FAULT_2_102]		= "Thermal Sensor",
	[TEXT_ID_FAULT_2_103]		= "K2 Coil Short",
	//curtis 1232e drive controller fault codes
	[TEXT_ID_FAULT_3_001]		= "E4 Timeout",
	[TEXT_ID_FAULT_3_038]		= "K3 Weld",
	[TEXT_ID_FAULT_3_039]		= "K3 Contact Open",
	[TEXT_ID_FAULT_3_045]		= "Pot Low Overcurrent",
	//[TEXT_ID_FAULT_3_042]		= "Sensor 1 Low",
	[TEXT_ID_FAULT_3_042]		= "Pedal Short",
	//[TEXT_ID_FAULT_3_041]		= "Sensor 1 High",
	[TEXT_ID_FAULT_3_041]		= "Pedal Open",
	//[TEXT_ID_FAULT_3_044]		= "Sensor 2 Low",
	//[TEXT_ID_FAULT_3_043]		= "Sensor 2 High",
	[TEXT_ID_FAULT_3_046]		= "EEPROM Fault",
	[TEXT_ID_FAULT_3_047]		= "HPD Fault",
	[TEXT_ID_FAULT_3_017]		= "Severe UnderVolt",
	[TEXT_ID_FAULT_3_018]		= "Severe OverVolt",
	[TEXT_ID_FAULT_3_023]		= "UnderVolt Cutback",
	[TEXT_ID_FAULT_3_024]		= "OverVolt Cutback",
	[TEXT_ID_FAULT_3_022]		= "OverTemp Cutback",
	[TEXT_ID_FAULT_3_015]		= "Severe UnderTemp",
	[TEXT_ID_FAULT_3_016]		= "Severe OverTemp",
	[TEXT_ID_FAULT_3_031]		= "K3 Coil Fault",
	[TEXT_ID_FAULT_3_032]		= "EM Brake Fault",
	[TEXT_ID_FAULT_3_014]		= "Precharge Fail",
	[TEXT_ID_FAULT_3_012]		= "Control Overcurrent",
	[TEXT_ID_FAULT_3_013]		= "Current Sensor",
	[TEXT_ID_FAULT_3_028]		= "Motor Hot Cutback",
	[TEXT_ID_FAULT_3_049]		= "Parameter Change",
	[TEXT_ID_FAULT_3_037]		= "Motor Open",
	[TEXT_ID_FAULT_3_069]		= "Supply Fault",
	[TEXT_ID_FAULT_3_029]		= "Motor Temp Sensor",
	[TEXT_ID_FAULT_3_068]		= "VCL Fault",
	[TEXT_ID_FAULT_3_025]		= "+5V Fault",
	[TEXT_ID_FAULT_3_071]		= "OS General",
	[TEXT_ID_FAULT_3_072]		= "PDO Timeout",
	[TEXT_ID_FAULT_3_036]		= "Encoder Fault",
	[TEXT_ID_FAULT_3_073]		= "Stall",
	[TEXT_ID_FAULT_3_089]		= "Motor Type",
	[TEXT_ID_FAULT_3_077]		= "Supervision Fault",
	[TEXT_ID_FAULT_3_087]		= "Motor Characterization",
	[TEXT_ID_FAULT_3_091]		= "VCL/OS Mismatch",
	[TEXT_ID_FAULT_3_092]		= "EM Brake Not Set",
	[TEXT_ID_FAULT_3_093]		= "Encoder LOS",
	[TEXT_ID_FAULT_3_088]		= "Encoder Pulse Fault",
	[TEXT_ID_FAULT_3_078]		= "Supervisor Incompatible",
	[TEXT_ID_FAULT_3_051]		= "Pedal Disable",
	[TEXT_ID_FAULT_3_056]		= "Low BDI",
	[TEXT_ID_FAULT_3_057]		= "EM Brake Type",
	//

};
