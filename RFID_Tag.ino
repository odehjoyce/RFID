 /*
To read a card using a mfrc522 reader on your Arduino SPI interface,make the following connections:

MFRC522 SCANNER      ARDUINO

MOSI                 Pin 11 / ICSP-4 It is SPI input to the RC522 module
MISO                 Pin 12 / ICSP-1 It acts as Master-In-Slave-Out when SPI interface is enabled
SCK                  Pin 13 / ISCP-3 It accepts clock pulses provided by the SPI bus Master i.e Arduino
SS/SDA               Pin 10          It acts as a signal input when SPI interface is enabled
RST                  Pin 9           RST is an input for reset and power-down
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd (8,2,4,5,6,7);
#include <SPI.h> /*Serial Periphal Interface bus Library*/
#include <RFID.h>

#define SS_PIN 10 //slave select pin
#define RST_PIN 9 //reset pin

RFID rfid(SS_PIN,RST_PIN); //represet a rfid reader object

int serNum[5];
int cards[][5] = { {73,31,219,178,63} }; //The card ID. Replace with actual card ID
bool access = false;

void setup()
{
  lcd.begin(16,2);
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
 
  if(!rfid.isCard())
  {   
    lcd.setCursor(0,0);
    lcd.print("Show your card");
    lcd.setCursor(0,1);
     lcd.print("to the scanner");
    
    delay(1000);
  }
}

void(* resetFunc) (void) = 0;  //Initialized at address 0 ?????

void loop()
{
  readCard();
}

void readCard()
{  
  if(rfid.isCard()) //Look for new cards
  {
    lcd.clear();
    if(rfid.readCardSerial()) //Selects one of the cards
    {
      lcd.setCursor(0,0);
      lcd.print(rfid.serNum[0]);
      lcd.print(rfid.serNum[1]);
      lcd.print(rfid.serNum[2]);
      lcd.print(rfid.serNum[3]);
      lcd.print(rfid.serNum[4]);
              
  
      Serial.println(rfid.serNum[0]);
      Serial.println(rfid.serNum[1]);
      Serial.println(rfid.serNum[2]);
      Serial.println(rfid.serNum[3]);
      Serial.println(rfid.serNum[4]);   
        
      for(int x = 0; x < sizeof(cards); x++)
      {
        for(int i = 0; i < sizeof(rfid.serNum); i++ )
        {
          if(rfid.serNum[i] != cards[x][i]) 
          {
            access = false;
            break;
          }
          else
          {
            access = true;
          }
        }
        if(access) break;
      }    
  }

 if(access)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WELCOME JOYCE");
    delay(1000);
    lcd.clear();    
  } 
 
  else 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("INVALID CARD");
    delay(1000); 
    lcd.clear();      
    resetFunc(); //Calls reset
  }
      
}
  rfid.halt();
}
