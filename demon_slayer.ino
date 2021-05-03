#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 12, 4, 5, 6, 7);

// Cursor Byte
  byte customChar[]={
    
    B00100,
    B00100,
    B00100,
    B01110,
    B01110,
    B11111,
    B11111,
    B01010
    
  };

// Target Left Byte
  byte targetLeft[] = {
    
    B00000,
    B00000,
    B00001,
    B00011,
    B00011,
    B00001,
    B00000,
    B00000
    
  };

// Target Center Byte
  byte targetCenter[] = {
    
    B01110,
    B11011,
    B10101,
    B01110,
    B01110,
    B10101,
    B11011,
    B01110
    
  };
// Target Right Byte

  byte targetRight[] = {
    
    B00000,
    B00000,
    B10000,
    B11000,
    B11000,
    B10000,
    B00000,
    B00000
    
};

// Global variables

  int hit = 0;
  int condition = 1;

  long unsigned int previousTime = 0;
  long unsigned int delayTime = 150;
  long unsigned int totalTime = 0;

  int targetPosition;
  int countHits = 0;
  int countMisses = 0;

  int timer = 0;

  
void setup() {

  lcd.begin(16,2);
  Serial.begin(9600);

//Creating characters

  lcd.begin(16,2);
  lcd.createChar(8,customChar);
  lcd.createChar(9,targetLeft);
  lcd.createChar(10,targetCenter);
  lcd.createChar(11,targetRight);

// Setting up the target

  randomSeed(analogRead(1));
  targetPosition = random(0,5);
  
  lcd.setCursor(3*targetPosition,0);
  lcd.write((uint8_t)9);
  lcd.setCursor(1+3*targetPosition,0);
  lcd.write((uint8_t)10);
  lcd.setCursor(2+3*targetPosition,0);
  lcd.write((uint8_t)11);

// Creating the interrupt

  attachInterrupt(0,buttonPressed,RISING);

// Setting up the Piezo

  pinMode(9,OUTPUT);
}
void loop() {
  
  int potValue;
  int cursorPosition;

  //While loop that contains the game's basic functionality.
  
  while (condition == 1) {
    
  potValue = analogRead(A0);

  //If statement that controls cursor position depending on potentiometer value
  
   if (potValue > 800) {
    cursorPosition = 4;
    hit = 5;
   }
   else if (potValue > 600) {
    cursorPosition = 3;
    hit = 4; 
   }
   else if (potValue > 400) {
    cursorPosition = 2;
    hit = 3; 
   }
   else if (potValue > 200) {
    cursorPosition = 1;
    hit = 2; 
   }
   else {
    cursorPosition = 0;
    hit = 1;
   }

//Clearing previous cursors

  for (int i = 1; i < 6; i++){
    lcd.setCursor(3*i-2,1);
    lcd.print(" ");
  }  

//Set cursor position

  lcd.setCursor(cursorPosition*3+1,1);
  lcd.write((uint8_t)8);
  delay(100);
  timer++;
  Serial.println(timer);

//Timer of 2 seconds, turns off the condition that lets the loop run.

  if (timer > 20){
    
    condition = 0;
  }
  
  }

//If statement that resets the loop

  if (condition == 0) {
    
    lcd.clear();

    //Case when time reaches 2 seconds, plays audio and displays miss count.

    if (timer > 20){
      
      countMisses++;
      
      lcd.print("Out of time!");
      lcd.setCursor(0,1);
      lcd.print("Total misses: ");
      lcd.setCursor(13,1);
      lcd.print(countMisses);
      tone(9,500,500);
      
    }

    //Case when target is hit, plays audio and displays current score.

    else if (cursorPosition == targetPosition){
      
      countHits++;
      
      lcd.print("Hit!");
      lcd.setCursor(0,1);
      lcd.print("Total hits: ");
      lcd.setCursor(11,1);
      lcd.print(countHits);
      tone(9,1000,500);
      
    }

    //Case when target is not hit, plays audio and displays miss count.
    
    else{
      
      countMisses++;
      
      lcd.print("Miss!");
      lcd.setCursor(0,1);
      lcd.print("Total misses: ");
      lcd.setCursor(13,1);
      lcd.print(countMisses);
      tone(9,500,500);
      
    }
    
    delay(1000);
    lcd.clear();

    //Resets miss count and score after reachin 5 on the miss count. Displays Game OVER and plays audio.

    if (countMisses > 4){
      lcd.print("GAME OVER!");
      lcd.setCursor(0,1);
      lcd.print("Final score: ");
      tone(9,300,500);
      lcd.setCursor(12,1);
      lcd.print(countHits);
      countHits = 0;
      countMisses = 0;
      delay(5000);
      lcd.clear();
    }

    //Resets the condition that runs the while loop that is turned off by the button.
    
    condition = 1;

    //Creates a new target
    
    targetPosition = random(0,5);
  
    lcd.setCursor(3*targetPosition,0);
    lcd.write((uint8_t)9);
    lcd.setCursor(1+3*targetPosition,0);
    lcd.write((uint8_t)10);
    lcd.setCursor(2+3*targetPosition,0);
    lcd.write((uint8_t)11);

    //Resets the timer
    
    timer = 0;
  }
  
}

    //Interrupt function triggered by a button. Debounced.

void buttonPressed(){
  
  totalTime = millis();
  if (totalTime - previousTime > delayTime) {

    condition = 0;
    
  }
  previousTime = millis();
}
