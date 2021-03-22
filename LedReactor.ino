#include <math.h>
#include <FastLED.h>

const char micpin = A0;
char prevState = 0;

//costanti e variabili relative alla striscia
const char ledpin = 6;
const char num_leds = 60;
CRGB leds[num_leds];

/*Definendo il logaritmo base 10 della base che verrà usata
 *per calcolare il logaritmo del valore preso dal microfono successivamente*/
const int log10led = log10(pow(512, 1.0f/(num_leds)));

//Inizializzando Striscia e Microfono
void setup(){
    pinMode(micpin, INPUT);
    FastLED.addLeds<NEOPIXEL, ledpin>(leds, num_leds);
}

void loop(){
    //Prendendo il valore più alto in 10 letture del microfono
    //Impostiamo 512 come valore perché il microfono ritorna un'onda
    int maxval = 512;
    for(u8 i = 0; i < 10; i++){
        int state = analogRead(micpin);
        if(state > maxval)
            maxval = state;
        delay(1);
    }
    //Se il numero di led accesi (in prevState) è diverso da quello ottenuto, accendi i rimanenti o spegni quelli in più
    if(maxval != prevState){
      prevState = maxval;

      /* Cambiando la base del logaritmo dividendo
      * il logaritmo base 10 del valore letto dal microfono meno 511 per spostare la scala tra 1 e 512
      * per il logaritmo base 10 della nuova base
      * Otteniamo quindi il numero di led che dovrebbero essere accesi*/
      maxval = log10(maxval - 511)/log10led;
      /* Facciamo questo perché il nostro senso dell'udito funziona in modo logaritmico
      * usare map sui valori (lineari) ottenuti dal microfono per ottenere i led da accendere
      * ci da un risultato che non ci sembra valido alla vista
      * Quindi trovando il logaritmo del valore lineare usando come base del logaritmo il numero di led + 1 per dare più probabilità agli ultimi led di accendersi
      * altrimenti si accenderebbe solo quando il microfono viene "saturato"*/

      if(maxval > prevState){
          for(char i = prevState; i < maxval; i++){
            //In base al led che stiamo accendendo, scegliamo il suo colore
            if(i < 10) leds[i].setRGB(255,0,0);
            else if(i < 20) leds[i].setRGB(255,0,128);
            else if(i < 30) leds[i].setRGB(255,0,204);
            else if(i < 40) leds[i].setRGB(226,0,255);
            else if(i < 50) leds[i].setRGB(183,0,255);
            else if(i < 60) leds[i].setRGB(0,255,243);
            FastLED.show();
          }
      } else if(maxval < prevState){
          for(char i = prevState-1; i >= maxval; i--){
            leds[i] = 0;
            FastLED.show();
          }
      }
    }
}
