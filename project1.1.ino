//#include <avr/io.h>// 
//#include "basicMethods.h"

long randNumber;
//hardware vars
const int row[8] = {
  8, 9, 10, 11, 12, 13, 16, 17
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  0, 1, 2, 3, 4, 5, 6, 7
};

//Patter that is repeated in the intro screen
const int introPattern[8][8] = { {1,1,2,2,2,1,1,0},
                                 {0,1,1,2,2,2,1,1},
                                 {1,0,1,1,2,2,2,1},
                                 {1,1,0,1,1,2,2,2},
                                 {2,1,1,0,1,1,2,2},
                                 {2,2,1,1,0,1,1,2},
                                 {2,2,2,1,1,0,1,1},
                                 {1,2,2,2,1,1,0,1} };

// 2-dimensional array of pixels:
int pixels[8][8];

long previousMillis = 0; 
long interval = 800;           // interval at which to update (milliseconds)

//game Varibles

//struct for a monster
const int numOfMon = 10;
bool gameStarted = false;


//This is the struct for a monster, and all od its moving parts
typedef struct monster {
   int x; //x location
   int y; //y location
   int sizeOf; // how long the monster is
   bool right; //is it horizontal or not
   bool alive; // is it active on the board
   int steps; //how many steps it has taken
   String Dir; // the direction which its moving
};

//array of monsters
monster monsterArray[numOfMon];


//plarter varibles
int playerX = 0;
int playerY = 0;
int playerLives = 3;
int highscore = 0;
int score = 0;

void setup() {
  //Initilizes ports
  DDRD = DDRD | B11111111;
  //Random seed initilized
  randomSeed(12989);
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(row[thisPin], OUTPUT);
    
    digitalWrite(row[thisPin], HIGH);
  }
  clearScreen();
}

void loop() {
  gameinit();
  resetGame();
  unsigned long currentMillis = millis();
  //inside should update the board evry interval
  if(currentMillis - previousMillis > interval && gameStarted == true) {
    // save the last time you update 
    previousMillis = currentMillis; 
    placeEnemy(); 
    clearScreen();
    updateEnemies();
    score = score + 1;
  }
  readSensors();
  refreshScreen();
}


//Methods

//Clears the screen and makes sures everything is low
void clearScreen(){
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = 0;
    }
  }
}

//converts the screen into 1.
void oneScreen(){
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = 1;
    }
  }
}

//draws the lives
void drawLives(int val){
  if (playerLives == 3) {
    draw3(val);
  } else if (playerLives == 2) {
    draw2(val);
  } else {
    draw1(val);
  }
}

void draw3(int val){
  pixels[1][2] = val;
  pixels[2][2] = val;
  pixels[3][2] = val;
  pixels[4][2] = val;
  pixels[5][2] = val;
  pixels[6][2] = val;

  pixels[1][3] = val;
  pixels[4][3] = val;
  pixels[6][3] = val;
  
  pixels[1][4] = val;
  pixels[4][4] = val;
  pixels[6][4] = val;
  
  pixels[1][5] = val;
  pixels[4][5] = val;
  pixels[6][5] = val;
}

void draw2(int val){

  pixels[1][2] = val;
  pixels[1][3] = val;
  pixels[1][4] = val;
  pixels[1][5] = val;

  pixels[4][2] = val;
  pixels[4][3] = val;
  pixels[4][4] = val;
  pixels[4][5] = val;

  pixels[6][2] = val;
  pixels[6][3] = val;
  pixels[6][4] = val;
  pixels[6][5] = val;

  pixels[5][2] = val;
  pixels[2][5] = val;
  pixels[3][5] = val;
}

void draw1(int val){
  pixels[1][3] = val;
  pixels[2][3] = val;
  pixels[3][3] = val;
  pixels[4][3] = val;
  pixels[5][3] = val;
  pixels[6][3] = val;

  pixels[1][2] = val;
  pixels[1][3] = val;
  pixels[1][4] = val;

  pixels[5][4] = val;
  
}

//Tunrs the port to all Lows
void clearPort() {
  PORTD = B00000000;
}

//converts an array of integers into a binary value
void arrayToPort(int row[]) {
  byte b = B00000000;
  for(int i = 0; i < 8; i++) {
    if (row[i] == 2) {
      b = bitWrite(b, 7 - i , 1);
    } else {
      b = bitWrite(b, 7 - i , 0);
    }
  }
  PORTD = b;
}

//converts an array of integers into a binary value but only 2
void arrayToPortHalf(int row[]) {
  byte b = B00000000;
  for(int i = 0; i < 8; i++) {
    if (row[i] >= 1) {
      b = bitWrite(b, 7 - i , 1);
    } else {
      b = bitWrite(b, 7 - i , 0);
    }
  }
  PORTD = b;
}

//function that indiviusally lightds up each row
void blinkLights(){
  for (int thisRow = 0; thisRow < 8; thisRow++) {
      digitalWrite(row[thisRow], LOW);
      //arrayToPort(pixels[thisRow]);
      arrayToPortHalf(pixels[thisRow]);
      delayMicroseconds(50);
      clearPort();
      digitalWrite(row[thisRow], HIGH);
    }

    for (int thisRow = 0; thisRow < 8; thisRow++) {
      digitalWrite(row[thisRow], LOW);
      arrayToPort(pixels[thisRow]);
      //arrayToPortHalf(pixels[thisRow]);
      delayMicroseconds(700);
      clearPort();
      digitalWrite(row[thisRow], HIGH);
    }
}

//shifts an array by one
void shiftOnce(int row){
  int tempVal = pixels[row][0]; 
  for(int i = 0; i < 7; i++){
    pixels[row][i] = pixels[row][i+1];
  }
  pixels[row][7] = tempVal;
}

//animation for the intro page
void introPage(){
  for(int i =0; i< 8; i++) {
    for(int j = 0; j < 8;j++){
      pixels[i][j] = introPattern[i][j];
    }
  }

  for(int i = 0; i< 8; i++) {
    for(int w =0; w< 8; w++) {
      shiftOnce(w);
    }
    for(int j = 0; j < 60; j++){
        blinkLights();
        if(playerMoved()) {
          clearScreen();
        }
      }
  }
  
}

//Draws blinking inverted numbers 1,2,3
void drawScore() {
  clearScreen();
  int temp = 0;
  for(int i = 7; i > -1; i--){
    for(int j = 7; j > -1; j--){
      temp = temp + 1;
      if(temp < score){
        pixels[i][j] = 2;
      } else if (temp < highscore) {
        pixels[i][j] = 1;
      }
      else {
        return;
      }
      for(int j = 0; j < 20; j++){
        blinkLights();
      }
    }
  }
}

void scoreUpdate(){
  if (score > highscore) {
    highscore = score;
  }
  score = 0;
}

void resetGame(){
  if (playerLives <= 0){
    pixels[playerX][playerY] = 0;
    floodFill(playerX,playerY,2);
    clearScreen();
    resetMonsterArray();
    startGame();
    drawScore();
    scoreUpdate();
    blinkPage();
  }
}

//determins wether or not to show the intro animation
void gameinit(){
  if (playerX != 0 && playerY != 0){
    gameStarted = true;
  }
  if (gameStarted == false){
    introPage();
  }
}

//checks for when the player moves in the intro to start the animation
bool playerMoved(){
  if(playerX != 0 && playerY != 0){
    return true;
  }
  return false;
}

//Resets the lives in the game
void startGame(){
  playerLives = 3;
}

//Finds the next daed monster in the array to wake up
int wakeUpNextEnemy(){
  for(int i = 0; i < numOfMon; i++) {
    if (monsterArray[i].alive == false){
      return i;
    }
  }
  return -1;
}



//places an enemy on the boards borders
void placeEnemy(){
  //this determines the wall that it starts out of: 0 = top, 1 = right, 2 = bottom, 3 = left.
  randNumber = random(0, 4);
  int enemyToWake = wakeUpNextEnemy();
  if (enemyToWake != -1) {
    monsterArray[enemyToWake].alive = true;
    monsterArray[enemyToWake].sizeOf = random(1,4);
    monsterArray[enemyToWake].right = random(0,2);
    monsterArray[enemyToWake].steps = 0;
    
    if (randNumber == 0) {
      monsterArray[enemyToWake].x = random(0,8);
      monsterArray[enemyToWake].y = 7;
      monsterArray[enemyToWake].Dir = String("d");
    } else if(randNumber == 1){
      monsterArray[enemyToWake].x = 0;
      monsterArray[enemyToWake].y = random(0,8);
      monsterArray[enemyToWake].Dir = String("r");
    }
    else if(randNumber == 2){
      monsterArray[enemyToWake].x = random(0,8);
      monsterArray[enemyToWake].y = 0;
      monsterArray[enemyToWake].Dir = String("u");
    } else {
      monsterArray[enemyToWake].x = 7;
      monsterArray[enemyToWake].y = random(0,8);
      monsterArray[enemyToWake].Dir = String("l");
    }
  }
}

//updates the player
void updatePlayer(){
  readSensors();
}

//determines weather the mosnters specific locaiton is valid
bool validLoc(int x, int y) {
  if (x >= 0 && x <= 7 && y >= 0 && y <= 7) {
    return true;
  }
  return false;
}

//Kills an individual monster
void killMonster(int mon){
  monsterArray[mon].alive = false;
  //add statistic to create new monsters
}

//Kills a ll the mosters in the array
void resetMonsterArray(){
  for(int i = 0; i < numOfMon; i++) {
    monsterArray[i].alive = false;
  }
}

//This flashes the lights displayed on the board. If it is a two it will blink them twice. If 1 it will do it once
//if zero it will not blink them at all
void blinkPage() {         
  for (int i = 0; i < 4; i++){
    oneScreen();
    drawLives(0);
    for(int j = 0; j < 70; j++){
      blinkLights();
    }
    clearScreen();
    drawLives(1);
    for(int w = 0; w < 70; w++){
      blinkLights();
    }
  }
}


//Checks wethere the player is being hit by one of the moving pieces.
void collissionWithPlayer(int x, int y) {
  if (x == playerX && y == playerY){
    if (playerLives == 1) {
      playerLives = playerLives - 1;
    } else {
      playerLives = playerLives - 1;
      clearScreen();
      blinkPage();
      resetMonsterArray();
    }
  }
}

//depending on the direction that it is moving, it determines how to draw in direction.
void drawMonster(int mon){
  bool shouldKill = true; // if nothing is drawn it will be rest to dead at the end.
  //checks each of the locations where the moster exists, and determines weather to draw it or not
  for (int i = 0; i < monsterArray[mon].sizeOf ; i++){
    if (monsterArray[mon].right) {
      if (monsterArray[mon].Dir == String("r")){
        //draw left
        if (validLoc(monsterArray[mon].x - i, monsterArray[mon].y)) {
          shouldKill = false;
          collissionWithPlayer(monsterArray[mon].x - i,monsterArray[mon].y);
          pixels[monsterArray[mon].x - i][monsterArray[mon].y] = 1;
        }
      } else {
        //draw right
        if (validLoc(monsterArray[mon].x + i, monsterArray[mon].y)) {
          shouldKill = false;
          collissionWithPlayer(monsterArray[mon].x + i,monsterArray[mon].y);
          pixels[monsterArray[mon].x + i][monsterArray[mon].y] = 1;
        }
      }

    } else {
      if (monsterArray[mon].Dir == String("d")){
        //draw up
        if (validLoc(monsterArray[mon].x, monsterArray[mon].y + i)) {
          shouldKill = false;
          collissionWithPlayer(monsterArray[mon].x,monsterArray[mon].y+i);
          pixels[monsterArray[mon].x][monsterArray[mon].y + i] = 1;
        }
      } else {
        //draw down
        if (validLoc(monsterArray[mon].x, monsterArray[mon].y - i)) {
          shouldKill = false;
          collissionWithPlayer(monsterArray[mon].x,monsterArray[mon].y-i);
          pixels[monsterArray[mon].x][monsterArray[mon].y - i] = 1;
        }
      }
    }
  }
  if (shouldKill){
    killMonster(mon);
  }

}


//Updates an individual enemy, and moves the head of their position
void updateEnemy(int mon) {
  //add to the origin
  if (monsterArray[mon].Dir == String("u")){
    monsterArray[mon].y = monsterArray[mon].y + 1;
  } else if (monsterArray[mon].Dir == String("r")){
    monsterArray[mon].x = monsterArray[mon].x + 1;
  } else if (monsterArray[mon].Dir == String("d")){
    monsterArray[mon].y = monsterArray[mon].y - 1; 
  } else {
    monsterArray[mon].x = monsterArray[mon].x -1;
  }
}
//Iterated over the enemie array and upadtes each move along with drawing them on the board.
void updateEnemies() {
  for(int i = 0; i < numOfMon; i++) {
    if(monsterArray[i].alive) {
      drawMonster(i);
      updateEnemy(i);
    }
  }
}

//THis reads the analog inpts and converts them to a range from 0 to 7. Got this from a arduino tutorial
void readSensors() {
  pixels[playerX][playerY] = 0;
  playerX = 7 - map(analogRead(A0), 0, 1023, 0, 7);
  playerY = map(analogRead(A1), 0, 1023, 0, 7);
  pixels[playerX][playerY] = 2;
}

//THis updates the pixels, in this case the only thing being updated constantly is the enemies
void updatePixels(){
  updateEnemies();
}

//All methods that can be added when the screen is refreshed
void refreshScreen() {
    blinkLights();
}


//Very modified version inspired from http://www.geeksforgeeks.org/flood-fill-algorithm-implement-fill-paint/
//recursevly perfoms flood fill and draws it.
void floodFillHelper( int x, int y, int prevC, int newC)
{
    // Base cases
  blinkLights();
  if (x < 0 || x >= 8 || y < 0 || y >= 8){
        return;
  }
  if (pixels[x][y] != prevC){
        return;
  }
 
  // Replace the color at (x, y)
  pixels[x][y] = newC;

  // Recur for north, east, south and west
  floodFillHelper(x+1, y, prevC, newC);
  floodFillHelper( x-1, y, prevC, newC);
  floodFillHelper( x, y+1, prevC, newC);
  floodFillHelper( x, y-1, prevC, newC);
}

//Recursive function that calls on helper to perfomr flood fil 
void floodFill(int x, int y, int newC)
{
    int prevC = pixels[x][y];
    floodFillHelper(x, y, prevC, newC);
}

//Helper function for debugginf that prints out the state of the board.
void printPixels(){
  for(int i=0; i<8; i++) {
    for(int j=0; j<8; j++){
           Serial.print(pixels[i][j]);
    }
   Serial.println("");
   }
  
   Serial.println("");
}

