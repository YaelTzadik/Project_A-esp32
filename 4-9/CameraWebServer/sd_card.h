#include <vector>
#ifndef SD_CARD_INTERFACE
#define SD_CARD_INTERFACE

#include "FS.h"      // SD Card ESP32
#include "SD_MMC.h"  // SD Card ESP32
#include <string>
#include "LandmarkConverter.h"
using namespace std;

void sd_flash_led() {
  File file = SD_MMC.open("/flash_file.txt", FILE_WRITE);

  if (!file) {
    Serial.println("Opening file to flash failed");
    return;
  }
  file.close();
}

void sd_read_targets() {
  File file = SD_MMC.open("/targets.txt", FILE_READ);

  if (!file) {
    Serial.println("Opening file to read targets failed");
    return;
  }
  string str = string(file.readString().c_str());
  file.close();

  // convert to array of points
  // Serial.printf(str.c_str());
  LandmarkConverter lc = LandmarkConverter();
  lc.convert_str_targets_to_arr(str);
}

void init_Micro_SD_Card() {
  // Start the MicroSD card

  Serial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin()) {
    Serial.println("MicroSD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD Card found");
    return;
  }
  // // sd_flash_led();
  // sd_read_targets();
  Serial.print("Read all targets from MicroSD Card");
}




#endif  //SD_CARD_INTERFACE