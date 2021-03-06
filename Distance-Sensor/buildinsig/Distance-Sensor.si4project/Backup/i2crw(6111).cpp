extern "C" {
 #include <stdlib.h>
 #include <string.h>
 #include <inttypes.h>
 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <sys/ioctl.h>
 #include <fcntl.h>
 #include <errno.h>
 #include <linux/i2c-dev.h>
 #include <linux/i2c.h>
 #include <unistd.h>
 #include "../../i2crw.h"
}




uint32_t I2Common::adapterNr = 0;
uint32_t I2Common::saveAddress = 0;
int32_t	 I2Common::file = -1;
char 	 I2Common::filename[20];


I2Common :: I2Common(void) 
{
	adapterNr = 0;
	saveAddress = 0;
	file  = -1;
}


void I2Common :: begin(uint32_t nr ,uint32_t saveAddr)
{
	
	
	adapterNr 	= nr;
	saveAddress = saveAddr;

	snprintf(filename, 19, "/dev/i2c-%d", adapterNr);
	file = open(filename, O_RDWR);
	if (file < 0) {
		perror("open i2c device error:");
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, saveAddress) < 0) {
		perror("i2c device ioctl error:");
		exit(1);
	}


}


static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data)
{
	__s32 err;
	
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;

	err = ioctl(file, I2C_SMBUS, &args);
	if (err == -1)
		err = -errno;
	return err;
		
        
}
   
static inline __s32 i2c_smbus_read_word_data(int file, __u8 command)
{

	union i2c_smbus_data data;
	int err;

	err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
			       I2C_SMBUS_WORD_DATA, &data);
	if (err < 0)
		return err;

	return 0x0FFFF & data.word;

}


static inline __s32 i2c_smbus_write_wdata(int file,__u8 command,__u16 value)
{
        union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_WORD_DATA, &data);
}


static inline __s32 i2c_smbus_read_block(int file, __u8 command,__u8 length,
                                              __u8 *values)
{
	union i2c_smbus_data data;
		int i, err;
	
		if (length > I2C_SMBUS_BLOCK_MAX)
			length = I2C_SMBUS_BLOCK_MAX;
		data.block[0] = length;
	
		err = i2c_smbus_access(file, I2C_SMBUS_READ, command,
					   length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN :
					I2C_SMBUS_I2C_BLOCK_DATA, &data);
		if (err < 0)
			return err;

		printf("Read value: ");
		for (i = 1; i <= data.block[0]; i++){
			values[i-1] = data.block[i];
			printf("%#x ",values[i-1]);
		}
		printf("\n");
		return data.block[0];


	return 0;
	
}

static inline __s32 i2c_smbus_write_block_notcontain_lenth(int file, __u8 command,
                                               __u8 length, __u8 *values)
{
#if 0
	union i2c_smbus_data data;
	int i;
	printf("reg : %#x  len:%d \n",command,length);
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;
	printf("write value: ");
	for (i = 1; i <= length; i++){
		data.block[i] = values[i-1];
		printf("%#x ",data.block[i]);
	}
	printf("\n");
	
	data.block[0] = length;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_BLOCK_DATA, &data);
#else
	union i2c_smbus_data data;
	int i;
	printf("reg : %#x  len:%d \n",command,length);
	if (length > I2C_SMBUS_BLOCK_MAX)
		length = I2C_SMBUS_BLOCK_MAX;
	printf("write value: ");
	
	for (i = 1; i <= length; i++){
		data.block[i] = values[i-1];
		printf("%#x ",data.block[i]);
		}
	data.block[0] = length;
	printf("\n");
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command,
				I2C_SMBUS_I2C_BLOCK_BROKEN, &data);
#endif
	
}			
											   

static inline __s32 i2c_smbus_read_byte_data(int file, __u8 command)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
                         I2C_SMBUS_BYTE_DATA,&data))
        return -1;
    else
        return 0x0FF & data.byte;
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command,
                                              __u8 value)
{
    union i2c_smbus_data data;
    data.byte = value;
    return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
                            I2C_SMBUS_BYTE_DATA, &data);
}


int32_t I2Common ::i2c_read_bdata( uint8_t address )
{
	char value;

	if ((value = i2c_smbus_read_byte_data(file, address) )< 0) {
		 return -1;
	}


	return value;
}

int32_t I2Common ::i2c_write_bdata(uint8_t address,uint8_t value)
{
	if (i2c_smbus_write_byte_data(file, address, value) < 0) {
		 return -1;
	}
	 return 0;
}
	

int32_t I2Common :: i2c_read_wdata(uint8_t reg)
{
	return i2c_smbus_read_word_data(file,reg);
}


int32_t I2Common :: i2c_write_wdata(uint8_t reg , uint16_t value)
{
	return i2c_smbus_write_wdata(file, reg, value);
}


int32_t I2Common ::  i2c_read_block_data( uint8_t command,uint8_t length,uint8_t*values )
{
	return i2c_smbus_read_block(file, command,length,values);
}

int32_t I2Common ::  i2c_write_block_data(uint8_t command, uint8_t length,uint8_t*values)
{
	return i2c_smbus_write_block_notcontain_lenth(file, command,length, values);
}


void I2Common ::end(void)
{
	close(file);
}


I2Common i2ccrw = I2Common();


