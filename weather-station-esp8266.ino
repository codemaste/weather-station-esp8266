#include "config.h"
/* config.h
// char wifissid[] = "";      // your network SSID (name)
// char wifipass[] = "";   // your network password
// char narodmonMac[] = "XX-XX-XX-XX-XX-XX";
*/

#include <FS.h>
#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>          //https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_ADS1015.h>
#include <Adafruit_AM2315.h>
#include <Adafruit_BMP085.h> 
#include <Adafruit_HTU21DF.h>
#include <Adafruit_BME280.h> 
#include <DHT.h> 
#include <U8g2lib.h>
/*
#include <RtcDS1307.h>
#include <sntp.h>
*/

#define DEBUG_SERIAL Serial

// GPIO Defines
#define I2C_SDA D2
#define I2C_SCL D1

// DHT22
#define DHT_PIN D3
#define DHT_VERSION DHT22
DHT dht(DHT_PIN, DHT_VERSION); //define temperature and humidity sensor

//RtcDS1307<TwoWire> Rtc(Wire);
Adafruit_AM2315 am2315;
Adafruit_BMP085 bmp;
Adafruit_BME280 bme;
Adafruit_ADS1115 ads(0x4A); //Здесь указываем адрес устройства
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// Handy timers
SimpleTimer timer;

/////////////////////////////////////////////////// wi fi server

bool wasConnected = false;
int wifiRetryCount = 0;
int whNarodmonSent = 0;
ESP8266WebServer HTTPserver(80);
unsigned int localUdpPort = 2390;      // local port to listen for UDP packets
WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP

// measured values
float co2 {-3000};
float dht_t {-3000};
float dht_h {-3000};
float htu_t {-3000};
float htu_h {-3000};
float am_t {-3000};
float am_h {-3000};
float bmp_t {-3000};
float bmp_p {-3000};
float bme_t {-3000};
float bme_h {-3000};
float bme_p {-3000};

//#define SRC_AVG_NUM 900
#define SRC_AVG_NUM 60
int mq4_src [SRC_AVG_NUM];
int mq136_src [SRC_AVG_NUM];
int mq137_src [SRC_AVG_NUM];
int dust_src [SRC_AVG_NUM];
float mq4 {-3000};
float mq136 {-3000};
float mq137 {-3000};
float dust {-3000};

///////////////////////////////////////////
int vent_mode = -1; // -1 auto, 0 off, 1, 2, 3, 4
int bound_1 = 500;
int bound_2 = 600;
int bound_3 = 9550;
int bound_4 = 9600;
int recomend_vent = 0;
int current_vent = 0;

//////////////////////////////////////////
float protect_dust = -1;
float protect_mq4 = -1;
float protect_mq136 = -1;
float protect_mq137 = -1;

// ntp and other stuff
#include "ntp.h"
#include "co2.h"
#include "display.h"
#include "web.h"
#include "narodmon.h"

///////////////////////////////////////////

float measureMqOne (int* src, int channel) {
  int s = 0;
  int countGood = 0;
  for (int i=0; i<SRC_AVG_NUM-1; i++) {
    src [i] = src [i+1];
    if (src [i] > 0 && src [i] < 10000) { 
      s += src [i]; 
      countGood ++; 
    }
  }
  src [SRC_AVG_NUM-1] = ads.readADC_SingleEnded(channel);
  s += src [SRC_AVG_NUM-1];
  return ((float)s) / ((float)countGood);
}

void measureMQ () {
//  manageVent (true); // turn ON dust LED
  
  mq136 = measureMqOne (mq136_src, 1);
  mq4 = measureMqOne (mq4_src, 2);
  mq137 = measureMqOne (mq137_src, 3);

//  delayMicroseconds (280);
//  dust = measureMqOne (dust_src, 0);

//  manageVent (false); // turn OFF dust LED
}

void runMeasurements() {
        float i = readCO2();
        if (i>100 && i<15000) co2 = i;
        else co2 = -2000;

        // htu21
        
        i = htu.readTemperature();
        if (i>-1000 && i<1000) htu_t = i;
        else htu_t = -2000;

        i =  htu.readHumidity();
        if (i>-1000 && i<1000) htu_h = i;
        else htu_h = -2000;

        // dht22

        i =  dht.readHumidity();
        if (i>-1000 && i<1000) dht_h = i;
        else dht_h = -2000;
        
        i = dht.readTemperature();
        if (i>-1000 && i<1000) dht_t = i;
        else dht_t = -2000;

        // bmp180

        i = bmp.readTemperature();
        if (i>-1000 && i<1000) bmp_t = i;
        else bmp_t = -2000;
        
        i = (float)0.0075 * bmp.readPressure();
        if (i>10 && i<1000) bmp_p = i;
        else bmp_p = -2000;

        // bme280

        i = bme.readTemperature();
        if (i>-1000 && i<1000) bme_t = i;
        else bme_t = -2000;

        i =  bme.readHumidity();
        if (i>-1000 && i<1000) bme_h = i;
        else bme_h = -2000;
        
        i = (float)0.0075 * bme.readPressure();
        if (i>10 && i<1000) bme_p = i;
        else bme_p = -2000;

        // am2315

        if (!am2315.readTemperatureAndHumidity (am_t, am_h)) {
          am_t = -2000;
          am_h = -2000;

          if (!am2315.begin ()) am_t = -2100;
        }
        
}

void manageVent (bool dustLed) {

  int vent_speed = 0;

  recomend_vent = 0;
  if (co2 >= bound_4) recomend_vent = 4;
  else if (co2 >= bound_3) recomend_vent = 3;
  else if (co2 >= bound_2) recomend_vent = 2;
  else if (co2 >= bound_1) recomend_vent = 1;

  if (vent_mode == -1) { // auto
    vent_speed = recomend_vent;
    
  } else {
    if (vent_mode >=0 && vent_mode <= 4) {
      vent_speed = vent_mode;
    }
  }

  if (mq4 >= protect_mq4) vent_speed = 0;

  current_vent = vent_speed;

  int outMask = 0xFF;

  if (vent_speed == 1) outMask = 0xFF ^ 16;
  if (vent_speed == 2) outMask = 0xFF ^ 16 ^ 64;
  if (vent_speed == 3) outMask = 0xFF ^ 16 ^ 32;
  if (vent_speed == 4) outMask = 0xFF ^ 16 ^ 32 ^ 128;

//  if (dustLed) outMask ^= 1 + 2 + 4 + 8; 

  Wire.beginTransmission (0x3F);
  Wire.write (outMask);
  Wire.endTransmission ();
}

void manageVentTimer () {
  manageVent (false);
}

void saveConfig () {
  File f = SPIFFS.open("/config.vent_mode.txt", "w");
  if (!f) {
          DEBUG_SERIAL.println("error writing config file.");
  } else {
    f.println (vent_mode);
    f.println ((int)(protect_dust * 100));
    f.println ((int)(protect_mq4 * 100));
    f.println ((int)(protect_mq136 * 100));
    f.println ((int)(protect_mq137 * 100));
    f.close();
  }  
}

void handleVentModeButton (int b) {
  vent_mode = b; 
  HTTPserver.sendHeader("Location", String("index.html"), true); 
  HTTPserver.send (302, "text/plain", "");
  saveConfig ();
}

void handleProtectButton () {
  int n = HTTPserver.arg ("n").toInt();
  float v = HTTPserver.arg ("v").toFloat();
  if (n == 1) protect_dust = v;
  if (n == 4) protect_mq4 = v;
  if (n == 136) protect_mq136 = v;
  if (n == 137) protect_mq137 = v;
  HTTPserver.sendHeader("Location", String("index.html"), true); 
  HTTPserver.send (302, "text/plain", "");
  saveConfig ();
}

void setup() {
        // Init serial ports
        DEBUG_SERIAL.begin(115200);
        SENSOR_SERIAL.begin(9600);

        // FS and config
        SPIFFS.begin();
        File f = SPIFFS.open("/config.vent_mode.txt", "r");
        if (!f) {
          DEBUG_SERIAL.println("config file open failed.");
//          SPIFFS.format();
        } else {
          DEBUG_SERIAL.println("config file open OK.");
          // vent mode
          String s=f.readStringUntil('\n');
          DEBUG_SERIAL.println("readed: ");
          DEBUG_SERIAL.println(s);
          int m = (int)s.toInt ();
          if (m >= -1 && m <= 4) {
            vent_mode = m;
          }
          // protect
          float p;
          s=f.readStringUntil('\n'); p = ((float)s.toInt ()) / 100.00; if (p >= -1 && p <= 10000) { protect_dust = p; }
          s=f.readStringUntil('\n'); p = ((float)s.toInt ()) / 100.00; if (p >= -1 && p <= 10000) { protect_mq4 = p; }
          s=f.readStringUntil('\n'); p = ((float)s.toInt ()) / 100.00; if (p >= -1 && p <= 10000) { protect_mq136 = p; }
          s=f.readStringUntil('\n'); p = ((float)s.toInt ()) / 100.00; if (p >= -1 && p <= 10000) { protect_mq137 = p; }
          f.close();
        }

        // Init I2C interface
        Wire.begin(I2C_SDA, I2C_SCL);
        dht.begin ();
//        Rtc.Begin ();
//Rtc.SetDateTime (12345678);
        
        if (!am2315.begin ()) am_t = -2000;
        if (!bmp.begin ()) bmp_t = -2000;
        if (!bme.begin (0x76)) bme_t = -2001;

        // Init display
        u8g2.begin();

        // Wi-Fi
        WiFi.mode (WIFI_STA);
        WiFi.begin (wifissid, wifipass);

        // averages
        for (int i=0; i<SRC_AVG_NUM; i++) {
          mq4_src [i] = -2000;
          mq136_src [i] = -2000;
          mq137_src [i] = -2000;
          dust_src [i] = -2000;
        }

        timer.setInterval (1000L, measureMQ);         // Setup a function to be called every 1 second
        timer.setInterval (10000L, runMeasurements);         // Setup a function to be called every 10 second
        timer.setInterval (500L, draw);         // Setup a function to be called every 0.5 second
        timer.setInterval (1000L, manageVentTimer);         // Setup a function to be called every 1 second
        timer.setInterval (15000L, sendNarodmon); // narodmon every 15 sec, also internal timer in narodmon.h

        // turn off nodemcu blue light
        pinMode (D4, OUTPUT);
        digitalWrite (D4, HIGH); 

        // Setup HTTP server
        HTTPserver.on("/", [](){HTTPserver.send (200, "text/html", HTTPgenIndexPage());});
        HTTPserver.on("/index.html", [](){HTTPserver.send (200, "text/html", HTTPgenIndexPage());});
        HTTPserver.on("/data.xml", [](){HTTPserver.send (200, "text/html", HTTPgenXML());});
        HTTPserver.on("/vent_auto", [](){ handleVentModeButton (-1); });
        HTTPserver.on("/vent_0", [](){ handleVentModeButton (0); });
        HTTPserver.on("/vent_1", [](){ handleVentModeButton (1); });
        HTTPserver.on("/vent_2", [](){ handleVentModeButton (2); });
        HTTPserver.on("/vent_3", [](){ handleVentModeButton (3); });
        HTTPserver.on("/vent_4", [](){ handleVentModeButton (4); });
        HTTPserver.on("/protect_set", [](){ handleProtectButton (); });
}

void loop() {

  if (WiFi.status () == WL_CONNECTED) {
    if (!wasConnected) {
      HTTPserver.begin ();
      
      ArduinoOTA.setHostname ("ESP8266-00001");
      ArduinoOTA.begin ();
      
      udp.begin (localUdpPort);
//      sendNTPpacket ();
    }   
    wasConnected = true;

    ArduinoOTA.handle ();
    
    HTTPserver.handleClient ();
/*
    // ntp answer received
    int cb = udp.parsePacket();
    if (cb) {
      DEBUG_SERIAL.print("packet received, length=");
      DEBUG_SERIAL.println(cb);
      parseNTP ();
    }
 */   
  } else {
    if (wifiRetryCount > 50) {
      DEBUG_SERIAL.println("wi-fi reconnect");
      WiFi.begin(wifissid, wifipass); // reconnect
      wifiRetryCount = 0;
    } else {
      DEBUG_SERIAL.println("wi-fi try");
      wifiRetryCount ++;
    }
    wasConnected = false;
  }

  timer.run();
  
  delay (100);
}
