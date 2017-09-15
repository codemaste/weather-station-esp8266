// ntp stuff
// https://abzman2k.wordpress.com/2015/09/24/esp8266-rtc-and-ntp/
// https://github.com/abzman/esp-stuff/blob/master/esp_mqtt_rtc/esp_mqtt_rtc.ino

/*
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

unsigned long sendNTPpacket()
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  //udp.beginPacket(address, 123); //NTP requests are to port 123

  udp.beginPacket("0.pool.ntp.org", 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

String returnDateTime(const RtcDateTime& dt)
{
  String datestring = "";
  if (dt.Month() < 10) {
    datestring += "0";
  }
  datestring += dt.Month();
  datestring += "/";
  if (dt.Day() < 10) {
    datestring += "0";
  }
  datestring += dt.Day();
  datestring += "/";
  datestring += dt.Year();
  datestring += " ";
  if (dt.Hour() < 10) {
    datestring += "0";
  }
  datestring += dt.Hour();
  datestring += ":";
  if (dt.Minute() < 10) {
    datestring += "0";
  }
  datestring += dt.Minute();
  datestring += ":";
  if (dt.Second() < 10) {
    datestring += "0";
  }
  datestring += dt.Second();
  return datestring;
}

void printDateTime(const RtcDateTime& dt)
{
  DEBUG_SERIAL.print(returnDateTime(dt));
}

void parseNTP () {
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // NTP contains four timestamps with an integer part and a fraction part
    // we only use the integer part here
    unsigned long t1, t2, t3, t4;
    t1 = t2 = t3 = t4 = 0;
    for (int i = 0; i < 4; i++)
    {
      t1 = t1 << 8 | packetBuffer[16 + i];
      t2 = t2 << 8 | packetBuffer[24 + i];
      t3 = t3 << 8 | packetBuffer[32 + i];
      t4 = t4 << 8 | packetBuffer[40 + i];
    }

    // part of the fractional part
    // could be 4 bytes but this is more precise than the 1307 RTC
    // which has a precision of ONE second
    // in fact one byte is sufficient for 1307
    float f1, f2, f3, f4;
    f1 = ((long)packetBuffer[20] * 256 + packetBuffer[21]) / 65536.0;
    f2 = ((long)packetBuffer[28] * 256 + packetBuffer[29]) / 65536.0;
    f3 = ((long)packetBuffer[36] * 256 + packetBuffer[37]) / 65536.0;
    f4 = ((long)packetBuffer[44] * 256 + packetBuffer[45]) / 65536.0;

    // NOTE:
    // one could use the fractional part to set the RTC more precise
    // 1) at the right (calculated) moment to the NEXT second!
    //    t4++;
    //    delay(1000 - f4*1000);
    //    RTC.adjust(DateTime(t4));
    //    keep in mind that the time in the packet was the time at
    //    the NTP server at sending time so one should take into account
    //    the network latency (try ping!) and the processing of the data
    //    ==> delay (850 - f4*1000);
    // 2) simply use it to round up the second
    //    f > 0.5 => add 1 to the second before adjusting the RTC
    //   (or lower threshold eg 0.4 if one keeps network latency etc in mind)
    // 3) a SW RTC might be more precise, => ardomic clock :)


    // convert NTP to UNIX time, differs seventy years = 2208988800 seconds
    // NTP starts Jan 1, 1900
    // Unix time starts on Jan 1 1970.
    #define SECONDS_FROM_1970_TO_2000 946684800
    const unsigned long seventyYears = 2208988800UL + 946684800UL; //library differences, it wants seconds since 2000 not 1970
    t1 -= seventyYears;
    t2 -= seventyYears;
    t3 -= seventyYears;
    t4 -= seventyYears;

    Serial.println("T1 .. T4 && fractional parts");
    printDateTime(RtcDateTime(t1)); DEBUG_SERIAL.println(f1, 4);
    printDateTime(RtcDateTime(t2)); DEBUG_SERIAL.println(f2, 4);
    printDateTime(RtcDateTime(t3)); DEBUG_SERIAL.println(f3, 4);
    printDateTime(RtcDateTime(t4)); DEBUG_SERIAL.println(f4, 4);
    DEBUG_SERIAL.println();

    t4 += (3 * 3600L);

    DEBUG_SERIAL.print("RTC before : ");
    printDateTime(Rtc.GetDateTime());  
    DEBUG_SERIAL.println();
    
    Rtc.SetDateTime (t4);

    DEBUG_SERIAL.print("RTC after : ");
    printDateTime(Rtc.GetDateTime());  
    DEBUG_SERIAL.println();  
}
*/
