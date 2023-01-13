/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"


#include "driver/i2s.h"
#include "soc/i2s_reg.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#include "swc_i2s.h"
#include "station_ap.h"
#include "swc_uart.h"
#include "swc_udp.h"
#include "fsm_types.h"
#include "swc_button.h"


uint8_t fsm_state;


void app_main(void)
{

    fsm_state = FSM_UNINIT;

    wifi_comp_init();
    i2s_init();
    
    init_gpio();
    xTaskCreate(vTask_uart,"swc_uart",4096,NULL,5,NULL);
    xTaskCreate(udp_client_task, "udp_client", 8192, NULL, 5, NULL);

    fsm_state = FSM_INIT;
}
