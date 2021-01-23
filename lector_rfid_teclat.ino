#include <Wire.h> 
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          
#define SS_PIN          10 
MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
byte nuidPICC[4];
String content;
int x = 0;
void setup() {
  Serial.begin(9600);
  // Start the I2C Bus as Master
  Wire.begin();
  SPI.begin();                // Init SPI bus
  rfid.PCD_Init();         // Init MFRC522 card
}
void loop() {
  // Noves targetes
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verifica que s'ha llegit el NUID
  if ( ! rfid.PICC_ReadCardSerial())
    return;
    content = "";
    for (byte i = 0; i < rfid.uid.size; i++) 
    {
       
       content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(rfid.uid.uidByte[i], HEX));

    }
    for (int i = 0; i < content.length(); i++) 
    {
       Wire.beginTransmission(9);
       Wire.write(content[i]);
       Wire.endTransmission();
    }
    Serial.print(content.c_str());
    delay(2000);
}
