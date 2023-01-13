idf_component_register(SRCS "swc_udp.c"
                       INCLUDE_DIRS "." "include"
                       PRIV_REQUIRES swc_i2s swc_fsm swc_uart)
#https://esp32.com/viewtopic.php?t=11771 whAT PRIV REQUIRES DO AND WHY SHOULD WE USE IT