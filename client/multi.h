#include "WiFi.h"
#include <U8g2lib.h>
#include <ESP32RotaryEncoder.h>
#include "Wire.h"

int current_i2c = 0;
#define TCAADDR 0x70

bool displays[8] = {false, false, false, false, false, false, false, false};

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void tcaselect(uint8_t i)
{
    if (i > 7)
        return;

    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

void i2cScan()
{
    Serial.println("Scanner::Start");

    for (uint8_t t = 0; t < 8; t++)
    {
        tcaselect(t);
        Serial.print("TCA Port #");
        Serial.println(t);

        for (uint8_t addr = 0; addr <= 127; addr++)
        {
            if (addr == TCAADDR)
                continue;

            Wire.beginTransmission(addr);
            if (!Wire.endTransmission())
            {
                Serial.print("Found I2C 0x");
                Serial.println(addr, HEX);
                displays[t] = true;
                u8g2.begin();
            }
        }
    }
    Serial.println("Scanner::End");
}