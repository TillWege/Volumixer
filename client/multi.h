#ifndef MULTI_H
#define MULTI_H

#include "Wire.h"
#include <U8g2lib.h>
#define TCAADDR 0x70

class Multiplexer
{
private:
    int active_device = 0;
    bool displays[8] = {false, false, false, false, false, false, false, false};

public:
    Multiplexer(U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2);

    void select_device(int device);
    int device_count();
    void next_device();
    void prev_device();
    int get_active_device();
};

#endif