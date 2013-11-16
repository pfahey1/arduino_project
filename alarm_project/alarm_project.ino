//Arduino Alarm Project
//Paul Fahey
//Computer Architecture

#include <LiquidCrystal.h>
#include <Password.h> 
#include <Keypad.h> 
#include <pitches.h>

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

 
int ledPin = 13;                // choose the pin for the LED
int inputPin = 9;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int pinSpeaker = 10;           //Set up a speaker on a PWM pin (digital 9, 10, or 11)


LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
Password arm_password = Password( "1234" );
Password disarm_password = Password("4321");

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() 
{
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(pinSpeaker, OUTPUT);
  
  lcd.begin(16, 2); 
  lcd.print("Type Password");
  lcd.setCursor(0,1);
  lcd.print("To Arm System");
 
 }

void loop()
{   
    
     keypad.getKey();
      
} //end of main loop

// duration in mSecs, frequency in hertz
void playTone( long duration, int freq) 
{
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    
    while (elapsed_time < duration) 
    {
        digitalWrite(pinSpeaker,HIGH);
        delayMicroseconds(period / 2);  
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
     
    }
    
}

void keypadEvent(KeypadEvent eKey)
{
  switch (keypad.getState())
  {
    case PRESSED:
	//lcd.print("Pressed: ");
        lcd.clear();
        lcd.setCursor(0,1);
	lcd.print(eKey);
	switch (eKey)
        {
	  case '*': check_armed_password(); break;
          
	  case '#': check_disarm_password(); break;
	  default: arm_password.append(eKey);
                    disarm_password.append(eKey);
        }
  }
}

void check_armed_password()
{
  
  if (arm_password.evaluate())
  {
    lcd.print("Success!");
    delay(3000);
    lcd.clear();
    lcd.print("System Armed"); 
    
    //Add code to run if it works
    while (arm_password.evaluate() )
    {
      val = digitalRead(inputPin);  // check if the input is HIGH
      
       if (val == HIGH) 
        {        
            digitalWrite(ledPin, HIGH);  // turn LED ON
          //  playTone(300, 160);   
     
            delay(150);
             lcd.clear();
              lcd.print("Motion detected!");
              delay(3000);
              lcd.setCursor(0,1);
              lcd.print("Intruder!");
              arm_password.reset();
              disarm_password.reset();
           }  
    }  
  }
  
  else
  {
    lcd.print("Wrong Password");
    delay(3000);
    lcd.clear();
    arm_password.reset();
    disarm_password.reset();
  lcd.print("Type Password");
  lcd.setCursor(0,1);
  lcd.print("To Arm System");
    //add code to run if it did not work
  }
}

void check_disarm_password()
{
  
  if (disarm_password.evaluate())
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(pinSpeaker, LOW);
    lcd.print("Success!");
    delay(3000);
    lcd.clear();
    lcd.print("System Disarmed");
   delay(3000); 
   lcd.clear();
   arm_password.reset();
    disarm_password.reset();
    lcd.print("Type Password");
  lcd.setCursor(0,1);
  lcd.print("To Arm System");
      
  }
  
  else
  {
    lcd.print("Wrong Password");
    delay(3000);
    lcd.clear();
    arm_password.reset();
    disarm_password.reset();
  lcd.print("Type Password");
  lcd.setCursor(0,1);
  lcd.print("To Disarm System");
    //add code to run if it did not work
  }
}


void setOffSystem()
{
     // read input value
    
  if (val == HIGH) 
  {      
   val = digitalRead(inputPin);    // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    playTone(300, 160);
    delay(150);
     lcd.clear();
      lcd.print("Motion detected!");
  }

   /* if (pirState == LOW) 
    {
      // we have just turned on
      lcd.clear();
      lcd.print("Motion detected!");
      
      // We only want to print on the output change, not state
      pirState = HIGH;
    }*/
   
  
 /* else 
    {
      digitalWrite(ledPin, LOW); // turn LED OFF
      playTone(0, 0);
      delay(300);    
      if (pirState == HIGH)
      {
      lcd.print("Motion finished!");
      delay(5000);
      lcd.clear();
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  } */      
}

void disarmSystem()
{
  digitalWrite(ledPin, LOW); // turn LED OFF
      playTone(0, 0);
      delay(300);    
      if (pirState == HIGH)
      {
      lcd.print("System Disarmed!");
      delay(5000);
      lcd.clear();
      // We only want to print on the output change, not state
      pirState = LOW;
      }
}

