#include <IRremote.h>   // 引用 IRRemote 函式库
# define AD5 A5   //定义模拟口A5

int Intensity = 0;//光照度值
 int PIR = 9;//PIR选择可重复触发模式

 int led = 13;

 //int redray = 3;
IRsend irsend;        // 定义 IRsend 物件来发射红外线讯号

void setup() {

  Serial.begin(9600);
  
  pinMode(led,OUTPUT);

  //pinMode(redray, OUTPUT);
  
  pinMode(PIR,INPUT);
  // put your setup code here, to run once:

}


void loop() 
{ 
  Intensity = analogRead(AD5);
  if(Intensity<15)
  {  
     if(digitalRead(PIR))
  {
  digitalWrite(led,HIGH);
   irsend.sendNEC(0x123456, 32);//这里的123456编码即灯带点亮的编码
  delay(200);
  }
  else
  {
  digitalWrite(led,LOW);
   delay(200); 
  } 
  }
}
