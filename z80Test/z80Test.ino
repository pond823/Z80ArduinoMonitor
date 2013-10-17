

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
const int WAIT = 11;


int clockState = LOW;             // clockState used to set the CLK

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 200;           // clock interval (milliseconds)
long previousMillis = 0;        // will store last time LED was updated
long clockCount = 0;
int memReqState = HIGH;
int m1State = HIGH;
int readState = HIGH;
int writeState = HIGH;
int addressValue =0;
int oldAddressValue =0;

boolean displayWrite = true;
boolean displayRead = true;


// memory locations
const int MEMORY_SIZE = 8;
// ld a,5; inc a; ld (07),a; 
byte memory[] = {0x4f,0x00,0x0,0x0, 0x0,0x3c,0x3C, 0x0, 0x0, 0x0, 0x0 };

// inc a; ld (0005), a;
//byte memory[] = {0x3C, 0x32, 0x00, 0x05,0x00,0x00, 0x0, 0x0, 0x0, 0x0 };

//byte memory[] = {0x0, 0x3c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

char binary[8] = {'0','0','0','0','0','0','0','0'};

void setup() {
  
  int addressPin;
 
  Serial.begin(9600);
  
  //Set the CLK pin to OUTPUT
  pinMode(CLOCK_PIN, OUTPUT); 
  pinMode(MEM_REQ, INPUT);
  pinMode(M1, INPUT);
  pinMode(READ, INPUT);
  pinMode(WRITE, INPUT);
  pinMode(WAIT,OUTPUT);

  
  
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
    digitalWrite(WAIT, HIGH);  
  
  
  //Reset the z80, leaving it HIGH
  pinMode(RESET_PIN, OUTPUT);
  for (int resetCount = 0; resetCount <3; resetCount++) {
    digitalWrite(RESET_PIN, LOW);
    delay(500);
    digitalWrite(RESET_PIN, HIGH);
    delay(500);
  }    
 

  
}

void loop()
{

  clockTick();
    
 
    addressValue = digitalBinaryPinsToInt(ADDRESS_LOW);

    //Simulate read from memory
    if (isMemReq() && isRead()) {
      if (addressValue < MEMORY_SIZE) {
        if (displayRead) {
          writeByteToBinaryPins(memory[addressValue], DATA_PINS);
          Serial.print("Read = ");
          Serial.println(memory[addressValue]);
          displayRead = false;
        }
      }
    } else {
      displayRead = true;
    }
    
    //Simulate write to memory
    if (isMemReq() && isWrite()) {
      if (addressValue < MEMORY_SIZE) {
        writeByteToMemoryArrayFromDataPins(memory, addressValue, DATA_PINS);
          if (displayWrite) {
          Serial.print("Write = ");
          Serial.println(memory[addressValue]);
          displayWrite = false;
        }
      }
    } else {
      displayWrite = true;
    }
    
    displayOnStateChange();
    
      

 
}

  int digitalBinaryPinsToInt(int startPin) {

    int position = 7;
    int binaryDigit = 1;
    int value = 0;
    for (int pin = startPin; pin < startPin+8; pin++) {
      
      if (digitalRead(pin) == HIGH) {
        value = value + binaryDigit;
        binary[position] ='1';
      } else {
        binary[position] ='0';
      }
      binaryDigit = binaryDigit * 2;
      position--;
    }
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
    //Serial.print(value);
    Serial.print(" = ");

    digitalWrite(WAIT, LOW);  
    for (int pin = startPin; pin < startPin+8; pin++) {
      pinMode(pin, OUTPUT);
      if (value & binaryDigit > 0) {
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin, LOW);
      }

      Serial.print(pin);
      Serial.print(":");
      Serial.print(binaryDigit );
      Serial.print("=");
      Serial.print( value & binaryDigit );
      Serial.print(" ");
      
            binaryDigit = binaryDigit * 2;
    }
    digitalWrite(WAIT, HIGH);  
    Serial.println();
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
  
  void clockTick() {
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
      Serial.print(clockCount++);
      Serial.print(" Memory = ");
      for(int i=0; i< MEMORY_SIZE; i++) {
        Serial.print(memory[i]);
        Serial.print(".");
        
      }
      Serial.print(" data = ");

      Serial.print(digitalBinaryPinsToInt(PORTD));
      Serial.println();
    }
  }
       
  void displayOnStateChange() {
   
    //Change to active states
    if (isMemReq() && memReqState == HIGH) {
        Serial.println("Memory Request Active");
        memReqState = LOW;
     }
      if (isM1() && m1State == HIGH) {
        Serial.println("M1 Active");
        m1State = LOW;
      }
      if (isRead() && readState == HIGH) {
        Serial.println("Read Active");
        readState = LOW;
      }
     if (isWrite() && writeState == HIGH) {
        Serial.println("**** Write Active");
        writeState = LOW;
     }
     
     //Change to inactive states
     if (!isMemReq() && memReqState == LOW) {
        Serial.println("Memory Request Off");
        memReqState = HIGH;
     }
      if (!isM1() && M1 == LOW) {
        Serial.println("M1 LOW");
        m1State = HIGH;
      }
      if (!isRead() && readState == LOW) {
        Serial.println("Read Off");
        readState = HIGH;
      }
     if (!isWrite() && writeState == LOW) {
        Serial.println("**** Write Off");
        writeState = HIGH;
     }
     
     if (addressValue != oldAddressValue) {
      Serial.print("Address Value = ");
      Serial.println(addressValue);
      oldAddressValue = addressValue;
     
    }

    
    
  }
    
