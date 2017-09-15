// Use U8g2 for i2c OLED Lib
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, I2C_SCL, I2C_SDA, U8X8_PIN_NONE);
byte yToPrint {0};
char sToPrint [40];

void drawLine () {
   u8g2.setFont(u8g2_font_5x7_mf);
   int x = (128 - u8g2.getStrWidth(sToPrint))/2;
   u8g2.drawStr(x, yToPrint, sToPrint);
   yToPrint += 8;
}

void draw() {
        u8g2.clearBuffer();
        yToPrint = 8;
/*        
        // rtc
        RtcDateTime d = Rtc.GetDateTime ();
        sprintf (sToPrint, "%d/%d/%d %d:%d:%d", d.Day(), d.Month(), d.Year(), d.Hour(), d.Minute(), d.Second());
        drawLine ();
*/
        // wi-fi
        if (WiFi.status () == WL_CONNECTED) {
          IPAddress ip = WiFi.localIP ();
          sprintf (sToPrint, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        } else {
           sprintf (sToPrint, "Wi-Fi... %d try", wifiRetryCount);
        }    
        drawLine ();

        yToPrint+=8;
        
        sprintf (sToPrint, "co2: %d ppm", (int)co2);
        drawLine ();

        sprintf (sToPrint, "mq4: %d", (int)mq4);
        drawLine ();

        sprintf (sToPrint, "bmp %d c %d mm", (int)bmp_t, (int)bmp_p);
        drawLine ();

        sprintf (sToPrint, "am %d.%d c %d.%d %", (int)(am_t), ((int)(am_t*10))%10, (int)(am_h), ((int)(am_h*10))%10);
        drawLine ();
        
        sprintf (sToPrint, "dht %d c %d %", (int)dht_t, (int)dht_h);
        drawLine ();

        sprintf (sToPrint, "htu %d c %d %", (int)htu_t, (int)htu_h);
        drawLine ();

        u8g2.sendBuffer();
}

