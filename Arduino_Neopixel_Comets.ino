/*
 * Erstellt von Tobias Mickisch
 * Erstellt am 11.07.2026
 *
 * Generiert auf einem RGB-Streifen (Neopixel) 15 Kometen mit zufälliger: Geschwindigkeit, Richtung und Schweiflänge. Der jeweilige Farbton ist fix.  
 * Auf einem längeren LED-Streifen sieht das ganze fast wie ein römisches Licht zu Silvester aus, meiner Meinung nach sehr schön.
*/
#define Interval_LED_Change_Min 1    // 10 Gibt an, wie niedrig der Delay zwischen dem Wechsel von der einen zu nächsten LED sein darf.   
#define Interval_LED_Change_Max 20   // 40 Gibt an, wie hoch der Delay zwischen dem Wechsel von der einen zu nächsten LED sein darf.
#define TailMin 4
#define TailMax 10
#define BRIGHTNESS 100 //0-255
#define PIN 6
#define NUM_LEDS 300
#define NUMBER_COMETS 15

#include <Adafruit_NeoPixel.h>

struct Comet {
    unsigned long previousMillis = 0;
    uint8_t interval = 0;
    bool newRnd = true;
    uint8_t dir = 0;
    uint16_t cnt = 0;
    uint8_t tail = 0;
};

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

const Color colors[] = {
    {255,   0,   0},
    {255, 120,   0},
    {255, 255,   0},
    {120, 255,   0},
    {  0, 255,   0},
    {  0, 255, 120},
    {  0, 255, 255},
    {  0, 120, 255},
    {  0,   0, 255},
    {120,   0, 255},
    {255,   0, 255},
    {255,   0, 120},
    {255,  80, 180},
    { 80, 180, 255},
    {255, 255, 255}
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);
bool StripShow = false;
Comet comets[NUMBER_COMETS];
const uint8_t NUMBER_COLORS = sizeof(colors) / sizeof(colors[0]);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));
  strip.setBrightness(BRIGHTNESS);
}

void loop() {
    unsigned long currentMillis = millis();
    
    for (int i = 0; i < NUMBER_COMETS; i++) {
        if (currentMillis - comets[i].previousMillis >= comets[i].interval) {
            comets[i].previousMillis = currentMillis;

            const Color& color = colors[i % NUMBER_COLORS];
            comet(&comets[i].newRnd,&comets[i].dir, &comets[i].cnt, &comets[i].interval,color.red,color.green,color.blue,&comets[i].tail);
        }
    }
    if(StripShow == true) {strip.show(); StripShow = false;} 
}


void comet(bool* newRnd, uint8_t* dir, int16_t* cnt, uint8_t* interval, uint8_t red, uint8_t green, uint8_t blue, uint8_t* tail) {

    uint8_t increment = 1;
         if (*interval>=10) {increment = 1;}
    else if (*interval>=5)  {increment = 2;}
    else if (*interval>=1)  {increment = 4;}

    if(*newRnd == true){ 
        *dir = random(1,100);
        *interval = random(Interval_LED_Change_Min,Interval_LED_Change_Max);
        *tail = random(TailMin,TailMax);
        if(*dir<=50) {
            *cnt = NUM_LEDS-1;
        } else {
            *cnt = 0;
        }
        *newRnd = false;
    }
            
    if(*dir<=50){          
        int d = 1;
        for(int i=0; i<*tail; i++){
          strip.setPixelColor(*cnt+i, green/d, red/d, blue/d);
          d*=2;
        }
        for(int i=0; i<increment; i++){
          strip.setPixelColor(*cnt+*tail+i, 0, 0, 0);
        }
       
        (*cnt) -= increment;
        if(*cnt<-*tail) {*newRnd = true;}

    } else {
        int d = 1;
        for(int i=0; i<*tail; i++){
          strip.setPixelColor(*cnt-i, green/d, red/d, blue/d);
          d*=2;
        }
        for(int i=0; i<increment; i++){
          strip.setPixelColor(*cnt-*tail-i, 0, 0, 0);
        }

        (*cnt) += increment;
        if(*cnt>NUM_LEDS+*tail) {*newRnd = true;}
    }

  StripShow = true;
}