
//*******************ACQUARIUM ENABLE**************************//
//Uncomment the line of the acquarium output you're going to use
#define ACQ_1         
//#define ACQ_2
//#define ACQ_3
//#define ACQ_4

//*******************SENSOR POWER PIN*************************//
//Uncomment this line for enamble.
//#define POWER_PIN 50

//*******************WATER LEVEL SENSOR**********************//
#ifdef ACQ_1
  #define WLS_1 A0
#endif
#ifdef ACQ_2
  #define WLS_2 A1
#endif
#ifdef ACQ_3
  #define WLS_3 A2
#endif
#ifdef ACQ_4
  #define WLS_4 A3
#endif

//******************LUX SENSOR*******************************//
#ifdef ACQ_1
  #define LX_1 A4
#endif
#ifdef ACQ_2
  #define LX_2 A5
#endif
#ifdef ACQ_3
  #define LX_3 A6
#endif
#ifdef ACQ_4
  #define LX_4 A7
#endif

long int Res_1Lux = 1000000;
float k = 1;

//*********DHT11 SENSOR (humidity & Air Temperature)*********//
#include <SimpleDHT.h>

#ifdef ACQ_1
  #define DHT_1 22
  SimpleDHT11 dht_1(DHT_1);
#endif
#ifdef ACQ_2
  #define DHT_2 23
  SimpleDHT11 dht_2(DHT_2);
#endif
#ifdef ACQ_3
  #define DHT_3 24
  SimpleDHT11 dht_3(DHT_3);
#endif
#ifdef ACQ_4
  #define DHT_4 25
  SimpleDHT11 dht_4(DHT_4);
#endif

//*******************WATER TEMPERATURE SENSOR**************//
#include <OneWire.h>
#include <DallasTemperature.h>

#ifdef ACQ_1
  #define WTS_1 38
  OneWire oneWire1(WTS_1);
  DallasTemperature wts_1(&oneWire1);
#endif
#ifdef ACQ_2
  #define WTS_2 39
  OneWire oneWire2(WTS_2);
  DallasTemperature wts_2(&oneWire2);
#endif
#ifdef ACQ_3
  #define WTS_3 40
  OneWire oneWire3(WTS_3);
  DallasTemperature wts_3(&oneWire3);
#endif
#ifdef ACQ_4
  #define WTS_4 41
  OneWire oneWire4(WTS_4);
  DallasTemperature wts_4(&oneWire4);
#endif

//****************AUTOMATIC FILLING**********************//
#include <Servo.h>

#define SERVOSWITCH_PIN 6
Servo servoSwitch;

//***************4RELE MODULE****************************//

#define PUMP 50
#define RESISTORS 51
#define FILTERS 52
#define BLOWERS 53

//****************************SETUP*******************//

void setup() {
  //Serial connection with raspberry pi
  Serial.begin(115200);

  //Servo switch init
  servoSwitch.attach(SERVOSWITCH_PIN);

  //WATER TEMPERATURE SENSOR
  #ifdef ACQ_1
    wts_1.begin();
  #endif
  #ifdef ACQ_2
    wts_2.begin();
  #endif
  #ifdef ACQ_3
    wts_3.begin();
  #endif
  #ifdef ACQ_4
    wts_4.begin();
  #endif

  //RELE pin settings
  pinMode(PUMP, OUTPUT);
  pinMode(RESISTORS, OUTPUT);
  pinMode(FILTERS, OUTPUT);
  pinMode(BLOWERS, OUTPUT);
}

String strs[20]; 

void loop() {
  //wait for Command available from Raspberry 
  while (Serial.available() == 0) {}     
  String str = Serial.readString();  //read until timeout
  str.trim();                        // remove any \r \n whitespace at the end of the String
  
  //SPLITTING THE COMMAND
  //COMMAND PROTOCOL
  //Sensor Request: 10
  //Switch Request: 20|on|on|on|on (Pump|Filters|Resistors|Blower)

  //Slitting the string using |
  splitString(str, '|');

  //Switching thse command
  switch(strs[0].toInt()){
    case 10:
      sendSensorsData();
      break;
    case 20:
      changeSwitchState();
      break;
    default:
      Serial.println("100|something went wrong");
      break;
  }
  
  
  delay(1000);
}


void sendSensorsData(){
  Serial.println("Sending data");
    
}

void changeSwitchState(){
  Serial.println("SwitchingState");
  if (strs[1] == "on")
    digitalWrite(PUMP, HIGH);
  else
    digitalWrite(PUMP, LOW);
    
  if (strs[2] == "on")
    digitalWrite(FILTERS, HIGH);
  else
    digitalWrite(FILTERS, LOW);
    
  if (strs[3] == "on")
    digitalWrite(RESISTORS, HIGH);
  else
    digitalWrite(RESISTORS, LOW);
    
  if (strs[4] == "on")
    digitalWrite(BLOWERS, HIGH);
  else
    digitalWrite(BLOWERS, LOW);
}

String acquariumSensorsData(int num){
  #ifdef ACQ_1
    
  #endif
  #ifdef ACQ_2
    
  #endif
  #ifdef ACQ_3
    
  #endif
  #ifdef ACQ_4
    
  #endif
}



void splitString(String str, char Separator){
  int stringCount = 0;
  while (str.length() > 0) {
    int index = str.indexOf('|');
    if (index == -1) // No space found
    {
      strs[stringCount++] = str;
      break;
    }
    else
    {
      strs[stringCount++] = str.substring(0, index);
      str = str.substring(index+1);
    }
  }
}
