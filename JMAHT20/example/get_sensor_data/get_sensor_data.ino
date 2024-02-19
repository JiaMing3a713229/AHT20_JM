#include <Arduino.h>
#include <driver/i2c.h>

#ifdef __cplusplus
extern "C"{
  #include "JM_AHT20.h"
}
#endif



#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_MASTER_NUM 0                                /*!< esp32內部有2個I2C控制器，我們使用第1個(0)控制器*/
#define I2C_MASTER_FREQ_HZ          400000              /*!< I2C master clock frequency，400KHz */
#define I2C_MASTER_TX_BUF_DISABLE   0                   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                   /*!< I2C master doesn't need buffer */


#define AHT20_ADDRESS 0x38  // AHT20 I2C 位址

AHT20 aht20;
char aht_buffer[50];

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

void aht20Task(void *pvParameter){

  vTaskDelay(100 / portTICK_PERIOD_MS);
  init_aht20(&aht20, AHT20_ADDRESS);
  int index = 0;
  while(1){

    index++;
    snprintf(aht_buffer, sizeof(aht_buffer), " Timestamp :%5d T: %2d H: %2d ", index, int(aht20.get_temperature(&aht20)), int(aht20.get_humidity(&aht20)));
    printf("%s \n", aht_buffer);
    vTaskDelay(500 / portTICK_PERIOD_MS);

  }
}



void setup() {

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.begin(115200);

  //初始化並且啟用ESP32 I2C通訊介面
  if(init_i2c() == ESP_OK){
    Serial.println("i2c initialize sucessed");
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.printf("start oled task \r\n");

  //建立並初始化顯示工作的Threads
  xTaskCreate(aht20Task, "aht20Task", 1024 * 2, NULL, 1, NULL);
}

void loop() {
  
}
