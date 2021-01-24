#include <Wire.h> 
#include <Keyboard.h> 
char x = " ";
String clau = "";
void setup() {
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9); 

  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Keyboard.begin();
  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB
    
    }
}
void receiveEvent(int bytes) {
  x = Wire.read();    // LLig del I2C i envia-ho al USB
  clau.concat(x);
  if (clau.length() == 12){
     Serial.println(clau);
     Keyboard.println(clau);
     clau="";
  }
  
}
void loop() {
 
}
 
