#include <vector>
#ifndef SD_CARD_INTERFACE
#define SD_CARD_INTERFACE

#include "FS.h"      // SD Card ESP32
#include "SD_MMC.h"  // SD Card ESP32
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

void sd_read_targets(LandmarkConverter* lc) {
  File file = SD_MMC.open("/targets.txt", FILE_READ);
  if (!file) {
    Serial.println("Opening file to read targets failed");
    return;
  }
  auto str = file.readString();
  file.close();
  // convert to array of points
  lc->convert_str_targets_to_arr(str);
}

void init_Micro_SD_Card(LandmarkConverter* lc) {
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
  sd_read_targets(lc);
  Serial.print("Read all targets from MicroSD Card");
  SD_MMC.end();
}




#endif  //SD_CARD_INTERFACE