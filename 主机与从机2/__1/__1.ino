# include<IRremote.h>

const int TrigPin = 2;
const int EchoPin = 3;
float cm = 15.0;
int light_PIN = 8;//定义灯带引脚为8
int RECV_PIN = 11;//定义红外接收器引脚为11
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  irrecv.enableIRIn();//使红外能接受
  pinMode(light_PIN,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  if(irrecv.decode(&results))
  {
          digitalWrite(light_PIN,HIGH);
      digitalWrite(TrigPin,LOW);//低高低电平发一个短时间脉冲去TrigPin
      delayMicroseconds(2);
      digitalWrite(TrigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(TrigPin,LOW);
    
      cm = pulseIn(EchoPin,HIGH)/58.0;//将回拨时间换算成cm
      cm = (int(cm*100.0))/100.0;//保留两位小数
  }//关灯后才开始测距
    if(results.value==0x123456||cm>30)
    {
    /*Serial.println(results.value,HEX);//以16进制换行输出接收代码
    Serial.println();//为了便于观看加一个空行*/
        digitalWrite(light_PIN,HIGH);
    }
  //关灯上床时因检测不到人但是还未躺下半夜有人回寝室情况灯带亮
 else
  {
    digitalWrite(light_PIN,LOW);
  }//未关灯或都躺下且无外来人时灯带熄灭
   if(cm>30&&(!irrecv.decode(&results)))
  {
    digitalWrite(light_PIN,HIGH);
  }//起夜时灯带亮
      irrecv.resume();
  // put your main code here, to run repeatedly:

}
