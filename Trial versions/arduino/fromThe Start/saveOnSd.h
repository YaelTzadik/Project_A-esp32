#include <custom_hal.hpp>
#include <cv_cpu_config.h>
#include <cvconfig.h>
#include <opencv_data_config.hpp>
#include <opencv_tests_config.hpp>

#include "driver/rtc_io.h"
#include <EEPROM.h>
#include "SD_MMC.h"     

// define the number of bytes you want to access
#define EEPROM_SIZE 1


int imageNum = 0;

void saveImageOnSD() {
  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
    
  camera_fb_t * fb = NULL;
  // Capture picture
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera Failed to Capture");
    return;
  }
  
// initialize EEPROM 
  EEPROM.begin(EEPROM_SIZE);
  imageNum = EEPROM.read(0) + 1;

  String path = "/image" + String(imageNum) +".jpg";

  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {
    file.write(fb->buf, fb->len); 
    Serial.printf("Saved file to path: %s\n", path.c_str());
    EEPROM.write(0, imageNum);
    EEPROM.commit();
  }
  file.close();
  esp_camera_fb_return(fb);
  
}