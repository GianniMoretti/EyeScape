#define PL 2

int pl1Value = 250;
bool up = true;

void setup() {
  analogWrite(PL, pl1Value);
  Serial.begin(9600);
}

void loop() {
  if(pl1Value == 250){
    up = false;
  }
  else if (pl1Value == 0){
    up = true;
  }

  if(up)
    pl1Value += 1;
  else
    pl1Value -= 1;

  //Serial.println(pl1Value);
  //analogWrite(PL, pl1Value);
  setPLValue(PL, pl1Value);
  
  delay(25);
}

void setPLValue(int pin, int value){
  if (value > 255 || value < 0)
    return;
    
  analogWrite(pin, value);
}
