#ifndef _JM_SHT20_H
#include <stdint.h>
#define _JM_SHT20_H

#define TRIGGER_CMD 0XAC
#define TRIGGER_DATA1 0X33
#define TRIGGER_DATA2 0X00

#define AHT20_CMD_INIT              0x71
#define AHT20_CMD_TRIGGER_MEASURE   0xAC
#define I2C_MASTER_NUM              0
struct i2c_dev{

    uint8_t slave_addr;
    float temperature;
    float humidity;

};


struct AHT20{

    struct i2c_dev *dev;
    /*
    ** @brief THE function used to get temperature from aht20
    */
    float (*get_temperature)(struct AHT20 *self);
    /*
    ** @brief THE function used to get humidity from aht20
    */
    float (*get_humidity)(struct AHT20 *self);
    /*
    ** @brief THE function used to free memory
    */
    void (*close)(struct AHT20 *self);

};

/*
** @brief THE function used to initialize aht20 and create a thread to handle sampling data
*/
int init_aht20(struct AHT20 *self, uint8_t dev_slave_addr);




#endif //_JM_SHT20_H