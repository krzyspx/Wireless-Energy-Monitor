/*

*/
#define _RED        "#FF0000"
#define _GREEN      "#00FF00"
#define _MAGENTA    "#FF00FF"
#define _YELLOW     "#FFFF00"
#define _BLACK      "#000000"
#define _WHITE      "#FFFFFF"
#define _WBLUE      "#00FFFF"
#define _PINK       "#FFB3B3"
#define _OLIVE      "#808000"
#define _BRAU       "#FF8000"
#define _karmin     "#FF0080"
#define _SEE        "#00FF80"
#define _fiolet     "#8000FF"
#define _BLUE       "#0080FF"

#define ESP01 1
#define D1MINI 2
int procesor = D1MINI;
#define led_wew   2
char ssid[] = "111111111111";
char pass[] = "22222222222222222";
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "45ec36xxxxxxxxxxxxxxxx9970b96ca"; 

#define treconnect 60 // czas testu polaczenia z serwerem BLYNK w sek
int liczreconnect = 10; // licnik czasu testĂłw Ĺ‚Ä…cznosci
int printon = 0; // flaga wysyĹ‚ania danych na monitor
int terminalon = 0; // flaga wysyĹ‚ania danych na terminal BLYNK

WidgetLED led1(V0); // led w aplikacji sygnalizujacy polaczenie mikroprocesor - aplikacja
WidgetTerminal terminal(V1);


void blynksetup() //nie blokujaca procedura polaczenia z BLYNK
{

  pinMode(led_wew, OUTPUT); //LED na module
  digitalWrite(led_wew, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  Blynk.connect();
  if (Blynk.connected()) Blynk.run();  else  Blynk.connect();
  if (Blynk.connected()) Serial.println("poloczenie OK");  else  Serial.println("poloczenie brak");
  Blynk.virtualWrite(V5, 0);
  Blynk.virtualWrite(V6, 0);
}

void blynkakcja()
{
  if (Blynk.connected()) Blynk.run();
}

int wskrunAr = 0;
void vLED_timer()//miganie vLED dla kontroli polaczenia
{
  wskrunAr = !wskrunAr ;
  if (wskrunAr == 0)led1.off(); else led1.on();
}

void testconnect() // test lacznosci z serwerem BLYNK bez blokowania programu glownego GS
{
  vLED_timer();
  if (Blynk.connected()) {
    digitalWrite(led_wew, HIGH); //wskaznik lacznosci z serwerem stan wysoki- wylaczenie LEDa - polaczenie OK
    if (procesor == D1MINI)  {
      Serial.println("connection OK");
    }
    liczreconnect = treconnect;
  } else {
    digitalWrite(led_wew, LOW);
    liczreconnect--;
    if (procesor == D1MINI)  Serial.println("connection DOWN");
    if (liczreconnect == 0) {
      Blynk.connect();
      liczreconnect = treconnect;
    }
  }
}

void blynk_timer() { //rozne rzeczy wykonywane cyklicznie
  testconnect(); // test lacznosci z serwerem
}

BLYNK_WRITE(V6) {
  terminalon = param.asInt();
}


