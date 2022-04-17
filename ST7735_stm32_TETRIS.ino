/*
  Disable other fonts.

  Make sure all the display driver and pin connections are correct by
  editing the User_Setup.h file in the TFT_eSPI library folder:

  ######################################################################### | Generic STM32F1 series,   | User_Setup.h:                                | TFT_CS   PA2
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ###### | maple dfu bootloader 2.0, | TFT_RGB_ORDER TFT_BGR, ST7735_GREENTAB160x80 | TFT_DC   PA1  +PA5,7
  ######################################################################### | usb CDC usart!!!          | #define STM32, #define ST7735_DRIVER         | TFT_RST  PA0
*////           (c)Airrr(r)          Apr.2022          v:1         17.04.22 |

#include <TFT_eSPI.h>          //Hardware-specific library
#include <SPI.h>
#include <PS2MouseHandler.h>   //Thanks to https://github.com/getis/Arduino-PS2-Mouse-Handler
#include "nextPic.h"
#include "levelPic.h"
#include "scorePic.h"

//Hardware (possible to choose mouse OR keys):

#define MOUSE_DATA PC15        //PS2 data
#define MOUSE_CLOCK PC14       //PS2 clock

#define mainButton PB9         //Key pause     All keys to ground.
#define leftButton PB8         //Key left
#define rightButton PB7        //Key right
#define rotButton PB6          //Key rotate
#define downButton PB5         //Key down

#define analogSeedPin PA4      //Leave unconnected.
#define soundPin PB4           //Sound. Optional. Piezo to ground.


TFT_eSPI tft = TFT_eSPI();
PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE);

int x = 40, y = 80;        //Mouse coords

const byte w = 11;
const byte h = 31;
word Pole[w][h];
int myX = 4, myY = 0;      //Starting coords
byte fig, figNext = 0;     //
int mSpeed = 0;            //Mouse speed  0-disable acceleration
byte figure[4][4];         //Current figure array
byte gameSpeed = 1;        //Speed ah? 1-9 -> game leveling
byte fallAcc = 10;         //Accelerated drop. on MButton or Keydown. lower-faster
double vremya = 0;         //Nado tak
double debncTime = 0;      //Mezhdu knopkami
byte rotation = 0;         //
bool novi = true;          //New game?
word score = 0;            //
bool mish = false;         //Mouseless mode
word autoRep = 200;        //Keys auto repeat delay
word levelUpLines = 10;    //Switch to next level. =< 999!!!


byte i[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}; //lame
byte j[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
byte l[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}};
byte p[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte s[4][4] = {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte t[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};
byte z[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}};

byte i01[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
byte j01[4][4] = {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
byte l01[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}};
byte p01[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte s01[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
byte t01[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};
byte z01[4][4] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};

byte i02[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}};
byte j02[4][4] = {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
byte l02[4][4] = {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
byte p02[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte s02[4][4] = {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte t02[4][4] = {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
byte z02[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}};

byte i03[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
byte j03[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte l03[4][4] = {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
byte p03[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
byte s03[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
byte t03[4][4] = {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
byte z03[4][4] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};

struct Bloki
{
  byte *block;
  word col;     //0-65535 FFFF
};

Bloki allBlocks[] =
{
  {*i, 0x07FF}, //TFT_CYAN        0x07FF
  {*j, 0x001F}, //TFT_BLUE        0x001F
  {*l, 0x915C}, //TFT_VIOLET      0x915C
  {*p, 0xFEA0}, //TFT_GOLD        0xFEA0
  {*s, 0x07E0}, //TFT_GREEN       0x07E0
  {*t, 0xF81F}, //TFT_MAGENTA     0xF81F
  {*z, 0xF800}, //TFT_RED         0xF800
};
Bloki allBlocks1[] =
{
  {*i01, 0x07FF}, //TFT_CYAN        0x07FF
  {*j01, 0x001F}, //TFT_BLUE        0x001F
  {*l01, 0x915C}, //TFT_VIOLET      0x915C
  {*p01, 0xFEA0}, //TFT_GOLD        0xFEA0
  {*s01, 0x07E0}, //TFT_GREEN       0x07E0
  {*t01, 0xF81F}, //TFT_MAGENTA     0xF81F
  {*z01, 0xF800}, //TFT_RED         0xF800
};
Bloki allBlocks2[] =
{
  {*i02, 0x07FF}, //TFT_CYAN        0x07FF
  {*j02, 0x001F}, //TFT_BLUE        0x001F
  {*l02, 0x915C}, //TFT_VIOLET      0x915C
  {*p02, 0xFEA0}, //TFT_GOLD        0xFEA0
  {*s02, 0x07E0}, //TFT_GREEN       0x07E0
  {*t02, 0xF81F}, //TFT_MAGENTA     0xF81F
  {*z02, 0xF800}, //TFT_RED         0xF800
};
Bloki allBlocks3[] =
{
  {*i03, 0x07FF}, //TFT_CYAN        0x07FF
  {*j03, 0x001F}, //TFT_BLUE        0x001F
  {*l03, 0x915C}, //TFT_VIOLET      0x915C
  {*p03, 0xFEA0}, //TFT_GOLD        0xFEA0
  {*s03, 0x07E0}, //TFT_GREEN       0x07E0
  {*t03, 0xF81F}, //TFT_MAGENTA     0xF81F
  {*z03, 0xF800}, //TFT_RED         0xF800
};


int noteTime[] = {  // Thanks to https://gist.github.com/HelgeSverre/33361e8a283624dfbbd6
  125, 500, 250, 250, 250, 125, 125, 250, 250, 500, 250, 250, 500, 250, 250, 750, 250, 500, 500, 500, 500, 500, 250, 500, 250, 500, 250, 250, 750, 250, 500, 250, 250, 500, 250, 250, 500, 500, 500, 500, 500, 500, 500, 250, 250, 250, 125, 125, 250, 250, 500, 250, 250, 500, 250, 250, 750, 250, 500, 500, 500, 500, 500, 250, 500, 250, 500, 250, 250, 750, 250, 500, 250, 250, 500, 250, 250, 500, 500, 500, 500, 500, 500, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 500, 500, 1000, 2000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 500, 500, 1000, 2000
};
int snd[] = {
  658, 1320, 990, 1056, 1188, 1320, 1188, 1056, 990, 880, 880, 1056, 1320, 1188, 1056, 990, 1056, 1188, 1320, 1056, 880, 880, 0, 1188, 1408, 1760, 1584, 1408, 1320, 1056, 1320, 1188, 1056, 990, 990, 1056, 1188, 1320, 1056, 880, 880, 0, 1320, 990, 1056, 1188, 1320, 1188, 1056, 990, 880, 880, 1056, 1320, 1188, 1056, 990, 1056, 1188, 1320, 1056, 880, 880, 0, 1188, 1408, 1760, 1584, 1408, 1320, 1056, 1320, 1188, 1056, 990, 990, 1056, 1188, 1320, 1056, 880, 880, 0, 660, 528, 594, 495, 528, 440, 419, 495, 660, 528, 594, 495, 528, 660, 880, 838, 660, 528, 594, 495, 528, 440, 419, 495, 660, 528, 594, 495, 528, 660, 880, 838
};


void setup(void) {  //-----------------------------------------SETUP-START--------------------------------------------
  Serial.begin(115200);
  //delay(2000);                                //Enable for software serial to comeup.
  //Serial.println("Start");

  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);

  randomSeed(analogRead(analogSeedPin) + millis()); //Randomize
  fig = random(0, 7);                               //Preselect figure.

  pinMode(mainButton, INPUT_PULLUP);                //Main (pause) button,
  pinMode(leftButton, INPUT_PULLUP);                //And four others.
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(rotButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(soundPin, OUTPUT);                        //Sound output.
  delay(10);

  if (digitalRead(mainButton) == 1) mish = true;    //Mouse mode. Press PAUSE on sturtup to run without mouse.

  startScreen();                                    //-----<------First entry point
}  //----------------------------------------------------------SETUP-END---------------------------------------------


void loop() {//--------------------------------------------LOOP-START--------------------------------------------------
  if (mish == true) {                   //Mouse?
    mouse.get_data();
    x = x + mouse.x_movement();
    y = y - mouse.y_movement();
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= 79) x = 79;
    if (y >= 159) y = 159;     // anyway i have coordintes of the mouse
    if ((mouse.button(0) == 1) && ((millis() - vremya) >= (fallAcc))) checkMove(0), vremya = millis();
    if (mouse.clicked(2)) rotate();
    if ((mouse.x_movement() > 0) && (abs(mouse.x_movement()) > mSpeed)) checkMove(1);  //Right
    if ((mouse.x_movement() < 0) && (abs(mouse.x_movement()) > mSpeed)) checkMove(-1); //Left
    //if (mouse.x_movement() > 0) myX = myX + abs(mouse.x_movement() / mSpeed), drawScreen();  //Some variant
    //if (mouse.x_movement() < 0) myX = myX - abs(mouse.x_movement() / mSpeed), drawScreen();
    //-------------------nice effect--------------------------------- if (mouse.button(2) == 0) tft.drawPixel(x, y, TFT_BLACK);
    //--------------------------------------------------------------- tft.drawPixel(x, y, random(65535)); //RGB
  }
  if ((digitalRead(rightButton) == 0) && (millis() - debncTime >= autoRep)) debncTime = millis(), checkMove(1);  //Button right
  if ((digitalRead(leftButton) == 0) && (millis() - debncTime >= autoRep)) debncTime = millis(), checkMove(-1);  //Button left
  if ((digitalRead(rotButton) == 0) && (millis() - debncTime >= autoRep)) debncTime = millis(), rotate();        //Button rotate
  if ((digitalRead(downButton) == 0) && ((millis() - vremya) >= (fallAcc))) checkMove(0), vremya = millis();     //Button down
  if ((digitalRead(mainButton) == 0) && (millis() - debncTime >= autoRep)) goPause();                            //Button pause

  if ((millis() - vremya) >= (1000 - fallAcc - (100 * (gameSpeed - 1)))) checkMove(0), vremya = millis();         // timed fall
  digitalWrite(soundPin, LOW);
}//---------------------------------------------------------LOOP-END-----------------------------------------------------


void drawScreen() { //-------------------------------------REDRAW Pole-START------------------------------------------------
  tft.fillRect(0, 0, 2, 179, TFT_DARKGREY);    // ///////
  tft.fillRect(58, 0, 2, 179, TFT_DARKGREY);   // /Pole
  tft.fillRect(0, 158, 60, 2, TFT_DARKGREY);   // ///////
  tft.drawRect(61, 17, 19, 25, TFT_DARKGREY);  // <- ramka next 61, 0, 19, 25

  for (byte hh = 0; hh < h; hh++) {            //  -----------Draw Pole
    for (byte ww = 0; ww < w; ww++) {
      if (Pole[ww][hh] == 0) tft.fillRect(ww * 5 + 3, hh * 5 + 3,  4,  4, tft.color565(12, 8, 4));  //very dark
      if (Pole[ww][hh] != 0) tft.fillRect(ww * 5 + 3, hh * 5 + 3,  4,  4, Pole[ww][hh]);            //all other colors
    }
  }

  for (byte i = 0; i < 4; i++) {               // --------Draw Figure @myX,myY
    for (byte ii = 0; ii < 4; ii++) {
      if (figure[ii][i] != 0) tft.fillRect((ii + myX) * 5 + 3, (i + myY) * 5 + 3,  4,  4, allBlocks[fig].col);
    }
  }
}//--------------------------------------------------------REDRAW Pole-END------------------------------------------------


void checkMove(int dir) { //-------------------------------CHECK move-START------------------------------------------------
  for (byte i = 0; i < 4; i++) { //       ---------Esli che - return!!!
    for (byte ii = 0; ii < 4; ii++) {
      if ((figure[ii][i] != 0) && (dir == 1) && (myX + ii >= w - 1)) return;             // right bound.
      if ((figure[ii][i] != 0) && (dir == -1) && (myX + ii <= 0)) return;                // left bound.
      if ((figure[ii][i] != 0) && (dir == 1) && (Pole[myX + ii + 1][myY + i])) return;   // right block.
      if ((figure[ii][i] != 0) && (dir == -1) && (Pole[myX + ii - 1][myY + i])) return;  // left block.
      if ((figure[ii][i] != 0) && (dir == 0) && (myY + i > h - 2)) {                     // bottom!!! Request next!
        savePole();
        fig = figNext, selectNext();
        tone(soundPin, 25, 50);
        return;
      }
      if ((figure[ii][i] != 0) && (dir == 0) && (Pole[myX + ii][myY + i + 1])) {         // block vnizu! Request next!
        savePole();
        fig = figNext, selectNext();
        tone(soundPin, 25, 50);
        return;
      }

    }
  }
  if (dir == 0) myY++;
  myX = myX + dir;
  drawScreen();
}//-------------------------------------------------------CHECK move-END-----------------------------------------------------


void selectNext() {  //-----------------------------Select block. And next too-START-----------------------------------------
  tft.fillRect(60, 86, 19, 10, TFT_BLACK);            // <- ramka 'score' clear
  tft.fillRect(66, 58, 7, 10, TFT_BLACK);             // <- ramka 'level' clear

  tft.pushImage(60, 0, nextPicWidth, nextPicHeight, nextPic);
  tft.pushImage(60, 44, levelPicWidth, levelPicHeight, levelPic);
  tft.pushImage(60, 68, scorePicWidth, scorePicHeight, scorePic);

  //tft.setTextColor(TFT_GREENYELLOW);               // Print some text
  tft.setTextColor(TFT_RED);
  tft.setCursor(68, 59);
  tft.print(gameSpeed);                              // Level

  tft.setCursor(62, 87);
  char tbuffer[4];
  sprintf(tbuffer, "%03d", score);
  tft.print(tbuffer);                                // Score

  tft.setTextColor(TFT_ORANGE);                      // Version :)
  tft.setCursor(62, 153);
  tft.print("v:1");

  if ((myY < 2) && (novi == false)) {                // ////////////////////    Game over!!!!!  //////////////////////////////////
    tft.fillScreen(TFT_BLACK);
    bool s = false;
    tft.setTextColor(TFT_GREEN);                                  // Show summary.
    tft.setCursor(0, 24);
    tft.print(F("Level: "));
    tft.println(gameSpeed);
    tft.println(" ");
    tft.print(F("Score: "));
    tft.println(score);
    tft.println(" ");
    tft.println(F("Total lines"));
    tft.print(F("removed:"));
    tft.setTextColor(TFT_SILVER);
    tft.setCursor(0, 130);
    tft.println(F("Push PAUSE"));
    tft.print(F("    to start."));
    tft.setTextColor(TFT_ORANGE);
    tft.setCursor(8, 153);
    tft.print(F("(c)Airrr(r)"));

    while (s == false) {                                       //__________________Wait for click while shining!__________________
      if (mish == true) mouse.get_data(), s = mouse.clicked(0);
      if (digitalRead(mainButton) == 0) s = true;
      tft.setTextColor(random(65535));
      tft.setCursor(1, 0);
      tft.print("--Game over--");
      tft.setTextColor(random(65535));
      tft.setCursor(35, 90);
      tft.println((gameSpeed - 1) * levelUpLines + score);
      delay(25);
    }

    for (byte hh = 0; hh < h; hh++) {                //Clear Pole[][] !!!!!!
      for (byte ww = 0; ww < w; ww++) {
        Pole[ww][hh] = 0;
      }
    }
    delay (1000);
    tft.fillScreen(TFT_BLACK);
    novi = true;
    score = 0;
    gameSpeed = 1;
    selectNext();
  }

  myY = 0, myX = 4;
  novi = false;

  byte iii = 0;                                   //Load Figure po "fig"
  for (byte i = 0; i < 4; i++) {
    for (byte ii = 0; ii < 4; ii++) {
      figure[ii][i] = allBlocks[fig].block[iii];
      iii++;
    }
  }

  figNext = random(0, 7);                         //Next
  byte tt[4][4];                                  //Select next and show
  iii = 0;
  tft.fillRect(62, 18, 17, 23, TFT_BLACK);        // <- ramka 'Next' clear
  for (byte i = 0; i < 4; i++) {
    for (byte ii = 0; ii < 4; ii++) {
      tt[ii][i] = allBlocks[figNext].block[iii];
      if (tt[ii][i] != 0) tft.fillRect(ii * 6 + 56 , i * 6 + 18,  5,  5, allBlocks[figNext].col);
      iii++;
    }
  }
  rotation = 0;
  vremya = millis();
  drawScreen();
} //-----------------------------------------------Select block. And next too-END-----------------------------------------



void savePole() { //---------------------------------ApplyArrays-START--------------------------------------------
  for (byte i = 0; i < 4; i++) {            //Copy small 4,4 figure into Pole array
    for (byte ii = 0; ii < 4; ii++) {
      if (figure[ii][i] != 0) Pole[myX + ii][myY + i] = allBlocks[fig].col;
    }
  }
  shiftPole();
}//--------------------------------------------------ApplyArrays-END--------------------------------------------



void shiftPole() {  //----------------------------------SHIFT-START------------------------------------------
  byte hh = 0;
  for (hh = h - 1; hh > 0; hh--) {                                 // Going up.
    byte lookLine = 0;                                             // Count found
    for (byte ww = 0; ww < w; ww++) {
      if (Pole[ww][hh] != 0) lookLine ++;
      if (lookLine == w) {                                         // Full line found @ level 'hh'. Get ready.
        score++;
        if ((score >= levelUpLines) && (gameSpeed >= 9)) goingFinal();       // score > 999! 9000 Lienes?
        if (score >= levelUpLines) tft.fillRect(60, 44, levelPicWidth, levelPicHeight, TFT_WHITE), delay(100), tft.pushImage(60, 44, levelPicWidth, levelPicHeight, levelPic), score = 0, gameSpeed++; // Flash)

        // remove line @hh. Animation then shift
        for (byte ww = 0; ww < w; ww++) {                           // Simply animation
          tft.fillRect(ww * 5 + 3, hh * 5 + 3,  4,  4, TFT_WHITE);
          tone(soundPin, 800 + (ww * 300), 50);
          delay(15);
        }

        for (byte del = hh; del > 1; del--) {                       // SHIFT. Ot hh(line) vverh do 0
          for (byte wi = 0; wi < w; wi++) {
            Pole[wi][del] = Pole[wi][del - 1];
          }
        }

        hh = h - 1;          //hz
        lookLine = 0;        //hz
        myY = 32, myX = 4;   //off-screen!! budet nash secret
        drawScreen();
        shiftPole();
      }

    }
  }
}//------------------------------------------------------SHIFT-END------------------------------------------


void rotate() { //--------------------------------------Rotation-START-------------------------------------

  byte rt[4][4];
  byte iii = 0;

  for (byte i = 0; i < 4; i++) {               //Load next rotation and check violations.  Esli che - return!!!
    for (byte ii = 0; ii < 4; ii++) {
      if (rotation == 0) rt[ii][i] = allBlocks1[fig].block[iii];
      if (rotation == 1) rt[ii][i] = allBlocks2[fig].block[iii];
      if (rotation == 2) rt[ii][i] = allBlocks3[fig].block[iii];
      if (rotation == 3) rt[ii][i] = allBlocks[fig].block[iii];
      iii++;
      if ((rt[ii][i] != 0) && (myX + ii >= w)) return;                       // right bound.
      if ((rt[ii][i] != 0) && (myX + ii < 0)) return;                        // left bound.
      if ((rt[ii][i] != 0) && ((Pole[myX + ii][myY + i]) != 0)) return;      // check blocks
    }
  }

  rotation ++;
  if (rotation > 3) rotation = 0;

  for (byte i = 0; i < 4; i++) {          //Copy Figure iz rt
    for (byte ii = 0; ii < 4; ii++) {
      figure[ii][i] = rt[ii][i];
    }
  }
  tone(soundPin, 1000, 3);
  drawScreen();
} //----------------------------------------------------Rotation-END-------------------------------------


void startScreen() {//---------------------------------------Welcome screen-START--------------------------------
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(8, 153);
  tft.print(F("(c)Airrr(r)"));     //13 chars long

  if (mish == true) {              //Main button not grounded: Mouse mode
    tft.setTextColor(TFT_RED);
    tft.setCursor(0, 0);
    tft.println(F("Trying mouse."));
    tft.println(F(" "));
    tft.setTextColor(TFT_DARKGREEN);
    tft.println(F("If stuck,"));
    tft.println(F("Check wiring"));
    tft.println(F("PS2 mouse"));
    tft.println(F("and reset."));
    tft.println(F(" "));
    tft.setTextColor(TFT_BROWN);
    tft.println(F("Or start game"));
    tft.println(F("With PAUSE"));
    tft.println(F("Button"));
    tft.println(F("pressed."));
    tft.println(F(" "));
    tft.println(F(" "));
    delay(500);

    if (mouse.initialise() != 0) {
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_RED);
      tft.setCursor(8, 100);
      tft.println(F("Mouse error!")), delay (30000);
    }

    tft.setTextColor(TFT_RED);
    tft.println("Ok!");
    delay(500);
  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(8, 153);
  tft.print(F("(c)Airrr(r)"));

  tft.setTextColor(TFT_SILVER);
  tft.setCursor(0, 130);
  tft.println(F("Push DOWN"));
  tft.print(F("    to start."));

  byte tn = 0;
  bool s = false;
  while (s == false) {             //____________________________________Wait start click!____________________________________
    if (mish == true) mouse.get_data(), s = mouse.clicked(0);
    if (digitalRead(downButton) == 0) s = true;

    byte iii = 0;                                 //Load TEMPORARAY Figure po random
    byte ff = random(0, 7);                       //tipa 'screensaver'
    byte xxt = random(0, 11) * 6;
    byte yyt = random(0, 16) * 6 + 10 ;
    byte trot = random(0, 3);

    for (byte i = 0; i < 4; i++) {
      for (byte ii = 0; ii < 4; ii++) {
        if (trot == 0) figure[ii][i] = allBlocks[ff].block[iii];
        if (trot == 1) figure[ii][i] = allBlocks1[ff].block[iii];
        if (trot == 2) figure[ii][i] = allBlocks2[ff].block[iii];
        if (trot == 3) figure[ii][i] = allBlocks3[ff].block[iii];
        if (figure[ii][i] != 0) tft.fillRect(ii * 6 + xxt - 5 , i * 6 + yyt,  5,  5, allBlocks[ff].col);
        iii++;
      }
    }

    if ((snd[tn] == 0) || (tn == 0)) tft.fillRect(0 , 8,  80,  121, TFT_BLACK);         //Smart Clear :)
    tft.setTextColor(random(65535));
    tft.setCursor(1, 0);
    tft.print(F("...ATETRIS..."));

    tone(soundPin, snd[tn], noteTime[tn]);        //----------SOUND
    yield();
    delay(noteTime[tn]);
    noTone(soundPin);
    tn++;
    if (tn >= 115) tn = 0, delay (500);           // Notes
  }

  tft.fillScreen(TFT_BLACK);
  delay(200);
  selectNext();    //Go!
} //------------------------------------------------------Welcome screen-END--------------------------------




void goingFinal() {  //-------------------------ENDSCREEN-START--------------------------------
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setCursor(8, 100);
  tft.println(F("Ok lol"));
  tft.println(F("You are the"));
  tft.println(F("first."));

  while (true) {  // ??
    delay(1);
  }

} //---------------------------------------------ENDSCREEN-END--------------------------------



void goPause() { //----------------------------------------PAUSE-START---------------------------------------------------
  tft.fillRect(8, 53, 44, 24, TFT_BLACK);         // <- ramka Pause
  word tempc = random(65500);

  debncTime = millis() + 500;
  bool s = false;
  while (s == false) {             //__________________________Wait click!
    if (mish == true) mouse.get_data(), s = mouse.clicked(0);
    if ((digitalRead(mainButton) == 0) && (millis() - debncTime >= autoRep)) s = true;

    tft.setCursor(16, 61);
    tft.setTextColor(tempc);
    tft.print("Pause");

    tempc++;
    if (tempc >= 65535) tempc = 0;
    delay(25);
  }
  tft.fillRect(12, 58, 35, 15, TFT_BLACK);        // <- ramka Pause
  debncTime = millis() + autoRep;
  vremya = millis();
}//-------------------------------------------------------PAUSE-END---------------------------------------------------


/*  Colors:
  TFT_BLACK       0x0000,  TFT_NAVY        0x000F,  TFT_DARKGREEN   0x03E0,  TFT_DARKCYAN    0x03EF,  TFT_MAROON      0x7800
  TFT_PURPLE      0x780F,  TFT_OLIVE       0x7BE0,  TFT_LIGHTGREY   0xD69A,  TFT_DARKGREY    0x7BEF,  TFT_BLUE        0x001F
  TFT_GREEN       0x07E0,  TFT_CYAN        0x07FF,  TFT_RED         0xF800,  TFT_MAGENTA     0xF81F,  TFT_YELLOW      0xFFE0
  TFT_WHITE       0xFFFF,  TFT_ORANGE      0xFDA0,  TFT_GREENYELLOW 0xB7E0,  TFT_PINK        0xFE19,  TFT_BROWN       0x9A60
  TFT_GOLD        0xFEA0,  TFT_SILVER      0xC618,  TFT_SKYBLUE     0x867D,  TFT_VIOLET      0x915C   */
