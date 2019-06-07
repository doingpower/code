# include<IRremote.h>
//11口3口有问题
const int TrigPin = 12;//非PWM
const int EchoPin = 10;//PWM

int RECV_PIN = 9;//定义红外接收器引脚为9(PWM)

const int AIN1 = 4;
const int AIN2 = 2;
const int PWMA = 5;
const int PWMB = 6;
const int BIN1 = 7;
const int BIN2 = 8;

int BV = 0, flag = 0, CCT = 0;
int BV_R = 0, BV_B = 0;
float cm = 15.0;
int getup = 1;

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

  if (irrecv.decode(&results))
  {

    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    cm = pulseIn(EchoPin, HIGH) / 58.0; //将回拨时间换算成cm
    cm = (int(cm * 100.0)) / 100.0; //保留两位小数
  }//关灯后才开始测距
  if (results.value == 0x00FF906F || (cm > 30 && getup == 1))
  {
    /*Serial.println(results.value,HEX);//以16进制换行输出接收代码
      Serial.println();//为了便于观看加一个空行*/
    if (results.value == 0x00FF906F) {
      results.value = 0;
    }
    analogWrite(PWMA, BV_B);
    analogWrite(PWMB, BV_R);

    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    cm = pulseIn(EchoPin, HIGH) / 58.0; //将回拨时间换算成cm
    cm = (int(cm * 100.0)) / 100.0; //保留两位小数

  }
  //关灯上床时因检测不到人但是还未躺下半夜有人回寝室情况灯带亮当人躺下时记录最后一次cm值
  else
  {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
  }//未关灯或都躺下且无外来人时灯带熄灭

  analogWrite(PWMA, BV_B);
  analogWrite(PWMB, BV_R);

  digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //将回拨时间换算成cm
  cm = (int(cm * 100.0)) / 100.0; //保留两位小数
  //人躺下后时刻更新cm值
  if (cm > 30 && (!irrecv.decode(&results)) && getup == 1)
  {

    analogWrite(PWMA, BV_B);
    analogWrite(PWMB, BV_R);
    getup = 0;
  }//起夜时灯带亮
  if ((getup == 0) && !irrecv.decode(&results))
  {
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);    //起夜进入阳台后灯带熄灭
    if (irrecv.decode(&results))
 
    analogWrite(PWMA, BV_B);
    analogWrite(PWMB, BV_R);//从阳台出来后灯亮
    getup = 1;
  }

  irrecv.resume();
  // put your main code here, to run repeatedly:

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
