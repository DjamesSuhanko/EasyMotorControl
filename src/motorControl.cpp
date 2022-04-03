#include "motorControl.h"

MotorControl::MotorControl()
{
    
}

void MotorControl::calibrate(motor target)
{
    
}
/* When this method is called, keeps running until 'target' parameter get the value 10.
This ocurrs because the variable 'target' is passed via serial and keeps 'listening' all eoc (end Of Course) from motor 'target'.
This method exists just to debug eoc before implement any movement, avoiding risks with oversteps from motor.
*/
void MotorControl::checkEOC(uint8_t target)
{
    while (target != 10){
        if (digitalRead(motorArray[target].eoc_left)){
            Serial.println("Left");
            delay(1000);
        }
        if (digitalRead(motorArray[target].eoc_left_bkp)){
            Serial.println("Left backup");
            delay(1000);
        }
        if (digitalRead(motorArray[target].eoc_right)){
            Serial.println("Right");
            delay(1000);
        }
        if (digitalRead(motorArray[target].eoc_right_bkp)){
            Serial.println("Right backup");
            delay(1000);
        }
        delay(10);
    }
}

statsPos MotorControl::moveMotor(uint32_t steps, motor target, ramp set_ramp)
{
    
}

int MotorControl::setupMotor(motor pins, uint8_t pos)
{
    if (pos > 9) return -1;
    this->motorArray[pos] = pins;
    return 0;
}

int MotorControl::setupMotor(motor pins)
{
    if (motorsAdded > 9) return -1;
    this->motorArray[motorsAdded] = pins;
    motorsAdded += 1;
    return 0;
}

int MotorControl::setupMotor(uint8_t pcf_addr)
{
    for (uint8_t i=0;i<3;i++){
        if (pcf857x_addr[i] == 0){
            pcf857x_addr[i] = pcf_addr;
            pcf_index = i;
            return;
        } 
    }
}