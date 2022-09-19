
long int Res_1Lux = 1000000;
float k = 1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int Val = analogRead(A0);
  float Vsig = (float)(5.0/1023.0)*Val;
  float Rf = 10000.0*((5.0/Vsig)-1);
  float Lux = (float)pow((Rf/Res_1Lux),(-1.0/k));
  Serial.println(Lux);
  delay(100);
}
