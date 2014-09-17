#include <Wire.h>
#include <avr/pgmspace.h> //to use PROGMEM

#define LEDMAX 210
#define LEDMIN 0
#define LEDSTEP 5
#define LEDPIN 9
#define DELAY 20
#define SLAVE_ADDRESS 0x04

int number = 0;
int state = 0;
 
double temp;

/*
 * Array for LED Gamma Correction.
 * See Reference 2 for more information.
*/
const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

/*
 * Get real LED value after gamma correction.
 */
int getLedValue(int value)
{
  if ((value > 255) || (value <0)) return 0;
  return pgm_read_byte(&gamma[value]);
}

/*
 * [Public] Set a LED bightness, value in 0..255 . Gamma correction will be done automically.
 */
void setLedValue(int value)
{
  analogWrite(LEDPIN, getLedValue(value));  
}
 
void setup() {
 pinMode(13, OUTPUT);
 
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
}
 
void loop() {
 delay(100);
 temp = GetTemp();
}
 
// callback for received data
void receiveData(int byteCount){
 
 while(Wire.available()) {
  number = Wire.read();
 
  if (number == 1){
   if (state == 0){
    digitalWrite(13, HIGH); // set the LED on
    state = 1;
   } else{
    digitalWrite(13, LOW); // set the LED off
    state = 0;
   }
  }
 
  if(number==2) {
   number = (int)temp;
  }
 }
}
 
// callback for sending data
void sendData(){
 Wire.write(number);
}
 
// Get the internal temperature of the arduino
double GetTemp(void)
{
  for(int fadeValue = LEDMIN ; fadeValue <= LEDMAX; fadeValue += LEDSTEP) { 
    setLedValue(fadeValue);       
    delay(DELAY);                            
  } 

  for(int fadeValue = LEDMAX ; fadeValue >= LEDMIN; fadeValue -= LEDSTEP) { 
    setLedValue(fadeValue);        
    delay(DELAY);                            
  } 
}
