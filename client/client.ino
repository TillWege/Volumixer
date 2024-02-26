#include "WiFi.h"
#include <U8g2lib.h>
#include <ESP32RotaryEncoder.h>
#include "Wire.h"
#include "multi.h"

const int rotSW = 13;
const int rotDT = 26;
const int rotCLK = 25;

RotaryEncoder rotaryEncoder(rotCLK, rotDT, rotSW);

int counter = 0;
int btn_prev;

void setup()
{
  Serial.println("Setup::Start");
  Serial.begin(115200);

  pinMode(rotSW, INPUT);

  btn_prev = digitalRead(rotSW);

  rotaryEncoder.setBoundaries(1, 10, true);
  rotaryEncoder.setEncoderType(EncoderType::HAS_PULLUP);
  rotaryEncoder.begin();

  counter = rotaryEncoder.getEncoderValue();

  Wire.begin();
  i2cScan();

  Serial.println("Setup::End");
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
