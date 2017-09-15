SoftwareSerial swSer(13, 15, false, 256); // GPIO15 (TX) and GPIO13 (RX)

//SoftwareSerial swSer(3, 1, false, 256); // GPIO1 (TX) and GPIO3 (RX)

#define SENSOR_SERIAL swSer
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
unsigned char response[7];

int readCO2() {
        // CO2
        bool header_found {false};
        char tries {0};

        SENSOR_SERIAL.write(cmd, 9);
        memset(response, 0, 7);

        // Looking for packet start
        while(SENSOR_SERIAL.available() && (!header_found)) {
                if(SENSOR_SERIAL.read() == 0xff ) {
                        if(SENSOR_SERIAL.read() == 0x86 ) header_found = true;
                }
        }

        if (header_found) {
                SENSOR_SERIAL.readBytes(response, 7);

                byte crc = 0x86;
                for (char i = 0; i < 6; i++) {
                        crc+=response[i];
                }
                crc = 0xff - crc;
                crc++;

                if ( !(response[6] == crc) ) {
                        DEBUG_SERIAL.println("CO2: CRC error: " + String(crc) + " / "+ String(response[6]));
                } else {
                        unsigned int responseHigh = (unsigned int) response[0];
                        unsigned int responseLow = (unsigned int) response[1];
                        unsigned int ppm = (256*responseHigh) + responseLow;
                        return ppm;
//                        DEBUG_SERIAL.println("CO2:" + String(co2));
                }
        } else {
                DEBUG_SERIAL.println("CO2: Header not found");
                return -3000;
        }

}

