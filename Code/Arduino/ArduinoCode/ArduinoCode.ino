/*TODO:
*Fix the convercion to String
*Water level sensor 
*Plafoniera e timer
*/


//*******************ACQUARIUM ENABLE**************************//
//Uncomment the line of the acquarium output you're going to use
#define ACQ_1         
//#define ACQ_2
//#define ACQ_3
//#define ACQ_4

//**************************COST******************************//
#define VIN 5 // V power voltage
#define R 10000 //ohm resistance value

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
  //Switch Request: 20|on|on|on (Filters|Resistors|Blower)

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
  String data = "SD|";
  //Lux
  luxData = luxSensors();
  data = data + luxData + "|";
  //WaterTemp
  waterTemp = waterTempSensors();
  data = data + waterTemp + "|";
  //Humidity and Air
  HumAtp = humidityAndAirTemp();
  data = data + HumAtp + "|";
  //Water Level sensor

  Serial.println(data);
}

void changeSwitchState(){
  Serial.println("SwitchingState");
/*   if (strs[1] == "on")
    digitalWrite(PUMP, HIGH);
  else
    digitalWrite(PUMP, LOW); */
    
  if (strs[1] == "on")
    digitalWrite(FILTERS, HIGH);
  else
    digitalWrite(FILTERS, LOW);
    
  if (strs[2] == "on")
    digitalWrite(RESISTORS, HIGH);
  else
    digitalWrite(RESISTORS, LOW);
    
  if (strs[3] == "on")
    digitalWrite(BLOWERS, HIGH);
  else
    digitalWrite(BLOWERS, LOW);
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

//***********************************SENSORS REQUEST****************************//

String humidityAndAirTemp(){
  String datau = "HUM=";
  String datat = "ATP=";

  byte temperature = 0;
  byte humidity = 0;

  #ifdef ACQ_1
    int err = SimpleDHTErrSuccess;
    if ((err = dht_1.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    //Da convertire a string
    datau = datau + (int)humidity + ",";
    datat = datat + (int)temperature + ",";
  #endif
  #ifdef ACQ_2
    int err = SimpleDHTErrSuccess;
    if ((err = dht_2.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    //Da convertire a string
    datau = datau + (int)humidity + ",";
    datat = datat + (int)temperature + ",";
  #endif
  #ifdef ACQ_3
    int err = SimpleDHTErrSuccess;
    if ((err = dht_3.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    //Da convertire a string
    datau = datau + (int)humidity + ",";
    datat = datat + (int)temperature + ",";
  #endif
  #ifdef ACQ_4
    int err = SimpleDHTErrSuccess;
    if ((err = dht_4.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    //Da convertire a string
    datau = datau + (int)humidity;
    datat = datat + (int)temperature;
  #endif
  
  return datau + "|" + datat;
}

String waterTempSensors(){
  String data = "WTS="
  //non si converte cosi a string
  #ifdef ACQ_1
    wts_1.requestTemperatures(); 
    tc = temp.getTempCByIndex(0);
    data + String(tc) + ","
  #endif
  #ifdef ACQ_2
    wts_1.requestTemperatures(); 
    tc = temp.getTempCByIndex(0);
    data + String(tc) + ","
  #endif
  #ifdef ACQ_3
    wts_1.requestTemperatures(); 
    tc = temp.getTempCByIndex(0);
    data + String(tc) + ","
  #endif
  #ifdef ACQ_4
    wts_1.requestTemperatures(); 
    tc = temp.getTempCByIndex(0);
    data + String(tc)
  #endif
  
  return data;
}


String luxSensors(){
  String data = "LUX="

  #ifdef ACQ_1
    sensorVal = analogRead(LX_1);
    lux=sensorRawToPhys(sensorVal);
    data + String(lux) + ","
  #endif
  #ifdef ACQ_2
    sensorVal = analogRead(LX_2);
    lux=sensorRawToPhys(sensorVal);
    data + String(lux) + ","
  #endif
  #ifdef ACQ_3
    sensorVal = analogRead(LX_3);
    lux=sensorRawToPhys(sensorVal);
    data + String(lux) + ","
  #endif
  #ifdef ACQ_4
    sensorVal = analogRead(LX_4);
    lux=sensorRawToPhys(sensorVal);
    data + String(lux)
  #endif
  
  return data;
}

int sensorRawToPhys(int raw){
  // Conversion rule
  float Vout = float(raw) * (VIN / float(1023));// Conversion analog to voltage
  float RLDR = (R * (VIN - Vout))/Vout; // Conversion voltage to resistance
  int phys=500/(RLDR/1000); // Conversion resitance to lumen
  return phys;
}


/*  CODICI SENSORI

#define POWER_PIN  7
#define SIGNAL_PIN A5

int value = 0; // variable to store the sensor value

void setup() {
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);   // configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF
}

void loop() {

  //lettura sensore livello dell'acqua
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print("Sensor value: ");
  Serial.println(value);

  delay(1000);
} 

*/
