#include <OneWire.h>;

int DS18S20_Pin = 13;
int score = 0;
int highscore = 0;
int counter = 0;
int switchState = 0;
int countdownTimer = 18;
float maxTemperature = 0;
float normalTemperature = 0;
unsigned long time;
boolean gameStatus = false;

OneWire ds(DS18S20_Pin);

// Segments
int a = A5; // Replacement for port 0 and 1
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;
int g = 7;

/*
 * Digit 1-4 = Score-display
 * Digit 5-8 = Highscore-display
 */
int digit1 = 11;
int digit2 = 10;
int digit3 = 9;
int digit4 = 8; 

int digit5 = A3;
int digit6 = A2;
int digit7 = A1;
int digit8 = A0;

void setup() {
  for(int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(12);
  if(switchState == HIGH) {
    maxTemperature = 0;
    score = 0;
    countdownTimer = 18;
    gameStatus = true;
  } 
  while(gameStatus) {
    game();
    timer();
    countdownDigit();
  }
  if(!gameStatus) {
    highScoreDigit();
  }
}

/*
 * The game calls for the methods it needs to run
 */
void game() {
  counter++;
  timer();
  displayChooser(); 
  timer();
  maxTempCalculated();
  scoreBasedOnHeat();
  timer();
}

/*
 * Timer to calculate and decrease the countdown for the current session
 */
void timer() {
  int temp = 0;
  time = millis();
  temp = time%1000;  
  if(temp == 0) {
    normalTemperature = getTemp();
    countdownTimer--;
  }
  if(countdownTimer == 0) {
    checkIfHighscore();    
    gameStatus = false;
  }
}

/*
 * Imported code from OneWire
 */
float getTemp() {
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -2;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -3;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}

/*
 * Checks if the temperature is the highest the program has seen for the current session
 */
void maxTempCalculated() {
  if(normalTemperature >= maxTemperature) {
    maxTemperature = normalTemperature;
  }
}

/*
 * A way to give score dynamicly without using a constant temperature (in case of irregularities between the players)
 */
void scoreBasedOnHeat() {
  // If temperature is above or equal maximum temperature (+ 30pts)
  if((normalTemperature >= maxTemperature) && (counter == 50)) {
    score += 30;
    counter = 0;
  }
  // If temperature is below maximum temperature (+ 10 pts)
  if((normalTemperature < maxTemperature) && (counter == 50)) {
    score += 10;
    counter = 0;
  }
}

/*
 * Chooses which display it's going to show numbers on
 */
void displayChooser() {
  // Highscore-display
  if((checkIfHighscore()) && (!gameStatus)) {
    highScoreDigit();
  } 
  else {
    // Score-display
    scoreDigit();
  }
}

/*
 * Method that decides how many digit-displays it's going to use on the highscore-LCD
 */
void highScoreDigit() {
  if(score >= 1000 && (!gameStatus)) {
    fourDisplays(5);
  }
  if(score >= 100 && (!gameStatus)) {
    threeDisplays(5);
  }
  if(score >= 10 && (!gameStatus)) {
    twoDisplays(5);
  }
  if(score <= 9 && (!gameStatus)) {
    oneDisplay(5);
  }
}

/*
 * Showing the countdown on the highscore display
 */
void countdownDigit() {
  countdownDisplay(5);
}

/*
 * Method that decides how many digit-displays it's going to use on the score-LCD
 */
void scoreDigit() {
  if(score >= 1000) {
    fourDisplays(1);
  }
  if(score >= 100) {
    threeDisplays(1);
  }
  if(score >= 10) {
    twoDisplays(1);
  }
  if(score <= 9) {
    oneDisplay(1);
  }	
}

/*
 * Checks if the current score is higher than the highscore on the machine
 */
boolean checkIfHighscore() {
  if(score >= highscore) {
    highscore = score;
    return true;
  }
  return false;
}

void resetDigits() {
  /*
   * To turn off a segment set it to LOW
   * To turn off a the digit set it to HIGH
   */
  // All segments off
  digitalWrite(A5, LOW);
  for(int i = 2; i < 8; i++) {
    digitalWrite(i, LOW);
  }

  // Score-display
  for(int j = 8; j < 12; j++) {
    digitalWrite(j, HIGH);
  }

  // Highscore-display
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);  
}

/*
 * Digit 1 to 4 is the score-display | Digit 5 to 8 is the highscore-display
 */
void switchDisplay(int x) {
  switch(x) {
  case 1: 
    digitalWrite(digit1, LOW); 
    break;
  case 2: 
    digitalWrite(digit2, LOW); 
    break;
  case 3: 
    digitalWrite(digit3, LOW); 
    break;
  case 4: 
    digitalWrite(digit4, LOW); 
    break;
  case 5:
    digitalWrite(digit5, LOW);
    break;
  case 6:
    digitalWrite(digit6, LOW);
    break;
  case 7:
    digitalWrite(digit7, LOW);
    break;
  case 8:
    digitalWrite(digit8, LOW);
    break;
  }
}

/*
 * Method to choose which number it's going to show in the display
 */
void switchDigit(int x) {
  switch(x) {
  case 0:
    digitZero();
    break;
  case 1: 
    digitOne();
    break;
  case 2: 
    digitTwo();
    break;
  case 3: 
    digitThree();
    break;
  case 4: 
    digitFour();
    break;
  case 5: 
    digitFive();
    break;
  case 6: 
    digitSix();
    break;
  case 7: 
    digitSeven();
    break;
  case 8: 
    digitEight();
    break;
  case 9: 
    digitNine();
    break;
  }
}

void digitZero() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void digitOne() {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void digitTwo() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void digitThree() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void digitFour() {
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void digitFive() {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void digitSix() {
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void digitSeven() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void digitEight() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void digitNine() {
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

/*
 * Method to show a number on the display
 */
void oneDisplay(int x) {
  switchDisplay(x);
  switchDigit(score);
  resetDigits();
}

/*
 * Method to show 2 numbers on the display at the same time
 */
void twoDisplays(int x) {
  int firstDigit = score - (10*(score/10));
  int secondDigit = (score - firstDigit)/10;

  switchDisplay(x);
  switchDigit(firstDigit);
  resetDigits();
  x++;
  switchDisplay(x);
  switchDigit(secondDigit);
  resetDigits();
}

/*
 * Method to show 3 numbers on the display at the same time
 */
void threeDisplays(int x) {
  int firstDigit = (score - ((score/100)*100))%10;
  int secondDigit = (score - ((score/100)*100))/10;
  int thirdDigit = score/100;

  switchDisplay(x);
  switchDigit(firstDigit);
  resetDigits();
  x++;
  switchDisplay(x);
  switchDigit(secondDigit);
  resetDigits();
  x++;
  switchDisplay(x);
  switchDigit(thirdDigit);
  resetDigits();
}

/*
 * Method to show all 4 numbers on the display at the same time
 */
void fourDisplays(int x) {
  int firstDigit = 0;
  int secondDigit = 0;
  int thirdDigit = 0;
  int fourthDigit = 0;
  if(checkIfHighscore() && (!gameStatus)) {
    firstDigit = (highscore - ((highscore/1000)*1000))%10;
    secondDigit = (highscore - ((highscore/1000)*1000)/10);
    thirdDigit = (highscore - ((highscore/1000)*1000))/100;
    fourthDigit = highscore/1000;
  }
  else {
    firstDigit = (score - ((score/1000)*1000))%10;
    secondDigit = (score - ((score/1000)*1000)/10);
    thirdDigit = (score - ((score/1000)*1000))/100;
    fourthDigit = score/1000;
  }
  switchDisplay(x);
  switchDigit(firstDigit);
  resetDigits();
  x++;
  switchDisplay(x);
  switchDigit(secondDigit);
  resetDigits();
  x++;
  switchDisplay(x);
  switchDigit(thirdDigit);
  resetDigits();
  x++;
  switchDisplay(x);
  switchDigit(fourthDigit);
  resetDigits();
}

/*
 * Method to show the countdown on the highscore-display
 */
void countdownDisplay(int x) {
  int firstDigit = 0;
  int secondDigit = 0;

  if(countdownTimer >= 10) {
    firstDigit = countdownTimer - (10*(countdownTimer/10));
    secondDigit = (countdownTimer - firstDigit)/10;
  }
  else {
    firstDigit = countdownTimer;
  }
  switchDisplay(x);
  switchDigit(firstDigit);
  resetDigits();
  if(countdownTimer >= 10) {
    x++;
    switchDisplay(x);
    switchDigit(secondDigit);
    resetDigits();
  }
}



