#include <Servo.h>

// Arduino Pins
const int trigPin1 = 7;
const int echoPin1 = 3;
const int trigPin2 = 8;
const int echoPin2 = 4;
const int trigPin3 = 9;
const int echoPin3 = 5;
const int trigPin4 = 10;
const int echoPin4 = 6;
const int trigPin5 = 11;
const int echoPin5 = 12;

const int speedPin = 14;
const int steerPin = 15;
// End Pins

const int distToFront = 100;

// PWM
const int forward = 1552; 
const int stop_val = 1520;
const int reverse = 1400;

const int right = 1500;
const int straight = 1667;
const int left = 1850;
// End PWM
 
int dist1,dist2,dist3,dist4,dist5;
unsigned long prevTime = 0;
const long interval = 10000;
unsigned long startTime;

//int check = 1;
int leftFlag = 0;
int rightFlag = 0;
int initialFlag = 1;
Servo speedControl, steerControl;


/*  -1--2--
 *  |     |
 *  |     |
 *  3     4
 *  |     |
 *  |     |
 *  5     |
 *  -------
 */

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
  
  speedControl.attach(speedPin);
  steerControl.attach(steerPin);
  Serial.begin(9600);
}

void loop(){
  //Mind the infinite loop()
 //turn_straight();
// steerControl.writeMicroseconds(1300);
 dist1 = getDistance(trigPin1, echoPin1);
 dist2 = getDistance(trigPin2, echoPin2);
 dist3 = getDistance(trigPin3, echoPin3);
 dist4 = getDistance(trigPin4, echoPin4);

  Serial.print("\ndist1:");
  Serial.print(dist1);
  Serial.print(" dist2: ");
  Serial.print(dist2);
  Serial.print(" dist3: ");
  Serial.print(dist3);
  Serial.print(" dist4: ");
  Serial.print(dist4);
  Serial.print("\n");
  
  if(initialFlag) initial_move();
  delay(2000);
  if(leftFlag){
    headLeft();
    dist3 = getDistance(trigPin3, echoPin3);
    dist5 = getDistance(trigPin5, echoPin5);
    delay(3000);
    turn_left();
    while(abs(dist3-dist5) != 0){
      forward_car();
      dist3 = getDistance(trigPin3, echoPin3);
      dist5 = getDistance(trigPin5, echoPin5);
    }
    Serial.print("\nParallel\n");
    Serial.print(" dist5: ");
    Serial.print(dist5);
    Serial.print(" dist3: ");
    Serial.print(dist3);
    Serial.print("\n");
    stop_car();
    turn_straight();
    
  }
  else if(rightFlag) headRight();
  
}

void initial_move(){
  turn_straight();
  unsigned long startTime = millis();
  unsigned long currentTime = millis();
  while(dist1 > 8 && dist2 > 8 && currentTime - startTime <= interval){ // check initial distance
    forward_car();
  
    dist1 = getDistance(trigPin1, echoPin1);
    dist2 = getDistance(trigPin2, echoPin2);
    currentTime = millis();
    if(dist1 <= 10 && dist2 > 12){
      leftFlag = 1;
      Serial.print(leftFlag);
      Serial.print("enter");
    }
    else if(dist2 <= 10 && dist1 > 12){
      rightFlag = 1;
    }
  }
  
  initialFlag = 0;
  stop_car();
}

void headLeft(){
  turn_right();
  dist3 = getDistance(trigPin3, echoPin3);
  dist4 = getDistance(trigPin4, echoPin4);
  while(abs(dist3-dist4) !=0){
    Serial.print("\nhead left\n");
    forward_car();
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
  }
  stop_car();
  turn_straight();
  leftFlag = 0;
}

void headRight(){ //todo
  
}

//void forward_left(){
//  turn_left();
//  unsigned long startTime = millis();
//  unsigned long currentTime = millis();
//  while(currentTime - startTime < 6000){
//    forward_car();
//    currentTime = millis();
//  }
//}
//
//void reverse_delay(){
//  unsigned long startTime = millis();
//  unsigned long currentTime = millis();
//  while(currentTime - startTime < 6000){
//    reverse();
//    currentTime = millis();
//  }
//}

int getDistance(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

void stop_car(){
  speedControl.writeMicroseconds(stop_val); // simulate pwm
}

//void reverse(){
//  speedControl.writeMicroseconds(reverse);
//}

void forward_car(){
  speedControl.writeMicroseconds(forward);
}

void turn_right(){
  steerControl.writeMicroseconds(right);
}

void turn_left(){
  steerControl.writeMicroseconds(left);
}

void turn_straight(){
  steerControl.writeMicroseconds(straight);
}
