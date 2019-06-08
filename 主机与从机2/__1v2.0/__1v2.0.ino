# include<IRremote.h>
//11口3口有问题
const int TrigPin = 12;//非PWM
const int EchoPin = 10;//PWM

const int RECV_PIN = 9;//定义红外接收器引脚为9(PWM)

const int light_pin = A0;

const int AIN1 = 4;
const int AIN2 = 2;
const int PWMA = 5;
const int PWMB = 6;
const int BIN1 = 7;
const int BIN2 = 8;

const int ON = 0x00FF30CF;
const int OFF = 0x00FF6897;
const int LIGHT_LEVEL = 900;

int BV = 0, flag = 0, CCT = 0;
int BV_R = 0, BV_B = 0;
int light_level = 0;//亮度
float cm = 15.0;

bool lying = 0, sleeping = 0, getup = 1, lamp = 0; //是否有人躺下,是否是睡觉时，是否起夜，房间是否亮的

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  irrecv.enableIRIn();//使红外能接收

  digitalWrite(AIN1, HIGH);
  digitalWrite(BIN1, HIGH);

  digitalWrite(AIN2, LOW);
  digitalWrite(BIN2, LOW);
}

void loop() {
  light();
  analogWrite(light_pin, light_level);
  if (light_level > LIGHT_LEVEL) { //关灯后测距
    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    cm = pulseIn(EchoPin, HIGH) / 58.0; //将回拨时间换算成cm
    cm = (int(cm * 100.0)) / 100.0; //保留两位小数
    if (cm > 30)
      lying = 0;
    else
      lying = 1;

    lamp = 0;
  }
  else
    lamp = 1;
  irrecv.resume();
  if (irrecv.decode(&results) && light_level > LIGHT_LEVEL) { //黑天了
    if (results.value == ON) { //床下有人，开灯
      analogWrite(PWMA, BV_B);
      analogWrite(PWMB, BV_R);
    }
    if (lying) { //床上有人躺下，关灯
      analogWrite(PWMA, 0);
      analogWrite(PWMB, 0);
      sleeping = 1;
    }
    if (sleeping && !lying) { //睡觉时起夜，开灯
      analogWrite(PWMA, BV_B);
      analogWrite(PWMB, BV_R);
      getup = 1;
    }
    if (results.value == OFF && getup) { //去厕所了，床下没人了，关灯
      analogWrite(PWMA, 0);
      analogWrite(PWMB, 0);
    }
    if (getup && lying) //起夜后又躺下了
      getup = 0;

  }
}

void light()
{
  String inString = "";
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    inString += (char)inChar;
    delay(24);
  }
  if (inString != "") {
    flag = inString[0] - '0';
    BV = inString[1] - '0';
    CCT = inString[2] - '0';
  }

  BV_set(CCT);
  if (flag) {

    analogWrite(PWMA, BV_B);
    analogWrite(PWMB, BV_R);
  }
  else {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
  }
}

void BV_set(int cct)
{
  BV_R = BV_B = BV * 10;
  if (cct < 5) {//变冷
    cct = 5 - cct;

    BV_B += 2 * BV * cct;
    BV_R -= 2 * BV * cct;

  }
  else {
    cct -= 4;

    BV_B -= 2 * BV * cct;
    BV_R += 2 * BV * cct;

  }

}
