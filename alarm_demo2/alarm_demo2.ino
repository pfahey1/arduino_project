/*
This is a simple alarm system 
that is activated by pressing a button
and  deactivated when you press the correct code
*/
#include <Password.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

Password password = Password( "1234" );

const byte ROWS = 4; // Four rows
const byte COLS = 3; //  columns

// Define the Keymap
char keys[ROWS][COLS] = {
{'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'*','0','#'}
};

byte rowPins[ROWS] = { 5, 4, 3, 2 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 8, 7, 6 };

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

const int buttonPin_door =9;    // Magnetic door contact
const int buttonPin_on=10;       // Power system  button
const int ledPin_alarm= 13;      // Alarm Led
int run =0;
int buttonState_door = 0;         // Door initial state
int previousButtonState_door=1;   // previous state of the door
int buttonState_on= 0;            // Power system  button initialstate 
unsigned long door_open_time = 0;
int val2=0;


void setup(){

  pinMode(buttonPin_door, INPUT);  
  pinMode(buttonPin_on,INPUT);
  pinMode(ledPin_alarm ,  OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  keypad.addEventListener(keypadEvent); 
  keypad.setDebounceTime(250);           //Keypad button time


} 

void loop(){
  previousButtonState_door = buttonState_door ;   // save what it was last time

  buttonState_door = digitalRead(buttonPin_door);   // get new state
  buttonState_on= digitalRead(buttonPin_on);

    

//conditions for  activation the  system

  if (buttonState_on == HIGH   ) {
    lcd.setCursor(0,0);
    lcd.print("run         ");
        lcd.setCursor(0,1);
    lcd.print("                                  ");

    run = HIGH;
  }
  
  
   if (run == HIGH  && buttonState_door == LOW && previousButtonState_door == HIGH)
  {
  if(val2 != HIGH)    
    {
    door_open_time = millis ();
    val2=HIGH;
    }
  
  
  if ((val2 == HIGH) || (millis () - door_open_time) >= 10000 ){
      digitalWrite(ledPin_alarm,HIGH);
      
    lcd.setCursor(0,0);
    lcd.print("alarm...          ");
    lcd.setCursor(0,1);
    lcd.print("                                  ");
  
}
  }
     keypad.getKey();
}


void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  case PRESSED:

    switch (eKey){
    case '#': 
      guessPassword(); 
      break;
  
    default:
      password.append(eKey);
      case 'Α': 
      run = HIGH;
    }
  }
}
void guessPassword(){

  if (password.evaluate()){
    digitalWrite(ledPin_alarm,LOW);
    val2=LOW;
    run = LOW;
    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("CORRECT PASSWORD ");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("system          ");
    lcd.setCursor(0,1);
    lcd.print("off       ");

    password.reset(); 
  }
  else{

    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("WRONG PASSWORD ");
    password.reset(); 
  }
}

