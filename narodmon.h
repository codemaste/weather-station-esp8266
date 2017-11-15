

void sendNarodmon () {

  if (abs (millis() - whNarodmonSent) < 6*60*1000) return;

  if (WiFi.status() != WL_CONNECTED) return;

  float sum_t = 0, sum_h = 0, sum_p = 0;
  int num_t = 0, num_h = 0, num_p = 0;

  if (am_t > -60 && am_t < 60) { sum_t += am_t; num_t++; }
  if (bme_t > -60 && bme_t < 60) { sum_t += bme_t; num_t++; }

  if (am_h > 3 && am_h < 110) { sum_h += am_h; num_h++; }
  if (bme_h > 3 && bme_h < 110) { sum_h += bme_h; num_h++; }

  if (bmp_p >= 600 && bmp_p <= 900) { sum_p += bmp_p; num_p++; }
  if (bme_p >= 600 && bme_p <= 900) { sum_p += bme_p; num_p++; }

  if (num_t > 0 && num_h > 0 && num_p > 0) {

    String s;
    char buf [512];

    s += "#";
    s += narodmonMac;
    s += "\n";
    s += "#0123450000000001#" + String (sum_t / num_t) + "\n";
    s += "#0123450000000002#" + String (sum_h / num_h) + "\n";
    s += "#0123450000000003#" + String (sum_p / num_p) + "\n";
    s += "##";

    s.toCharArray (buf, 512);
  
    udp.beginPacket ("narodmon.ru", 8283);
    udp.write (buf, s.length ());
    udp.endPacket ();

    DEBUG_SERIAL.println (s);

    whNarodmonSent = millis();
  }
}

