#include <Servo.h>
const int trigPin3 = 9;
const int echoPin3 = 5;
const int trigPin5 = 11;
const int echoPin5 = 12;
const int speedPin = 14;
const int steerPin = 15;
const bool if_right = true;
const bool if_straight = true;

long duration;
int distance;
Servo speedControl, steerControl; //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time

const int forward = 1602; // 1549: lowest speed
const int stop_val = 1520; //1200: stop
const int reverse = 1400;

const int right = 1300;
const int straight = 1667;
const int left = 1800;

void setup(){
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);
  
  speedControl.attach(speedPin);
  steerControl.attach(steerPin);
  Serial.begin(9600);
}

void loop() {
  int dist3 = getDistance(trigPin3, echoPin3);
  int dist5 = getDistance(trigPin5, echoPin5);
  Serial.print("\ndist3:");
  Serial.print(dist3);
  Serial.print("\ndist5:");
  Serial.print(dist5);
  if(dist5 <= 15){
    stop_car();
    turn_left();
  }
  else{
    forward_car();
    go_straight();
  }
  
}
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

void forward_car(){
  speedControl.writeMicroseconds(forward);
}

void turn_right(){
  steerControl.writeMicroseconds(right);
}

void turn_left(){
  steerControl.writeMicroseconds(left);
}

void go_straight(){
  steerControl.writeMicroseconds(straight);
}
