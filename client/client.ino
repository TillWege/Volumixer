#include "WiFi.h"
#include <U8g2lib.h>
#include <ESP32RotaryEncoder.h>
#include "Wire.h"

const int rotSW = 13;
const int rotDT = 26;
const int rotCLK = 25;

RotaryEncoder rotaryEncoder(rotCLK, rotDT, rotSW);

int counter = 0;
int btn_prev;

int current_i2c = 0;
#define TCAADDR 0x70

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void tcaselect(uint8_t i)
{
  if (i > 7)
    return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup()
{
  Serial.begin(115200);
  pinMode(rotSW, INPUT);

  btn_prev = digitalRead(rotSW);

  Serial.println("Init");

  rotaryEncoder.setBoundaries(1, 10, true);
  rotaryEncoder.setEncoderType(EncoderType::HAS_PULLUP);
  rotaryEncoder.begin();

  counter = rotaryEncoder.getEncoderValue();

  Wire.begin();

  Serial.println("\nTCAScanner ready!");

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
        u8g2.begin();
      }
    }
  }
  Serial.println("\ndone");
}

void loop()
{
  counter = rotaryEncoder.getEncoderValue();

  int btn = digitalRead(rotSW);
  if (btn != btn_prev)
  {
    if (btn == HIGH)
    {
      Serial.println("Button Released");
    }
    else
    {
      Serial.println("Button Pressed");
    }
  }
  btn_prev = btn;

  tcaselect(0);

  u8g2.clearBuffer();                           // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);           // choose a suitable font
  u8g2.drawStr(0, 10, "Hello World!");          // write something to the internal memory
  u8g2.drawStr(0, 30, String(counter).c_str()); // write something to the internal memory
  u8g2.sendBuffer();                            // transfer internal memory to the display

  tcaselect(1);

  u8g2.clearBuffer();                           // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);           // choose a suitable font
  u8g2.drawStr(0, 10, "Hello World!");          // write something to the internal memory
  u8g2.drawStr(0, 30, String(counter).c_str()); // write something to the internal memory
  u8g2.sendBuffer();                            // transfer internal memory to the display

  delay(10);
}
