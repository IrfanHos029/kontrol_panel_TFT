#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "DHT.h"
#include <TouchScreen.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#define PIN 10
#define DHTTYPE DHT11
#define MINPRESSURE 300
#define MAXPRESSURE 1000
#define lampu_1 13
#define lampu_2 12
#define lampu_3 11

uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin
const int TS_LEFT = 133, TS_RT = 915, TS_TOP = 942, TS_BOT = 162;

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint p;
Adafruit_GFX_Button butt_1, butt_2, butt_3, butt_suhu;
Adafruit_GFX_Button butt_back;
DHT sensor(PIN, DHTTYPE);

int pixel_x, pixel_y;         //Touch_getXY() updates global vars

int margin = 5;
int btnWidth = 100;
int btnHeight = 40;

#define BLACK     0x0000
#define GREY      0x5555
#define BLUE      0x001F
#define RED         0xF800
#define GREEN     0x07E0
#define DARKGREEN     0x05C0
#define CYAN      0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define GOLD      0xDDC0
#define WHITE     0xFFFF

#define XGRAPH 50
#define YGRAPH 45
#define HGRAPH 135
#define WGRAPH 21 //LEBAR GRAPH HARUS GAN
int newHeight;
int oldHeight = 0;
int heightDiff;

bool stateButt1;
bool stateButt2;
bool stateButt3;
enum pageId{
  MENU,
  SENSOR
};
int page = MENU,oldPage = -1;

void setup(void) {
   Serial.begin(9600);
  pinMode(lampu_1,OUTPUT);
  pinMode(lampu_2,OUTPUT);
  pinMode(lampu_3,OUTPUT);
  sensor.begin();
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
 
  page = MENU;

}
bool pres;
void loop(void) {

     switch(page)
     {
      case MENU :
        if(page != oldPage)  showScreen(); 
        butt_1.press(pres && butt_1.contains(pixel_x, pixel_y));
        butt_2.press(pres && butt_2.contains(pixel_x, pixel_y));
        butt_3.press(pres && butt_3.contains(pixel_x, pixel_y));
        butt_suhu.press(pres && butt_suhu.contains(pixel_x, pixel_y));
        if(butt_1.justReleased()){ butt_1.drawButton(); };
        if(butt_2.justReleased()){ butt_2.drawButton(); };
        if(butt_3.justReleased()){ butt_3.drawButton(); };
         if(butt_suhu.justReleased()){ butt_suhu.drawButton(); };
  
        if(butt_1.justPressed()){ butt_1.drawButton(false); stateButt1 = !stateButt1; };
        if(butt_2.justPressed()){ butt_2.drawButton(false); stateButt2 = !stateButt2; };
        if(butt_3.justPressed()){ butt_3.drawButton(false); stateButt3 = !stateButt3; };
        if(butt_suhu.justPressed()){ butt_suhu.drawButton(false); tft.fillScreen(BLACK); page = SENSOR; };

        if(stateButt1 && page == MENU){  tft.fillCircle(250,100,15,GREEN); }
        else          {  tft.fillCircle(250,100,15,RED); }
      
        if(stateButt2 && page == MENU){  tft.fillCircle(250,150,15,GREEN); }
        else          {  tft.fillCircle(250,150,15,RED); }
      
        if(stateButt3 && page == MENU){  tft.fillCircle(250,200,15,GREEN); }
        else          {  tft.fillCircle(250,200,15,RED); }
      
        break;


     case SENSOR :
       if(page != oldPage)  showSensor(); 
       butt_back.press(pres && butt_back.contains(pixel_x, pixel_y));
       if(butt_back.justReleased()){ butt_back.drawButton(); };
       if(butt_back.justPressed()){ butt_back.drawButton(false); tft.fillScreen(BLACK); page = MENU; };
       break;
     }
 
Serial.println(String() + "page:" + page);
if (oldPage == page){ pres = Touch_getXY();     }
else{ pres=false; }

if(stateButt1){delay(100); digitalWrite(lampu_1,HIGH); } else { digitalWrite(lampu_1,LOW); }
if(stateButt1){delay(100); digitalWrite(lampu_2,HIGH); } else { digitalWrite(lampu_2,LOW); }
if(stateButt1){delay(100); digitalWrite(lampu_3,HIGH); } else { digitalWrite(lampu_3,LOW); }
  
}

void showScreen()
{ tft.fillScreen(BLACK);
   tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.setCursor(0, 10);
  tft.print("PANEL CONTROLL");
  tft.fillRect(0,50,tft.width(),5,RED);

   butt_1.initButton(&tft,  100 , 100, 2 * btnWidth, btnHeight, WHITE, GOLD, BLACK, " TL ", 2);
   butt_2.initButton(&tft,  100 , 150, 2 * btnWidth, btnHeight, WHITE, GOLD, BLACK, "GUDANG", 2);
   butt_3.initButton(&tft,  100 , 200, 2 * btnWidth, btnHeight, WHITE, GOLD, BLACK, " KERJA ", 2);
   butt_suhu.initButton(&tft,  240 , 20, 100, btnHeight, WHITE, WHITE, RED, "SUHU", 2);
   butt_1.drawButton(false);
   butt_2.drawButton(false);
   butt_3.drawButton(false);
   butt_suhu.drawButton(false);
   oldPage = page;
}

void showSensor()
{ 
float suhu = sensor.readTemperature();
tft.fillScreen(BLACK);
tft.setRotation(1);
tft.fillCircle(XGRAPH, YGRAPH, (WGRAPH/2), BLACK);
tft.fillRect((XGRAPH-(WGRAPH/2)), YGRAPH, WGRAPH, HGRAPH, BLACK);
tft.fillCircle(XGRAPH, (HGRAPH+YGRAPH), ((WGRAPH/2)+10), BLACK);
tft.fillCircle(XGRAPH, YGRAPH, (WGRAPH/2)-3, WHITE);
tft.fillRect((XGRAPH-(WGRAPH/2)+3), YGRAPH, (WGRAPH-6), HGRAPH, WHITE);
tft.fillCircle(XGRAPH, (HGRAPH+YGRAPH), ((WGRAPH/2)+7), WHITE);
tft.fillCircle((XGRAPH+WGRAPH), YGRAPH, 3, BLACK);
tft.setTextColor(BLACK);
tft.setTextSize(2);
tft.setCursor(((XGRAPH+WGRAPH)+8), (YGRAPH-5));
tft.print("100");
tft.setTextSize(1);
tft.print("0");
tft.setTextSize(2);
tft.print("C");
tft.fillCircle((XGRAPH+WGRAPH), ((YGRAPH+(HGRAPH/2))-(WGRAPH/2)), 3, BLACK);
tft.setTextColor(BLACK);
tft.setTextSize(2);
tft.setCursor(((XGRAPH+WGRAPH)+8), (((YGRAPH+(HGRAPH/2))-(WGRAPH/2)))-5);
tft.print("50");
tft.setTextSize(1);
tft.print("0");
tft.setTextSize(2);
tft.print("C");
tft.fillCircle((XGRAPH+WGRAPH), ((YGRAPH+HGRAPH)-((WGRAPH/2)+7)), 3, BLACK);
tft.setTextColor(BLACK);
tft.setTextSize(2);
tft.setCursor(((XGRAPH+WGRAPH)+8), (((YGRAPH+HGRAPH)-((WGRAPH/2)+7))-5));
tft.print("0");
tft.setTextSize(1);
tft.print("0");
tft.setTextSize(2);
tft.print("C");
//tft.fillCircle(175, 225, 26, RED);
tft.fillRect((XGRAPH-(WGRAPH/2)+5), YGRAPH, (WGRAPH-10), HGRAPH, RED);
tft.fillCircle(XGRAPH, (HGRAPH+YGRAPH), ((WGRAPH/2)+5), RED);
tft.setCursor(100, 100);
tft.setTextColor(RED);
if(isnan(suhu))
{
    tft.setTextSize(4);
    tft.print("error");
    suhu = 0;
}
else
{
    tft.setTextSize(4);
    tft.print(suhu);
    tft.setTextSize(2);
    tft.print("0");
    tft.setTextSize(4);
    tft.print("C");
}
tft.fillRect(130, 255, 80, 4, RED);
if(suhu <= 100){
newHeight = map(suhu,0, 100, 0, (HGRAPH-(WGRAPH/2)));
heightDiff = oldHeight-newHeight; // only draw new part of bar graph for faster display
if (oldHeight != newHeight) tft.fillRect((XGRAPH-(WGRAPH/2)+5), YGRAPH+((HGRAPH-suhu)-((WGRAPH/2)+7)), (WGRAPH-10), HGRAPH-((HGRAPH-suhu)-((WGRAPH/2)+7)), RED);
tft.fillRect((XGRAPH-(WGRAPH/2)+5), YGRAPH, (WGRAPH-10), ((HGRAPH-suhu)-((WGRAPH/2)+7)), WHITE);
if (suhu==100)tft.fillCircle(XGRAPH, YGRAPH, (WGRAPH/2)-5, RED);
oldHeight=newHeight; // remember how high bar is
}
tft.fillCircle(XGRAPH, (HGRAPH+YGRAPH), ((WGRAPH/2)+5), RED);
  
   butt_back.initButton(&tft,  240 , 200,btnWidth, btnHeight, WHITE, WHITE, RED, "BACK", 2);
   butt_back.drawButton(false);
   oldPage = page;
  
}

bool Touch_getXY(void)
{
  p = ts.getPoint();
  pinMode(YP, OUTPUT);          //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
      if (tft.width() <= tft.height()) { //Portrait
          pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
          pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
      } else {
          pixel_x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());
          pixel_y = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
      }
  }
  return pressed;
}
