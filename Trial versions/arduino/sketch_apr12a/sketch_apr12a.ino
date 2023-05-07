/*
ESP32-CAM Smile detection (face-api.js)
https://github.com/justadudewhohacks/face-api.js/
Author : ChungYi Fu (Kaohsiung, Taiwan)  2021-7-4 15:30
https://www.facebook.com/francefu
AP IP: 192.168.4.1
http://192.168.xxx.xxx             //網頁首頁管理介面
http://192.168.xxx.xxx:81/stream   //取得串流影像        網頁語法 <img src="http://192.168.xxx.xxx:81/stream">
http://192.168.xxx.xxx/capture     //取得影像           網頁語法 <img src="http://192.168.xxx.xxx/capture">
http://192.168.xxx.xxx/status      //取得視訊參數值
自訂指令格式 http://192.168.xxx.xxx?cmd=P1;P2;P3;P4;P5;P6;P7;P8;P9
http://192.168.xxx.xxx?ip                      //取得APIP, STAIP
http://192.168.xxx.xxx?mac                     //取得MAC位址
http://192.168.xxx.xxx?digitalwrite=pin;value  //數位輸出
http://192.168.xxx.xxx?analogwrite=pin;value   //類比輸出
http://192.168.xxx.xxx?digitalread=pin         //數位讀取
http://192.168.xxx.xxx?analogread=pin          //類比讀取
http://192.168.xxx.xxx?touchread=pin           //觸碰讀取
http://192.168.xxx.xxx?restart                 //重啟電源
http://192.168.xxx.xxx?flash=value             //閃光燈 value= 0~255
http://192.168.xxx.xxx?servo=pin;value         //伺服馬達 value= 0~180
http://192.168.xxx.xxx?relay=pin;value         //繼電器 value = 0, 1
http://192.168.xxx.xxx?uart=value              //UART
http://192.168.xxx.xxx?framesize=value          //解析度 value = 10->UXGA(1600x1200), 9->SXGA(1280x1024), 8->XGA(1024x768) ,7->SVGA(800x600), 6->VGA(640x480), 5 selected=selected->CIF(400x296), 4->QVGA(320x240), 3->HQVGA(240x176), 0->QQVGA(160x120), 11->QXGA(2048x1564 for OV3660)
http://192.168.xxx.xxx?quality&val=value        //畫質 value = 10 ~ 63
http://192.168.xxx.xxx?brightness=value         //亮度 value = -2 ~ 2
http://192.168.xxx.xxx?contrast=value           //對比 value = -2 ~ 2
http://192.168.xxx.xxx?saturation=value         //飽和度 value = -2 ~ 2 
http://192.168.xxx.xxx?special_effect=value     //特效 value = 0 ~ 6
http://192.168.xxx.xxx?hmirror=value            //水平鏡像 value = 0 or 1 
http://192.168.xxx.xxx?vflip=value              //垂直翻轉 value = 0 or 1 
視訊參數說明
https://heyrick.eu/blog/index.php?diary=20210418
*/

//輸入WIFI連線帳號密碼
const char* ssid = "LAPTOP";
const char* password = "12345678";

//輸入AP端連線帳號密碼  http://192.168.4.1
const char* apssid = "esp32-cam";
const char* appassword = "12345678";  //AP密碼至少要8個字元以上

String lineNotifyToken = "";  //Line Notify Token


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"        //video
#include "soc/soc.h"           //used for unstable power supply(reboot)
#include "soc/rtc_cntl_reg.h"  //used for unstable power supply(reboot)
#include "html_interface.cpp"
#include "varifier.cpp"
#include "img_converters.h"

String Feedback = "";  //回傳客戶端訊息
//指令參數值
String Command = "", cmd = "", P1 = "", P2 = "", P3 = "", P4 = "", P5 = "", P6 = "", P7 = "", P8 = "", P9 = "";
// mods
boolean motion_detector = false;
//anti spoofing tracker
HumanVerifier varifier;
//指令拆解狀態值
byte ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;
uint8_t* temp_buf;
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled

//安可信ESP32-CAM模組腳位設定
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

WiFiServer server(80);
WiFiClient client;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //關閉電源不穩就重開機的設定

  Serial.begin(115200);
  Serial.setDebugOutput(true);  //開啟診斷輸出
  Serial.println();

  //視訊組態設定
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
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  //drop down frame size for higher initial frame rate
  sensor_t* s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);  //UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA  設定初始化影像解析度

  //閃光燈
  ledcAttachPin(4, 4);
  ledcSetup(4, 5000, 8);

  WiFi.mode(WIFI_AP_STA);  //其他模式 WiFi.mode(WIFI_AP); WiFi.mode(WIFI_STA);

  //指定Client端靜態IP
  //WiFi.config(IPAddress(192, 168, 201, 100), IPAddress(192, 168, 201, 2), IPAddress(255, 255, 255, 0));

  for (int i = 0; i < 2; i++) {
    WiFi.begin(ssid, password);  //執行網路連線

    delay(1000);
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    long int StartTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ((StartTime + 5000) < millis()) break;  //等待10秒連線
    }

    if (WiFi.status() == WL_CONNECTED) {                                                    //若連線成功
      WiFi.softAP((WiFi.localIP().toString() + "_" + (String)apssid).c_str(), appassword);  //設定SSID顯示客戶端IP
      Serial.println("");
      Serial.println("STAIP address: ");
      Serial.println(WiFi.localIP());
      Serial.println("");

      for (int i = 0; i <= 2; i++) {  //2 quick low flashes at successfull connection
        ledcWrite(4, 2);
        delay(100);
        ledcWrite(4, 0);
        delay(100);
      }
      break;
    }
  }
  delay(3000);
  //create emplty arr for testing //todo: remove
  temp_buf = (uint8_t*)malloc(1024);
  for (int i = 0; i < 1024; i++) {
    temp_buf[i] = 10;
  }
  for (int i = 0; i <= 3; i++) {  //3 quick low flashes at successfull connection
    ledcWrite(4, 2);
    delay(100);
    ledcWrite(4, 0);
    delay(100);
  }

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  server.begin();
}

void loop() {
  Feedback = "";
  Command = "";
  cmd = "";
  P1 = "";
  P2 = "";
  P3 = "";
  P4 = "";
  P5 = "";
  P6 = "";
  P7 = "";
  P8 = "";
  P9 = "";
  motion_detector = false;
  ReceiveState = 0, cmdState = 1, strState = 1, questionstate = 0, equalstate = 0, semicolonstate = 0;
  //set qualifiers



  client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      delay(1);
      if (client.available()) {
        char c = client.read();

        getCommand(c);  //將緩衝區取得的字元拆解出指令參數

        if (c == '\n') {
          if (currentLine.length() == 0) {

            if (cmd == "getstill") {  //取得視訊截圖
              Serial.println("^^getstill");
              write_stream();
            } else if (cmd == "status") {  //取得視訊狀態
              status();
              //Serial.println("^^status");
            } else {  //取得管理首頁
              mainpage();
              //Serial.println("^^main");
            }

            Feedback = "";
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if ((currentLine.indexOf("/?") != -1) && (currentLine.indexOf(" HTTP") != -1)) {
          if (Command.indexOf("stop") != -1) {  //若指令中含關鍵字stop立即斷線 -> http://192.168.xxx.xxx/?cmd=aaa;bbb;ccc;stop
            client.println();
            client.println();
            client.stop();
          }
          currentLine = "";
          Feedback = "";
          ExecuteCommand();
        }
      }
    }

    delay(1);
    client.stop();
  }
}

void ExecuteCommand() {
  //Serial.println("");
  //Serial.println("Command: "+Command);
  if (cmd != "getstill") {
    Serial.println("cmd= " + cmd + " ,P1= " + P1 + " ,P2= " + P2 + " ,P3= " + P3 + " ,P4= " + P4 + " ,P5= " + P5 + " ,P6= " + P6 + " ,P7= " + P7 + " ,P8= " + P8 + " ,P9= " + P9);
    Serial.println("");
  }

  //自訂指令區塊  http://192.168.xxx.xxx?cmd=P1;P2;P3;P4;P5;P6;P7;P8;P9
  if (cmd == "your cmd") {
    // You can do anything
    // Feedback="<font color=\"red\">Hello World</font>";   //可為一般文字或HTML語法
  }
  /*
  else if (cmd=="ip") {  //查詢APIP, STAIP
    Feedback="AP IP: "+WiFi.softAPIP().toString();    
    Feedback+="<br>";
    Feedback+="STA IP: "+WiFi.localIP().toString();}
  else if (cmd=="hmirror") {  //水平鏡像
    sensor_t * s = esp_camera_sensor_get();
    s->set_hmirror(s, P1.toInt());  } else if (cmd=="vflip") {  //垂直翻轉
    sensor_t * s = esp_camera_sensor_get();
    s->set_vflip(s, P1.toInt());  }
  */
  else if (cmd == "mac") {  //查詢MAC位址
    Feedback = "STA MAC: " + WiFi.macAddress();
  } else if (cmd == "restart") {  //重設WIFI連線
    ESP.restart();
  } else if (cmd == "digitalwrite") {  //數位輸出
    ledcDetachPin(P1.toInt());
    pinMode(P1.toInt(), OUTPUT);
    digitalWrite(P1.toInt(), P2.toInt());
  } else if (cmd == "digitalread") {  //數位輸入
    Feedback = String(digitalRead(P1.toInt()));
  } else if (cmd == "analogwrite") {  //類比輸出
    if (P1 == "4") {
      ledcAttachPin(4, 4);
      ledcSetup(4, 5000, 8);
      ledcWrite(4, P2.toInt());
    } else {
      ledcAttachPin(P1.toInt(), 9);
      ledcSetup(9, 5000, 8);
      ledcWrite(9, P2.toInt());
    }
  } else if (cmd == "analogread") {  //類比讀取
    Feedback = String(analogRead(P1.toInt()));
  } else if (cmd == "touchread") {  //觸碰讀取
    Feedback = String(touchRead(P1.toInt()));
  } else if (cmd == "restart") {  //重啟電源
    ESP.restart();
  } else if (cmd == "flash") {  //閃光燈
    ledcAttachPin(4, 4);
    ledcSetup(4, 5000, 8);
    int val = P1.toInt();
    ledcWrite(4, val);
  } else if (cmd == "servo") {  //伺服馬達
    ledcAttachPin(P1.toInt(), 3);
    ledcSetup(3, 50, 16);

    int val = 7864 - P2.toInt() * 34.59;
    if (val > 7864)
      val = 7864;
    else if (val < 1638)
      val = 1638;
    ledcWrite(3, val);
  } else if (cmd == "relay") {  //繼電器
    pinMode(P1.toInt(), OUTPUT);
    digitalWrite(P1.toInt(), P2.toInt());
  } else if (cmd == "uart") {  //UART
    //Line Notify (Smile)
    if (P1 == "happy") {
      // Serial.println("seen happy face");
      varifier.saw_smile();
      //sendCapturedImage2LineNotify(lineNotifyToken);
    }
  } else if (cmd == "resetwifi") {  //重設網路連線
    for (int i = 0; i < 2; i++) {
      WiFi.begin(P1.c_str(), P2.c_str());
      Serial.print("Connecting to ");
      Serial.println(P1);
      long int StartTime = millis();
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if ((StartTime + 5000) < millis()) break;
      }
      Serial.println("");
      Serial.println("STAIP: " + WiFi.localIP().toString());
      Feedback = "STAIP: " + WiFi.localIP().toString();

      if (WiFi.status() == WL_CONNECTED) {
        WiFi.softAP((WiFi.localIP().toString() + "_" + P1).c_str(), P2.c_str());
        for (int i = 0; i < 2; i++) {  //若連不上WIFI設定閃光燈慢速閃爍
          ledcWrite(4, 10);
          delay(300);
          ledcWrite(4, 0);
          delay(300);
        }
        break;
      }
    }
  } else if (cmd == "framesize") {
    int val = P1.toInt();
    sensor_t* s = esp_camera_sensor_get();
    s->set_framesize(s, (framesize_t)val);
  } else if (cmd == "quality") {  //畫質
    sensor_t* s = esp_camera_sensor_get();
    s->set_quality(s, P1.toInt());
  } else if (cmd == "contrast") {  //對比
    sensor_t* s = esp_camera_sensor_get();
    s->set_contrast(s, P1.toInt());
  } else if (cmd == "brightness") {  //亮度
    sensor_t* s = esp_camera_sensor_get();
    s->set_brightness(s, P1.toInt());
  } else if (cmd == "saturation") {  //飽和度
    sensor_t* s = esp_camera_sensor_get();
    s->set_saturation(s, P1.toInt());
  } else if (cmd == "special_effect") {  //特效
    sensor_t* s = esp_camera_sensor_get();
    s->set_special_effect(s, P1.toInt());
  } else if (cmd == "sobel") {  //todo: it sends on but changes to "1"
    if (P1 == "1" || P1 == "on") {
      motion_detector = true;
      Serial.println("sobel on");
    } else if (P1 == "0" || P1 == "off") {
      motion_detector = false;
      Serial.println("sobel off");
    }
  } else {
    Feedback = "Command is not defined.";
  }
  if (Feedback == "") Feedback = Command;
}

//拆解命令字串置入變數
void getCommand(char c) {
  if (c == '?') ReceiveState = 1;
  if ((c == ' ') || (c == '\r') || (c == '\n')) ReceiveState = 0;

  if (ReceiveState == 1) {
    Command = Command + String(c);

    if (c == '=') cmdState = 0;
    if (c == ';') strState++;

    if ((cmdState == 1) && ((c != '?') || (questionstate == 1))) cmd = cmd + String(c);
    if ((cmdState == 0) && (strState == 1) && ((c != '=') || (equalstate == 1))) P1 = P1 + String(c);
    if ((cmdState == 0) && (strState == 2) && (c != ';')) P2 = P2 + String(c);
    if ((cmdState == 0) && (strState == 3) && (c != ';')) P3 = P3 + String(c);
    if ((cmdState == 0) && (strState == 4) && (c != ';')) P4 = P4 + String(c);
    if ((cmdState == 0) && (strState == 5) && (c != ';')) P5 = P5 + String(c);
    if ((cmdState == 0) && (strState == 6) && (c != ';')) P6 = P6 + String(c);
    if ((cmdState == 0) && (strState == 7) && (c != ';')) P7 = P7 + String(c);
    if ((cmdState == 0) && (strState == 8) && (c != ';')) P8 = P8 + String(c);
    if ((cmdState == 0) && (strState >= 9) && ((c != ';') || (semicolonstate == 1))) P9 = P9 + String(c);

    if (c == '?') questionstate = 1;
    if (c == '=') equalstate = 1;
    if ((strState >= 9) && (c == ';')) semicolonstate = 1;
  }
}

//設定選單初始值取回json格式
void status() {
  //回傳視訊狀態
  sensor_t* s = esp_camera_sensor_get();
  String json = "{";
  json += "\"framesize\":" + String(s->status.framesize) + ",";
  json += "\"quality\":" + String(s->status.quality) + ",";
  json += "\"brightness\":" + String(s->status.brightness) + ",";
  json += "\"contrast\":" + String(s->status.contrast) + ",";
  json += "\"saturation\":" + String(s->status.saturation) + ",";
  json += "\"special_effect\":" + String(s->status.special_effect) + ",";
  json += "\"vflip\":" + String(s->status.vflip) + ",";
  json += "\"hmirror\":" + String(s->status.hmirror);
  json += "}";

  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
  client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
  client.println("Content-Type: application/json; charset=utf-8");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Connection: close");
  client.println();

  for (int Index = 0; Index < json.length(); Index = Index + 1024) {
    client.print(json.substring(Index, Index + 1024));
  }
}

void mainpage() {
  Serial.println("main page setup");
  //回傳HTML首頁或Feedback
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
  client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
  client.println("Content-Type: text/html; charset=utf-8");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Connection: close");
  client.println();

  String Data = "";
  if (cmd != "")
    Data = Feedback;
  else
    Data = String((const char*)INDEX_HTML);

  for (int Index = 0; Index < Data.length(); Index = Index + 1024) {
    client.print(Data.substring(Index, Index + 1024));
  }
}

void write_stream() {
  //回傳JPEG格式影像
  camera_fb_t* fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
  client.println("Access-Control-Allow-Methods: GET,POST,PUT,DELETE,OPTIONS");
  client.println("Content-Type: image/jpeg");
  client.println("Content-Disposition: form-data; name=\"imageFile\"; filename=\"picture.jpg\"");
  client.println("Content-Length: " + String(fb->len));
  client.println("Connection: close");
  client.println();

  uint8_t* fbBuf = fb->buf;
  size_t fbLen = fb->len;
  sobelMotionDetector(fb);
  for (size_t n = 0; n < fbLen; n = n + 1024) {
    if (n + 1024 < fbLen) {
      client.write(fbBuf, 1024);
      fbBuf += 1024;
    } else if (fbLen % 1024 > 0) {
      size_t remainder = fbLen % 1024;
      client.write(temp_buf, remainder);
    }
  }
  esp_camera_fb_return(fb);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  Serial.println("sent still");
}
void sobelMotionDetector(camera_fb_t* fb) {
  Serial.println(fb->format);
  uint8_t* bmp_buf;
  size_t bmp_len;
  esp_err_t err = frame2bmp(fb, &bmp_buf, &bmp_len);
  if (err != ESP_OK) {
    Serial.printf("Frame to BMP conversion failed with error 0x%x", err);
    return;
  }
  Serial.println("&&");
  Serial.print(bmp_buf[0]);
}
/*not using this function
String sendCapturedImage2LineNotify(String token) {
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "Camera capture failed";
  }
   
  WiFiClientSecure client_tcp;
  client_tcp.setInsecure();   //run version 1.0.5 or above
  Serial.println("Connect to notify-api.line.me");
  if (client_tcp.connect("notify-api.line.me", 443)) {
    Serial.println("Connection successful");
    
    String message = "ESP32-CAM";
    String head = "--Taiwan\r\nContent-Disposition: form-data; name=\"message\"; \r\n\r\n" + message + "\r\n--Taiwan\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--Taiwan--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;
  
    client_tcp.println("POST /api/notify HTTP/1.1");
    client_tcp.println("Connection: close"); 
    client_tcp.println("Host: notify-api.line.me");
    client_tcp.println("Authorization: Bearer " + token);
    client_tcp.println("Content-Length: " + String(totalLen));
    client_tcp.println("Content-Type: multipart/form-data; boundary=Taiwan");
    client_tcp.println();
    client_tcp.print(head);
    
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0;n<fbLen;n=n+1024) {
      if (n+1024<fbLen) {
        client_tcp.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client_tcp.write(fbBuf, remainder);
      }
    }  
    
    client_tcp.print(tail);
    esp_camera_fb_return(fb);

    String getResponse="",Feedback="";
    int waitTime = 10000;   // timeout 10 seconds
    long startTime = millis();
    boolean state = false;
    
    while ((startTime + waitTime) > millis()) {
      Serial.print(".");
      delay(100);      
      while (client_tcp.available())  {
          char c = client_tcp.read();
          if (state==true) Feedback += String(c);        
          if (c == '\n') {
            if (getResponse.length()==0) state=true; 
            getResponse = "";
          } 
          else if (c != '\r')
            getResponse += String(c);
          startTime = millis();
       }
       if (Feedback.length()>0) break;
    }
    Serial.println();
    client_tcp.stop();
    return Feedback;
  }
  else {
    return "Connected to notify-api.line.me failed.";
  }
}*/