//ATETRIS ATmega version (328 or 32u4)!
//(c)Airrr(r)          May.2022          v:2         07.05.22
//
//ATmega328:       ATmega32u4:
//SCL -> 13          -> 15  (clk)
//SDA -> 11          -> 16  (mosi)
//RES -> A3          -> --
//DC  -> A2          -> --
//CS  -> 10          -> --
//BLK -> 8           -> --

//ALSO ATmega328 and ATmega32u4 has different "self voltage measurement" procedures.
//See "void readVoltage()". line ~544


#include <TFT_ST7735.h>       //Thanks to https://github.com/Bodmer/TFT_ST7735
#include <SPI.h>
#include <EEPROM.h>           //Save game
#include "LowPower.h"         //And sleep

#include "nextPic.h"
#include "levelPic.h"
#include "scorePic.h"

//------------------------------DEFINES FOR ATmegas-------------------------------------
#define mainButton  2         //Key pause     All keys to ground. UP
#define leftButton  3         //Key left
#define rightButton 4         //Key right
#define rotButton   5         //Key rotate
#define downButton  6         //Key down
#define analogSeedPin A0      //Leave unconnected.
#define soundPin    7         //Sound. Optional. Piezo to ground.
#define lcdOff      8         //Turn off lcd while grounded.
//------------------------------DEFINES FOR ATmega328-----------------------------------



TFT_ST7735 tft = TFT_ST7735();

const byte w = 11;
const byte h = 31;
byte Pole[w][h];
int myX = 4, myY = 0;      //Starting coords
byte fig, figNext = 0;     //
byte figure[4][4];         //Current figure array
byte gameSpeed = 1;        //Speed ah? 1-9 -> game leveling
byte fallAcc = 10;         //Accelerated drop. on MButton or Keydown. lower-faster
unsigned long vremya = 0;         //Nado tak
unsigned long debncTime = 0;      //Mezhdu knopkami
byte rotation = 0;         //
bool novi = true;          //New game?
word score = 0;            //
long volt = 0;             //Battery voltage


word autoRep = 200;        //Keys auto repeat delay
word levelUpLines = 1000;  //Switch to next level. 1000 max!!! x9 levels = 9000 lines.


bool i[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}}; //lame
bool j[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
bool l[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}};
bool p[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool s[4][4] = {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool t[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};
bool z[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}};

bool i01[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
bool j01[4][4] = {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
bool l01[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}};
bool p01[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool s01[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
bool t01[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};
bool z01[4][4] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};

bool i02[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}};
bool j02[4][4] = {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
bool l02[4][4] = {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
bool p02[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool s02[4][4] = {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool t02[4][4] = {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
bool z02[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}};

bool i03[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
bool j03[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool l03[4][4] = {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
bool p03[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
bool s03[4][4] = {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}};
bool t03[4][4] = {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
bool z03[4][4] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}};

word colors[8] = {0, 0x07FF, 0x001F, 0x915C, 0xFEA0, 0x07E0, 0xF81F, 0xF800};

struct Bloki
{
  bool *block;
  word col;     //0-65535 FFFF
};

Bloki allBlocks[] =
{
  {*i, 0x07FF}, //TFT_CYAN        0x07FF   //1
  {*j, 0x001F}, //TFT_BLUE        0x001F   //2
  {*l, 0x915C}, //TFT_VIOLET      0x915C   //3
  {*p, 0xFEA0}, //TFT_GOLD        0xFEA0   //4
  {*s, 0x07E0}, //TFT_GREEN       0x07E0   //5
  {*t, 0xF81F}, //TFT_MAGENTA     0xF81F   //6
  {*z, 0xF800}, //TFT_RED         0xF800   //7
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
  //Serial.begin(115200);
  //delay(2000);                                //Enable for software serial to comeup.
  //Serial.println("Start");

  tft.init();
  tft.setRotation(2);
  tft.invertDisplay(true);

  randomSeed(analogRead(analogSeedPin) + millis()); //Randomize
  fig = random(0, 7);                               //Preselect figure.

  pinMode(mainButton, INPUT_PULLUP);                //Main (pause) button,
  pinMode(leftButton, INPUT_PULLUP);                //And four others.
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(rotButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(soundPin, OUTPUT);                        //Sound output.
  pinMode(lcdOff, OUTPUT);                          //LCD show.
  delay(10);
  digitalWrite(lcdOff, 1);                          //Turn LCD on.


  startScreen();                                    //-----<------First entry point
}  //----------------------------------------------------------SETUP-END---------------------------------------------


void loop() {//--------------------------------------------LOOP-START--------------------------------------------------

  if ((digitalRead(rightButton) == 0) && (millis() - debncTime >= autoRep)) debncTime = millis(), checkMove(1);  //Button right
  if ((digitalRead(leftButton) == 0) && (millis() - debncTime >= autoRep)) debncTime = millis(), checkMove(-1);  //Button left
  if ((digitalRead(rotButton) == 0) && (millis() - debncTime >= autoRep)) debncTime = millis(), rotate();        //Button rotate
  if ((digitalRead(downButton) == 0) && ((millis() - vremya) >= (fallAcc))) checkMove(0), vremya = millis();     //Button down
  if ((digitalRead(mainButton) == 0) && (millis() - debncTime >= autoRep)) goPause();                            //Button pause

  if ((millis() - vremya) >= (1000 - fallAcc - (100 * (gameSpeed - 1)))) checkMove(0), vremya = millis();        // timed fall
  digitalWrite(soundPin, LOW);
}//---------------------------------------------------------LOOP-END-----------------------------------------------------


void drawScreen() { //-------------------------------------REDRAW Pole-START------------------------------------------------
  for (byte hh = 0; hh < h; hh++) {            //  -----------Draw Pole
    for (byte ww = 0; ww < w; ww++) {
      if (Pole[ww][hh] == 0) tft.fillRect(ww * 5 + 3, hh * 5 + 3,  4,  4, tft.color565(12, 8, 4));  //very dark
      if (Pole[ww][hh] != 0) tft.fillRect(ww * 5 + 3, hh * 5 + 3,  4,  4, colors[Pole[ww][hh]]);            //all other colors
    }
  }

  for (byte i = 0; i < 4; i++) {               // --------Draw Figure @myX, myY
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

  drawIcon(nextPic, 60, 0, nextPicWidth, nextPicHeight);
  drawIcon(levelPic, 60, 44, levelPicWidth, levelPicHeight);
  drawIcon(scorePic, 60, 68, scorePicWidth, scorePicHeight);

  tft.fillRect(0, 0, 2, 179, TFT_DARKGREY);          // ///////
  tft.fillRect(58, 0, 2, 179, TFT_DARKGREY);         // /Pole
  tft.fillRect(0, 158, 60, 2, TFT_DARKGREY);         // ///////
  tft.drawRect(61, 17, 19, 25, TFT_DARKGREY);        // <- ramka next

  tft.setTextColor(TFT_RED);                         // Print some text
  tft.setCursor(68, 59);
  tft.print(gameSpeed);                              // Level

  tft.setCursor(61, 87);
  char tbuffer[4];
  sprintf(tbuffer, "%03d", score);
  tft.print(tbuffer);                                // Score

  tft.setTextColor(TFT_ORANGE);                      // Version :)
  tft.setCursor(61, 153);
  tft.print("v:2");

  tft.fillRect(60, 132, 20, 19, TFT_BLACK);         // <- ramka batareiki
  tft.drawRect(61, 141, 18, 10, TFT_LIGHTGREY);      // <- zloi batarey
  tft.drawFastVLine(79, 144, 4, TFT_LIGHTGREY);
  tft.setCursor(61, 132);
  readVoltage();                                    // Battery
  float v = (float)volt / 1000;
  tft.setTextColor(TFT_DARKGREY);
  tft.print(v, 1);
  tft.fillRect(62, 142, (map(v, 2.5, 3.3, 1, 16)), 8, TFT_DARKGREY);            //Battery level (16pix lenght)(62, 142, 16, 8, TFT_RED)

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
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 130);
    tft.println(F("Push PAUSE"));
    tft.print(F("    to start."));
    tft.setTextColor(TFT_ORANGE);
    tft.setCursor(8, 153);
    tft.print(F("(c)Airrr(r)"));

    while (s == false) {                                       //__________________Wait for click while shining!__________________
      if (digitalRead(mainButton) == 0) s = true;
      tft.setTextColor(random(65535));
      tft.setCursor(1, 0);
      tft.print(F("--Game over--"));
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
    clearE();                                        //Clear EEPROM
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
      if (figure[ii][i] != 0) Pole[myX + ii][myY + i] = fig + 1;   //1->8
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
        if (score >= levelUpLines) score = 0, gameSpeed++, tft.fillRect(60, 44, levelPicWidth, levelPicHeight, TFT_WHITE), delay(100), drawIcon(levelPic, 60, 44, levelPicWidth, levelPicHeight); // Flash)

        // remove line @hh. Animation then shift
        for (byte ww = 0; ww < w; ww++) {                           // Simply animation
          tft.fillRect(ww * 5 + 3, hh * 5 + 3,  4,  4, TFT_WHITE);
          tone(soundPin, 800 + (ww * 300), 40);
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
  tft.print(F("(c)Airrr(r)"));

  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 130);
  tft.println(F("Push DOWN"));
  tft.print(F("    to start."));

  byte tn = 0;
  bool s = false;
  while (s == false) {             //____________________________________Wait start click!____________________________________
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

    tone(soundPin, snd[tn], noteTime[tn]);                                 //----------SOUND
    yield();
    delay(noteTime[tn]);
    noTone(soundPin);
    tn++;
    if (tn >= (sizeof(snd) / sizeof(snd[0]))) tn = 0, delay (500);         // Notes  // >=115
  }

  tft.fillScreen(TFT_BLACK);
  delay(200);
  load();
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
  tft.fillRect(8, 53, 44, 24, TFT_BLACK);                    // <- ramka Pause
  word tempc = random(65500);

  debncTime = millis() + 500;
  unsigned long sleepTime = millis();
  bool s = false;
  int totalSlp = 9;                    // Seconds
  save();                              // to EEPROM!
  delay(333);

  while (s == false) {                 //__________________________Wait click!______________________________

    if ((digitalRead(mainButton) == 0) && (millis() - debncTime >= autoRep)) s = true;

    if (totalSlp >= 0) {
      tft.setTextColor(tempc);
      tft.setCursor(10, 61);
      tft.print(F("Pause "));
      tempc++;
      if (tempc >= 65535) tempc = 0;
    }

    if (((millis() - sleepTime) >= 1000) && (totalSlp >= 0)) {       //Once per sec
      tft.fillRect(44, 60, 7, 9, TFT_BLACK);                         //<- ramka Countdown
      tft.print(totalSlp);                                           //Count 9->0 sec and sleep
      totalSlp--;
      sleepTime = millis();
      if (totalSlp < 0) {                         //Turn LCD off after 10 sec. And put to sleep.
        delay(500);
        digitalWrite(lcdOff, 0);
        attachInterrupt(0, wakeUp, LOW);
        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
        detachInterrupt(0);
      }
    }
    delay(50);
  }

  tone(soundPin, 300, 50);                             //Beep on wake.
  digitalWrite(lcdOff, 1);                             //Turn LCD on.
  delay(500);
  tft.fillRect(9, 58, 42, 15, TFT_BLACK);              // <- ramka Pause
  debncTime = millis() + autoRep;
  vremya = millis();
}//-------------------------------------------------------PAUSE-END---------------------------------------------------

void readVoltage() {  //Read self voltage. Thanks to the internet.
  // Read 1.1V reference against AVcc
  
  //ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);                   //For ATmega328    uncommect only one!
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);       //For ATmega32u4
  
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  volt = ADCL;
  volt |= ADCH << 8;
  volt = 1126400L / volt;                    ///// 1125300 = 1.0989*1024*1000
}

void save() {//------------------------------------------EEPROM--------------------------------------------------------
  word addrE = 0;
  for (byte hh = 0; hh < h; hh++) {                //  -----------Save Pole to EEPROM
    for (byte ww = 0; ww < w; ww++) {
      EEPROM.put(addrE, Pole[ww][hh]);
      addrE++;
    }
  }
  EEPROM.put(addrE, gameSpeed);                   // Game LEVEL byte
  addrE++;
  EEPROM.put(addrE, score);                       // Game SCORE word
}


void load() {
  word addrE = 0;
  for (byte hh = 0; hh < h; hh++) {                //  -----------Load Pole from EEPROM
    for (byte ww = 0; ww < w; ww++) {
      EEPROM.get(addrE, Pole[ww][hh]);
      if (Pole[ww][hh] > 8) clearE();              // if garbage found?
      addrE++;
    }
  }
  EEPROM.get(addrE, gameSpeed);
  if ((gameSpeed > 9) || (gameSpeed < 0)) clearE();// if garbage found?
  if (gameSpeed == 0) gameSpeed = 1;               // ficha!
  addrE++;
  EEPROM.get(addrE, score);
  if ((score > 1000) || (score < 0)) clearE();     // if garbage found?
}


void clearE() {
  for (byte hh = 0; hh < h; hh++) {                //  -----------Clear Pole
    for (byte ww = 0; ww < w; ww++) {
      Pole[ww][hh] = 0;
    }
  }
  for (word ia = 0 ; ia < EEPROM.length() ; ia++) { //  -----------And clear eeprom
    EEPROM.write(ia, 0);
  }
  score = 0;
  gameSpeed = 1;
}
//--------------------------------------------------------EEPROM--------------------------------------------------------
/*  Colors:
  TFT_BLACK       0x0000,  TFT_NAVY        0x000F,  TFT_DARKGREEN   0x03E0,  TFT_DARKCYAN    0x03EF,  TFT_MAROON      0x7800
  TFT_PURPLE      0x780F,  TFT_OLIVE       0x7BE0,  TFT_LIGHTGREY   0xD69A,  TFT_DARKGREY    0x7BEF,  TFT_BLUE        0x001F
  TFT_GREEN       0x07E0,  TFT_CYAN        0x07FF,  TFT_RED         0xF800,  TFT_MAGENTA     0xF81F,  TFT_YELLOW      0xFFE0
  TFT_WHITE       0xFFFF,  TFT_ORANGE      0xFDA0,  TFT_GREENYELLOW 0xB7E0,  TFT_PINK        0xFE19,  TFT_BROWN       0x9A60
  TFT_GOLD        0xFEA0,  TFT_SILVER      0xC618,  TFT_SKYBLUE     0x867D,  TFT_VIOLET      0x915C   */

//====================================================================================
// This is the function to draw the icon stored as an array in program memory (FLASH)
//====================================================================================

// To speed up rendering we use a 64 pixel buffer
#define BUFF_SIZE 64

// Draw array "icon" of defined width and height at coordinate x,y
// Maximum icon size is 255x255 pixels to avoid integer overflow

void drawIcon(const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  // Set up a window the right size to stream pixels into
  tft.setAddrWindow(x, y, x + width - 1, y + height - 1);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;

  // Fill and send "nb" buffers to TFT
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    tft.pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    tft.pushColors(pix_buffer, np);
  }
}

void wakeUp()
{
  // Just a handler for the pin interrupt.
}
