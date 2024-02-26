#include "WiFi.h"
#include <U8g2lib.h>
#include "Wire.h"

const int ButtonPin = 13;
int current_i2c = 0;
int counter = 0;
int btn_prev;
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
  pinMode(ButtonPin, INPUT);
  btn_prev = digitalRead(ButtonPin);

  Serial.println("Init");

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
  int btnState = digitalRead(ButtonPin);
  if (btnState == LOW && btn_prev == HIGH)
  {
    if (current_i2c == 0)
    {
      current_i2c = 1;
    }
    else
    {
      current_i2c = 0;
    }
  }
  btn_prev = btnState;

  tcaselect(current_i2c);

  u8g2.clearBuffer();                           // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);           // choose a suitable font
  u8g2.drawStr(0, 10, "Hello World!");          // write something to the internal memory
  u8g2.drawStr(0, 30, String(counter).c_str()); // write something to the internal memory
  u8g2.sendBuffer();                            // transfer internal memory to the display
  delay(100);
  counter++;
}
