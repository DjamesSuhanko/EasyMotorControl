#include <Arduino.h>
#include <Wire.h>
/*
extern "C" {
#include <stdlib.h>
#include "esp_system.h"
}
*/

uint8_t eoc_size; //! Number of end-of-courses

uint16_t pcf16bits; //! If using PCF8575, we have 16 bits to control the motor and end of course
/*! Stop with any anomaly condition. Checked all time, in each step.
This variable can be triggered by external button or any internal anomaly detected */
bool emergency_stop;

typedef struct {
    uint32_t stage_size;       //! Max size, defined during alignment
    uint32_t stage_position;   //! Actual position in stage
    uint32_t stage_steps2end;  //! How much steps we have yet
    uint32_t steps_from_zero;  //! How much steps since zero
} statsPos; //! 1 per motor

//! create motor instances. In Arduino Leonardo was tested with D5 -> EN, D6 -> DIR, D7 -> PUL. Using PCF8574, only pcf_motor.* variable needs to be used.
typedef struct {
    uint8_t pin_dir        = 0; //! Direction to move
    uint8_t pin_pul        = 0; //! Pullup pin
    uint8_t pin_en         = 0; //! Enable pin
    uint8_t eoc_left       = 0; //! End Of Course turning left
    uint8_t eoc_left_bkp   = 0; //! End Of Course backup - if there isn't, let it as 0
    uint8_t eoc_right      = 0; //! End Of Course turning right
    uint8_t eoc_right_bkp  = 0; //! End Of Course backup - if there isn't, let it as 0
} motor;

/*! The ramp will smoothetly increase speed of motor until 100%. This condition reduce effort over their axis from inertial moment.
As exemplified, the movement have 5 divisions in 1000ms, starting with 0ms between each step, so, if we whish 100 steps, the conditions gets:
1000 / 5 = 200 ms per division
200 / 10 = 20 times of 10ms per step, so, go to next division

10 / 5 = 2 ms to decrease in delay_per_steps; this means, the next one will have 20 times of 8ms delay (20*8 = 160ms; discards 40ms)
Next, 8-2=6, so we will have 20 times of 6ms (20*6 = 120ms; discards 80ms)
Next, 6-2=4, so we will have 20 times of 4ms per step (20*4 = 80; discards 120ms)
Next, 4-2=2, so we will have 20 times of 2ms per step (20*2 = 40; discards 160ms)
Next, 2-2=0, so we dont have delay anymore. Motor is out of initial inertia.

To stop, calculate amount of steps needed to complete operation and subtract from total steps to run. When triggered, make the reverse operation.
This variables are passed as function argument to moveMotor function, from class MotorControl.
*/
typedef struct {
    uint16_t delay_per_step;  //! ex.: 10ms
    uint16_t amount_of_delay; //! ex.: 1000ms
    uint16_t divisions;       //! ex.: 5
} ramp;

class MotorControl{
    public:
        enum {RO, RE, DE, DI}; //RS485: Receiver Out - connect to RX; Receiver Enable - LOW to receive data; Drivver Enable - HIGH to sign data send; Driver In - connect to TX

        MotorControl();
        statsPos moveMotor(uint32_t steps, motor target, ramp set_ramp); //! Returns all conditions of movement
        
        void calibrate(motor target);                                                     //! Will get start and end position, to grantee limits under maximum possible steps to specifc motor
        void checkEOC(uint8_t target);                                                    //! Only to teste by hand all end of course configured. This method prints position in serial
        void runMotor();
        void setupRS485(uint8_t RO_pin, uint8_t RE_pin, uint8_t DE_pin, uint8_t DI_pin);
        void rs485send(String msg);
        void rs485receive(String msg);
        
        int setPCFaddr(uint8_t addr, uint8_t arr_pcf_pos);                                //! Use to set address value for PCF8574. 
        int setupMotor(motor pins, uint8_t arr_motors_pos);                               //! Add a motor to motor array in 'arr_motors' position (until 10 units). This method is overloaded
        int setupMotor(motor pins);                                                       //! Auto-positioning in array. This method is overloaded   
        
    private:
        motor motorArray[10];   
        uint8_t motorsAdded     = 0;    //! counter to motors added 
        uint8_t pcf8574_addr[3] = {0};  //! 3 bytes to alocate addresses of PCF8574. If using CD74HC4067, just let the values like the same, as zero. Use setPCFaddr(addr,pos) to configure it.   
        uint8_t rs485_pins[4]   = {0};  //! set pins RO, RE, DE and DI using the method rs485Setup(RO,RE,DE,DI)
};

