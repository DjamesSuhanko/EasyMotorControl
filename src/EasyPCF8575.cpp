#include <EasyPCF8575.h>

EasyPCF8575::EasyPCF8575()
{
    this->pcf_address      = 0;   //must be changed in setup() function
    this->byte_left_value  = 255; //This is the initial value when starting PCF8575
    this->byte_right_value = 255; //Same above
    this->i2c_exists       = false;
}

void EasyPCF8575::changeBytesValue(uint8_t *value)
{
    //create a local buffer
    uint8_t localBuf[2];

    //read two bytes from pcf
    Wire.requestFrom(this->pcf_address,2);
    if (Wire.available()){
        Wire.readBytes(localBuf,2);
    }

    //apply mask and write to pcf
    //.

}

void EasyPCF8575::changeBytesValue(uint16_t value)
{

}

void EasyPCF8575::changeLeftByteValue(uint8_t value)
{

}

void EasyPCF8575::changeRightByteValue(uint8_t value)
{

}

uint8_t EasyPCF8575::findPCFaddr()
{
    byte error, address;

    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
 
        if (error == 0){
            this->i2c_exists = true;
            return address;
        }
        else if (error==4){
            return 0;
        }    
    }
    if (this->i2c_exists == false){
        return 0;
    }
}

uint8_t EasyPCF8575::getBitValue(uint8_t bit0UpTo15)
{
    uint8_t localBuf[2];
    Wire.requestFrom(this->pcf_address,2);
    if (Wire.available()){
        Wire.readBytes(localBuf,2);
    }

    uint16_t bothBytes = (localBuf[0] << 8) | (localBuf[1] << 0);
    return bothBytes&(1<<bit0UpTo15);
}

uint8_t *EasyPCF8575::getBytesValueAsBytes()
{

}

uint16_t EasyPCF8575::getBytesValueAsInt()
{

}

uint8_t EasyPCF8575::getLeftBitValue(uint8_t bit0upTo7)
{

}

uint8_t EasyPCF8575::getLeftByteValue()
{

}

uint8_t EasyPCF8575::getRightBitValue(uint8_t bit0upTo7)
{

}

uint8_t EasyPCF8575::getRightByteValue()
{

}

void EasyPCF8575::setAllBitsDown()
{

}

void EasyPCF8575::setAllBitsUp()
{

}

void EasyPCF8575::setBitDown(uint8_t bit0upTo15)
{

}

void EasyPCF8575::setBitUp(uint8_t bit0upTo15)
{

}

void EasyPCF8575::setInvertBit(uint8_t bit0upTo15)
{

}

void EasyPCF8575::setLeftBitDown(uint8_t bit0upTo7)
{

}

void EasyPCF8575::setLeftBitUp(uint8_t bit0upTo7)
{

}

void EasyPCF8575::setNewBytesValue(uint8_t *value)
{

}

void EasyPCF8575::setNewBytesValue(uint16_t value)
{

}

void EasyPCF8575::setNewLeftByteValue(uint8_t value)
{

}

void EasyPCF8575::setNewRightByteValue(uint8_t value)
{

}

void EasyPCF8575::setRightBitDown(uint8_t bit0upTo7)
{

}

void EasyPCF8575::setRightBitUp(uint8_t bit0upTo7)
{

}

void EasyPCF8575::startI2C(uint8_t sda_pin, uint8_t scl_pin, uint8_t pcf_addr)
{
    Wire.begin(sda_pin, scl_pin);
    this->pcf_address = pcf_addr >126 ? this->findPCFaddr() : pcf_addr;
    this->i2c_exists = this->pcf_address > 0 ? true : false;
}

void EasyPCF8575::startI2C(uint8_t pcf_addr)
{
    Wire.begin();
    this->pcf_address = pcf_addr >126 ? this->findPCFaddr() : pcf_addr;
    this->i2c_exists = this->pcf_address > 0 ? true : false;
}