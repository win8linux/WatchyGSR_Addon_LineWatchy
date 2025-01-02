// ---------------------- Line-for-Watchy 0.2 -------------------------------
/////////////////////////////////////////////////////////////////////////////
// Analog Watchface for Watchy E-Paper Watch
// Design inspired by Line Watch Face for Google Wear OS by Seahorsepip
// https://play.google.com/store/apps/details?id=com.seapip.thomas.line_watchface
//
// Copyright (c) 2021 Thomas Schaefer
// thomas@blinky.berlin
// Copyright (c) 2024 Julius Enriquez
// win8linux@gmail.com
/////////////////////////////////////////////////////////////////////////////
// Hardware Setup: SQFMI E-Paper Watch "Watchy"
// https://watchy.sqfmi.com/
// - ESP32-PICO-D4
// - E-Paper Display GDEH0154D67
// - 3-Axis Accelerometer BMA423
// - Real Time Clock DS3231
// - Tactile Buttons K2-1114SA-A4SW-06
// - Vibration Motor 1020
// - Battery LiPo Battery 3.7V 200mAh
/////////////////////////////////////////////////////////////////////////////
// Free Culture License:
// This work is licensed under a Creative Commons
// >>> Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0) <<<
// Adaptations and commercial use are allowed AS LONG AS OTHERS SHARE ALIKE
// License terms: https://creativecommons.org/licenses/by-sa/4.0/
/////////////////////////////////////////////////////////////////////////////

#include "LineWatchy.h"

WatchyGSR watchyGSR;

struct tm timeinfo;
String datetime;

//inherit and extend WatchyGSR class
class WatchFace : public WatchyGSR {
  public:
    //override this method to customize how the watch face looks
    void drawWatchFace() {
   
      uint8_t myHour;
      uint8_t myMinute;
      uint8_t radius;
      float angle_hourScale;
      float angle_minuteScale;
      float angle_hourHand;
      float angle_minuteHand;
      float pi = 3.1415926535897932384626433832795;
      
      WatchyGSR::display.fillScreen(watchyGSR.ForeColor());
      WatchyGSR::display.setTextColor(watchyGSR.BackColor());
      WatchyGSR::display.setFont(&DSEG7_Classic_Regular_39);
      
      // draw outer circle
      //WatchyGSR::display.drawCircle(100, 100, 99, watchyGSR.BackColor());
      //WatchyGSR::display.drawCircle(100, 100, 98, watchyGSR.BackColor());

      myHour   = WatchTime.Local.Hour > 12 ? WatchTime.Local.Hour - 12 : WatchTime.Local.Hour;
      myMinute = WatchTime.Local.Minute;
      
      angle_hourScale   = 2*pi/12;
      angle_minuteScale = 2*pi/60;
      angle_hourHand    = angle_hourScale*(myHour-3)+2*pi/720*myMinute;
      angle_minuteHand  = angle_minuteScale*(myMinute-15);
      
      // draw minute scale
      radius = 98;
      for (uint8_t i=0; i<60; i++) {
        WatchyGSR::display.drawLine(100, 100, 100+radius*cos(angle_minuteScale*i), 100+radius*sin(angle_minuteScale*i), watchyGSR.BackColor());
      }
      WatchyGSR::display.fillCircle(100, 100, 93, watchyGSR.ForeColor());

      // draw hour scale
      radius = 98;
      for (uint8_t i=0; i<12; i++) {
        drawHand (radius, angle_hourScale*i);
      }
      WatchyGSR::display.fillCircle(100, 100, 88, watchyGSR.ForeColor());

      // draw hour hand
      //radius = 45;
      //drawHand (radius, angle_hourHand, watchyGSR.BackColor());

      // draw minute hand
      radius = 98;
      drawHand (radius, angle_minuteHand);

      // draw center point
      WatchyGSR::display.fillCircle(100, 100, 45, watchyGSR.ForeColor());

      // draw helping lines
      //WatchyGSR::display.drawCircle(100, 100, 45, watchyGSR.BackColor());
      //WatchyGSR::display.drawCircle(100, 100, 25, watchyGSR.BackColor());

      // positioning of hour WatchyGSR::display for DSEG7_Classic_Regular_39 font
      if (WatchTime.Local.Hour > 9 && WatchTime.Local.Hour < 20) {
        WatchyGSR::display.setCursor(58, 120);
      } else {
        WatchyGSR::display.setCursor(68, 120);
      }
      
      // WatchyGSR::display hour (with a leading zero, if necessary)
      if(WatchTime.Local.Hour < 10){
          WatchyGSR::display.print("0");
      }
      WatchyGSR::display.print(WatchTime.Local.Hour);

      // draw helping lines
      //WatchyGSR::display.drawLine(100, 0, 100, 200, watchyGSR.BackColor());
      //WatchyGSR::display.drawLine(0, 100, 200, 100, watchyGSR.BackColor());
  }

// helper function for hands drawing
  void drawHand(uint8_t _radius_, float _angle_) {
    float x = _radius_*cos(_angle_);
    float y = _radius_*sin(_angle_);
    WatchyGSR::display.drawLine(99, 99, 99+x, 99+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(99, 100, 99+x, 100+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(99, 101, 99+x, 101+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(100, 99, 100+x, 99+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(100, 100, 100+x, 100+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(100, 101, 100+x, 101+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(101, 99, 101+x, 99+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(101, 100, 101+x, 100+y, watchyGSR.BackColor());
    WatchyGSR::display.drawLine(101, 101, 101+x, 101+y, watchyGSR.BackColor());
  }
};

bool isWifiConnected = watchyGSR.WiFiStatus() == WL_CONNECTED;
