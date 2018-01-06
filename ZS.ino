#include<LCD5110_Basic.h>
#define TP 16
#define EP 17
#define LEDC 14
#define LEDD 15
#define Yz 2
#define Zz 4
#define Yf 6
#define Zf 7
#define RX 0
#define TX 1
#define jishukaishi 13
#define jishujieshu 18
#define chaochejieshu 19

LCD5110 myGLCD(8,9,10,12,11);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

float cm;
int jiance1;
int jiance2;
int flag;
int start;
int whole;
float timee;
float speedd;
char c;

void Dshanshuo(int zw){
  digitalWrite(LEDC,HIGH);
  delay(zw);
  digitalWrite(LEDC,LOW);
  delay(zw);
  }
void Cshanshuo(int dxx){
  digitalWrite(LEDD,HIGH);
  delay(dxx);
  digitalWrite(LEDD,LOW);
  delay(dxx);
  }
void qianjin(){
  digitalWrite(Zz,HIGH);
  digitalWrite(Yz,HIGH);
  digitalWrite(Zf,LOW);
  digitalWrite(Yf,LOW);
  analogWrite(3,150);
  analogWrite(5,150);
  }
void daoche(int ck){
  digitalWrite(Zf,HIGH);
  digitalWrite(Yf,HIGH);
  digitalWrite(Zz,LOW);
  digitalWrite(Yz,LOW);
  analogWrite(3,ck);//3右轮
  analogWrite(5,ck);//5左轮
  }
void chaoche(){
  qianjin();
  if(jiance1 == LOW){
    start = millis();
    }
  if(jiance2 == LOW){
    whole = millis();
    }
  timee = whole - start;
  speedd = 50/timee;//50是我瞎写的，到时候在车上测距离
  if(speedd>100){//100也是我瞎写的，到时候算吧
    do{
    digitalWrite(Zz,HIGH);
    digitalWrite(Yz,HIGH);
    digitalWrite(Zf,LOW);
    digitalWrite(Yf,LOW);
    analogWrite(3,100);
    analogWrite(5,100);
    myGLCD.printNumI("overtake",LEFT,0);
    }while(flag == LOW);
    start = 0;
    whole = 0;
    myGLCD.printNumI("done",LEFT,0);
    }
    qianjin();
    Cshanshuo(2000);
  }
void ceju(){
  daoche(100);
  digitalWrite(TP,LOW);
  delayMicroseconds(2);
  digitalWrite(TP,HIGH);
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  cm = pulseIn(EP, HIGH)/58.0;
  if(cm>200){
    daoche(150);
    Dshanshuo(2000);
    }
  if(150<cm<200){
    daoche(120);
    Dshanshuo(1000);
    }
  if(100<cm<150){
    daoche(90);
    Dshanshuo(500);
    }
  myGLCD.printNumI(cm,RIGHT,0);
  }
void setup() {
  myGLCD.InitLCD();
  
  pinMode(TX,OUTPUT);
  pinMode(RX,INPUT);//TX
  pinMode(Yz,OUTPUT);//前进右轮
  pinMode(Zz,OUTPUT);//前进左轮
  pinMode(Yf,OUTPUT);//后退右轮
  pinMode(Zf,OUTPUT);//后退左轮

  pinMode(jishukaishi,INPUT);//计速开始
  pinMode(jishujieshu,INPUT);//计速结束
  pinMode(chaochejieshu,INPUT);//超车结束 

  pinMode(TP,OUTPUT);
  pinMode(EP,INPUT);

  pinMode(LEDC,OUTPUT);
  pinMode(LEDD,OUTPUT);
}

void loop() {
  myGLCD.setFont(SmallFont);
  c = Serial.read();
  jiance1 = digitalRead(jishukaishi);
  jiance2 = digitalRead(jishujieshu);
  flag = digitalRead(chaochejieshu);

  if(c == '1'){
  chaoche();
  }
  if(c == '2'){
  ceju(); 
  }
  
}
