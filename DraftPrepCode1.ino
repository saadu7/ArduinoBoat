#include <Servo.h>

int motor1 = 4;         //right front 
int motor2 = 5;         //right back
int motor3 = 6;         //left front
int motor4 = 7;        //left back

int trig = 8;
int echo = 9;
int servopin =10;

Servo myservo;                      //creates an object of the class Servo and names it myservo
const int servostart = 90;         //stores servo motor start point, esssentially the angle where it starts from

int leftdistance = 0;             //initialises 2 variables that will store the distance on the left and right side, to make a decision on which side to turn if a collision is about to occur
int rightdistance = 0;              
 
float distanceleft = 0;
float distanceright = 0;
int a;
float pingTravelTime, fdistance;

void setup() {                     // setup code that only runs once
             // begins serial transmission with the serial monitor. so alerts and whatnot can be receive
  pinMode(motor1, OUTPUT);         // sets the mode (input or output) for each of the components, along with the pin they are attached at
  pinMode(motor2, OUTPUT);

  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //pinMode(servopin,OUTPUT)  - not sure if I need this line if .attach function is already used to attach servo to a particular piN
  myservo.attach(servopin);
}
int getdistance() {                             //main function used to receive the distance, which returns as a variable
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);                     //this is when the ping is launched
 
  pingTravelTime = pulseIn(echo, HIGH);        //echo will stay HIGH until the ping makes it back, and then will become LOW, as this statement is used to look at the echo pin for a HIGH pulse. giving us a recording of time elapsed when the echo pin becomes LOW again
  fdistance = (pingTravelTime*0.0334) / 2;     //since the measurement recorded was in time (microseconds) , this equation is used to convert it into distance in cm
                                               //and it involved the distance to and from a point, we divide it by 2                                            
  return fdistance;
}

int leftsee() {                                  //seperate functions for gauging the right and left distance, and works by moving the servo
  myservo.write(servostart);                     //so that it faces the approproate direction and then just uses the same getdistace() function
  delay(1000);
  myservo.write(170);
  delay(1000);
  distanceleft = getdistance();
  myservo.write(servostart);
  return distanceleft;
}

int rightsee() {
  myservo.write(servostart);
  delay(1000);
  myservo.write(5);
  delay(1000);
  distanceright = getdistance();
  myservo.write(servostart);
  return distanceright;
}

void forward() {                                    //group of simple functions that just manipulate the motors in varying ways, to have movement,
  digitalWrite(motor1, HIGH);                       //stopping, and rotation 
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
}

void backward() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
}

void stop() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
}

void turnleft() {
  digitalWrite(motor1, LOW);                         //right front
  digitalWrite(motor2, HIGH);                        //right back
  digitalWrite(motor3, HIGH);                        //left front
  digitalWrite(motor4, LOW);                         //left back
}

void turnright() {
  digitalWrite(motor1, HIGH);                        //right front
  digitalWrite(motor2, LOW);                         //right back
  digitalWrite(motor3, LOW);                         //left front
  digitalWrite(motor4, HIGH);                        //left back
}

void loop() {                       //main code that runs repeatedly
  fdistance = getdistance();                    
 
  if (fdistance <= 20) {    
    delay(300);
    stop();
    delay(200);
    leftdistance = leftsee();
    delay(200);
    rightdistance = rightsee();

 
    if (leftdistance >= rightdistance) {               //compares distance on each side to make a decision, then moves forward in that direction
      turnleft();
      delay(500);
      stop();
      delay(500);
      rightdistance = rightsee();                     //gets right distance again as it turns to the left, which should be the same as the front distance and moves the boat forward
      while (rightdistance <= fdistance) {             //until the obstacle is not there, which is when the boat turns right, allowing it to again face the finish line
      forward();
      rightdistance = rightsee();
      delay(100);
      }
      turnright();
      delay(500);
      stop();
    } else {                                           //alternate case where it gets left distance again as it turns to the right, which should be the same as the front distance and moves the boat forward
                                                       //until the obstacle is not there, which is when the boat turns left, allowing it to again face the finish line               
      turnright();
      delay(500);
      stop();
      delay(500);
      leftdistance = leftsee();
      while (leftdistance <= fdistance) {
      forward();
      leftdistance = leftsee();
      delay(100);
      }
      turnleft();
      delay(500);
      stop();
  }
   }else {
    forward();                                     
  }
}

