#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(38);
DallasTemperature temp(&oneWire);

float tc = 0;
float tf = 0;

void setup(void) {
  Serial.begin(115200);
  temp.begin();
}

void loop(void) { 
  temp.requestTemperatures(); 
  tc = temp.getTempCByIndex(0);
  tf = temp.toFahrenheit(tc);
  Serial.print(" C  ");
  Serial.print(tc);
  Serial.print(" F  ");
  Serial.println(tf);
  delay(1000);
}
