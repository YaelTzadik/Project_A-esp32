


#include "esp_camera.h"
#include <WiFi.h>
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "WiFi.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
#include <SD_MMC.h>
#include "esp_camera.h"
#include "Arduino.h"
#include <EEPROM.h>   
         // read and write from flash memory


#define CAMERA_MODEL_AI_THINKER // Has PSRAM
// define the number of bytes you want to access
#define EEPROM_SIZE 1


int pictureNumber = 0;

// Replace with your network credentials

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

boolean takeNewPhoto = false;
boolean streamVideo = false;

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="eng">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            text-align: center;
        }

        .column {
            float: left;
            width: 33%;
        }

        /* Clear floats after the columns */
        .row:after {
            content: "";
            display: table;
            clear: both;
        }
    </style>
    <title>Door System</title>
    <style>
        body, h1, h2, h3, h4, h5, h6 {
            font-family: Trebuchet MS, sans-serif;
        }
    </style>
</head>
<body>
<div id="container">
    <h2>ESP32-CAM</h2>
    <div style="padding: 10px ; background-color:#dcedc1 " class="row">
        <h3>Stream Video</h3>
        <div class="column">
            <button onclick="startStream();">start stream</button>
        </div>
        <div class="column">
            <figure>
                <div id="stream-container" class="image-container hidden">
                    <img id="stream" src="" crossorigin>
                </div>
            </figure>
        </div>
        <div class="column">

        </div>
    </div>
    <div style="padding: 10px ; background-color: #a8e6cf" class="row">
        <h3>Viola Jones</h3>
        <div class="column">
            <button onclick="capture();">Take High Quality Still</button>
        </div>
        <div class="column">
            <img src="temp_face.jpg" id="photo_with_rec" width="25%">
        </div>
        <div class="column">

        </div>
    </div>
    <div style="padding: 10px ; background-color:#ffd3b6" class="row">
        <h3>Smile Test</h3>
        <div class="column">
            <button onclick="smile();" id="smile_btn" disabled="disabled">Im Smiling</button>
            <button onclick="testSmile()" id="smile_test_btn">Smile Test</button>
        </div>
        <div class="column">
            <img src="temp_face.jpg" id="photo_smile" width="25%">
        </div>
        <div class="column">
            <output id="smile_test_output"></output>
        </div>
    </div>
    <div style="padding: 10px ; background-color:#ffaaa5" class="row">
        <h3>Flash Test</h3>
        <div class="column">
            <button onclick="capture();">Take High Quality Still</button>
        </div>
        <div class="column">
            <img src="temp_face.jpg" id="photo_flash" width="25%">
        </div>
        <div class="column">
        </div>
    </div>
    <div style="padding: 10px ; background-color:#ff8b94" class="row">
        <h3>Who Is It?</h3>
        <div class="column">
            <button onclick="capture()">Face Anchors Points</button>
        </div>
        <div class="column">
            <img src="temp_face.jpg" id="photo_rec" width="25%">
        </div>
        <div class="column">
        </div>
    </div>
</div>

</body>
<script>
    const IDLE = 0;
    const NO_SMILE = 1;
    const REC_SMILE = 2;
    const RESET_SMILE = 3;
    var smile_status = IDLE;
    var smile_time_out;

    var output_txt = document.getElementById("smile_test_output");
    var smile_button = document.getElementById("smile_btn");
    var smile_test_button = document.getElementById("smile_test_btn");

    function capture() {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', "/capture", true);
        xhr.send();
    }

    function testSmile() {
        if (smile_status === IDLE) {  //search smile
            output_txt.value = "Smile Please";
            smile_status = NO_SMILE;
            smile_test_button.disabled = true;
            smile_button.disabled = false;
            smile_time_out = setTimeout(testSmile, 5000);
        } else if (smile_status === NO_SMILE) {  //reset
            output_txt.value = "Smile Not Found - resetting";
            smile_test_button.disabled = true;
            smile_button.disabled = true;
            smile_status = RESET_SMILE;
            smile_time_out = setTimeout(testSmile, 5000);
        } else if (smile_status === REC_SMILE) {      //good
            output_txt.value = "Congrats- Smile Captured";
            smile_status = RESET_SMILE;
            smile_button.disabled = true;
            smile_test_button.disabled = true;
            smile_time_out = setTimeout(testSmile, 5000);
        } else {
            output_txt.value = "";
            smile_status = IDLE;
            smile_test_button.disabled = false;
            smile_button.disabled = true;
        }
    }

    function smile() {
        smile_status = REC_SMILE;
        testSmile();
        clearTimeout(smile_time_out);
    }

    var is_streaming = false;

    function startStream() {
       var xhr = new XMLHttpRequest();
        xhr.open('GET', "/stream", true);
        xhr.send();

        var stream_view = document.getElementById("stream");
        is_streaming = !is_streaming;
        if (is_streaming) {
            var baseHost = document.location.origin
            var streamUrl = baseHost + ':80'
            stream_view.src = `${streamUrl}/stream`;
        } else
            stream_view.src = ``;
    }

</script>
</html>

)rawliteral";

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Connect to Wi-Fi\Access
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();



  // Print ESP32 Local IP Address
  Serial.print("Camera Ready! Use '");
  Serial.println(IP);
  Serial.println("' to connect");

  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // OV2640 camera module
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    takeNewPhoto = true;
    request->send_P(200, "text/plain", "Taking Photo");
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
  });

  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest * request) {
    streamVideo =! streamVideo;
  });

  // Start server
  server.begin();

}

void loop() {
  if (takeNewPhoto) {
    capturePhotoSaveSpiffs();
    takeNewPhoto = false;
  }
  if(streamVideo)
  {
    startVideoStream();
    streamVideo = false;
  }
  delay(1);
}

// Capture Photo and Save it to SPIFFS
void capturePhotoSaveSpiffs( void ) {
  Serial.begin(115200);
  Serial.println("trying to take photo");
}

// Capture Photo and Save it to SPIFFS
void startVideoStream( void ) {

}



