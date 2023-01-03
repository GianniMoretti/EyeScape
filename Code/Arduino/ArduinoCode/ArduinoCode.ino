/*TODO:
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
  #define WTS_1 32
  OneWire oneWire1(WTS_1);
  DallasTemperature wts_1(&oneWire1);
#endif
#ifdef ACQ_2
  #define WTS_2 33
  OneWire oneWire2(WTS_2);
  DallasTemperature wts_2(&oneWire2);
#endif
#ifdef ACQ_3
  #define WTS_3 34
  OneWire oneWire3(WTS_3);
  DallasTemperature wts_3(&oneWire3);
#endif
#ifdef ACQ_4
  #define WTS_4 35
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

//***************LED MODULE****************************//
#define LED_1 2
#define LED_2 3
#define LED_3 4
#define LED_4 5

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
  //LED pin 
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
}

String strs[20]; 

void loop() {
  //wait for Command available from Raspberry 
  while (Serial.available() == 0) {}     
  String str = Serial.readString();  //read until timeout
  str.trim();                        // remove any \r \n whitespace at the end of the String
  
  //SPLITTING THE COMMAND
  //COMMAND PROTOCOL
  //Sensor Request:     10
  //Switch Request:     20|on|on|on (Filters|Resistors|Blower)
  //Pump Activation:    30|on
  //LED:                40|index|percentage
  //SERVO SELECTION:    50|index

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
    case 30:
      refillPump();
      break;
    case 40:
      LEDRequest();
      break;
    case 50:
      SERVORequest();
      break;
    default:
      Serial.println("100|something went wrong");
      break;
  }
  
  delay(100);
}


void sendSensorsData(){
  String data = "SD|";
  //Lux
  String luxData = luxSensors();
  data = data + luxData + "|";
  //WaterTemp
  String waterTemp = waterTempSensors();
  data = data + waterTemp + "|";
  //Humidity and Air
  String HumAtp = humidityAndAirTemp();
  data = data + HumAtp + "|";
  //Water Level sensor
  String Wls = waterLevelSensors();
  data = data + Wls;

  Serial.println(data);
}

void changeSwitchState(){
  if (strs[1] == "on"){
    digitalWrite(FILTERS, HIGH);
  }
  else
    digitalWrite(FILTERS, LOW);
    
  if (strs[2] == "on"){
    digitalWrite(RESISTORS, HIGH);
  }
  else
    digitalWrite(RESISTORS, LOW);
    
  if (strs[3] == "on"){
    digitalWrite(BLOWERS, HIGH);
  }
  else
    digitalWrite(BLOWERS, LOW);

  Serial.println("Arduino: All the device state are updated");
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
    else{
      datau = datau + (int)humidity + ",";
      datat = datat + (int)temperature + ",";
    }
  #endif
  #ifdef ACQ_2
    int err = SimpleDHTErrSuccess;
    if ((err = dht_2.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    else{
      datau = datau + (int)humidity + ",";
      datat = datat + (int)temperature + ",";
    }
  #endif
  #ifdef ACQ_3
    int err = SimpleDHTErrSuccess;
    if ((err = dht_3.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    else{
      datau = datau + (int)humidity + ",";
      datat = datat + (int)temperature + ",";
    }
  #endif
  #ifdef ACQ_4
    int err = SimpleDHTErrSuccess;
    if ((err = dht_4.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      datau = datau + "err,";
      datat = datat + "err,";
    }
    else{
      datau = datau + (int)humidity + ",";
      datat = datat + (int)temperature + ",";
    }
  #endif
  
  return datau + "|" + datat;
}

String waterTempSensors(){
  String data = "WTS=";
  //non si converte cosi a string
  #ifdef ACQ_1
    wts_1.requestTemperatures(); 
    float tc = wts_1.getTempCByIndex(0);
    data = data + String(tc) + ",";
  #endif
  #ifdef ACQ_2
    wts_2.requestTemperatures(); 
    float tc = wts_2.getTempCByIndex(0);
    data = data + String(tc) + ",";
  #endif
  #ifdef ACQ_3
    wts_3.requestTemperatures(); 
    float tc = wts_3.getTempCByIndex(0);
    data = data + String(tc) + ",";
  #endif
  #ifdef ACQ_4
    wts_4.requestTemperatures(); 
    float tc = wts_4.getTempCByIndex(0);
    data = data + String(tc);
  #endif
  
  return data;
}


String luxSensors(){
  String data = "LUX=";

  #ifdef ACQ_1
    float sensorVal = analogRead(LX_1);
    int lux = sensorRawToPhys(sensorVal);
    data = data + String(lux) + ",";
  #endif
  #ifdef ACQ_2
    float sensorVal = analogRead(LX_2);
    int lux = sensorRawToPhys(sensorVal);
    data = data + String(lux) + ",";
  #endif
  #ifdef ACQ_3
    float sensorVal = analogRead(LX_3);
    int lux = sensorRawToPhys(sensorVal);
    data = data + String(lux) + ",";
  #endif
  #ifdef ACQ_4
    float sensorVal = analogRead(LX_4);
    int lux = sensorRawToPhys(sensorVal);
    data = data + String(lux);
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

String waterLevelSensors(){
  String data = "WLS=";
  
  #ifdef ACQ_1
    float Val = analogRead(WLS_1);
    float sensorVal = ValtoCm(Val);
    data = data + String(sensorVal) + ",";
  #endif
  #ifdef ACQ_2
    float Val = analogRead(WLS_2);
    float sensorVal = ValtoCm(Val);
    data = data + String(sensorVal) + ",";
  #endif
  #ifdef ACQ_3
    float Val = analogRead(WLS_3);
    float sensorVal = ValtoCm(Val);
    data = data + String(sensorVal) + ",";
  #endif
  #ifdef ACQ_4
    float Val = analogRead(WLS_4);
    float sensorVal = ValtoCm(Val);
    data = data + String(sensorVal);
  #endif
  
  return data;
}

float ValtoCm(float val){
  return map(val, 0,700,0,4);
}

void refillPump(){
  if (strs[1] == "on"){
    digitalWrite(PUMP, HIGH);
    Serial.println("Refill on");
  }
  else{
    digitalWrite(PUMP, LOW); 
    Serial.println("Refill off");
  }
}

void LEDRequest(){
  int index = strs[1].toInt();
  int perc = strs[2].toInt();

  int val = map(perc, 0, 100, 0, 255);

  if (val > 255 || val < 0)
    return;

  switch(index){
    case 1: 
      analogWrite(LED_1, val);
      Serial.println("Arduino: Led 1 set value " + strs[2] + " ok");
    break;
        case 2: 
      analogWrite(LED_2, val);
      Serial.println("Arduino: Led 2 set value " + strs[2] + " ok");
    break;
        case 3: 
      analogWrite(LED_3, val);
      Serial.println("Arduino: Led 3 set value " + strs[2] + " ok");
    break;
        case 4: 
      analogWrite(LED_4, val);
      Serial.println("Arduino: Led 4 set value " + strs[2] + " ok");
    break;
    default:
      Serial.println("110|Index Led Not Valid");
    break;
  }
}

void SERVORequest(){
  int index = strs[1].toInt();

  switch(index){
    case 1: 
      servoSwitch.write(0);
      Serial.println("Arduino: Index 1 ok");
    break;
        case 2: 
      servoSwitch.write(60);
      Serial.println("Arduino: Index 2 ok");
    break;
        case 3: 
      servoSwitch.write(120);
      Serial.println("Arduino: Index 3 ok");
    break;
        case 4: 
      servoSwitch.write(180);
      Serial.println("Arduino: Index 4 ok");
    break;
    default:
      Serial.println("110|Index Servo Not Valid");
    break;
  }
}
