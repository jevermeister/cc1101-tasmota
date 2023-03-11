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

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char SmartRCCommands[] PROGMEM = "SmartRC|"  // SmartRC Prefix
  "Test|"
  "Init|";

void (* const SmartRCCommand[])(void) PROGMEM = {
  &CmndSmartRCTest,
  &CmndSmartRCInit};

void CmndSmartRCTest(void) {
  if(ELECHOUSE_cc1101.getCC1101()) { // Check the CC1101 Spi connection.
    AddLog(LOG_LEVEL_INFO, PSTR("CC1101 SPI connected!"));
  }else{
    AddLog(LOG_LEVEL_INFO, PSTR("CC1101 SPI not connected!"));
  }

  ResponseCmndDone();
}
void CmndSmartRCInit(void) {
  SmartRCInit();
  ResponseCmndDone();
}

/*********************************************************************************************\
 * Tasmota Functions (Helpers)
\*********************************************************************************************/

void SmartRCInit(){
  
  //SPI
  if (PinUsed(GPIO_SPI_CLK) && PinUsed(GPIO_SPI_MISO) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CS)) {
    //should be used with VSPI (18, 19, 23, 5)
    ELECHOUSE_cc1101.setSpiPin(Pin(GPIO_SPI_CLK), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_CS));
    if(ELECHOUSE_cc1101.getCC1101()) { // Check the CC1101 Spi connection.
      ELECHOUSE_cc1101.Init();
      ELECHOUSE_cc1101.setMHZ(433.92); //default is 433.92MHZ
      ELECHOUSE_cc1101.setPA(10);  //default is max
    }
    if (PinUsed(GPIO_CC1101_GDO0) && PinUsed(GPIO_CC1101_GDO2)){
      ELECHOUSE_cc1101.setGDO(Pin(GPIO_CC1101_GDO0),Pin(GPIO_CC1101_GDO2));
    }
  }

  //TX e.g. GPIO 2
  if (PinUsed(GPIO_CC1101_GDO0)){
    ELECHOUSE_cc1101.SetTx();
  }

  //RX e.g. GPIO 4
  if (PinUsed(GPIO_CC1101_GDO2)){
    ELECHOUSE_cc1101.SetRx();
  }
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
        break;

      case FUNC_COMMAND:
        result = DecodeCommand(SmartRCCommands, SmartRCCommand);
        break;

      case FUNC_INIT:
        SmartRCInit();
        break;
    }
  }
  return result;
}

#endif