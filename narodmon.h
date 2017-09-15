

void sendNarodmon () {

  if (abs (millis() - whNarodmonSent) < 6*60*1000) return;

  if (WiFi.status() != WL_CONNECTED) return;

  if (am_t > -60 && am_t < 60 && am_h >= 3 && am_h <= 110 && bmp_p >= 600 && bmp_p <= 900) {

    String s;
    char buf [512];

    s += "#";
    s += narodmonMac;
    s += "\n";
    s += "#0123450000000001#" + String (am_t) + "\n";
    s += "#0123450000000002#" + String (am_h) + "\n";
    s += "#0123450000000003#" + String (bmp_p) + "\n";
    s += "##";

    s.toCharArray (buf, 512);
  
    udp.beginPacket ("narodmon.ru", 8283);
    udp.write (buf, s.length ());
    udp.endPacket ();

    DEBUG_SERIAL.println (s);

    whNarodmonSent = millis();
  }
}

