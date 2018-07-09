#include "../../vl53l0x_i2c_platform.h"
#include "../../vl53l0x_def.h"
#include <stdio.h>
#include <stdlib.h>




#ifdef __ARDUINO__

int VL53L0X_i2c_init(void) {
  Wire.begin();
  return VL53L0X_ERROR_NONE;
}

#else
int VL53L0X_i2c_init(unsigned int nr ,unsigned int saveAddr) {
  i2ccrw.begin(nr ,saveAddr);
  return VL53L0X_ERROR_NONE;
}

#endif


int VL53L0X_write_multi(uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count) {
#ifdef __ARDUINO__
  Wire.beginTransmission(deviceAddress);
  Wire.write(index);

  while(count--) {
    Wire.write((uint8_t)pdata[0]);
    pdata++;
  }
  Wire.endTransmission();
 #else

	
	switch(count){

		case 1:
			i2ccrw.i2c_write_bdata(index, pdata[0]);
			break;
		case 2:{
			uint16_t hexvalue;
			hexvalue = pdata[1]<< 8|pdata[0];
			printf("write pdata( 1 0): %x %x  jon:%#x \n",pdata[1],pdata[0],hexvalue);
			i2ccrw.i2c_write_wdata(index, hexvalue);
			break;}
		default:
			i2ccrw.i2c_write_block_data(index, count,pdata);
			break;
	}
	
	
 #endif
  return VL53L0X_ERROR_NONE;
}

int VL53L0X_read_multi(uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count) {
#ifdef __ARDUINO__

  Wire.beginTransmission(deviceAddress);
  Wire.write(index);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (byte)count);
  while (count--) {
    pdata[0] = Wire.read();
    pdata++;
  }
 #else
  	//i2ccrw.i2c_read_block_data(index, count,pdata);
 	switch(count){

		case 1:
			*pdata = i2ccrw.i2c_read_bdata(index);
			break;
		case 2:{
			uint16_t hexvalue;
			hexvalue = i2ccrw.i2c_read_wdata(index);
			pdata[0] = (hexvalue & 0x00FF);
			pdata[1] = (hexvalue & 0xFF00) >> 8;
			printf("read pdata( 1 0): %x %x  jon:%#x \n",pdata[1],pdata[0],hexvalue);
			break;}
		default:
			i2ccrw.i2c_read_block_data(index, count,pdata);
			break;
	}
 	
 #endif
  return VL53L0X_ERROR_NONE;
}

int VL53L0X_write_byte(uint8_t deviceAddress, uint8_t index, uint8_t data) {
  return VL53L0X_write_multi(deviceAddress, index, &data, 1);
}

int VL53L0X_write_word(uint8_t deviceAddress, uint8_t index, uint16_t data) {
  uint8_t buff[2];
  buff[1] = data & 0xFF;
  buff[0] = data >> 8;
  return VL53L0X_write_multi(deviceAddress, index, buff, 2);
}

int VL53L0X_write_dword(uint8_t deviceAddress, uint8_t index, uint32_t data) {
  uint8_t buff[4];

  buff[3] = data & 0xFF;
  buff[2] = data >> 8;
  buff[1] = data >> 16;
  buff[0] = data >> 24;

  return VL53L0X_write_multi(deviceAddress, index, buff, 4);
}

int VL53L0X_read_byte(uint8_t deviceAddress, uint8_t index, uint8_t *data) {
  return VL53L0X_read_multi(deviceAddress, index, data, 1);
}

int VL53L0X_read_word(uint8_t deviceAddress, uint8_t index, uint16_t *data) {
  uint8_t buff[2];
  int r = VL53L0X_read_multi(deviceAddress, index, buff, 2);

  uint16_t tmp;
  tmp = buff[0];
  tmp <<= 8;
  tmp |= buff[1];
  *data = tmp;

  return r;
}

int VL53L0X_read_dword(uint8_t deviceAddress, uint8_t index, uint32_t *data) {
  uint8_t buff[4];
  //int r = VL53L0X_read_multi(deviceAddress, index, buff, 4);
  int r = VL53L0X_read_multi(deviceAddress, index, buff, 4);

  uint32_t tmp;
  tmp = buff[0];
  tmp <<= 8;
  tmp |= buff[1];
  tmp <<= 8;
  tmp |= buff[2];
  tmp <<= 8;
  tmp |= buff[3];

  *data = tmp;

  return r;
}
