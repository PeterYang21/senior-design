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
const int trigPin6 = 19;
const int echoPin6 = 20;
const int trigPin7 = 17;
const int echoPin7 = 18;

const int speedPin = 14;
const int steerPin = 15;
// End Pins

const int distToFront = 100;

// PWM
const int forward =  1505; //1553; 
const int stop_val = 1450;
const int reverse = 1400;

const int right = 1500;
const int straight = 1667;
const int left = 1850;
// End PWM

// Flag in iterations
int leftFlag = 0, rightFlag = 0, straightFlag = 0;
int initialFlag = 1;
// End Flag

int lot_flag; // 0 left parking lot; 1 right parking lot
int dist1,dist2,dist3,dist4,dist5,dist6,dist7;
const long interval = 10000;
Servo speedControl, steerControl;


/*  -1-7-2--
 *  |     |
 *  |     |
 *  3     4
 *  |     |
 *  |     |
 *  5     6
 *  -------
 */

void setup() {
  lot_flag = 1; //set default parking lot

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
  pinMode(trigPin6, OUTPUT);
  pinMode(echoPin6, INPUT);
  pinMode(trigPin7, OUTPUT);
  pinMode(echoPin7, INPUT);
  
  speedControl.attach(speedPin);
  steerControl.attach(steerPin);
  Serial.begin(9600);
}

void loop(){ //Mind the infinite loop()
  
 dist1 = getDistance(trigPin1, echoPin1);
 dist2 = getDistance(trigPin2, echoPin2);
 dist3 = getDistance(trigPin3, echoPin3);
 dist4 = getDistance(trigPin4, echoPin4);
 dist5 = getDistance(trigPin5, echoPin5);
 dist6 = getDistance(trigPin6, echoPin6); 
 dist7 = getDistance(trigPin7, echoPin7);
 
  Serial.print("\ndist1:");
  Serial.print(dist1);
  Serial.print(" dist2: ");
  Serial.print(dist2);
  Serial.print(" dist3: ");
  Serial.print(dist3);
  Serial.print(" dist4: ");
  Serial.print(dist4);
  Serial.print(" dist5: ");
  Serial.print(dist5);
  Serial.print(" dist6: ");
  Serial.print(dist6);
  Serial.print("\n");

  /* 
   * stage 1
   */
  if(initialFlag)
  {
    initial_move();
  }
  delay(1000); // currently add delay between stage

  /* 
   * stage 2
   */
   
  // s2-case1: car heading left
  if(leftFlag)
  {
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    if(dist3 > dist4)
    {
      headLeft_2(); // special stage 2
      turn_right();
    }
    else
    {
      headLeft(); // normal stage 2
      delay(1000);
      turn_left(); // *** start entering stage 3
    }

    /*
     * stage 3
     */
    set_parallel_1(); // todo: merge set_parallel & set_parallel_1?

    straightFlag = 1;
  }
  
  // s2-case2: car heading right
  else if(rightFlag)
  { 
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    if(dist3 < dist4)
    {
      headRight_2(); // special stage2
      turn_left();
    }
    else
    {
      headRight(); // stage2
      delay(1000);
      steerControl.writeMicroseconds(1450); // turn right
    }

    /*
     * stage 3
     */
    set_parallel_1(); // todo: merge set_parallel & set_parallel_1?

    straightFlag = 1;
  }

  // s2-case3: car heading straight
  if(straightFlag)
  { 
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    dist5 = getDistance(trigPin5, echoPin5);
    dist6 = getDistance(trigPin6, echoPin6);
    if(abs(dist3-dist4) <=1 || abs(dist5-dist6) <= 1) // middle of the road
    {
      Serial.print("check final stage\n");
      check_finalStage();
    }
    else 
    {
      Serial.print("Other straight cases\n"); // todo: other cases of straight
      if(dist4-dist3 > 1)
      {
        Serial.print("straightInLeft\n");
        straightInLeft();
        check_finalStage();
      }
      else
      {
        Serial.print("straightInRight\n");
        straightInRight();
        check_finalStage();
      }
    }
  }

  /*
   * clear all
   */
  initialFlag = 0;
  leftFlag = 0;
  rightFlag = 0;
  straightFlag = 0; 
  Serial.print("stop car in forward path\n");
  stop_car();
  
  // todo: safety, other special cases, merge some functions
  // todo: read commnands through serial, reset all flags by remote controllers?

  /*
   * add garage parking step here
   */


      turn_straight();
    delay(2000);
    
  if(lot_flag == 0){
// park to left lot  
      Serial.print("\n Park to left lot ");

//      forward_car();
      speedControl.writeMicroseconds(forward);
      steerControl.writeMicroseconds(1930);
      delay(1200);


      dist1 = getDistance(trigPin1, echoPin1);
      Serial.print("\n step1 sensor1: ");
      Serial.print(dist1);
    

      while(dist1 > 14){
//      forward_car();
      speedControl.writeMicroseconds(forward);
        steerControl.writeMicroseconds(1930);
        
        dist1 = getDistance(trigPin1, echoPin1);
        Serial.print("\n while loop sensor1: ");
        Serial.print(dist1);
     
      }
    
    stop_car();
    turn_straight();
    Serial.print("\n straight & stop \n");
    delay(3000);
//      forward_car();
      speedControl.writeMicroseconds(forward);
    steerControl.writeMicroseconds(1300);
    delay(1000);

    dist3 = getDistance(trigPin3, echoPin3);
    Serial.print("\n step2 sensor3: ");
    Serial.print(dist3);

    dist5 = getDistance(trigPin5, echoPin5);
    Serial.print("\n step2 sensor5: ");
    Serial.print(dist5);   
    
    while(dist3 != dist5){ // todo: 1
//      forward_car();
      speedControl.writeMicroseconds(forward);
      steerControl.writeMicroseconds(1250);

      dist3 = getDistance(trigPin3, echoPin3);
      Serial.print("\n while sensor3: ");
      Serial.print(dist3);

      dist5 = getDistance(trigPin5, echoPin5);
      Serial.print("\n while sensor5: ");
     Serial.print(dist5); 
    }

  } else if(lot_flag == 1){


    
//park to right lot
      Serial.print("\n Park to right lot ");

//      forward_car();
      speedControl.writeMicroseconds(forward);
      steerControl.writeMicroseconds(1295);
      delay(1200);

    //stop_car();

      dist2 = getDistance(trigPin2, echoPin2);
      Serial.print("\n step1 sensor2: ");
      Serial.print(dist2);
    

      while(dist2 > 13){
//        forward_car();
        speedControl.writeMicroseconds(forward);
        steerControl.writeMicroseconds(1295);
        
        dist2 = getDistance(trigPin2, echoPin2);
        Serial.print("\n while loop sensor2: ");
        Serial.print(dist2);
      }
    
    stop_car();
    turn_straight();
    Serial.print("\n straight & stop \n");
    delay(3000);

//      forward_car();
      speedControl.writeMicroseconds(forward);
    steerControl.writeMicroseconds(1910);
    delay(1000);

    dist4 = getDistance(trigPin4, echoPin4);
    Serial.print("\n step2 sensor4: ");
    Serial.print(dist4);

    dist6 = getDistance(trigPin6, echoPin6);
    Serial.print("\n step2 sensor6: ");
    Serial.print(dist6);   
    
    while (dist4 != dist6 ){
//      forward_car();
      speedControl.writeMicroseconds(forward);
      steerControl.writeMicroseconds(1910);

      dist4 = getDistance(trigPin4, echoPin4);
      Serial.print("\n while sensor4: ");
      Serial.print(dist4);

      dist6 = getDistance(trigPin6, echoPin6);
      Serial.print("\n while sensor6: ");
      Serial.print(dist6);
    }
    
  }

  stop_car();
  turn_straight();
  delay(3000);

  dist7 = getDistance(trigPin7, echoPin7);
  Serial.print("\n step3 sensor7: ");
  Serial.print(dist7);
  
//      forward_car();
      speedControl.writeMicroseconds(forward);
      
  while(dist7 > 12){
//      forward_car();
      speedControl.writeMicroseconds(forward);
    Serial.print("\n step3 go straight ");
    
    dist7 = getDistance(trigPin7, echoPin7);
    Serial.print("\n step3 sensor7: ");
    Serial.print(dist7);

  }

  stop_car();
  turn_straight();
  Serial.print("\n end \n");
  delay(20000);
     exit(0);

}

void initial_move() // stage1
{ 
  turn_straight();
  unsigned long startTime = millis();
  unsigned long currentTime = millis();
  int dist3_ori = getDistance(trigPin3, echoPin3);
  int dist4_ori = getDistance(trigPin4, echoPin4);
  Serial.print("\ndist3 original: ");
  Serial.print(dist3_ori);
  
  while(true)
  {
    currentTime = millis();
    dist1 = getDistance(trigPin1, echoPin1);
    dist2 = getDistance(trigPin2, echoPin2);
    
    dist3 = getDistance(trigPin3, echoPin3);
    
    if(dist1 > 8 && dist2 > 5) // todo: Jose case
    { // check initial distance
      forward_car(); 
      if(dist1 <= 10)
      {
          leftFlag = 1;
          Serial.print("\nleftFlag\n");
          Serial.print(leftFlag);
          Serial.print("enter");
      }
      else if(dist2 <= 7)
      {
          rightFlag = 1;
          Serial.print("\nrightFlag\n");
          Serial.print(rightFlag);
          Serial.print("enter");
      }
    }

//    if(abs(dist3-dist5) <= 1 || abs(dist4-dist6) <= 1) // car is straight but pretty close to one side of the road
//    {
//      
//    }
//    
    if(currentTime - startTime > 2000)
    {   
       Serial.print("Time Out\n");
       dist3 = getDistance(trigPin3, echoPin3);
       dist4 = getDistance(trigPin4, echoPin4);
       int diff3 = dist3 - dist3_ori;
       int diff4 = dist4 - dist4_ori;
       Serial.print("Dist3 after time out: ");
       Serial.print(dist3);
       
       if(abs(diff3) > 1 && abs(diff4) > 1) // not too straight
       { 
          if(diff3 > 0 && diff4 < 0) rightFlag = 1;
          if(diff4 > 0 && diff3 < 0) leftFlag = 1;
          straightFlag = 0;
          Serial.print("RightFlag: ");
          Serial.print(rightFlag);
          Serial.print(" LeftFlag: ");
          Serial.print(leftFlag);
       }
       else if(abs(diff3) <= 1 || abs(diff4) <= 1)
       { // todo: perfect straight & not in middle
          Serial.print("\nperfect straight & not in middle\n");
          straightFlag = 1;
          rightFlag = 0;
          leftFlag = 0;
       }
       break;
    }

    if((dist1 <= 8)|| (dist2 <= 5)) // if too close to sideways
    { 
        break;
    }
    
//    if((dist1 <= 8 && abs(dist3-dist5)>1 )|| (dist2 <= 4 && abs(dist4-dist6>1)) // if too close to sideways
//    { 
//        break;
//    }
   }
   
  initialFlag = 0;
  stop_car();
  Serial.print("End stage 1\n");
  Serial.print("RightFlag: ");
  Serial.print(rightFlag);
  Serial.print(" LeftFlag: ");
  Serial.print(leftFlag);
  Serial.print(" StraightFlag: ");
  Serial.print(straightFlag);
}

void headLeft()
{
  turn_right();
  dist3 = getDistance(trigPin3, echoPin3);
  dist4 = getDistance(trigPin4, echoPin4);
  while(abs(dist3-dist4) > 1)
  {
    Serial.print("\nNormal Stage1 of head left\n");
    forward_car();
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
  }
  stop_car();
  turn_straight();
  leftFlag = 0;
}

void headRight()
{ 
  steerControl.writeMicroseconds(1900); // turn left
  dist3 = getDistance(trigPin3, echoPin3);
  dist4 = getDistance(trigPin4, echoPin4);
  while(abs(dist3-dist4) > 1){ // todo: check 5 & 6
    Serial.print("\nNormal stage2 of head right\n");
      Serial.print(" dist3: ");
      Serial.print(dist3);
      Serial.print(" dist4: ");
      Serial.print(dist4);
    forward_car();
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
  }
  stop_car();
  turn_straight();
  rightFlag = 0;
}

void headRight_2()
{
  Serial.print("Special stage2: headRight_2\n");
  turn_straight();

  while(true)
  {
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    Serial.print("Dist3: ");
    Serial.print(dist3);
    Serial.print(" Dist4: ");
    Serial.print(dist4);
 
    if(abs(dist3-dist4)==0)
    {
      Serial.print("\nCar center in middle :");
      break;
    }
    else
    {
      forward_car();
    }
  }
  stop_car();
  rightFlag = 0;
  Serial.print("End special stage2\n");
  
  delay(1000);
}

void headLeft_2()
{
  Serial.print("Special stage2: headLeft_2\n");
  turn_straight();

  while(true)
  {
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    Serial.print(" Dist3: ");
    Serial.print(dist3);
    Serial.print("\nDist4: ");
    Serial.print(dist4);
    if(abs(dist3-dist4)==0)
    {
      break;
    }
    else{
      forward_car();
    }
  }
  stop_car();
  leftFlag = 0;
  Serial.print("End special stage2\n");
  
  delay(1000);
}

int getDistance(int trigPin, int echoPin)
{
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

int check_finalPosition()
{
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    Serial.print("check final position: dist3: ");
    Serial.print(dist3);
    Serial.print("check final position: dist4: ");
    Serial.print(dist4);
    if(dist3 > 100 || dist4 > 100) // car goes over forward path
    {
      return 1;
    }
    return 0;
}

void check_finalStage()
{
    while(true)
    {
//      forward_car();
      dist3 = getDistance(trigPin3, echoPin3);
      dist4 = getDistance(trigPin4, echoPin4);
      dist5 = getDistance(trigPin5, echoPin5);
      dist6 = getDistance(trigPin6, echoPin6); 
      Serial.print("\nCheck final stage\n");
      Serial.print("dist3: ");
      Serial.print(dist3);
      Serial.print(" dist4: ");
      Serial.print(dist4);
      Serial.print(" dist5: ");
      Serial.print(dist5);
      Serial.print(" dist6: ");
      Serial.print(dist6);

      if(check_finalPosition())
      {
        Serial.print("Break check_finalStage\n");
        break;
      }

      forward_car();
  
      if(dist3 == dist5 || dist4 == dist6) // Todo
      {
        // still straight
        turn_straight();
      }
      else
      {
        
        if(dist5-dist3 > 1 || dist4-dist6 > 1)
        {
          Serial.print("\nhead left in final stage\n");
          turn_right();
          //steerControl.writeMicroseconds(1470); // manual input
          set_parallel();
        }
        else if(dist3-dist5 > 1 || dist6-dist4 > 1)
        {
          Serial.print("\nhead right in final stage\n");
          turn_left();
          //steerControl.writeMicroseconds(1730);
          set_parallel();
        }
        turn_straight();
      }
    }
    Serial.print("stop car2\n");
    stop_car();
}

void set_parallel(){
//    dist3 = getDistance(trigPin3, echoPin3); // no need to update distances outside
//    dist4 = getDistance(trigPin4, echoPin4);
//    dist5 = getDistance(trigPin5, echoPin5);
//    dist6 = getDistance(trigPin6, echoPin6);
    
    while(abs(dist4-dist6) > 1 || abs(dist3-dist5) > 1)
    { // check if parallel
        if(check_finalPosition())
        {
          Serial.print("Break set_parallel\n");
          break;
        }
       forward_car(); // could remove?
       dist4 = getDistance(trigPin4, echoPin4);
       dist6 = getDistance(trigPin6, echoPin6);
       dist5 = getDistance(trigPin5, echoPin5);
       dist3 = getDistance(trigPin3, echoPin3);
        Serial.print("\nNot Parallel\n");
        Serial.print(" dist4: ");
        Serial.print(dist4);
        Serial.print(" dist6: ");
        Serial.print(dist6);
        Serial.print(" dist3: ");
        Serial.print(dist3);
        Serial.print(" dist5: ");
        Serial.print(dist5);
    }
    Serial.print("\nIs Parallel\n");
}

void set_parallel_1()
{
//    dist3 = getDistance(trigPin3, echoPin3);
//    dist4 = getDistance(trigPin4, echoPin4);
//    dist5 = getDistance(trigPin5, echoPin5);
//    dist6 = getDistance(trigPin6, echoPin6);
    
//    while(dist4 != dist6 || dist3 != dist5)
    while(abs(dist4-dist6) > 1 || abs(dist3-dist5) > 1)
    { // check if parallel
      if(check_finalPosition())
      {
        Serial.print("Break set_parallel_1\n");
        break;
      }
       forward_car();
       dist4 = getDistance(trigPin4, echoPin4);
       dist6 = getDistance(trigPin6, echoPin6);
       dist5 = getDistance(trigPin5, echoPin5);
       dist3 = getDistance(trigPin3, echoPin3);
    }
    Serial.print("\nParallel\n");
    Serial.print(" dist4: ");
    Serial.print(dist4);
    Serial.print(" dist6: ");
    Serial.print(dist6);
    Serial.print(" dist3: ");
    Serial.print(dist3);
    Serial.print(" dist5: ");
    Serial.print(dist5);

    stop_car();
    turn_straight();
}

void straightInLeft()
{
  turn_right();
  while(dist3 != dist4) //////////
  {
    if(check_finalPosition())
    {
       turn_straight();
       return;
    }
    forward_car();
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    Serial.print("\nKLAJFKLJADJFLAFJKstraightInLeft dist3: ");
    Serial.print(dist3);
    Serial.print(" dist4: ");
    Serial.print(dist4);
  }
  turn_left();
  set_parallel_1();
}

void straightInRight()
{
  turn_left();
  while(dist3-dist4 > 1)
  {
    if(check_finalPosition())
    {
       turn_straight();
       return;
    }
    forward_car();
    dist3 = getDistance(trigPin3, echoPin3);
    dist4 = getDistance(trigPin4, echoPin4);
    Serial.print("\nstraightInRight dist3: ");
    Serial.print(dist3);
    Serial.print(" dist4: ");
    Serial.print(dist4);
  }
  turn_right();
  set_parallel_1();
}

/* 
 * pwm simulations
 */
void stop_car(){
  speedControl.writeMicroseconds(stop_val); 
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
