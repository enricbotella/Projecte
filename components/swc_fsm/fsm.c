// Future task Create a function to copy fsm state to local using critical sections

//Create function that writes to global function fsm_state using semaphore

void write_fsm(uint8_t * local_fsm){
    //Copy local_fsm to global fsm_state using semaphore
    xSemaphoreTake( xSemaphore, portMAX_DELAY );
    fsm_state = *local_fsm;
    xSemaphoreGive( xSemaphore );
}

//Function that reads from fsm and copìes to local function
void read_fsm(uint8_t * local_fsm){
    //Copy globla fsm to local state using semaphore
    xSemaphoreTake( xSemaphore, portMAX_DELAY );
    *local_fsm = fsm_state;
    xSemaphoreGive( xSemaphore );
}