#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <Servo.h>

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

// ros::NodeHandle nh;
ros::NodeHandle nh;

Servo servo;
enum Motor{LEFT, RIGHT};
const int servoPin = 11;
const  int enAPin = 6;//   Left  motor PWM speed control
const  int in1Pin  =  7;//   Left  motor  Direction  1
const  int in2Pin  =  5;//   Left  motor  Direction  2
const  int in3Pin  =  4;//   Right  motor  Direction  1
const  int in4Pin  =  2;//   Right  motor  Direction  2
const  int enBPin = 3;//   Right  motor PWM speed control

int left_wheel = 0;
int right_wheel = 0;
int angle = 90;

void go (enum Motor m, int speed)
{
  if (speed > 255)
  {
    speed = 255;
  }
  else if (speed < -255)
  {
    speed = -255;
  }
  if ( m == LEFT && speed >= 0)
  {
    digitalWrite( in3Pin, LOW);
    digitalWrite( in4Pin, HIGH);
    analogWrite( enBPin, speed);
  }
  else if ( m == LEFT && speed < 0)
  {
    digitalWrite( in3Pin, HIGH);
    digitalWrite( in4Pin, LOW);
    analogWrite( enBPin, -speed);
  }
  else if ( m == RIGHT && speed >= 0)
  {
    digitalWrite( in1Pin, HIGH);
    digitalWrite( in2Pin, LOW);
    analogWrite( enAPin, speed);
  }
  else if ( m == RIGHT && speed < 0)
  {
    digitalWrite( in1Pin, LOW);
    digitalWrite( in2Pin, HIGH);
    analogWrite( enAPin, -speed);
  }
}

void executeMsg()
{
  servo.write(angle);
  // delay(15);
}

void engineCb(const geometry_msgs::Twist& engine_msg)
{
  left_wheel = engine_msg.linear.x;
  right_wheel = engine_msg.linear.y;
  angle = engine_msg.angular.x;
  executeMsg();
  go(LEFT, left_wheel);
  go(RIGHT, right_wheel);
}

ros::Subscriber<geometry_msgs::Twist> sub_engine("xbox_controller/engine", &engineCb );

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
  nh.initNode();
  //nh.subscribe(sub);
  nh.subscribe(sub_engine);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(1);
}
