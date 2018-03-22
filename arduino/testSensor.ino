#include <Servo.h>
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;
int value = 0; // set values you need to zero
Servo speedControl, steerControl; //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time

const int forward = 1380;//1380;
const int stop_val = 1200;
const int reverse = 1100;
//const int right = ;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  speedControl.attach(11); // SPEED
  steerControl.attach(12); // STEER
  Serial.begin(9600);
}

void loop() {
  int dist = getDistance();
  //printf("%f\n",dist);
  Serial.print("\n");
  Serial.print(dist);
  if(dist < 15){
    stop_car();
  }
  else{
    forward_car();
  }
}

int getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void stop_car(){
  speedControl.writeMicroseconds(stop_val); // simulate pwm
}

void forward_car(){
  speedControl.writeMicroseconds(forward);
}

void turn_right(){
//  steerControl.writeMicroseconds();
}

void turn_left(){
  //steerControl.writeMicroseconds();
}

