/*

*/

#define led_czerw   0
int Tmoc = 10;

void ledsetup() {
  pinMode(led_czerw, OUTPUT); //LED do symulacji LEDa licznika energii
  digitalWrite(led_czerw, HIGH);
}

BLYNK_WRITE(V10) {
  Tmoc = param.asInt();

}

int flaga_led = 0;
void led_timer()
{
  flaga_led = !flaga_led;
  if (flaga_led)  akcja.updateInterval(2, (Tmoc * 100)); else akcja.updateInterval(2, 50);
  digitalWrite(led_czerw, flaga_led);
}

