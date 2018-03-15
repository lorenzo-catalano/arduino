#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define pin_A 6
#define pin_B 7
#define pin_button 2
int counter = 0; 
int aState;
int aLastState;  
unsigned long currentTime;
unsigned long loopTime;
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;
int count = 0;

unsigned int entries = 4;
String menu[] = {
  "Auto",
  "Rainbow",
  "Manual",
  "Off"
  };
char menuManual[] = {
  '.',
  'R', 
  'G',
  'B'
  };

 void setup() { 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  pinMode (pin_A,INPUT);
  pinMode (pin_B,INPUT);
  Serial.begin (9600);
  currentTime = millis();
  loopTime = currentTime; 
  pinMode(pin_button, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(pin_button), click, RISING);
 } 
 
int selected = 0;
int selectedColor=0;
boolean editing=false;

int menutype=0;
int current = 0;
int redraw = 1;
int start = 0;
int window = 4;
int r=30,g=70,b=200;

void click() {
  boolean menuchanged = false;
  switch(menutype){
    case 0:
      if(current==2){
        menutype = 1;
        menuchanged=true;
        }
      if(menuchanged){
        current=0;
        selected=0;
        editing=false;
      }else{
        selected=current;
      }
      break;
    case 1:
      if(current==0){
        menutype = 0;
        menuchanged=true;
        }
      if(menuchanged){
        current=0;
        selected=0;
        editing=false;
      }else{
        selected=current;
        editing=!editing;
        if(editing){
          selectedColor=current;
        }else{
          selectedColor=0;
        }
        
      }
      break;
  }
  redraw=1;
}

int barwidth= display.width()-18;
int maxc = 255;
void loop()  {

 

  currentTime = millis();
  if(currentTime >= (loopTime)){
    if (digitalRead(pin_button) != HIGH) {
      click();
      delay(200);
    }
    loopTime = currentTime+200;
  }
    
  encoder_A = digitalRead(pin_A);    // Read encoder pins
  encoder_B = digitalRead(pin_B);
  if((!encoder_A) && (encoder_A_prev)){
    
    if(encoder_B) {
      if(editing){
        switch(selectedColor){
          case 1:
            if(r>0)r-=5;
            break;
          case 2:
            if(g>0)g-=5;
            break;
          case 3:
            if(b>0)b-=5;
            break;
          default:break;
          }
      }else{
        if(current>0)current--;
      }
    }else{
      if(editing){
        switch(selectedColor){
          case 1:
            if(r<maxc)r+=5;
            break;
          case 2:
            if(g<maxc)g+=5;
            break;
          case 3:
            if(b<maxc)b+=5;
            break;
          default:break;
          }
      }else{
        if(current<entries-1)current++;
      }
      
    }
    start=0;
    redraw=1;
  }
  encoder_A_prev = encoder_A;
    
  
  if(redraw){
    display.clearDisplay();
    
    if(menutype==1){
      display.setCursor(0,0);
      
      for(int i=start;i<(start+window);i++){
        if(current == i){
          display.print(">");
        }else{
          display.print(" ");
        }
        display.print(menuManual[i]);
        if(i>0 && selectedColor==i)display.print("*");
          
        int value=0;
        switch(menuManual[i]){
          case 'R':
            value = map(r, 0, 255, 0, barwidth);
            display.fillRect(18,9+8*(i-1), value, 6, WHITE);
            display.drawRect(18,9+8*(i-1), barwidth, 6, WHITE);
            break;
          case 'G':
            value = map(g, 0, 255, 0, barwidth);
            display.fillRect(18,9+8*(i-1), value, 6, WHITE);
            display.drawRect(18,9+8*(i-1), barwidth, 6, WHITE);
            break;
          case 'B':
            value = map(b, 0, 255, 0, barwidth);
            display.fillRect(18,9+8*(i-1), value, 6, WHITE);
            display.drawRect(18,9+8*(i-1), barwidth, 6, WHITE);
            break;
        }
        display.println("");
      }
      display.setCursor(63,0);
      display.print(r);
      display.print(" ");
      display.print(g);
      display.print(" ");
      display.print(b);
      display.display();       
    }else{
      display.setCursor(0,0);
      for(int i=start;i<(start+window);i++){
        if(current == i){
          display.print(">");
        }else{
          display.print(" ");
        }
        display.println(menu[i]);
      }       
      display.display(); 
    }
    
    redraw = 0;
  }
}
