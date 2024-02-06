## AHT20 Liabrary
___
#### AHT20是一款基於i2c通訊匯流排進行資料傳輸的高精度溫濕度感測器，本節將介紹如何以ESP32使用JM_AHT20來讀取溫度和濕度的資料。

### 1. 下載此JMAHT20壓縮檔，並匯入至Arduino
![image](https://hackmd.io/_uploads/r1WpipJoa.png)
___
![image](https://hackmd.io/_uploads/Hk2N3akjT.png)

### 2. include JMAHT20.h
```c
#ifdef __cplusplus
extern "C"{
  #include "JM_AHT20.h"
}
#endif
```
### 3. 啟用並配置ESP32 I2C通訊界面
```c
#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_MASTER_NUM 0                                /*!< esp32內部有2個I2C控制器，我們使用第1個(0)控制器*/
#define I2C_MASTER_FREQ_HZ          400000              /*!< I2C master clock frequency，400KHz */
#define I2C_MASTER_TX_BUF_DISABLE   0                   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                   /*!< I2C master doesn't need buffer */


static esp_err_t i2c_master_init(int sda_io_num, int scl_io_num){

    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_io_num,
        .scl_io_num = scl_io_num,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
          .clk_speed = I2C_MASTER_FREQ_HZ,
        }
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t init_i2c(void){

  while(i2c_master_init(SDA_PIN, SCL_PIN) != ESP_OK){

        vTaskDelay(20 / portTICK_PERIOD_MS);

  }
  return ESP_OK;

}
```
### 3. 實例化AHT20 為 aht20
```c
AHT20 aht20;
```
### 4. 初始化 aht20
```c
init_aht20(&aht20, AHT20_ADDRESS);
```
### 5. 讀取溫度/濕度
```c
aht20.get_temperature(&aht20)
aht20.get_humidity(&aht20)
```
---
# **JMAHT20 API**
## **init_aht20**
---
##### This function used to initialize aht20 and create a thread to handle sampling data
*  struct aht20: 實例化之後的aht20物件
*  uint8_t slave_addr: aht20 在i2c中的從端位址
---
## **get_temperature**
---
##### This function used to get temperature from aht20
* struct aht20: 實例化之後的aht20物件
#### return : temperature (float)
---

## **get_humidity**
---
##### This function used to get humidity from aht20
* struct aht20: 實例化之後的aht20物件
#### return : humidity (float)
---

### 範例程式
[https://github.com/JiaMing3a713229/AHT20_JM](https://github.com/JiaMing3a713229/AHT20_JM)
---
### 執行結果
![image](https://hackmd.io/_uploads/HJVylCkop.png)


