#include "Servo_motor.h"

int arg = 0;
int index = 0;
static char chr;
char cmd;

char argv1[32];
char argv2[32];

double angle_1 = 0;
double angle_2 = 0;

void resetCommand() 
{
  cmd = '\0';
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
    
  arg = 0;
  index = 0;
}


/************************************************************
*   Arduino 串口通信协议的实现，主要是接收来自 上位机 的字符串指令，然后
*根据协议规定的字段进行分割、存储。进而调用电机控制函数，将目标期望角度按顺
*序执行下去。 协议帧最后一位代表机械臂下一次需要执行的状态，具体如下：
* z ：执行运动规划，抓取物体，放置物体；
* Y ：开始正式抓取之前，需要将躺下的机械臂控制到立起来的初始状态。
* X ：结束抓取动作后，需要将立着的机械臂放回到躺下的状态。
************************************************************/
void recv_cmd()
{
  while (Serial.available() > 0)
  {
    chr = Serial.read();
    if ((chr == 'z'))
    {
      if (arg == 1) 
        argv1[index] = '\0';
      else if (arg == 2) 
        argv2[index] = '\0';
              
      angle_1 = atof(argv1);
      angle_2 = atof(argv2);  
        
      Servo1_control(angle_1);
      Servo2_control(angle_2);
          
      Serial.print("Recv: ");
      Serial.print(angle_1);
      Serial.print(" ");
      Serial.print(angle_2);
      Serial.println(" .exec ok!");
      
      resetCommand();
    }
    else if (chr == ' ') 
    {
      if (arg == 0) 
        arg = 1;
      else if (arg == 1)  
      {
        argv1[index] = '\0';
        arg = 2;
        index = 0;
      }
      else if (arg == 2)  
      {
        argv2[index] = '\0';
        arg = 3;
        index = 0;
      }      
      continue;
    }
    else
    {
      if (arg == 0)
      {
        cmd = chr;
      }
      else if (arg == 1) 
      {
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) 
      {
        argv2[index] = chr;
        index++;
      }
    }
  }
}
