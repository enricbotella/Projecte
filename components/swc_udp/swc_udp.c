#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"


#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "swc_i2s.h"
#include "swc_udp.h"
#include "fsm_types.h"
#include "swc_uart.h"

#include "driver/i2s.h"
//#include "soc/i2s_reg.h"


#define MAX_SIZE_SEND  (512u)

static const char *TAG = "UDP";

//uint8_t uninit_client[2] = {0x01,0x00};
uint8_t init_client[2]= {0x01,0x01};
uint8_t send_pos[2] = {0x01,0x02};
uint8_t full_comm[2] = {0x01,0x03};
uint8_t shut_down[2] = {0x01,0x04};
uint8_t nested[2] = {0x01,0x05};
uint32_t counter_fail_socket = 0; 
uint32_t counter_fail_msg = 0;
uint16_t counter_send_pos = 0;
size_t  aux_byte_write;

uint8_t packet_pos (char * tx_buffer, uint8_t * local_pos, uint8_t type)
{   
    uint8_t size_data;
    size_data = POS_SIZE;
    if(type == 0x00){
    memcpy(tx_buffer,&init_client,sizeof(init_client));
    }
    else if(type == 0x01){
        memcpy(tx_buffer,&nested,sizeof(nested));
    }
    memcpy(tx_buffer + sizeof(init_client),&size_data,sizeof(size_data));
    memcpy(tx_buffer + sizeof(init_client) + sizeof(size_data),local_pos,POS_SIZE);

    return  sizeof(init_client) + sizeof(size_data) + POS_SIZE;
}



void udp_client_task(void *pvParameters)
{   
    char tx_buffer[1024]; 
    char rx_buffer[1032];
    int addr_family = 0;
    //Memory to allocate position of DWM
    uint8_t* local_pos = malloc(sizeof(uint8_t)*POS_SIZE); 

 
    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR); //Set Host IP
        dest_addr.sin_family = AF_INET; // SET IPV4
        dest_addr.sin_port = htons(PORT); //Set Port
        addr_family = AF_INET; // SET IPV4
    
        //Create socket with IPv4 address format, UDP type socket and protocol UDP.    
        int sock = socket(addr_family, SOCK_DGRAM, IPPROTO_UDP); 
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            esp_restart(); // The ESP32 is restarted to avoid unexpected behavior.
        }
        // Set timeout. As udp is connectionless, we need to set timeout for recvfrom.
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

        
        while (fsm_state != FSM_UNINIT) {

            size_t bytes_read;
            size_t bytes_write = 0;

            uint8_t local_fsm = fsm_state;
            memcpy(local_pos,pos_raw,POS_SIZE);

            //printf("Local fsm: %d",local_fsm);

            //printf("Before i2s read");

    
            switch(local_fsm){ 

                case FSM_UNINIT:

                    //NO COMMUNICATION WITH SERVER

                break;

                case FSM_INIT:


                    vTaskDelay(200/portTICK_PERIOD_MS);

                    // BUILD THE PACKET

                    bytes_write = packet_pos(tx_buffer,local_pos,0);

                break;

                case FSM_COMM:

                    counter_send_pos +=1;
                    
                    if (counter_send_pos < POS_TIMEOUT)
                    {
                        // 255 Chunk max for 16k  before collecting garbage
                        i2s_read(I2S_RX_PORT,(void *)i2s_read_buff,
                                CHUNK_SEND*sizeof(int32_t), &bytes_read, portMAX_DELAY); 

                        bytes_write = sample_32bit_to_16bit(i2s_read_buff,bytes_read,
                                                            tx_buffer,SAMPLES_TO_SEND,0);
                        
                    }
                    else{
                        
                        counter_send_pos = 0;
                        bytes_write = packet_pos(tx_buffer,local_pos,1);
                        i2s_read(I2S_RX_PORT,(void *)i2s_read_buff,
                                CHUNK_SEND*sizeof(int32_t), &aux_byte_write, portMAX_DELAY);
                        bytes_write += sample_32bit_to_16bit(i2s_read_buff,aux_byte_write,
                                                            tx_buffer,SAMPLES_TO_SEND,1);
                    
                    }
                    
                break;

                default:
                break;
            }

            int err = sendto(sock,(void *) tx_buffer, bytes_write, 0, 
                            (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                if(counter_fail_socket  >= MAX_FAIL){
                    fsm_state = FSM_INIT;
                    counter_fail_socket = 0;
                }
                counter_fail_socket += 1;
                break;

            }
            //ESP_LOGI(TAG, "Message sent");

            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);

            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, 
                            (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0) {
                //ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);

                if(counter_fail_socket  >= MAX_FAIL){
                    local_fsm = FSM_INIT;
                    counter_fail_socket = 0;
                }

                counter_fail_socket += 1;

                break;
            }
            // Data received
            else {
                //ESP_LOGI(TAG, "Received %d bytes from %s:", len, HOST_IP_ADDR);  only for debug

                 // first byte 0x03-> request to change state
                if (local_fsm == FSM_INIT && rx_buffer[0] == 0x03 && rx_buffer[1] == 0x02)
                {
                    local_fsm = FSM_COMM;
                    counter_fail_socket = 0;
                    counter_fail_msg = 0;
                }
                else if(local_fsm == FSM_COMM && rx_buffer[0] == 0x01 
                                              && rx_buffer[1] == 0x03)
                {
                    size_t bytes_to_write = 0;
                    //Copy data to DMA Bufer
                   i2s_write(I2S_TX_PORT,rx_buffer + 2,len-2,(size_t*) &bytes_to_write,20);
                }
                else if(local_fsm == FSM_COMM && rx_buffer[0] == 0x01 && rx_buffer[1] == 0x04)
                {
                    local_fsm = FSM_INIT; // Change state to init after stop comm from server
                }
                else
                {

                    if(counter_fail_msg >= MAX_FAIL)
                        {
                            local_fsm = FSM_INIT;
                            counter_fail_msg = 0;
                        }

                    counter_fail_msg += 1;
                }
                
            }

            fsm_state = local_fsm;

            

        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
