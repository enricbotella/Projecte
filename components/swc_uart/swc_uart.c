//All modules written in C must contain a definition file (*.h) with the constants, macros, global variables and functions prototypes which will have a public interest (belong to the interface of the module).

/*****************************************************************************
| Language:       |  C
|-----------------|------------------------------------------------------------
| Project:        | 
|------------------------------------------------------------------------------
|    Date  - Coder.                Description
|
|------------------------------------------------------------------------------
| FILE DESCRIPTION:
|
******************************************************************************/
/* -------------------------------- Includes -------------------------------- */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "swc_uart.h"
#include "fsm_types.h"

/* -------------------------------- Defines --------------------------------- */
/* ------------------------------- Data Types ------------------------------- */
/* ---------------------------- Global Variables ---------------------------- */
uint8_t* data;
//uint8_t uart_state = 0;
QueueHandle_t uart_queue;
position_t pos;
uint8_t* pos_raw;
/* --------------------------- Routine prototypes --------------------------- */
void uart_init(void);
/* -------------------------------- Routines -------------------------------- */

//Set driver

void uart_init(void)
{   

    data = (uint8_t*) malloc(BUF_SIZE);                     //Allocate memory for data incoming from DEV.
    pos_raw = (uint8_t*) calloc(POS_SIZE,sizeof(uint8_t));  //Definition of global 
                                                            //variable pos_raw. With 18 bytes of the position data
    
    uart_config_t uart_config = {  //Structure with uart  driver parameters
        .baud_rate = BAUD_RATE,     //115200 bauds
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Set UART log level
    esp_log_level_set(TAG_UART, ESP_LOG_INFO);

    ESP_LOGI(TAG_UART, "Start  UART configuration.");

    // Install UART driver using an event queue here, 
    //which will create an event when data is read and a buffer to store received data.
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE, 0, 10, &uart_queue, 0));
    vTaskDelay(20);

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    ESP_LOGI(TAG_UART, "UART set pins, mode and install driver.");

    // Set UART pins as per Config settings
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, 17, 16, UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE ));


}




void read_write(void)
{   
    uart_event_t event; 
    uint8_t pos_req[2] = {0x02,0x00}; // request  position payload

    // Send request  position payload to uart 
    if (uart_write_bytes(UART_NUM, (const void *) &pos_req, sizeof(pos_req)) != sizeof(pos_req)) { 
        ESP_LOGE(TAG_UART, "Data not sent");
        
    }
    else if(xQueueReceive( uart_queue, (void *) &event, portMAX_DELAY)){ // Wait for data from uart

            bzero(data, BUF_SIZE); //Clear data buffer
            switch(event.type)  
            {
                case UART_DATA: // Data received

                //ESP_LOGI(TAG_UART,"event size = %d \n", (int) event.size ); 
                uart_read_bytes(UART_NUM, data ,event.size, portMAX_DELAY); // Read data received
            
                // Check if response equals to expected response payload.
                if((int) event.size == 18 && (uint8_t) data[2] == 0 && (uint8_t) data[0] == 64 && (uint8_t) data[4] == 13 ) 
                {   

                    memcpy(pos_raw,data,event.size); // copy data to global variable pos_raw

                }
                default:

                break;
            }
    }
    else{
        ESP_LOGI(TAG_UART,"No data received \n");
    }

}

void vTask_uart(void *pvParameters)
{   
    uint8_t local_fsm = fsm_state;

    uart_init();


    while(1)
    {   
        local_fsm = fsm_state;
        
        if(local_fsm != FSM_UNINIT){

            read_write();
        }
        else{

        }
        vTaskDelay(15000/ portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);

}