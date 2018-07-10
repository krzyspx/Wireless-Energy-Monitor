//https://community.openenergymonitor.org/t/wemos-d1-mini-esp8266-pulse-counting/4405


#define BLYNK_MSG_LIMIT 0
#include <EEPROM.h>
#include <Timers.h> //  my favorite timer 
Timers <7> akcja; //

#include "blynk.h" //obsĹ‚uga BLYNKa
#include "led.h"
#include "els.h" // 

void setup()
{
  ledsetup();
  if (procesor == D1MINI) {
    Serial.begin(115200);  delay(100);
    Serial.println();  Serial.println(F(__FILE__));  //
  }
  akcja.attach(0, 3000, blynk_timer); //
  akcja.attach(1, 1000, sendUptime); //
  akcja.attach(2, 1000, led_timer);

  blynksetup(); // setup biblioteki BLYNK
  elssetup();   //setup dla programu monitora

}
void loop()
{
  akcja.process(); // timer
  blynkakcja();    //BLYNK

  elsakcja(); // monitor energii
}









