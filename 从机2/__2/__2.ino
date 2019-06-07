#include <LiquidCrystal.h>
#include <IRremote.h>

int RECV_PIN = 9;//红外一体化接收头连接到Arduino的9号引脚
int LCD_light = 6;//lcd背光正极

IRrecv irrecv(RECV_PIN);
decode_results results;//用于存储编码结果的对象

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);//RS E D4 D5 D6 D7ON

bool flag_switch = 0;
String str = "";
int BV_level = 0, CCT_level = 0, AUW_level = 0;//亮度，色温，背光亮度

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 1);
  Serial.begin(9600);
  irrecv.enableIRIn();//初始化红外解码
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (irrecv.decode(&results)) {
    dump(results);
    irrecv.resume();
  }
  
}

void dump(decode_results results)
{
  switch (results.value) {
    case 0xFFC23D://开关键play
      if (flag_switch) {//已开灯
        
        flag_switch = 0;
                       
        Serial.print(str_set());//关灯
        
        lcd.clear();        
        analogWrite(LCD_light, 0);
      }
      else {//已关灯
        
        flag_switch = 1;
        
        Serial.print(str_set());//开灯
        
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);             
        analogWrite(LCD_light, AUW_level * 5);        
      }
      break;
      
     case 0xFFA857://“+”键，亮度升高
      if (flag_switch) {//已开机
        
        BV_level = (BV_level+1) % 10;//亮度等级+1
        
        Serial.print(str_set());//亮度等级+1
        
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);
      }
      break;
      
     case 0xFFE01F://“-”键，亮度降低
      if (flag_switch) {//已开机
        
        BV_level = (BV_level+9) % 10;//亮度等级-1
        
        Serial.print(str_set());//亮度等级-1
        
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);
      }
      break;
      
     case 0xFF02FD://“NEXT”键，色温升高
      if (flag_switch) {//已开机
        
        CCT_level = (CCT_level+1) % 10;//色温等级+1
        
        Serial.print(str_set());//色温等级+1
        
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);
      }
      break;
      
     case 0xFF22DD://“PREV”键，色温降低
      if (flag_switch) {//已开机
        
        CCT_level = (CCT_level+9) % 10;//色温等级-1
        
        Serial.print(str_set());//色温等级-1
        
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);
      }
      break;
      
     case 0xFFE21D://“CH+”键，背光亮度+1
      if (flag_switch) {//已开机
        
        AUW_level = (AUW_level+1) % 10;//背光亮度+1
        
        analogWrite(LCD_light, AUW_level * 5);
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);
      }
      break;
      
     case 0xFFA25D://“CH-”键，背光亮度-1
      if (flag_switch) {//已开机
        
        AUW_level = (AUW_level+9) % 10;//背光亮度-1
        
        analogWrite(LCD_light, AUW_level * 5);
        lcd.clear();
        lcd.print("BV:");
        lcd.print(BV_level, DEC);
        lcd.print(" CCT:");
        lcd.print(CCT_level, DEC);
        lcd.print(" AUW:");
        lcd.print(AUW_level, DEC);
      }
      break;
  }
}

String str_set(void)
{
  str = "";
  str += (char)('0' + flag_switch);
  str += (char)('0' + BV_level);
  str += (char)('0' + CCT_level);
  return str;
}
