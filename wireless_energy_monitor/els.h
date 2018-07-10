int els_pin = 12;// czujnik
//int els_pin = 4;// symulacja
bool pool_int = 0; // wybor metody pomiaru impulsu 0 pooling; 1 interrupt

volatile long pulseCount;   //Used to measure global power.
volatile long pulseCountperiod;   //Used to measure power period
volatile unsigned long pulseTime, lastTime;
volatile double power, elapsedkWh, elapsedkWhperiod; //power and energy

int ppwh = 1; //1000 pulses/kwh = 1 pulse per wh
double mocmin = 20000;
double mocmax = 0;

void onPulse() // The interrupt routine pomiar impulsu przerwaniem
{
  if ((micros() - pulseTime) > 200000) {//filtr krótkich impulsów moc > 20 kW
    lastTime = pulseTime;
    pulseTime = micros();
    pulseCount++;
    pulseCountperiod++;
    power = (3600000000.0 / (pulseTime - lastTime)) / ppwh; //Calculate power
    elapsedkWh = (1.0 * pulseCount / (ppwh * 1000)); //count energy in kWh
    elapsedkWhperiod = (1.0 * pulseCountperiod / (ppwh * 1000)); //count energy in kWh
    Serial.print("In ");   Serial.print((pulseTime - lastTime)); Serial.print("  moc "); Serial.print(power, 2);  Serial.print(" W / energia ");  Serial.print(elapsedkWh, 3); Serial.println(" kWh");
  }
}

void set_p_i() {
  if (!pool_int) {
    detachInterrupt(els_pin);
  } else {
    attachInterrupt(els_pin, onPulse, FALLING);//RISING
  }
}

void elssetup() {
  Blynk.virtualWrite(V10, Tmoc );
  Blynk.setProperty(V0, "color", _YELLOW);
  Blynk.setProperty(V10, "color", _YELLOW);
  Blynk.setProperty(V15, "offLabel", "POOL");
  Blynk.setProperty(V15, "onLabel", "INT");
  pinMode(els_pin, INPUT);
  set_p_i();
}

void minmax() {
  if ( power > mocmax)mocmax = power;
  if ( power < mocmin)mocmin = power;
}

void sendUptime()
{
  minmax();
  Blynk.virtualWrite(11, power); // pin virtual
  Blynk.virtualWrite(12, elapsedkWh);
  Blynk.virtualWrite(13, pulseCount);
  Blynk.virtualWrite(14, elapsedkWhperiod);
  Blynk.virtualWrite(17, mocmax);
  Blynk.virtualWrite(18, mocmin);
}

byte stan_els = 0;
unsigned long time_els = 0;
void elsakcja() {  //pomiar impulsu w poolingu
  unsigned  long duration;
  if (!pool_int) {
    if (stan_els == digitalRead(els_pin)); else { //czy stan na pinie zmienil sie
      stan_els = digitalRead(els_pin);
      if (stan_els) {                             //czy stan wysoki
        if ((micros() - time_els) > 200000)       //eliminacja fałszywych impulsów
        {
          unsigned long   temp_time_els = micros(); //millis();
          duration = temp_time_els - time_els;   //dlugosc impulsu
          time_els = temp_time_els;  // zapamietanie czasu
          power = (3600000000.0 / ((duration)));  //przeliczenie na kWh
          pulseCount++;  //licznik impulsow
          elapsedkWh = (1.0 * pulseCount / (ppwh * 1000)); //count energy in kWh
          pulseCountperiod++;
          elapsedkWhperiod = (1.0 * pulseCountperiod / (ppwh * 1000)); //count energy in kWh
          Serial.print("P ");    Serial.print((duration)); Serial.print("  moc "); Serial.print(power);  Serial.print(" W / energia ");  Serial.print(elapsedkWh, 3); Serial.println(" kWh");
        }
      }
    }
  }
}



BLYNK_CONNECTED() {
  Blynk.syncVirtual(V13); //odtworzenie zapamiętanych wartości
  Blynk.syncVirtual(V14); //odtworzenie zapamiętanych wartości
}
BLYNK_WRITE(V13) // odtworzenie pulseCount
{
  double d = param.asDouble();
  pulseCount = d;
}
BLYNK_WRITE(V14) // odtworzenie pulseCountperiod
{
  double d = param.asDouble();
  pulseCountperiod = d;
}
BLYNK_WRITE(V15) // ustawienie pooling/ przerwanie
{
  pool_int = param.asInt();
  set_p_i();
}
BLYNK_WRITE(V16) // przycisk reset conter period
{
  pulseCountperiod = 0;
  mocmin = 20000;
  mocmax = 0;

}


