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
/* ------------------------------- Data Types ------------------------------- */
/* ---------------------------- Global Variables ---------------------------- */
/* --------------------------- Routine prototypes --------------------------- */
void udp_client_task(void *pvParameters);
/* -------------------------------- Routines -------------------------------- */