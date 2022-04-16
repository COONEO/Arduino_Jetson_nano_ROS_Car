#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  


int last_servo1_PWM;
int last_servo2_PWM;

void Servo1_control(double current_angle);
void Servo2_control(double current_angle);

void init_Servo_motor()
{
  last_servo1_PWM= 90;
  last_servo2_PWM = 50;

  // attaches the servo on pin 6 to the servo object
  myservo1.attach(6);  
  myservo2.attach(7);  

  // setup the init position
  myservo1.write(90);
  myservo2.write(50);
  Serial.println("init Begin posetion ok!");
  
}


/******************************************************
*函数名： Servo1_control
*MG996舵机位置：最下面一个舵机 ，绕垂直于地面的轴旋转。
*MG996舵机旋转角度范围：【0，200】。
*PWM范围【0，180】。
*起始位置0度PWM值：90 
*功能：控制MG996舵机 1 旋转指定的角度；
*     PWM为【0-90】：向右旋转【0，100】为正；
*     PWM为【90-180】向左旋转【0，-100】为负；
*     
*参数： current_angle参数范围： [-100,+100] 单位度  
******************************************************/
void Servo1_control(double current_angle)
{
  double PWM_Vel;
  if(current_angle > 0)                 //  右侧区域
    PWM_Vel = 90 + current_angle * (180.0 / 200.0) ;
  else                                // 左侧区域  
    PWM_Vel = 90 - (- current_angle) * (180.0 / 200.0) ;


    if(PWM_Vel > 180 || PWM_Vel <0 )
      return;
    if(PWM_Vel > last_servo1_PWM)
    {
      for (int pos = last_servo1_PWM; pos <= PWM_Vel; pos += 1) 
      { 
        myservo1.write(pos);              
        delay(10);                       
      }
    }
    else
    {
      for (int pos = last_servo1_PWM; pos >= PWM_Vel; pos -= 1) 
      { 
        myservo1.write(pos);              
        delay(10);                       // waits 10ms for the servo to reach the position
      }
    }
    last_servo1_PWM = PWM_Vel;


}

/******************************************************
*函数名： Servo2_control
*MG996舵机位置：第二个舵机 ，控制末端前后旋转
*MG996舵机旋转角度范围：【0，200】。
*PWM范围【0，180】。
*起始位置0度PWM值：50  
*功能：控制MG996舵机 2 旋转指定的角度；
*     【0-50】:【0，-55】度，向后旋转，规定为负；
*     【50-180】:【0，125】度，向前旋转，规定为正；
*     
*     
*参数：current_angle参数范围 [-55,+125] 单位度   
******************************************************/
void Servo2_control(double current_angle)
{
    double PWM_Vel;
  if(current_angle > 0)                 //  右侧区域
    PWM_Vel = 50 + current_angle * (180.0 / 200.0) ;
  else                                // 左侧区域  
    PWM_Vel = 50 - (- current_angle) * (180.0 / 200.0) ;

    if(PWM_Vel > 180 || PWM_Vel <0 )
      return;
    if(PWM_Vel > last_servo2_PWM)
    {
      for (int pos = last_servo2_PWM; pos <= PWM_Vel; pos += 1) 
      { 
        myservo2.write(pos);              
        delay(10);                       
      }
    }
    else
    {
      for (int pos = last_servo2_PWM; pos >= PWM_Vel; pos -= 1) 
      { 
        myservo2.write(pos);              
        delay(10);                       // waits 10ms for the servo to reach the position
      }
    }
    last_servo2_PWM = PWM_Vel;
}
