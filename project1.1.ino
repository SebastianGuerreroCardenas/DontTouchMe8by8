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
long interval = 1000;           // interval at which to update (milliseconds)

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
   char Dir; // the direction which its moving
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
  randomSeed(129);
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(row[thisPin], OUTPUT);
    
    digitalWrite(row[thisPin], HIGH);
  }

  clearScreen();
  pixels[0][0] = 1;
  pixels[0][1] = 2;
}

void loop() {

  unsigned long currentMillis = millis();

  //inside should update the board evry interval
  if(currentMillis - previousMillis > interval) {
    // save the last time you update 
    previousMillis = currentMillis;  
  
  }

  Serial.println(playerX);
  Serial.println(playerY);
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
    monsterArray[enemyToWake].right = random(0,1);
    monsterArray[enemyToWake].steps = 0;
    
    if (randNumber == 0) {
      monsterArray[enemyToWake].x = random(0,8);
      monsterArray[enemyToWake].y = 7;
      monsterArray[enemyToWake].Dir = "d";
    } else if(randNumber == 1){
      monsterArray[enemyToWake].x = 0;
      monsterArray[enemyToWake].y = random(0,8);
      monsterArray[enemyToWake].Dir = "l";
    }
    else if(randNumber == 2){
      monsterArray[enemyToWake].x = random(0,8);
      monsterArray[enemyToWake].y = 0;
      monsterArray[enemyToWake].Dir = "u";
    } else {
      monsterArray[enemyToWake].x = 7;
      monsterArray[enemyToWake].y = random(0,8);
      monsterArray[enemyToWake].Dir = "r";
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

void killMonster(){
  
}

void drawMonster(){
  
}

void updateEnemy(monster mon) {
  
}

void updateEnemies() {
  for(int i = 0; i < numOfMon; i++) {
    updateEnemy(monsterArray[i]);
  }
}

void readSensors() {
  pixels[playerX][playerY] = 0;
  playerX = 7 - map(analogRead(A0), 0, 1023, 0, 7);
  playerY = map(analogRead(A1), 0, 1023, 0, 7);
  pixels[playerX][playerY] = 2;
}

void updatePixels(){
  updatePlayer();
  updateEnemies();
  checkCollisions();
}

void refreshScreen() {
    updatePixels();
    blinkLights();
}




