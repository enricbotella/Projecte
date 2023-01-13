idf_component_register(SRCS "station_ap.c"
                       INCLUDE_DIRS "." "include"
                       PRIV_REQUIRES nvs_flash esp_event esp_system esp_event )
#https://esp32.com/viewtopic.php?t=11771 whAT PRIV REQUIRES DO AND WHY SHOULD WE USE IT