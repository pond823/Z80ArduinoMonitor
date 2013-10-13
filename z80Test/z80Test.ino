

// constants won't change. Used here to 
// set pin numbers:
const int ledPin =  13;      // the number of the LED pin
const int resetPin = 12;
// Variables will change:
const int DATA_PINS = 30;

int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated



// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 10;           // interval at which to blink (milliseconds)
int addressPin =0;

void setup() {
  // set the digital pin as output:
   Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
  for (addressPin=2; addressPin<10; addressPin++) {
    pinMode(addressPin, INPUT);  
  }
    for (addressPin=40; addressPin<48; addressPin++) {
    pinMode(addressPin, INPUT);  
  }
  for (int data=DATA_PINS; data < 9; data++) {
    pinMode(data, OUTPUT);
    digitalWrite(data, LOW);
  }
  
  Serial.println("Pins set up");
  
  //Reset the z80, leaving it HIGH
  pinMode(resetPin, OUTPUT);
  for (int resetCount = 0; resetCount <3; resetCount++) {
    digitalWrite(resetPin, LOW);
    delay(500);
    digitalWrite(resetPin, HIGH);
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
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
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
