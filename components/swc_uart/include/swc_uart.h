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
/* -------------------------------- Defines --------------------------------- */

#define BUF_SIZE  (256)
#define BAUD_RATE (115200)
#define TAG_UART "SWC_UART"
#define ECHO_READ_TOUT (3)
#define UART_NUM (2)
#define POS_SIZE (18)
/* ------------------------------- Data Types ------------------------------- */
typedef struct
{
    uint8_t type;
    uint8_t length;
    uint8_t err_code;
    uint8_t type2;
    uint8_t length2;
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t pos_z;
    uint8_t q;

} position_t;
/* ---------------------------- Global Variables ---------------------------- */
extern uint8_t* data;
//extern uint8_t uart_state;
extern position_t pos;
extern uint8_t* pos_raw;
/* --------------------------- Routine prototypes --------------------------- */
void vTask_uart(void *pvParameters);
/* -------------------------------- Routines -------------------------------- */
