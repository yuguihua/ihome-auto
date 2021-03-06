#include <Servo.h>
#define debug ;

Servo myservo;//定义舵机变量名

String currentSpeed = "FULL";

//小车运动模式
static int autoMode = 0; // 0 手动

//电机默认调速,200 900s能转180度
const int SPEED = 200;

int driveSpeed = 200;

int FullSpeed = 255;
int MiddleSpeed = 185;
int LowSpeed = 150;

int SpeedDiff = 35;

//电机1
int input1 = 2;
int input2 = 4;

//电机2
int input3 = 7;
int input4 = 8;

int speedAoutput = 11;
int speedBoutput = 10;

//使能A
int speedA = 200;
int speedB = 200;

int LEDPIN = 13;

int LED_R = 3;
int LED_L = 5;

//左右电机差异微调
float RATE = 8/9;

//光电感应
int eLight = 12;

const int TrigPin = A1;           //设定SR04连接到Arduino引脚

const int EchoPin = A0;        //设定SR04连接到Arduino引脚

int length = 0;
int radius = 0;
int pre_len = 0;

float Kp = 0.25;
float Kd = 0.15;

char inChar;
String inputString = "";

//打开二个前大灯
void trunOn() {
  trunOnRight();
  trunOnLeft();
}

//关闭二个大灯
void trunOff() {
  trunOffRight();
  trunOffLeft();
}

// 打开右大灯
void trunOnRight() {
  digitalWrite(LED_R, HIGH);
}

// 关闭右大灯
void trunOffRight() {
  digitalWrite(LED_R, LOW);
}

// 打开左大灯
void trunOnLeft() {
  digitalWrite(LED_L, HIGH);
}

// 关闭左大灯
void trunOffLeft() {
  digitalWrite(LED_L, LOW);
}


//是否接近最小距离了
bool isClosed() {
  int closed = 0;
  for (int i = 0; i < 20; i++) {
    if (digitalRead(eLight) == HIGH) {
      closed ++;
    }
  }
  if (closed >= 10) {
    return true;
  }
  return false;
}

/**
   二个灯同时闪
*/
void flash() {
  for (int i = 0; i < 8; i++) {
    if (i % 2 == 0) {
      trunOnRight();
      trunOnLeft();
    }
    else {
      trunOffRight();
      trunOffLeft();
    }
    delay(150);
  }
  delay(150);
}
/**
 * 闪大灯，左，右可传参数
 */
void flashLight(char type) {
  int pin = 0;
  if (type == 'R') {
    pin = LED_R;
  }
  else if (type == 'L') {
    pin = LED_L;
  }
  for (int i = 0; i < 4; i++) {
    if (i % 2 == 0) {
      digitalWrite(pin, HIGH);
    }
    else {
      digitalWrite(pin, LOW);
    }
    delay(150);
  }
  delay(150);
}

int getSpeed() {
  /**
   * 可处理速度
   */
  return driveSpeed;
}
/**
 * 获取转动每度要多少时间
 */
int getTime(){
  float secs = SPEED/driveSpeed*900/180;
  return round(secs);
}
void forward(int delayTime = 0) {
  int s = getSpeed();

  //forward 向前转
  //左边
  analogWrite(speedBoutput, s);
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
  //右边
  analogWrite(speedAoutput, s);
  digitalWrite(input3, HIGH);
  digitalWrite(input4, LOW);

  if (delayTime > 0) {
    delay(delayTime);
  }
}
//停止
void stop(int delayTime = 0) {
  // 停止
  //左边
  analogWrite(speedBoutput, 0);
  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);
  //右边
  analogWrite(speedAoutput, 0);
  digitalWrite(input3, LOW);
  digitalWrite(input4, LOW);
  if (delay > 0) {
    delay(delayTime);
  }
}
//后退
void back(int delayTime = 0) {
  int s = getSpeed();
  //左边
  analogWrite(speedBoutput, s);
  digitalWrite(input1, LOW);
  digitalWrite(input2, HIGH);
  //右边
  analogWrite(speedAoutput, s);
  digitalWrite(input3, LOW);
  digitalWrite(input4, HIGH);

  if (delayTime > 0) {
    delay(delayTime);
  }

}

void turnRight(int delayTime = 0) {
  int s = getSpeed();
  //trun right
  //左边
  analogWrite(speedBoutput, s);
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
  //右边
  analogWrite(speedAoutput, s);
  digitalWrite(input3, LOW);
  digitalWrite(input4, HIGH);

  if (delayTime > 0) {
    delay(delayTime);
  }
}

void turnLeft(int delayTime = 0) {
  int s = getSpeed();
  //trun right
  //左边
  analogWrite(speedBoutput, s);
  digitalWrite(input1, LOW);
  digitalWrite(input2, HIGH);
  //右边
  analogWrite(speedAoutput, s);
  digitalWrite(input3, HIGH);
  digitalWrite(input4, LOW);

  if (delayTime > 0) {
    delay(delayTime);
  }
}

/**
   舵机控件居中
*/
void  headCenter() {
  myservo.write(90);
}

// 声波传感器转到左边
void  headLeft() {
  myservo.write(140);
}

// 声波传感器转到右边
void  headRight() {
  myservo.write(20);
}

// 声波传感器转到指定角度
void  headTurnPositon(int pos) {
  myservo.write(pos);
}

//初始化
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//连接到串行端口，波特率为9600
  /**
   * 二个电机输出
   */
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(input3, OUTPUT);
  pinMode(input4, OUTPUT);

  /**
   * 二个大灯输出
   */
  pinMode(LED_R, OUTPUT);
  pinMode(LED_L, OUTPUT);

  /**
   * 二个电机使能输出
   */
  pinMode(speedAoutput, OUTPUT);
  pinMode(speedBoutput, OUTPUT);

  /**
   * 声波传感器
   */
  pinMode(TrigPin, OUTPUT);    //连接SR04的引脚

  pinMode(EchoPin, INPUT);      //设置输入状态

  

  //光电感应
  pinMode(eLight, INPUT);

  //电机
  myservo.attach(9);//定义舵机接口，9

  //声波传感器默认居中
  headCenter();
  
  delay(100);
  //初始化成功，闪灯提示下
  
  flash();

}

/**
 * 获取距离
 */
float getDistance(){
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);//设置引脚的输出电压为高电平或者低电平
  float distance_cm = pulseIn(EchoPin,HIGH)/58.0;//算成厘米
  distance_cm = (int(distance_cm * 100.0))/100.0;//保留两位小数
  return distance_cm;
}

/**
 * 发送串口数据到树莓派
 */
void sendToRasp(String type, String value) {
  String text = type + "," + value + "#";
  for (int i = 0; i < text.length(); i++)
  {
        Serial.write(text[i]);   // Push each char 1 by 1 on each loop pass
  }
}

/**
 * 响应串口动作
 */

void doAction(String oper, String value) {
  //获取转动时间
  int secs = getTime();
  //不同操作
  if (oper == "flash") {
    flash();
  }
  //控制小车移动,根据时间长短
  else if(oper == "move"){
    int index = value.indexOf('-');
    String direct = value.substring(0, index);
    String data = value.substring(index + 1, value.length());
    int times = data.toInt();
    if (direct == "F"){
      forward(times);
      stop();
    }
    else if (direct =="B"){
      back(times);
      stop();
    }
    else if (direct == "L"){
      turnLeft(times);
      stop();
    }
    else if (direct == "R"){
      turnRight(times);
      stop();
    } 
    
  }

    //控制小车移动,根据角度距离
  else if(oper == "move"){
    int indexi = value.indexOf('-');
    String directi = value.substring(0, indexi);
    String valuei = value.substring(indexi + 1, value.length());
    int angel = valuei.toInt();
    if (directi == "f"){
      forward(angel*secs);
      stop();
    }
    else if (directi == "b"){
      back(angel*secs);
      stop();
    }
    else if (directi == "l"){
      turnLeft(angel*secs);
      stop();
    }
    else if (directi == "r"){
      turnRight(angel*secs);
      stop();
    } 
    
  }
  
  //开左灯
  else if (oper == "OLL") {
    if (value == "0") {
      trunOffLeft();
    }
    else {
      trunOnLeft();
    }
  }
  //开右灯
  else if (oper == "ORL") {
    if (value == "0") {
      trunOffRight();
    }
    else {
      trunOnRight();
    }
  }
  //闪右大灯
  else if (oper == "FR") {
    flashLight('R');
  }
  //闪左大灯
  else if (oper == "FL") {
    flashLight('L');
  }
  //左调头
  else if (oper == "TL") {
    turnLeft(secs*180);
    stop();
  }
  //右调头
  else if (oper == "TR") {
    turnRight(secs*180);
    stop();
  }
  // 自动出避障模式
  else if(oper == "AUTO"){
    if(value=="0"){
      autoMode = 0;
    }
    else{
      autoMode = 1;
    }
  }
  else if (oper == "HL") {
    headLeft();
  }
  else if (oper == "HR") {
    headRight();
  }
  else if (oper == "HC") {
    headCenter();
  }
  /**
   * 发送距离,当前位置
   */
  else if (oper == "DS") {
    String ds = (String)getDistance();
    sendToRasp("DS", ds);
  }
  /**
   * 发送距离,左边
   */
  else if (oper == "LDS") {
    headLeft();
    delay(500);
    String ds = (String)getDistance();
    delay(500);
    sendToRasp("LDS", ds);
  }
  /**
   * 发送距离,右边
   */
  else if (oper == "RDS") {
    headRight();
    delay(500);
    String ds = (String)getDistance();
    delay(500);
    sendToRasp("RDS", ds);
  }
  /**
   * 发送距离,中间
   */
  else if (oper == "CDS") {
    headCenter();
    delay(500);
    String ds = (String)getDistance();
    delay(500);
    sendToRasp("CDS", ds);
  }
  //油门
  else if (oper == "YM") {
    int ym = value.toInt();
    driveSpeed = ym;
  }
  //前进,停止，左转，右转
  else if (oper == "CTL") {
    if (value == "F") {
      if (isClosed()) {
        stop();
      }
      forward(500);
      stop();
    }
    else if (value == "S") {
      stop();
    }
    else if (value == "B") {
      back(500);
      stop();
    }
    else if (value == "L") {
      turnLeft(secs*30);
      stop();
    }
    else if (value == "R") {
      turnRight(secs*30);
      stop();
    }
  }

}

/**
 * 自动避障模式
 */
static int autoMove(){
  int minDistance = 35;
   //自动模式
  if(autoMode==1){
    //如果小于0重新获取下距离
    bool closed = isClosed();
    if(!closed){
      stop();
      headRight();
      delay(500);
      float rightDistance = getDistance();
      #ifdef debug
        Serial.print("rightDistance=");
        Serial.println(rightDistance);
      #endif
      headLeft();
      delay(500);
      float leftDistance = getDistance();
      #ifdef debug
        Serial.print("leftDistance=");
        Serial.println(leftDistance);
      #endif
      headCenter();
      //获取下时间
      int secs = getTime();
      if(leftDistance < minDistance && rightDistance < minDistance){
        // 太近了后退下
        back(200);
        if(leftDistance>rightDistance){
          #ifdef debug
            Serial.println("trun left");
          #endif
          trunOnLeft();
          turnLeft(secs*30);
          trunOffLeft();
        }
        else{
          #ifdef debug
            Serial.println("trun right");
          #endif
          trunOnRight();
          turnRight(secs*30);
          trunOffRight();
        }
        forward();
      }
      else if(rightDistance>leftDistance){
        back(200);
        trunOnRight();
        turnRight(secs*30);
        trunOffRight();
        forward();
        #ifdef debug
          Serial.println("trun right");
        #endif
      }
      else if(leftDistance>rightDistance){
        #ifdef debug
          Serial.println("trun left");
        #endif
        back(200);
        trunOnLeft();
        turnLeft(secs*30);
        trunOffLeft();
        forward();
      }
    }
    else{
      forward();
    }
  }
    
}


void loop() {
  while (Serial.available())
  {
    inChar = Serial.read();
    if (inChar == '\n'  || inChar == '\r') {
      continue;
    }
    if (inChar == '#') {
      break;
    }
    inputString += inChar;
    delay(2);
  }

  if (inputString != "") {
    //Serial.println("inputString:" + inputString);
    int index = inputString.indexOf(',');
    String oper = inputString.substring(0, index);
    String value = inputString.substring(index + 1, inputString.length());
    doAction(oper, value);
  }
  inputString = "";

  //自动模式
  autoMove();


}
