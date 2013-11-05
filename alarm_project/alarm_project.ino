

// Uses a PIR sensor to detect movement, buzzes a buzzer
// more info here: http://blog.makezine.com/projects/pir-sensor-arduino-alarm/
// email me, John Park, at jp@jpixl.net
// based upon:
// PIR sensor tester by Limor Fried of Adafruit
// tone code by michael@thegrebs.com

// include the library code:

#include <LiquidCrystal.h>
#include <Password.h> 
#include <Keypad.h> 

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
Password password = Password( "1234" );

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

boolean disarm = password.evaluate();

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
void playTone(long duration, int freq) 
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
	  case '*': checkPassword(); break;
          
	  case '#': password.reset(); break;
	  default: password.append(eKey);
        }
  }
}

void checkPassword()
{
  
  if (password.evaluate())
  {
    lcd.print("Success!");
    delay(3000);
    lcd.clear();
    lcd.print("System Armed"); 
   
    //Add code to run if it works
    while (password.evaluate())
    
    {
      setOffSystem();
      
      if(disarm)
      {
        break;
      }
    }
        
  }    
  
  else
  {
    lcd.print("Wrong Password");
    delay(5000);
    lcd.clear();
    //add code to run if it did not work
  }
}

void setOffSystem()
{
   val = digitalRead(inputPin);  // read input value
    
  if (val == HIGH) 
  {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    playTone(300, 160);
    delay(150);

    if (pirState == LOW) 
    {
      // we have just turned on
      lcd.clear();
      lcd.print("Motion detected!");
      
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } 
  
  else 
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
  }       
}

