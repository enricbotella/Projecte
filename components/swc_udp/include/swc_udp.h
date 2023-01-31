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
#define HOST_IP_ADDR "ADD_HOST_IP_HERE"
#define PORT (4000u)
#define MAX_FAIL (10000u)
#define POS_TIMEOUT (500u)
#define CHUNK (32u)
#define CHUNK_SEND (255u)
#define TX_BUFF_SIZE (1024u)
#define RX_BUFF_SIZE (1032u)
#define TRUE (1u)
#define FALSE (0u)
/* ------------------------------- Data Types ------------------------------- */
struct udp_t {
    int sock;
    struct sockaddr_in dest_addr;
    struct sockaddr_in src_addr;
    struct timeval timeout;
    char tx_buffer[TX_BUFF_SIZE];
    char rx_buffer[RX_BUFF_SIZE];
    uint8_t socket_init;
    uint8_t error_rx;
};
/* ---------------------------- Global Variables ---------------------------- */
/* --------------------------- Routine prototypes --------------------------- */
void udp_client_task(void *pvParameters);
void vTaskReceiveUDP(void *pvParameters);
void vTaskSendUDP(void *pvParameters);
/* -------------------------------- Routines -------------------------------- */