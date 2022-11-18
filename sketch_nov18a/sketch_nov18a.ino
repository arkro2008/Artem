#include <LiquidCrystal_I2C.h >
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
byte proesd=100;
MFRC522 mfrc522(SS_PIN, RST_PIN);   

MFRC522::MIFARE_Key key;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

void loop() {
   if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    byte sector         = 1;
    byte blockAddr      = 4;
    byte trailerBlock   = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);
    
    // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
// Read data from the block
    Serial.println(F("Reading data from block "));  
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();
    
}
void dump_byte_array(byte *buffer, byte bufferSize) {
  byte pog,pogi,pogie,pop;
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
        if(i==0){
          pog =buffer[i];
        }
        if(i==1){
          pogi=buffer[i];
        }
        if(i==2){
          pogie=buffer[i];
        }
        if(i==3){
          pop=buffer[i];
        }
    }
    Serial.println(pog,HEX);
    lcd.setCursor(1,0);
    lcd.print(pog,HEX);
    lcd.setCursor(2,0);
    lcd.print(",");
    lcd.setCursor(3,0);
    lcd.print(pogi,HEX);
    lcd.setCursor(4,0);
    lcd.print(",");
    lcd.setCursor(5,0);
    lcd.print(pogie,HEX);
    lcd.setCursor(6,0);
    lcd.print(",");
    lcd.setCursor(1,1);
    lcd.print(pop);
    //lcd.setCursor(10,0);
    //lcd.print(".");
}
