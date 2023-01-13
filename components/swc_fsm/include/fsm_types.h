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
/* ------------------------------- Data Types ------------------------------- */
enum states{
    FSM_UNINIT,
    FSM_INIT,
    FSM_COMM,
};


/* ---------------------------- Global Variables ---------------------------- */
//Semaphore for fsm_state
//extern SemaphoreHandle_t xSemaphore;
//Global fsm_state
extern uint8_t fsm_state;
//Struct for fsm_state and sempahore
/*
typedef struct{
    uint8_t fsm_state;
    SemaphoreHandle_t xSemaphore;
}fsm_state_t;
*/
/* --------------------------- Routine prototypes --------------------------- */
/* -------------------------------- Routines -------------------------------- */