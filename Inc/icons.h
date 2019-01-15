/*
 * icons.h
 *
 * Definitions for graphic display icons
 *
 * Created: 8/20/2013
 *  Author: chris.parker
 */ 

#ifndef ICONS_H_INCLUDED
#define ICONS_H_INCLUDED

//#include <asf.h>


extern const uint8_t gk_iconAdvanceLogo[];
extern const uint8_t gk_iconNilfiskLogo[];

extern const uint8_t gk_iconAdjArrowDown[];
extern const uint8_t gk_iconAdjArrowNone[];
extern const uint8_t gk_iconAdjArrowUp[];
extern const uint8_t gk_iconBattery[];
extern const uint8_t gk_iconBatteryBar[];
extern const uint8_t gk_iconBOP[];
extern const uint8_t gk_iconCallKey[];
extern const uint8_t gk_iconCharging[];
extern const uint8_t gk_iconChargingBar[];
extern const uint8_t gk_iconClickonMode[];
extern const uint8_t gk_iconDetergentFlask[];
extern const uint8_t gk_iconEstop[];
extern const uint8_t gk_iconHourglass[];
extern const uint8_t gk_iconInvalidKey[];
extern const uint8_t gk_iconKeyError[];
extern const uint8_t gk_iconLevel1Box[];
extern const uint8_t gk_iconLevel1Fill[];
extern const uint8_t gk_iconLevel2Box[];
extern const uint8_t gk_iconLevel2Fill[];
extern const uint8_t gk_iconLevel3Box[];
extern const uint8_t gk_iconLevel3Fill[];
extern const uint8_t gk_iconLevel4Box[];
extern const uint8_t gk_iconLevel4Fill[];
extern const uint8_t gk_iconLockout[];
extern const uint8_t gk_iconLvcMode[];
extern const uint8_t gk_iconNoKey[];
//extern const uint8_t gk_iconNoOperator[];
extern const uint8_t gk_iconPowerBtn[];
extern const uint8_t gk_iconPurgeMode[];
extern const uint8_t gk_iconRtf[];
extern const uint8_t gk_iconScrubBrush[];
extern const uint8_t gk_iconSolutionFaucet[];
extern const uint8_t gk_iconTank[];
extern const uint8_t gk_iconTankEmpty[];
extern const uint8_t gk_iconTankLevelBtm[];
extern const uint8_t gk_iconTankLevelMid[];
extern const uint8_t gk_iconTankLevelTop[];
extern const uint8_t gk_iconVacMode[];
extern const uint8_t gk_iconWandMode[];
extern const uint8_t gk_iconWarning[];
extern const uint8_t gk_iconWrench[];

//panel test items
extern const uint8_t gk_iconLargeBtnOff[];
extern const uint8_t gk_iconLargeBtnOn[];
extern const uint8_t gk_iconMedBtnOff[];
extern const uint8_t gk_iconMedBtnOn[];
extern const uint8_t gk_iconSmallBtnOff[];
extern const uint8_t gk_iconSmallBtnOn[];
extern const uint8_t gk_iconMenuInfo[];
extern const uint8_t gk_iconLedOnOff[];


//get an icon's dimensions
uint32_t Icon_Width (const uint8_t *glyph);
uint32_t Icon_Height (const uint8_t *glyph);


#endif /* ICONS_H_INCLUDED */