#include "Recv_command.h"

void setup() 
{
  Serial.begin(115200);
  init_Servo_motor();
  
}

void loop()
{
  recv_cmd();
}
