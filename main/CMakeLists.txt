idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES   swc_button
                                    swc_fsm 
                                    swc_udp 
                                    swc_uart 
                                    swc_wifi  
                                    swc_i2s 
                                    nvs_flash )
