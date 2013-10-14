

// constants won't change. Used here to 
// set pin numbers:
const int CLOCK_PIN =  13;      // the number of the LED pin
const int RESET_PIN = 12;
const int DATA_PINS = 30;
const int ADDRESS_LOW = 2;
const int ADDRESS_HIGH = 40;

int clockState = LOW;             // clockState used to set the CLK

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 10;           // clock interval (milliseconds)
long previousMillis = 0;        // will store last time LED was updated


void setup() {
  
  int addressPin;
 
  Serial.begin(9600);
  
  //Set the CLK pin to OUTPUT
  pinMode(CLOCK_PIN, OUTPUT); 
  
  //Set the address pins to INPUT
  for (addressPin=ADDRESS_LOW; addressPin<ADDRESS_LOW+8; addressPin++) {
    pinMode(addressPin, INPUT);  
  }
    for (addressPin=ADDRESS_HIGH; addressPin<ADDRESS_HIGH+8; addressPin++) {
    pinMode(addressPin, INPUT);  
  }
  
  //Set the DATA pins to output and set to LOW so default operation is NOP
  for (int data=DATA_PINS; data < 9; data++) {
    pinMode(data, OUTPUT);
    digitalWrite(data, LOW);
  }
  
  Serial.println("Pins set up");
  
  //Reset the z80, leaving it HIGH
  pinMode(RESET_PIN, OUTPUT);
  for (int resetCount = 0; resetCount <3; resetCount++) {
    digitalWrite(RESET_PIN, LOW);
    delay(500);
    digitalWrite(RESET_PIN, HIGH);
    delay(500);
  }    
 
    Serial.println("Reset set");
  
}

void loop()
{

  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (clockState == LOW)
      clockState = HIGH;
    else
      clockState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(CLOCK_PIN, clockState);
    displayAddress();
    
  }
 
}

  int digitalBinaryPinsToInt(int startPin) {
    char binary[8] = {'0','0','0','0','0','0','0','0'};
    int position = 7;
    int binaryDigit = 1;
    int value = 0;
    for (int pin = startPin; pin < startPin+8; pin++) {
      
      if (digitalRead(pin) == HIGH) {
        value = value + binaryDigit;
        binary[position] ='1';
      }
      binaryDigit = binaryDigit * 2;
      position--;
    }
    Serial.println();
    for (int i=0; i<8;i++) {
      Serial.print(binary[i]);
    }
    Serial.print(" = ");
    Serial.println(value);
    return value;
  }


  void displayAddress()
  {

    digitalBinaryPinsToInt(2);

  }
