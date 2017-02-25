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


// 2-dimensional array of pixels:
int pixels[8][8];

long previousMillis = 0; 
long interval = 500;           // interval at which to update (milliseconds)

//game Varibles

//struct for a monster
const int numOfMon = 10;

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

int playerX = 5;
int playerY = 5;
int playerLives = 3;

void setup() {
  //Serial.begin(9600);
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
  //pixels[0][0] = 1;
  //pixels[7][0] = 1;
  //pixels[0][5] = 2;
//  monsterArray[0].alive = true;
//  monsterArray[0].x = 5;
//  monsterArray[0].y = 5;
//  monsterArray[0].Dir = String("r");
//  monsterArray[0].sizeOf = 3;
  
}

void loop() {

  unsigned long currentMillis = millis();

  //inside should update the board evry interval
  if(currentMillis - previousMillis > interval) {
    // save the last time you update 
    previousMillis = currentMillis; 
    placeEnemy(); 
    clearScreen();
    updateEnemies();
    
  
  }
  //Serial.println(monsterArray[0].right);
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

//Tunrs the port to all Lows
void clearPort() {
  PORTD = B00000000;
}

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

bool checkCollisions(){
  for(int i = 0; i < numOfMon; i++) {
    //monsterArray[i]    
  }
}

void resetGame(){
  
}

void startGame(){
  
}

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

void updatePlayer(){
  readSensors();
}

bool validLoc(int x, int y) {
  if (x >= 0 && x <= 7 && y >= 0 && y <= 7) {
    return true;
  }
  return false;
}

void killMonster(int mon){
  monsterArray[mon].alive = false;
  //add statistic to create new monsters
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
          pixels[monsterArray[mon].x - i][monsterArray[mon].y] = 1;
        }
      } else {
        //draw right
        if (validLoc(monsterArray[mon].x + i, monsterArray[mon].y)) {
          shouldKill = false;
          pixels[monsterArray[mon].x + i][monsterArray[mon].y] = 1;
        }
      }

    } else {
      if (monsterArray[mon].Dir == String("d")){
        //draw up
        if (validLoc(monsterArray[mon].x, monsterArray[mon].y + i)) {
          shouldKill = false;
          pixels[monsterArray[mon].x][monsterArray[mon].y + i] = 1;
        }
      } else {
        //draw down
        if (validLoc(monsterArray[mon].x, monsterArray[mon].y - i)) {
          shouldKill = false;
          pixels[monsterArray[mon].x][monsterArray[mon].y - i] = 1;
        }
      }
    }
  }
  if (shouldKill){
    killMonster(mon);
  }

}

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
//could possibly draw at this stage
void updateEnemies() {
  for(int i = 0; i < numOfMon; i++) {
    if(monsterArray[i].alive) {
      updateEnemy(i);
      drawMonster(i);
    }
  }
}

void readSensors() {
  pixels[playerX][playerY] = 0;
  playerX = 7 - map(analogRead(A0), 0, 1023, 0, 7);
  playerY = map(analogRead(A1), 0, 1023, 0, 7);
  pixels[playerX][playerY] = 2;
}

int convertX(int x){
  return 8 - (1 + x);
}

void updatePixels(){
  updateEnemies();
  //checkCollisions();
}

void refreshScreen() {
    //updatePixels();
    blinkLights();
    //clearScreen();
}




