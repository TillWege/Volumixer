#include "multi.h"

Multiplexer::Multiplexer(U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2)
{
    Serial.println("Scanner::Start");

    for (uint8_t t = 0; t < 8; t++)
    {
        select_device(t);
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
            }
        }
    }

    for (uint8_t t = 0; t < 8; t++)
    {
        if (displays[t])
        {
            select_device(t);
            u8g2.begin();
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_ncenB08_tr);

            u8g2.drawStr(0, 10, "Hello World!");
            u8g2.sendBuffer();
        }
    }

    Serial.println("Scanner::End");
}

void Multiplexer::select_device(int device)
{
    if (device > 7)
        return;

    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << device);
    Wire.endTransmission();
    active_device = device;
}

int Multiplexer::device_count()
{
    int count = 0;
    for (uint8_t t = 0; t < 8; t++)
    {
        if (displays[t])
            count++;
    }
    return count;
}

void Multiplexer::next_device()
{
    while (true)
    {
        active_device++;
        if (active_device > 7)
            active_device = 0;
        if (displays[active_device])
            break;
    }
    select_device(active_device);
}

void Multiplexer::prev_device()
{
    while (true)
    {
        active_device--;
        if (active_device < 0)
            active_device = 7;
        if (displays[active_device])
            break;
    }
    select_device(active_device);
}

int Multiplexer::get_active_device()
{
    return active_device;
}