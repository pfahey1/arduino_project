#include <Keypad.h>
char* secretCode = "1234";
int position = 0;
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[rows] = {5,4,3,2};
byte colPins[cols] = {8,7,6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int redPin = 13;
int greenPin = 12;
int pirState = LOW;  
int val = 0; 
int inputPin = 9; 
int pinSpeaker = 10;
void setup()
{
pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(pinSpeaker, OUTPUT);
setLocked(true);
Serial.begin(9600);
}
void loop()
{
  val = digitalRead(inputPin);
char key = keypad.getKey();
if (key == '*' || key == '#')
{
position = 0;
setLocked(true);
}
if (key == secretCode[position])
{
position ++;
}
if (position == 4)
{
setLocked(false);
}
delay(100);
}
void setLocked(int locked)

{
if (locked && val == LOW)
{ 
digitalWrite(redPin, LOW);
digitalWrite(greenPin, LOW);
Serial.println(val);
 playTone(0, 0);
}
else if (val == HIGH)
{
  
digitalWrite(redPin, HIGH);
digitalWrite(greenPin, LOW);
 playTone(300, 160);
//Serial.println(val);
 
}
}

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
