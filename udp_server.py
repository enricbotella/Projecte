import socket
import pyaudio
import signal
import sys

localIP     = "192.168.1.38"

localPort   = 4000

bufferSize  = 2048

counter_max_shutdown = 9999999

counter_shutdown = 0

counter_start_comm = 0

counter_max_pos = 500

server_state = 0

shutdown_flag = 0

error_flag = 0

com_flag = 0

bytesToSend = b'\x00'

postion = b''

address = None

# Set up the pyaudio recording

CHUNK = 300  #frames per buffer in outgoing stream to udp. 
            #Small as possible to avoid delays
FORMAT = pyaudio.paInt16 # 16bits per sample pcm
CHANNELS = 1 # Mono audio
RATE = 16000 #16Khz
input_p = pyaudio.PyAudio() #Create the pyaudio object
output_p = pyaudio.PyAudio() #Create the pyaudio object
CHUNK2 = 2048  # Frames per buffer to store data received from esp32


def callback(in_data, frame_count, time_info, status):
    
    global address
    global server_state
    
    if address is not None and server_state == 3:
        UDPServerSocket.sendto(b'\x01\x03' + in_data, address)

    return (None, pyaudio.paContinue)

#Stream that reads from pc mic and writes to udp  
input_stream = input_p.open(format=FORMAT,   
                channels=CHANNELS,
                rate=RATE,
                input=True,
                frames_per_buffer=CHUNK,
                stream_callback=callback)


# Stream that reads from udp incoming packets and writes to speakers  
output_stream = output_p.open(format=pyaudio.paInt16,  
                            channels=CHANNELS,
                            rate=RATE,
                            output=True,
                            frames_per_buffer=CHUNK2)


#This is the handler for the keyboard interrupt. It will shutdown 
# the server and close the streams
def sigint_handler(signal, frame): 
    print("Keyboard interrupt")
    output_stream.stop_stream()
    output_stream.close()
    input_stream.stop_stream()
    input_stream.close()
    input_p.terminate()
    output_p.terminate()
    sys.exit(0)

signal.signal(signal.SIGINT, sigint_handler) #Set the interrupt handler


# Create a datagram socket
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)



# Bind to address and ip

UDPServerSocket.bind((localIP, localPort))


print("UDP server up and listening")

# Listen for incoming datagrams
while(True):
     #Waits for incoming messages
    message,address = UDPServerSocket.recvfrom(1024) 


    if(message[:2] == b'\x01\x03'):
        server_state = 3
        #Writes audio to speakers
        output_stream.write(message[2:])


        #Writes 230 frames of audio from the PC mic  
        # to the payload of udp stream
        #bytesToSend = b'\x01\x03' + input_stream.read(230) 


    elif(message[:2] == b'\x01\x05'):

        server_state = 3
        #write the audio to the speakers 
        output_stream.write(message[23:]) 
        #Saves position
        position = message[2:21]
         #Writes 230 frames of audio to the  payload of udp stream
        #bytesToSend = b'\x01\x03' + input_stream.read(230) 

        #print("0x",position) 


    elif(message[:2] == b'\x01\x01'): #means that the client is present
        com_flag = 0
        shutdown_flag = 0
        postion = message[2:] #get the position of the client
        bytesToSend = b'\x01\x01' #send message server is present
        server_state = 1 #server is waiting to start full comm
 
    else:

        bytesToSend = b'\x01\x00' #Unknown packet received


    if(shutdown_flag == 0):

        counter_shutdown += 1

        if(counter_shutdown == counter_max_shutdown):

            shutdown_flag = 1
            counter_shutdown = 0

    if(shutdown_flag == 1):
        
        bytesToSend = b'\x01\x04'



    if(com_flag == 0 and server_state == 1):

        counter_start_comm += 1

        if(counter_start_comm == counter_max_pos):

            com_flag = 1
            counter_start_comm = 0


    if(com_flag == 1 and server_state == 1):

        bytesToSend = b'\x03\x02'

    if server_state != 3:
        UDPServerSocket.sendto(bytesToSend, address)

