#define rDirpin 16
#define rSpeedpin 3
#define lSpeedpin 5
#define lDirpin 14

#define pin_trig_mid 10
#define pin_echo_mid 7


short blacksensors = 0;
float mas[4] = {1.7, 1.5, 1.4, 1};
bool flag = 0;
int V = 220;
float P = 0.12;
int lm, rm, U, F, Fp, midsensor = 0;
float d = 0.30;
unsigned short sensors[8];

void setup() {
  // put your setup code here, to run once:
  pinMode(rDirpin, OUTPUT);
  pinMode(lSpeedpin, OUTPUT);
  pinMode(rSpeedpin, OUTPUT);
  pinMode(lDirpin, OUTPUT);

  pinMode(pin_trig_mid, OUTPUT);
  pinMode(pin_echo_mid, INPUT);
}

void loop() {
  // на белом 100-250 на чёрном 700-900+
  get_data();
  //  F = sensors[0]-sensors[7];
  //Serial.println(sensors[0]);
  //Serial.println(sensors[7]);
  //delay(1000);
  Serial.println(midsensor);
  if (midsensor <= 20 && midsensor != 0) {
    get_data();
    go(-100,-100);
    delay(100);
    go(0,0);
    delay(50);
    go(200, 0);
    delay(250);
//    go(0,0);
//    delay(1000);
    //    go(-100,-100);
    //    delay(100);
    //    go(0,0);
    //    delay(2000);
    get_data();
    while (sensors[0] < 500 && sensors[7] < 500) {
      go(80, 140);
      get_data();
    }
    delay(50);
    
    go(200,0);
    delay(150);
    get_data();
  }

  //    delay(500);
  //  for(int i=0;i<8;i++){
  //    Serial.print(i);
  //    Serial.print(": ");
  //    Serial.println(sensors[i]);
  //  }
  //  delay(1000);
  if (blacksensors != 0) {
    F = (sensors[0] * mas[0] + sensors[1] * mas[1] + sensors[2] * mas[2] + sensors[3] * mas[3] - sensors[4] * mas[3] - sensors[5] * mas[2] - sensors[6] * mas[1] - sensors[7] * mas[0]) / blacksensors;
  }
  U = F * P + (F - Fp) * d;
  lm = V - U;
  rm = V + U;

  go(lm, rm);

  Fp = F;
}


void go(int lSpeed, int rSpeed) {
  int lv = constrain (lSpeed, -255, 255);
  int rv = constrain (rSpeed, -255, 255);

  if (lSpeed < 0) {
    digitalWrite(lDirpin, 0);
  } else {
    digitalWrite(lDirpin, 1);
  }
  if (rSpeed < 0) {
    digitalWrite(rDirpin, 1);
  } else {
    digitalWrite(rDirpin, 0);
  }
  analogWrite(lSpeedpin, abs(lv));
  analogWrite(rSpeedpin , abs(rv));
}

void get_data() {
  blacksensors = 0;
  sensors[0] = analogRead(9);
  sensors[1] = analogRead(8);
  sensors[2] = analogRead(7);
  sensors[3] = analogRead(6);
  sensors[4] = analogRead(3);
  sensors[5] = analogRead(2);
  sensors[6] = analogRead(1);
  sensors[7] = analogRead(0);
  long int duration;
  digitalWrite(pin_trig_mid, LOW);
  delayMicroseconds(5);
  digitalWrite(pin_trig_mid, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig_mid, LOW);
  duration = pulseIn(pin_echo_mid, HIGH, 12550);
  midsensor = (duration / 2) / 29.1;

  for (int i = 0; i < 8; i++) {
    if (sensors[i] > 250) {
      blacksensors++;
    }
  }
}
