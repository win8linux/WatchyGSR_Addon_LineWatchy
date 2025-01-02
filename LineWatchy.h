#ifndef LINE_WATCHY_H
#define LINE_WATCHY_H
#include "DSEG7_Classic_Regular_39.h"
#include "GxEPD2.h"
#include "Watchy_GSR.h"

class LineWatchy {
    public:
        void drawWatchFace();
        void drawHand(uint8_t _radius_, float _angle_);
};

#endif