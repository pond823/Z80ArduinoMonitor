

// constants won't change. Used here to 
// set pin numbers:
const int CLOCK_PIN =  13;      // the number of the LED pin
const int RESET_PIN = 12;
const int DATA_PINS = 30;
const int ADDRESS_LOW = 2;
const int ADDRESS_HIGH = 40;

const int MEM_REQ = 21;
const int M1 = 20;
const int READ = 19;
const int WRITE = 18;


int clockState = LOW;             // clockState used to set the CLK

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // clock interval (milliseconds)
long previousMillis = 0;        // will store last time LED was updated
long clockCount = 0;
int memReqState = HIGH;
int m1State = HIGH;
int readState = HIGH;

// memory locations
const int MEMORY_SIZE = 8;
byte memory[] = {0xC3, 0xFF, 0xFF,0x3A,0x10, 0x0, 0x0, 0x0, 0x0 };

void setup() {
  
  int addressPin;
 
  Serial.begin(9600);
  
  //Set the CLK pin to OUTPUT
  pinMode(CLOCK_PIN, OUTPUT); 
  pinMode(MEM_REQ, INPUT);
  pinMode(M1, INPUT);
  pinMode(READ, INPUT);
  
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
    
    Serial.print("Clock counter = ");
    Serial.println(clockCount++);
    Serial.print("Address bus = ");
    int addressValue = digitalBinaryPinsToInt(ADDRESS_LOW);
    
    if (isMemReq()) {
      Serial.println("Memory Request Active");
    }
    if (isM1()) {
      Serial.println("M1 Active");
    }
    if (isRead()) {
      Serial.println("Read Active");
    }
        if (isWrite()) {
      Serial.println("Write Active");
    }
    //Simulate read from memory
    if (isMemReq() && isRead()) {
      if (addressValue < MEMORY_SIZE) {
        writeByteToBinaryPins(memory[addressValue], DATA_PINS);
        Serial.print("Written to data = ");
        Serial.println(memory[addressValue]);
      }
    }
    //Simulate write to memory
    if (isMemReq() && isWrite()) {
      if (addressValue < MEMORY_SIZE) {
        writeByteToBinaryPins(memory[addressValue], DATA_PINS);
        Serial.print("Written to data = ");
        Serial.println(memory[addressValue]);
      }
    }
    
    
    Serial.println("--------");
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
    for (int i=0; i<8;i++) {
      Serial.print(binary[i]);
    }
    Serial.print(" = ");
    Serial.println(value);
    return value;
  }
  
  boolean isMemReq() {
    return !digitalRead(MEM_REQ);
  }
  
  boolean isM1() {
    return !digitalRead(M1);
  }
  
  boolean isRead() {
    return !digitalRead(READ);
  }
    boolean isWrite() {
    return !digitalRead(WRITE);
  }
  
  void writeByteToBinaryPins(byte value, int startPin) {
    int binaryDigit =1;
    for (int pin = startPin; pin < startPin+8; pin++) {
      pinMode(pin, OUTPUT);
      if (value % binaryDigit) {
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin, LOW);
      }
      binaryDigit = binaryDigit * 2;
    }
  }
  
  void writeByteToMemoryArrayFromDataPins(byte memory[], int location, int startPin) {
    int binaryDigit =1;
    int value = 0;
    if (location >= MEMORY_SIZE) return;
    
    for (int pin = startPin; pin < startPin+8; pin++) {
         pinMode(pin, INPUT);
         if (digitalRead(pin) == HIGH) {
           value = value + binaryDigit;
         }
         binaryDigit = binaryDigit * 2;
    }
    
    memory[location] = value;
    
  }
        
    
