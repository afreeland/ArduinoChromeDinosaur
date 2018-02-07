#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to -1!
#define TFT_DC     8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!

// How fast the game is moving
int movingRate = 10;
uint16_t defaultColor = ST7735_BLACK;

// The timer used to store how long before we increase difficulty
uint32_t difficultyTime;
uint32_t difficultyThreshold = 7000;

int groundX = 5; // Initial x value for our ground animation sequence...this gets modified by movingRate to simulate speed
int dinosaurY = 85; // The resting y value for the dinosaur character (starting with the tail...left to right)
int cactusY = 80; // Cactus Y position...probably not changing
int cactusX = 130; // Cactus x position...this gets modified by movingRate

// Used to help with timing our animation for the dinosaur running
uint32_t legTime;
int leg1Length = 3;
int leg2Length = 1;

int btn = 2; // The button a user is pressing to control action
bool isJumping = false; // Whether the dinosaur jumping animation is happening
int jumpHeight = 0; // The current height the dinosaur is jumping (the y value that we are setting)
int jumpTargetHeight = 70; // Controls how height the dinosaur will jump
unsigned long score = 0; // Store the score
void setup() {
  Serial.begin(9600);
  Serial.print("Dinosaur game from Chrome");
  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(1);
  Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST7735_WHITE);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  pinMode(btn, INPUT_PULLUP);

  // put your main code here, to run repeatedly:
  //drawDinosaur(ST7735_BLACK);
  legTime = millis();
  difficultyTime = millis();

  tft.setCursor(80, 1);
  tft.setTextColor(defaultColor);
  tft.setTextSize(1);
  tft.println("HI");
  
  tft.setCursor(95, 1);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(1);
  tft.println("1205");

  
}


void loop() {
  score = (millis() / 1000);
  displayScore(score);
  int val = digitalRead(btn);     // read the input pin
  if(val == 0 && isJumping == false){
    isJumping = true;
  }

  if(isJumping && jumpHeight <= jumpTargetHeight){
    jumpHeight +=30;
    if(jumpHeight >= jumpTargetHeight){
      isJumping = false;
    }
  }

  if(!isJumping && jumpHeight > 0){
    jumpHeight -= 30;
  }

//  if(isJumping && jumpHeight >= 1){
//    jumpHeight--;
//
//    if(jumpHeight == 1){
//      jumpHeight = 0;
//      isJumping = false;
//    }
//  }
  
  Serial.println(val);
  
  groundX = groundX - movingRate;
  cactusX = cactusX - movingRate;

  clearBars();
  ground(groundX, movingRate);
  drawDinosaur(dinosaurY - jumpHeight);
  cactus(cactusX, movingRate);
  
  horizon();
  
  
  
  delay(50);

  uint32_t currentTime = millis();

  if(groundX < - 95){
    groundX = 5;
    
  }

  if(cactusX < -20){
    cactusX = 130;
  }

  
  // Check if enough time has passed to trigger our difficulty threshold
  // If so, then lets increase speed/difficulty
  if( (currentTime - difficultyTime) > difficultyThreshold){
    // Lets not get too ridiculous here
    if(movingRate < 60)
      movingRate = movingRate + 1;
    Serial.println("Increase: " + String(movingRate));
    difficultyTime = millis();
  }
  
}

void displayScore(unsigned long userScore){
  char buffer[9];
  // sprintf allows us to nice format our score to match the games
  // The %05d does the following:
  // Declares that we are formatting a decimal number
  // Additionally declares we want to lead with the number '0' for those digits not set yet
  // https://forum.arduino.cc/index.php?topic=371117.0
  sprintf(buffer, "%05d", userScore );
  Serial.println(buffer);
  
  tft.setCursor(130, 1);
  tft.setTextColor(ST7735_BLACK, ST7735_WHITE);
  tft.setTextSize(1);
  
  tft.println(String(buffer));
}

void clearBars(){
  // Under horizon
  tft.fillRect(0, 93, tft.width(), 18, ST7735_WHITE);

  // Over Horizon
  tft.fillRect(30, 15, 130, 77, ST7735_WHITE);
}
void cactus(int originX, int movingRate){
  // Clear previous
//  tft.fillRect(originX, cactusY, 25, 40, ST7735_RED);
  if(originX > -5){
    // Left Arm
    tft.fillRoundRect(originX, cactusY + 7, 4, 13, 3, ST7735_BLACK);
    tft.fillRect(originX + 3, cactusY + 17, 3, 3, ST7735_BLACK);
  
    // Body
    tft.fillRect(originX + 6, cactusY + 5, 5, 24, ST7735_BLACK);
    tft.fillRoundRect(originX + 6, cactusY, 5, 15, 3, ST7735_BLACK);
    // Right Arm
    tft.fillRect(originX + 11, cactusY + 13, 3, 3, ST7735_BLACK);
    tft.fillRoundRect(originX + 12, cactusY + 3, 4, 13, 3, ST7735_BLACK);
  }
}

void horizon(){
  tft.fillRect(0, 92, tft.width(), 1, defaultColor);
}

void ground(int originX, int movingRate){
  int iterations = 2;
  int originY = 95;
  
  // Clear out our screen
//  tft.fillRect(0, originY, tft.width(), 5, ST7735_WHITE);

  for(int i = 0; i < iterations; i++){
    int actualOriginX = originX - movingRate;
    Serial.println("Actual Origin: " + String(actualOriginX));
    int blk1 = actualOriginX;
    int blk2 = actualOriginX + 5;
    int blk3 = actualOriginX + 10;
    int blk4 = actualOriginX + 20;
    int blk5 = actualOriginX + 30;
    int blk6 = actualOriginX + 33;
    int blk7 = actualOriginX + 41;
    int blk8 = actualOriginX  + 55;
    int blk9 = actualOriginX + 60;
    int blk10 = actualOriginX + 65;
    int blk11 = actualOriginX + 82;
    int blk12 = actualOriginX + 84;
    int blk13 = actualOriginX + 88;
    tft.drawPixel(blk1, originY + 3, defaultColor);
    tft.fillRect(blk2, originY + 4, 4, 1, defaultColor);
    tft.fillRect(blk3, originY, 3, 1, defaultColor);
    tft.fillRect(blk4, originY + 2, 2, 1, defaultColor);
    tft.drawPixel(blk5, originY + 1, defaultColor);
    tft.fillRect(blk6, originY + 2, 4, 1, defaultColor);
    tft.fillRect(blk7, originY + 1, 2, 1, defaultColor);
  
    tft.drawPixel(blk8, originY + 3, defaultColor);
    tft.fillRect(blk9, originY + 2, 2, 1, defaultColor);
    tft.fillRect(blk10, originY, 4, 1, defaultColor);
  
    tft.drawPixel(blk11, originY + 2, defaultColor);
    tft.fillRect(blk12, originY + 1, 2, 1, defaultColor);
    tft.fillRect(blk13, originY + 2, 3, 1, defaultColor);

    int repeatOffset = 130;
    tft.drawPixel(blk1 + (repeatOffset * 1), originY + 3, defaultColor);
    tft.fillRect(blk2 + (repeatOffset * 1), originY + 4, 4, 1, defaultColor);
    tft.fillRect(blk3 + (repeatOffset * 1), originY, 3, 1, defaultColor);
    tft.fillRect(blk4 + (repeatOffset * 1), originY + 2, 2, 1, defaultColor);
    tft.drawPixel(blk5 + (repeatOffset * 1), originY + 1, defaultColor);
    tft.fillRect(blk6 + (repeatOffset * 1), originY + 2, 4, 1, defaultColor);
    tft.fillRect(blk7 + (repeatOffset * 1), originY + 1, 2, 1, defaultColor);

    tft.drawPixel(blk8 + (repeatOffset * 1), originY + 3, defaultColor);
    tft.fillRect(blk9 + (repeatOffset * 1), originY + 2, 2, 1, defaultColor);
    tft.fillRect(blk10 + (repeatOffset * 1), originY, 4, 1, defaultColor);

    tft.drawPixel(blk11 + (repeatOffset * 1), originY + 2, defaultColor);
    tft.fillRect(blk12 + (repeatOffset * 1), originY + 1, 2, 1, defaultColor);
    tft.fillRect(blk13 + (repeatOffset * 1), originY + 2, 3, 1, defaultColor);
  }
  
}

void drawDinosaur(int y) {
  // Using our tail as the reference everything else is based off of
  int originX = 4;
  int originY = y;
  int animThreshold = 250;

  
  
  // Current time in milliseconds
  uint32_t t = millis();
  
  
  // 1.5 seconds
  if((t - legTime) >= animThreshold){
    Serial.println("changing size");
    leg1Length = 1;
    leg2Length = 2;
    legTime = millis();
  }else{
    Serial.println("keeping size");
    leg1Length = 2;
    leg2Length = 1;
  }

  

  // Clear the upper section above ground
//  if(jumpHeight > 0)
    tft.fillRect(0, 95-jumpTargetHeight-12, 30, jumpTargetHeight + 9, ST7735_WHITE);
//    tft.fillRect(0, 95-jumpTargetHeight-21, 30, jumpTargetHeight, defaultColor);

  // Clear the leg area of our rex
  int topLeftX = originX + 3;
  int topLeftY = originY + 12;
  int boxWidth = 10;
  int boxHeight = 7;
  tft.fillRect(topLeftX, topLeftY, boxWidth, boxHeight, ST7735_WHITE);

  // X, Y, Width, Height, Color
  // Body (Left to Right)
  tft.fillRect(originX, originY, 1, 10, defaultColor);
  tft.fillRect(originX + 1, originY + 4, 1, 7, defaultColor);
  tft.fillRect(originX + 2, originY + 5, 1, 7, defaultColor);
  tft.fillRect(originX + 3, originY + 6, 1, 6, defaultColor);
  tft.fillRect(originX + 4, originY + 6, 1, 8, defaultColor);
  tft.fillRect(originX + 5, originY + 4, 1, 10, defaultColor);
  tft.fillRect(originX + 6, originY + 2, 2, 12, defaultColor);
  tft.fillRect(originX + 8, originY + 1, 1, 11, defaultColor);
  tft.fillRect(originX + 9, originY, 1, 14, defaultColor);
  tft.fillRect(originX + 10, originY - 1, 1, 15, defaultColor);
  tft.fillRect(originX + 11, originY - 9, 1, 24, defaultColor);
  tft.fillRect(originX + 12, originY - 11, 1, 26, defaultColor);
  tft.fillRect(originX + 13, originY - 10, 1, 20, defaultColor);
  
  // Head (Top to Bottom)
  tft.fillRect(originX + 13, originY - 11, 8, 1, defaultColor);
  tft.fillRect(originX + 14, originY - 10, 9, 7, defaultColor);
  tft.fillRect(originX + 14, originY - 3, 3, 2, defaultColor);
  tft.fillRect(originX + 14, originY - 1, 6, 1, defaultColor); // Jaw

  // Eye
  tft.fillRect(originX + 14, originY - 8, 1, 1, ST7735_WHITE);
  
  // Arms
  tft.fillRect(originX + 14, originY + 3, 3, 2, defaultColor);
  tft.fillRect(originX + 16, originY + 5, 1, 2, defaultColor);


  // Leg 1
  tft.fillRect(originX + 5, originY + 14, 3, 1, defaultColor);
  tft.fillRect(originX + 5, originY + 15, 2, 1, defaultColor);
  tft.fillRect(originX + 5, originY + 16, 1, leg1Length, defaultColor); // Used to animate leg length (run)
  tft.fillRect(originX + 5, originY + 16 + leg1Length, 2, 1, defaultColor);

  // Leg 2
  tft.fillRect(originX + 10, originY + 14, 3, 1, defaultColor);
  tft.fillRect(originX + 10, originY + 15, 2, 1, defaultColor);
  tft.fillRect(originX + 10, originY + 16, 1, leg2Length, defaultColor); // Used to animate leg length (run)
  tft.fillRect(originX + 10, originY + 16 + leg2Length, 2, 1, defaultColor);

  // Clear leg glitch from sticking
//  if(isJumping)
//    tft.fillRect(topLeftX, 92, boxWidth, boxHeight, ST7735_WHITE);

}
