#include <math.h>
#include <FastLED.h>

//mic variables
const char mic_data_input = A0;
char prevState = 0;

//led variables
const char led_data_output = 6;
const char num_leds = 60;
CRGB leds[num_leds];

//math definitions
#define log10led log10(pow(512, 1.0f/(num_leds+1)))


void setup(){
    pinMode(mic_data_input, INPUT);
    FastLED.addLeds<NEOPIXEL, led_data_output>(leds, num_leds);
}

void loop(){
    int maxval = 512;
    for(u8 i = 0; i < 10; i++){
        int state = analogRead(mic_data_input);
        if(state > maxval) 
            maxval = state;
        delay(1);
    }
    maxval = log10(maxval - 511)/log10led;
    if(maxval != prevState){
        if(maxval > prevState){
            for(char i = prevState; i < maxval; i++)
            {
              leds[i] = CRGB::Blue;
              FastLED.show();  
            }
        } else if(maxval < prevState){
            for(char i = prevState-1; i >= maxval; i--)
            {
              leds[i] = CRGB::Black;
              FastLED.show();  
            }
        }
        prevState = maxval;
    }
}
