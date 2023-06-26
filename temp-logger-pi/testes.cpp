/*
#include <Arduino.h>
#include <Wire.h>
#include "WiFi.h"

//#define BUTTONPIN 4
//#define LEDPIN 12
//#define PERIOD 70
#define MCP9808_ADDRESS 0x18
#define MCP9808_REG_AMBIENT 0x05
#define MCP9808_REG_MANUF_ID 0x06
#define MCP9808_REG_RESOLUTION 0x08


void setup() {

  Wire.begin();
  Serial.begin(115200);
  Serial.println("i2c");
}



void loop() {
  //Wire.beginTransmission(MCP9808_ADDRESS);
  //Wire.write(MCP9808_REG_AMBIENT);
  //Wire.endTransmission();
  //Wire.requestFrom(MCP9808_ADDRESS, 2);

  //delay(10);
  Wire.beginTransmission(MCP9808_ADDRESS);
  Wire.write(MCP9808_REG_AMBIENT);
  Wire.endTransmission();


  Wire.requestFrom(MCP9808_ADDRESS, 2);
  //uint8_t count = Wire.available();

  uint8_t upperByte = Wire.read();
  Serial.println(upperByte);
  uint8_t lowerByte = Wire.read();
  Serial.println(lowerByte);
  upperByte &= 0x0F;
  Serial.println(upperByte);
  float val = (upperByte * 16 + (float)lowerByte / 16);

  //float val = (upperByte << 8 | lowerByte) / 16.0;
  //wire.endTransmission();

  Serial.println(val);
  //Serial.println(count);
  delay (500);
}





/*
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("mais uma volta");
  if(digitalRead(BUTTONPIN)==HIGH){

    int pulse=0;
    int pulsnt=300;

    while(digitalRead(BUTTONPIN)==HIGH){

      digitalWrite(LEDPIN,HIGH);
      delay(pulse>>6);
      pulse+=1;
      Serial.println(pulse);

      digitalWrite(LEDPIN,LOW);
      delay(pulsnt>>6);
      pulsnt-=1;
      if(pulsnt<0){
        digitalWrite(LEDPIN,HIGH);
        while(digitalRead(BUTTONPIN)==HIGH){

        }
      }
      Serial.println(pulsnt);

    }
  }
  else{
    Serial.println("off");
    }



}
*/

/*

void setup()
{
Serial.begin(115200);
// Set ESP32 wifi to station mode
WiFi.mode(WIFI_STA);
WiFi.disconnect();
delay(100);
Serial.println("Setup done");
}
void loop()
{
Serial.println("scan start");
// WiFi.scanNetworks will give total found wifi networks
int n = WiFi.scanNetworks();
Serial.println("scan done");
if (n == 0) {
Serial.println("no networks found");
} else {
Serial.print(n);
Serial.println(" networks found");
for (int i = 0; i < n; ++i) {
// Print SSID and signal strength
Serial.print(i + 1);
Serial.print(": ");
Serial.print(WiFi.SSID(i));
Serial.print(" (");
Serial.print(WiFi.RSSI(i));
Serial.print(")");
Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
delay(10);
        }
    }
Serial.println("");
// delay of 5 sec
delay(5000);
}

*/

/*
 SRAM   Arduino
 1 CS   10(CS)
 2 SO   12(MISO)
 3 -    -
 4 Vss  GND
 5 SI   11(MOSI)
 6 SCK  13(SCK)
 7 HOLD <-- 100k ohm -- 3.3V
 8 Vcc  3.3V

include...

SPISRAM myRAM(10); // CS pin
char buffer[128];

void setup()
{
  pinMode(5, OUTPUT);
  Serial.begin(9600);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);

  Serial.println("Byte write...");
  myRAM.write(0,'H');
  myRAM.write(1,'e');
  myRAM.write(2,'l');
  myRAM.write(3,'l');
  myRAM.write(4,'o');
  myRAM.write(5,' ');

  Serial.println("Byte read...");
  Serial.println(myRAM.read(0));
  Serial.println(myRAM.read(1));
  Serial.println(myRAM.read(2));
  Serial.println(myRAM.read(3));
  Serial.println(myRAM.read(4));
  Serial.println(myRAM.read(5));
}

void loop()
{
}
*/

/*
#include <Arduino.h>
#include <SPI.h>

#define CS_PIN 5 // Replace with the actual CS pin connected to the SRAM chip

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // Disable the SRAM chip initially
}

void loop() {
  uint16_t address = 0x0000;
  uint8_t data = 0xAB;
  uint8_t readData = 0x00;

  // Write the data to the SRAM chip
  digitalWrite(CS_PIN, LOW); // Enable the SRAM chip
  SPI.transfer(0x02); // Send the "Write" command to the chip
  SPI.transfer((address >> 8) & 0xFF); // Send the upper byte of the address
  SPI.transfer(address & 0xFF); // Send the lower byte of the address
  SPI.transfer(data); // Send the data to write to the memory
  digitalWrite(CS_PIN, HIGH); // Disable the SRAM chip

  delay(100); // Wait for a moment

  // Read the data from the SRAM chip
  digitalWrite(CS_PIN, LOW); // Enable the SRAM chip
  SPI.transfer(0x03); // Send the "Read" command to the chip
  SPI.transfer((address >> 8) & 0xFF); // Send the upper byte of the address
  SPI.transfer(address & 0xFF); // Send the lower byte of the address
  readData = SPI.transfer(0x00); // Read the data from the memory
  digitalWrite(CS_PIN, HIGH); // Disable the SRAM chip

  // Print the read data to the serial monitor
  Serial.println(readData, HEX);

  delay(1); // Wait for a second before repeating
}
*/

////////////////////CODIGO TESTE MEMORIA E SENSOR///////////////////////
/*
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "23K256SRAM.h"

#define MCP9808_ADDRESS 0x18
#define MCP9808_REG_AMBIENT 0x05
#define MCP9808_REG_MANUF_ID 0x06
#define MCP9808_REG_RESOLUTION 0x08
#define CS_PIN 5

void setup(){
  uint16_t addr = 0x0000;
  uint8_t data = 0xFF;
  uint8_t readData = 0x00;

  Wire.begin();
  Serial.begin(115200);
  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  uint8_t arraySensor[16];
  uint8_t arrayMem[16];


  while(1){

    Serial.print("array sensor:");

    for(int i = 16; i>0; i--){
      Wire.beginTransmission(MCP9808_ADDRESS);
      Wire.write(MCP9808_REG_AMBIENT);
      Wire.endTransmission();

      Wire.requestFrom(MCP9808_ADDRESS, 2);
      //uint8_t count = Wire.available();

      uint8_t upperByte = Wire.read();
      uint8_t lowerByte = Wire.read();
      upperByte &= 0x0F;
      arraySensor[16-i] = (uint8_t)(upperByte * 16 + (float)lowerByte / 16);
      Wire.endTransmission();

      //writeByte(addr, &arraySensor[16-i]);

      Serial.print(arraySensor[16-i]);
      Serial.print(";");

      delay (50);

    }class tempid
    Serial.println("");


    writeArray(addr, &arraySensor[0], sizeof(arraySensor));
    readArray(addr, &arrayMem[0], sizeof(arrayMem));


    Serial.print("array sram:");

    for(int i = 0; i < sizeof(arrayMem); i++){

      //arrayMem[i] = readByte(addr);
      Serial.print(arrayMem[i]);
      Serial.print(";");

    }
    Serial.println("");

    addr += 16;

    delay(1000);

  }
}

void loop(){}
*/

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "23K256SRAM.h"
#include <RTClib.h>
#include <ctime>

const char *compDate = __DATE__;
const char *compHour = __TIME__;

char *fullDate;
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{

  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  fullDate = new char[strlen(compDate) + strlen(compHour) + 2];
  strcpy(fullDate, compDate);
  strcat(fullDate, " ");
  strcat(fullDate, compHour);

  Serial.println(fullDate);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
  }
}

void loop()
{

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}