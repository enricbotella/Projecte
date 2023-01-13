
<!-- ABOUT THE PROJECT -->
## About The Project



Code for ESP32 of a Nurse Calling System. The ESP32 is intended to be used with the DWM1001-DEV which provides accurate positioning.
An I2S MIC for audio capturing and an DAC + speaker for playback.

Real Time Audio streaming using UDP.



## Usage

To set wifi credentials go to swc_wifi->station_ap.c:

##     Change the below entries to strings with
##     the config you want - ie #define WIFI_SSID "ADD YOUR SSID"

    #define WIFI_SSID      "ADD YOUR SSID"
    #define WIFI_PASS      "ADD_YOUR_PASSWORD"

To set Host and Port IPgot to swc_udp -> include -> swc_udp.h

## Change the below entries
## 
    #define HOST_IP_ADDR "ADD_HOST_IP_HERE"
    #define PORT (4000u)

