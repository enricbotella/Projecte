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
#include "stdio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "fsm_types.h"
#include "swc_button.h"

/* -------------------------------- Defines --------------------------------- */
/* ------------------------------- Data Types ------------------------------- */
/* ---------------------------- Global Variables ---------------------------- */
/* --------------------------- Routine prototypes --------------------------- */
/* -------------------------------- Routines -------------------------------- */

static void IRAM_ATTR gpio_isr_handler(void* arg)
{   
    if(fsm_state == FSM_INIT){
        fsm_state = FSM_COMM;
    }
    
}


//init gpio
void init_gpio (void)
{
    //Configuration of the pin
    gpio_config_t io_conf = {

        //bit mask of the pins, use GPIO5 here
        .pin_bit_mask = 1ULL<< 5,
        //Sets input mode
        .mode = GPIO_MODE_INPUT,
        //Enables pullup mode 
        .pull_up_en = GPIO_PULLUP_ENABLE,
        //Disable pulldown mode
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        //Sets interrupt when edge goes from high to low
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    //configure gpio driver with the given settings
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    //install gpio isr service
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3));

    //hook isr handler for specific gpio pin
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_NUM_5, gpio_isr_handler, NULL));

}
