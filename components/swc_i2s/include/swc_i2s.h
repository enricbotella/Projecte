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
//I2S PORT
#ifndef I2S_RX_PORT
#define I2S_RX_PORT (0u)
#endif

#ifndef I2S_TX_PORT
#define I2S_TX_PORT (1u)
#endif
//Default pins 

#ifndef I2S_DATA_IN
    #define I2S_DATA_IN (32u)
#endif

#ifndef I2S_WS_RX
    #define I2S_WS_RX (25u) 
#endif

#ifndef I2S_BCLK_RX
    #define I2S_BCLK_RX (33u)
#endif

#ifndef I2S_DATA_OUT
    #define I2S_DATA_OUT (14u)
#endif

#ifndef I2S_WS_TX
    #define I2S_WS_TX (26u)
#endif

#ifndef I2S_BCLK_TX
    #define I2S_BCLK_TX (27u)
#endif


//Driver Config

#ifndef I2S_SAMPLE_RATE
    #define I2S_SAMPLE_RATE (16000lu) 
#endif

#ifndef I2S_BITS_SAMPLE
    #define I2S_BITS_SAMPLE (32u) 
#endif

#ifndef I2S_SAMPLE_RATE_TX
    #define I2S_SAMPLE_RATE_TX (16000lu) 
#endif

#ifndef I2S_BITS_SAMPLE_TX
    #define I2S_BITS_SAMPLE_TX (16u) 
#endif

//Samles to send

#define SAMPLES_TO_SEND (512u)

/* ------------------------------- Data Types ------------------------------- */

typedef struct {
        char chunk_id[4]; /*!< Contains the letters "RIFF" in ASCII form big endian */
        uint32_t chunk_size; /*!< This is the size of the rest of the chunk following this number */
        char chunk_format[4]; /*!< Contains the letters "WAVE"  big endian*/
    } descriptor_chunk_t; /*!< Canonical WAVE format starts with the RIFF header */


typedef struct {
    char subchunk_id[4]; /*!< Contains the letters "fmt " */
    uint32_t subchunk_size; /*!< This is the size of the rest of the Subchunk which follows this number */
    uint16_t audio_format; /*!< PCM = 1, values other than 1 indicate some form of compression */
    uint16_t num_channels; /*!< Mono = 1, Stereo = 2, etc. */
    uint32_t sample_rate; /*!< 8000, 44100, etc. */
    uint32_t byte_rate; /*!< SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t block_align; /*!< NumChannels * BitsPerSample/8 */
    uint16_t bits_per_sample;
} fmt_subchunk_t;


typedef struct {
    char subchunk2_id[4]; /*!< Contains the letters "data" */
    uint32_t subchunk2_size; /*!< This is the number of bytes in the data */
} data_subchunk_t;

typedef struct 
{   
    descriptor_chunk_t descriptor_chunk;
    fmt_subchunk_t fmt_subchunk;
    data_subchunk_t data_subchunk;
    
} wav_header_t; //http://soundfile.sapp.org/doc/WaveFormat/

/* ---------------------------- Global Variables ---------------------------- */
extern int32_t*  i2s_read_buff;
extern int8_t*  i2s_write_buff;

/* --------------------------- Routine prototypes --------------------------- */
void i2s_init(void);
void i2s_read_data(void);
wav_header_t wav_head_init(void);

size_t  sample_32bit_to_16bit(int32_t* i2s_read_buff, size_t bytes_read, char * buffer ,size_t max_size,uint8_t type);
/* -------------------------------- Routines -------------------------------- */
