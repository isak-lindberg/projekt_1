//Tid att gå upp ur sängen: 10 min, tid att äta frukost: 20 min, tid att borsta tänderna och göra mig i ordning: 15 min, tid att gå till vagnen: 15 min, tid vagnen tar på sig och tiden att gå från kapell: 25 minuter
//Tid från att gå upp till vagnen: 60 min, tid från gå på vagnen till skola 30 min

//Jag inkluderar alla olika libraries jag kan tänka mig behöva för projektet

#include <RtcDS3231.h>
#include <Wire.h>
#include "U8glib.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//Jag definierar vilken pin min NeoPixel ska vara kopplad till, samt hur många lampor NeoPixeln har

#define LED_PIN 6
const byte neoPixels = 24;

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);

//Jag kopplar min RTC-Modul definierar min NeoPixel

RtcDS3231<TwoWire> rtcModule(Wire);
Adafruit_NeoPixel strip(neoPixels, LED_PIN, NEO_GRB + NEO_KHZ800);

//Jag definierar resten av mina variabler, days hours minutes och seconds ska som det låter bevara tiden från RTC-Modulen, vagn är hur många minuter tills vagnen går, och ljustid ska bli hur många lampor som lyser

byte days;
byte hours;
byte minutes;
byte seconds;

int vagn;
int ljustid;

//Min setup, jag kör igång serial monitoren, börjar min RTC klocka och tar in tiden.

void setup() {
  Serial.begin(9600);
  rtcModule.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  rtcModule.SetDateTime(compiled);

//Setup för min NeoPixel

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  strip.begin();
  strip.show();
  strip.setBrightness(10);
}

//I min loop kallar jag på mina tre funktioner
//INPUT: Ingen
//OUTPUT: Inget

void loop() {
  updateOled();
  updateRtc();
  updateNeopixel();
}

//Funktionen för min NeoPixel. Den mappar ut sekunder till variabeln ljustid. Beroende på värdet på variabeln vagn som jag får ur min Oled-displays funktion updateOled() visar min NeoPixel rött, gult eller rött, och räknar sedan upp mot minuten med fler och fler ljus tills den går runt till 0 där den går ner igen
//INPUT: Inget
//OUTPUT: Inget

void updateNeopixel(void) {


  ljustid = map(seconds, 0, 60, 0, 24);
 

  if (vagn >= 90) {
    strip.setPixelColor(ljustid, strip.Color(0, 255, 0));
    strip.show();
    
    if (seconds > 58) {
    strip.clear();
    }
  }
    else if (90 > vagn >= 75) {
    strip.setPixelColor(ljustid, strip.Color(0, 128, 128));
    strip.show();
    
    if (seconds > 58) {
    strip.clear();
    }
    }
    else {
    strip.setPixelColor(ljustid, strip.Color(255, 0, 0));
    strip.show();
    
    if (seconds > 58) {
    strip.clear();
    }
    }
}

//Funktionen för min RTC-Modul, den börjar med att använda inbyggda funktionen rtcModule.GetDateTime(), för att kolla vad klockan är. Den lagrar sedan dessa värden i variablerna jag skapade tidigare. Den printar även som ett test värdena på tiden i serial monitoren
//INPUT: Inget
//OUTPUT: Tid i timmar, minuter, sekunder och dagar

void updateRtc(void) {
  RtcDateTime now = rtcModule.GetDateTime();

  days = now.DayOfWeek();
  hours = now.Hour();
  minutes = now.Minute();
  seconds = now.Second();
  Serial.println(String(hours) + ": " + String(minutes) + ": " + String(seconds) + ", " + String(days) + "d");
}

//Funktionen för min OLED-Display. Den börjar med att välja font, och gör sedan lite beräkningar genom villkorstest beroende på veckodag för att ta reda på vilken minut av dagen vagnen går. Värdena är tagna från västtrafiks tidtabell för 10 vagnen och ger ett värde på variabeln vagn.
//INPUT: Inget
//OUTPUT: Värdet på variabeln vagn

void updateOled(void) {
  oled.setFont(u8g_font_helvB10);
  oled.firstPage();

  if (days == 1) {
    vagn = 718 - 60 * hours - minutes;
    do {
      oled.drawStr(0, 30, String(vagn).c_str());
      oled.drawStr(25, 30, "m till vagnen");
    } while (oled.nextPage());
  }
  else if (days == 2) {
    vagn = 460 - 60 * hours - minutes;
    do {
      oled.drawStr(0, 30, String(vagn).c_str());
      oled.drawStr(25, 30, "m till vagnen");
    } while (oled.nextPage());
  }
  else if (days == 3) {
    vagn = 469 - 60 * hours - minutes;
    do {
      oled.drawStr(0, 30, String(vagn).c_str());
      oled.drawStr(25, 30, "m till vagnen");
    } while (oled.nextPage());
  }
  else if (days == 4) {
    vagn = 538 - 60 * hours - minutes;
    do {
      oled.drawStr(0, 30, String(vagn).c_str());
      oled.drawStr(25, 30, "m till vagnen");
    } while (oled.nextPage());
  }
  else if (days == 5) {
    vagn = 548 - 60 * hours - minutes;
    do {
      oled.drawStr(0, 30, String(vagn).c_str());
      oled.drawStr(25, 30, "m till vagnen");
    } while (oled.nextPage());
  }
  Serial.println(String(vagn));

}
