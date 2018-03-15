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
  attachInterrupt(digitalPinToInterrupt(pin_button), select, RISING);
 } 
int selected = 2;
int selectedColor=0;

int menutype=0;
int current = 0;
int redraw = 1;
int start = 0;
int window = 4;
int r=30,g=70,b=200;

void select() {
  int oldmenutype=menutype;
  selected=current;
  switch(menutype){
    case 0:      
      if(selected==2)menutype = 1;
      break;
    case 1:
      selectedColor=current;
      if(selected==0)menutype = 0;
      break;
  }
  if(menutype!=oldmenutype){current=0;selected=0;};
  redraw=1;
}
int barwidth= display.width()-40;

void loop()  {

  currentTime = millis();
  if(currentTime >= (loopTime)){
    
    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B); 
    if((!encoder_A) && (encoder_A_prev)){
      if(encoder_B) {
        if(current>0)current--;
//        if(current>(window-1)){
//          start = (window-1);
//        }else if(current>0){
//          start = current-1;
//        }else{
//          start=0;
//        }
      }else{
        if(current<entries-1)current++;
//        if(current>2 && current<(entries-1)){
//          start = current-2;
//        }else if(current>=(entries-1)){
//          start = current-(window-1);
//        }else{
//          start=0;
//        }
      }
      start=0;
      redraw=1;
    }
    encoder_A_prev = encoder_A;
    loopTime = currentTime;
  }
  if(redraw){
    display.clearDisplay();
    display.setCursor(64,0);
    display.print(menutype);
    display.print(" ");
    display.print(selected);
    display.print(" ");
    display.print(current);
    if(menutype==1){
      display.setCursor(0,0);
      for(int i=start;i<(start+window);i++){
        if(current == i){
          display.print(">");
        }else{
          display.print(" ");
        }
        display.print(menuManual[i]);
        if(selectedColor==i)display.print("*");
          
        int value=0;
        switch(menuManual[i]){
          case 'R':
            value = map(r, 0, 255, 0, barwidth);
            display.fillRect(20,9+8*(i-1), value, 6, WHITE);
            display.drawRect(20,9+8*(i-1), display.width()-20, 6, WHITE);
            break;
          case 'G':
            value = map(g, 0, 255, 0, barwidth);
            display.fillRect(20,9+8*(i-1), value, 6, WHITE);
            display.drawRect(20,9+8*(i-1), display.width()-20, 6, WHITE);
            break;
          case 'B':
            value = map(b, 0, 255, 0, barwidth);
            display.fillRect(20,9+8*(i-1), value, 6, WHITE);
            display.drawRect(20,9+8*(i-1), display.width()-20, 6, WHITE);
            break;
        }
        display.println("");
      }
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
