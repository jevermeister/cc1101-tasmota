/*
  xdrv_101_rcswitch.ino - CC1101 Library for Tasmota based on LSatans SmartRC

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef USE_SMARTRC
/*********************************************************************************************\
 * CC1101 Library https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
\*********************************************************************************************/

#define XDRV_101  101

#define USE_SPI
#include <ELECHOUSE_CC1101_SRC_DRV.h>

struct smartRCConfig {
  int16_t powerLevel;
  float frequency;
  bool CCMode;
  int8_t trxState;
  bool CRC;
  int8_t syncMode;
  int8_t modulation;
};
smartRCConfig smartRCConfig {12, 433.92, 0, 2, 1, 2, 0};

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char SmartRCCommands[] PROGMEM = "SmartRC|"  // SmartRC Prefix
  "testSPI|"
  "setPA|"
  "setMHZ|"
  "setCCMode|"
  "SetTx|"
  "SetRx|"
  "setCrc|"
  "setSyncMode|"
  "setModulation|";

void (* const SmartRCCommand[])(void) PROGMEM = {
  &CmndSmartRCtestSPI,
  &CmndSmartRCsetPA,
  &CmndSmartRCsetMHZ,
  &CmndSmartRCsetCCMode,
  &CmndSmartRCSetTx,
  &CmndSmartRCSetRx,
  &CmndSmartRCsetCrc,
  &CmndSmartRCsetSyncMode,
  &CmndSmartRCsetModulation};

void CmndSmartRCtestSPI(void) {
  if(ELECHOUSE_cc1101.getCC1101()) { // Check the CC1101 Spi connection.
    AddLog(LOG_LEVEL_INFO, PSTR("CC1101 SPI connected!"));
  }else{
    AddLog(LOG_LEVEL_INFO, PSTR("CC1101 SPI not connected!"));
  }
  ResponseCmndDone();
}

void CmndSmartRCsetPA(void) {
  if(ELECHOUSE_cc1101.getCC1101()) {
    if(XdrvMailbox.payload > -30 || XdrvMailbox.payload < 12){
      smartRCConfig.powerLevel = XdrvMailbox.payload;
      ELECHOUSE_cc1101.setPA(smartRCConfig.powerLevel);
      ResponseCmndNumber(smartRCConfig.powerLevel);
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("Not a valid value!"));
    } 
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCsetMHZ(void) {
  if(ELECHOUSE_cc1101.getCC1101()) {
    if(XdrvMailbox.data_len > 0){
      smartRCConfig.frequency = CharToFloat(XdrvMailbox.data);
      ELECHOUSE_cc1101.setPA(smartRCConfig.frequency);
      ResponseCmndFloat(smartRCConfig.frequency, 2);
    }
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCsetCCMode(void) {
  if(ELECHOUSE_cc1101.getCC1101()) {
    if(XdrvMailbox.payload > -1 && XdrvMailbox.payload < 2){
      smartRCConfig.CCMode = XdrvMailbox.payload;
      ELECHOUSE_cc1101.setCCMode(smartRCConfig.CCMode);
      ResponseCmndNumber(smartRCConfig.CCMode);
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("Not a valid value!"));
    } 
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCSetTx(void){
  if(ELECHOUSE_cc1101.getCC1101()) {
    smartRCConfig.trxState = 1;
    ELECHOUSE_cc1101.SetTx();
    ResponseCmndNumber(ELECHOUSE_cc1101.getMode());
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCSetRx(void){
  if(ELECHOUSE_cc1101.getCC1101()) {
    smartRCConfig.trxState = 2;
    ELECHOUSE_cc1101.SetRx();
    ResponseCmndNumber(ELECHOUSE_cc1101.getMode());
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCsetCrc(void) {
  if(ELECHOUSE_cc1101.getCC1101()) {
    if(XdrvMailbox.payload > -1 && XdrvMailbox.payload < 2){
      smartRCConfig.CRC = XdrvMailbox.payload;
      ELECHOUSE_cc1101.setCrc(smartRCConfig.CRC);
      ResponseCmndNumber(smartRCConfig.CRC);
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("Not a valid value!"));
    } 
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCsetSyncMode(void) {
  if(ELECHOUSE_cc1101.getCC1101()) {
    if(XdrvMailbox.payload > -1 && XdrvMailbox.payload < 8){
      smartRCConfig.syncMode = XdrvMailbox.payload;
      ELECHOUSE_cc1101.setSyncMode(smartRCConfig.syncMode);
      ResponseCmndNumber(smartRCConfig.syncMode);
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("Not a valid value!"));
    } 
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

void CmndSmartRCsetModulation(void) {
  if(ELECHOUSE_cc1101.getCC1101()) {
    if(XdrvMailbox.payload > -1 && XdrvMailbox.payload < 5){
      smartRCConfig.modulation = XdrvMailbox.payload;
      ELECHOUSE_cc1101.setModulation(smartRCConfig.modulation);
      ResponseCmndNumber(smartRCConfig.modulation);
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("Not a valid value!"));
    } 
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("CC1101 SPI not connected!"));
  }
}

/*********************************************************************************************\
 * Tasmota Functions (Helpers)
\*********************************************************************************************/

void smartRCInit(){
  
  //SPI
  if (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MISO) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CS)) {
    //should be used with VSPI (18, 19, 23, 5)
    ELECHOUSE_cc1101.setSpiPin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_CS));
    if(ELECHOUSE_cc1101.getCC1101()) { // Check the CC1101 Spi connection.
      ELECHOUSE_cc1101.Init();
      ELECHOUSE_cc1101.setMHZ(smartRCConfig.frequency);
      ELECHOUSE_cc1101.setPA(smartRCConfig.powerLevel);
    }
    if (PinUsed(GPIO_CC1101_GDO0) && PinUsed(GPIO_CC1101_GDO2)){
      ELECHOUSE_cc1101.setGDO(Pin(GPIO_CC1101_GDO0),Pin(GPIO_CC1101_GDO2));
    }
  }

  //TX e.g. GPIO 2
  if (PinUsed(GPIO_CC1101_GDO0) && smartRCConfig.trxState == 1){
    ELECHOUSE_cc1101.SetTx();
  }

  //RX e.g. GPIO 4
  if (PinUsed(GPIO_CC1101_GDO2) && smartRCConfig.trxState == 2){
    ELECHOUSE_cc1101.SetRx();
  }
}

void smartRCReceive(){
  
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv101(uint32_t function)
{
  bool result = false;

  if (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MISO) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CS)) {
    switch (function) {

      case FUNC_EVERY_50_MSECOND:
        if (PinUsed(GPIO_CC1101_GDO2) && smartRCConfig.trxState == 2 && smartRCConfig.CCMode == 1) {
          smartRCReceive();
        }
        break;

      case FUNC_COMMAND:
        result = DecodeCommand(SmartRCCommands, SmartRCCommand);
        break;

      case FUNC_INIT:
        smartRCInit();
        break;
    }
  }
  return result;
}

#endif