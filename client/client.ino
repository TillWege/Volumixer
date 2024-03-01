#include "WiFi.h"
#include <ESP32RotaryEncoder.h>
#include "Wire.h"
#include "multi.h"
#include <U8g2lib.h>

const int rotSW = 5;
const int rotDT = 18;
const int rotCLK = 19;
const int slider = 15;

RotaryEncoder *rotaryEncoder;
Multiplexer *multiplexer;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

int counter[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int btn_prev;

#include "ADS1X15.h"

ADS1115 ADS(0x48);

void setup()
{
  Serial.println("Setup::Start");
  Serial.begin(115200);
  Wire.begin();

  multiplexer = new Multiplexer(u8g2);

  pinMode(rotSW, INPUT);
  pinMode(slider, INPUT);

  btn_prev = digitalRead(rotSW);

  rotaryEncoder = new RotaryEncoder(rotCLK, rotDT, rotSW);
  rotaryEncoder->setBoundaries(-10, 10, false);
  rotaryEncoder->setEncoderType(EncoderType::HAS_PULLUP);
  rotaryEncoder->begin();

  u8g2.begin();

  Serial.println("Setup::End");
  ADS.begin();
  ADS.setGain(0);
}

void loop()
{
  int newC = rotaryEncoder->getEncoderValue();
  counter[multiplexer->get_active_device()] += newC;

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
      multiplexer->next_device();
    }
  }
  btn_prev = btn;

  Serial.print("Slider: ");
  int16_t val = ADS.readADC(0);
  Serial.println(map(val, 0, 21000, 0, 100));

  float f = ADS.toVoltage(2); // voltage factor
  Serial.print("\tAnalog0: ");
  Serial.print(val);
  Serial.print('\t');
  Serial.println(val * f, 3);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.drawStr(0, 10, "Hello World!");
  u8g2.drawStr(0, 20, String(counter[multiplexer->get_active_device()]).c_str());
  u8g2.sendBuffer();

  delay(10);
}
