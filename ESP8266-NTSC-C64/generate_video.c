/******************************************************************************
 * Copyright 2013-2015 Espressif Systems
 *           2015 <>< Charles Lohr
 *           2017 Hrvoje Cavrak
 *           2018 Jan Ostman
 *           
 *           This has been rewritten to generate hires NTSC mono graphics
 */

#include "slc_register.h"
#include <c_types.h>
#include "user_interface.h"
#include "pin_mux_register.h"
#include "dmastuff.h"
#include "cpu.h"

#define WS_I2S_BCK 1
#define WS_I2S_DIV 2

#define LINE_BUFFER_LENGTH 159
#define NTSC_LINES 525
#define LINETYPES 6

#define XRES320   //320x200 resolution
#define YOFFS 0

#define SYNC_LEVEL    0x00000000
#define WHITE_LEVEL   0xffffffff
#define BLACK_LEVEL   0x88888888

//1111 1111 1110 0000 0000 0011 1111 1111
//1000 0000 0000 1111 1111 1110 0000 0000
//0001 1111 1111 1100 0000 0000 0111 1111
//1111 0000 0000 0001 1111 1111 1100 0000
//0000 0011 1111 1111 1000 0000 0000 1111
//1111 1110 0000 0000 0011 1111 1111 1000



#define colorburst1 0xffe003ff&0xaaaaaaaa|0x88888888
#define colorburst2 0x800ffe00&0xaaaaaaaa|0x88888888
#define colorburst3 0x1ffc007f&0xaaaaaaaa|0x88888888
#define colorburst4 0xf001ffc0&0xaaaaaaaa|0x88888888
#define colorburst5 0x03ff800f&0xaaaaaaaa|0x88888888
#define colorburst6 0xfe003ff8&0xaaaaaaaa|0x80808080






#define colorburst   

extern uint8_t charROM[1024];
extern uint8_t screenmem[1000];
extern uint8_t colormem[1000];

uint16_t offset;

uint32_t i2s_dma_buffer[LINE_BUFFER_LENGTH*LINETYPES];
static struct sdio_queue i2sBufDesc[NTSC_LINES];

int current_pixel_line;

LOCAL void slc_isr(void) {
        struct sdio_queue *finishedDesc;
        uint32 slc_intr_status;
        uint8_t x;

        //Grab int status
        slc_intr_status = READ_PERI_REG(SLC_INT_STATUS);

        //clear all intr flags
        WRITE_PERI_REG(SLC_INT_CLR, 0xffffffff);

        if (slc_intr_status & SLC_RX_EOF_INT_ST) {
                //The DMA subsystem is done with this block: Push it on the queue so it can be re-used.
                finishedDesc=(struct sdio_queue*)READ_PERI_REG(SLC_RX_EOF_DES_ADDR);

                struct sdio_queue * next = (struct sdio_queue *)finishedDesc->next_link_ptr;
                uint32_t * buffer_pointer = (uint32_t*)next->buf_ptr;
                /*
                buffer_pointer[17]=0x88888888;
                buffer_pointer[18]=0x88888888;
                buffer_pointer[19]=0x88888888;
                buffer_pointer[20]=0x88888888;
                buffer_pointer[21]=0x88888888;
                buffer_pointer[22]=0x88888888;
                */                
                if( next->unused > 1) {
                  
                  current_pixel_line = 0;
                  offset=0;
                  if ( next->unused == 2) {
                    #ifdef XRES320
                    offset=0;
                    #endif
                  }
                }

                else if( next->unused )
                {
                
                #ifdef XRES320
                        uint8_t pixel_column = 30;
                        uint8_t y = current_pixel_line>>3;
                        uint8_t *video_line = &screenmem[y*40];
                        uint8_t *char_rom = &charROM[0];
                                            
                        //buffer_pointer[17]=colorburst1;
                        //buffer_pointer[18]=colorburst2;
                        //buffer_pointer[19]=colorburst3;
                        //buffer_pointer[20]=colorburst4;
                        //buffer_pointer[21]=colorburst5;
                        //buffer_pointer[22]=colorburst6;
                        

                        /* For each byte in the line */
                        for( x = 0; x < 40; x++ )
                        {

                                /* encode each 32bit word for the video bytes */ 
                                uint8_t charbyte = video_line[x];
                                uint8_t gfxbyte = char_rom[(current_pixel_line&0x07)+(charbyte<<3)];
                                if (read6502(addr_cursor_x) == x &&
                                    read6502(addr_cursor_y) == y &&
                                    read6502(addr_cursor_flash) == 0 ) {
                                      gfxbyte^=0xff;
                                    }
                                uint32_t character = 0x88888888;
                                if (gfxbyte&128) character |= 0xfff00000;
                                if (gfxbyte&64) character |=  0x000fff00;
                                if (gfxbyte&32) character |=  0x000000ff;
                                buffer_pointer[pixel_column++] = character;
                                
                                character = 0x88888888;
                                if (gfxbyte&32) character |=0xf0000000;
                                if (gfxbyte&16) character |=0x0fff0000;
                                if (gfxbyte&8) character |= 0x0000fff0;
                                if (gfxbyte&4) character |= 0x0000000f;
                                buffer_pointer[pixel_column++] = character;

                                character = 0x88888888;
                                if (gfxbyte&4) character |= 0xff000000;
                                if (gfxbyte&2) character |= 0x00fff000;
                                if (gfxbyte&1) character |= 0x00000fff;                                                                
                                buffer_pointer[pixel_column++] = character;
                                                               
                        }

                        current_pixel_line++;
                }
                #endif
                
        }
}


/* NTSC signals */
#define SHORT_SYNC_INTERVAL 6
#define LONG_SYNC_INTERVAL  73
#define SERRATION_PULSE_INT   67
#define BACK_PORCH    20
#define NORMAL_SYNC_INTERVAL  12
#define LINE_SIGNAL_INTERVAL  147
#define PIXEL_LINE_RESET_EVEN 38-12 
#define PIXEL_LINE_RESET_ODD 299-12 


#define SHORT_SYNC  0
#define LONG_SYNC   1
#define BLACK_SIGNAL    2
#define SHORT_TO_LONG   3
#define LONG_TO_SHORT   4
#define LINE_SIGNAL   5

//Initialize I2S subsystem for DMA circular buffer use
void ICACHE_FLASH_ATTR videoinit() {
        int x, y;

        uint32_t * line = i2s_dma_buffer;

           uint8_t single_line_timings[20] = { 
                 SHORT_SYNC_INTERVAL,  LONG_SYNC_INTERVAL,   SHORT_SYNC_INTERVAL,  LONG_SYNC_INTERVAL + 1, 
                 SERRATION_PULSE_INT,  NORMAL_SYNC_INTERVAL, SERRATION_PULSE_INT,  NORMAL_SYNC_INTERVAL + 1, 
                 NORMAL_SYNC_INTERVAL, LINE_SIGNAL_INTERVAL, 
                 SHORT_SYNC_INTERVAL,  LONG_SYNC_INTERVAL,   SERRATION_PULSE_INT, NORMAL_SYNC_INTERVAL + 1, 
                 SERRATION_PULSE_INT,  NORMAL_SYNC_INTERVAL, SHORT_SYNC_INTERVAL, LONG_SYNC_INTERVAL + 1, 
                 NORMAL_SYNC_INTERVAL, LINE_SIGNAL_INTERVAL, 
 
          }; 


           uint32_t single_line_levels[20] = { 
                 SYNC_LEVEL, BLACK_LEVEL, SYNC_LEVEL,  BLACK_LEVEL, 
                 SYNC_LEVEL, BLACK_LEVEL, SYNC_LEVEL,  BLACK_LEVEL, 
                 SYNC_LEVEL, BLACK_LEVEL, 
                 SYNC_LEVEL, BLACK_LEVEL, SYNC_LEVEL,  BLACK_LEVEL, 
                 SYNC_LEVEL, BLACK_LEVEL, SYNC_LEVEL,  BLACK_LEVEL, 
                 SYNC_LEVEL, BLACK_LEVEL, 
         }; 


        uint8_t i, signal;

        for (signal = 0; signal < 20; signal++) {
                for (i=0; i < single_line_timings[signal]; i++, *line++ = single_line_levels[signal]);
                if (signal==9||signal==19) {
                        /*
                        line[-142]=colorburst1;
                        line[-141]=colorburst2;
                        line[-140]=colorburst3;
                        line[-139]=colorburst4;
                        line[-138]=colorburst5;
                        line[-137]=colorburst6;
                        */
                }
   
        }        



        /* Reference: http://martin.hinner.info/vga/pal_tv_diagram_interlace.jpg */

        uint16_t ntsc_lines[48] = { 
     /* Even Field */ 
 

     3,   SHORT_SYNC,    0, 
     6,   LONG_SYNC,     0, 
     9,   SHORT_SYNC,    0, 
 
 
     40-YOFFS+1,  BLACK_SIGNAL,  0, 
     240+YOFFS+1, LINE_SIGNAL,   1, 
     263, BLACK_SIGNAL,  0, 
      
     /* Odd Field */ 
 
 
     265, SHORT_SYNC,    0, 
     266, SHORT_TO_LONG, 0, 
     268, LONG_SYNC,     0, 
     269, LONG_TO_SHORT, 0, 
     271, SHORT_SYNC,    0, 
 
 
     302-YOFFS+1, BLACK_SIGNAL,  0, 
     502+YOFFS+1, LINE_SIGNAL,   1, 
     525, BLACK_SIGNAL,  0, 
   }; 


        uint16_t *ntsc_line = ntsc_lines;

        //Initialize DMA buffer descriptors in such a way that they will form a circular
        //buffer.
        for (x=0; x<NTSC_LINES; x++) {
                i2sBufDesc[x].owner=1;
                i2sBufDesc[x].eof=1;
                i2sBufDesc[x].sub_sof=0;
                i2sBufDesc[x].datalen=LINE_BUFFER_LENGTH*4;
                i2sBufDesc[x].blocksize=LINE_BUFFER_LENGTH*4;
                i2sBufDesc[x].next_link_ptr=(int)((x<(NTSC_LINES-1)) ? (&i2sBufDesc[x+1]) : (&i2sBufDesc[0]));

                if (*ntsc_line <= x + 1)
                        ntsc_line += 3;

                i2sBufDesc[x].buf_ptr = (uint32_t)&i2s_dma_buffer[(*(ntsc_line + 1))*LINE_BUFFER_LENGTH];
                i2sBufDesc[x].unused = *(ntsc_line + 2);

        }

        i2sBufDesc[PIXEL_LINE_RESET_EVEN].unused = 2; 
        i2sBufDesc[PIXEL_LINE_RESET_ODD].unused = 3; 



        /* I2S DMA initialization code */

        //Reset DMA
        SET_PERI_REG_MASK(SLC_CONF0, SLC_RXLINK_RST|SLC_TXLINK_RST);
        CLEAR_PERI_REG_MASK(SLC_CONF0, SLC_RXLINK_RST|SLC_TXLINK_RST);

        //Clear DMA int flags
        SET_PERI_REG_MASK(SLC_INT_CLR,  0xffffffff);
        CLEAR_PERI_REG_MASK(SLC_INT_CLR,  0xffffffff);

        //Enable and configure DMA
        CLEAR_PERI_REG_MASK(SLC_CONF0, (SLC_MODE<<SLC_MODE_S));
        SET_PERI_REG_MASK(SLC_CONF0,(1<<SLC_MODE_S));
        SET_PERI_REG_MASK(SLC_RX_DSCR_CONF,SLC_INFOR_NO_REPLACE|SLC_TOKEN_NO_REPLACE);
        CLEAR_PERI_REG_MASK(SLC_RX_DSCR_CONF, SLC_RX_FILL_EN|SLC_RX_EOF_MODE | SLC_RX_FILL_MODE);

        CLEAR_PERI_REG_MASK(SLC_TX_LINK,SLC_TXLINK_DESCADDR_MASK);
        SET_PERI_REG_MASK(SLC_TX_LINK, ((uint32)&i2sBufDesc[1]) & SLC_TXLINK_DESCADDR_MASK); //any random desc is OK, we don't use TX but it needs something valid

        CLEAR_PERI_REG_MASK(SLC_RX_LINK,SLC_RXLINK_DESCADDR_MASK);
        SET_PERI_REG_MASK(SLC_RX_LINK, ((uint32)&i2sBufDesc[0]) & SLC_RXLINK_DESCADDR_MASK);

        //Attach the DMA interrupt
        ets_isr_attach(ETS_SLC_INUM, slc_isr, 0);
        //Enable DMA operation intr
        WRITE_PERI_REG(SLC_INT_ENA,  SLC_RX_EOF_INT_ENA);
        //clear any interrupt flags that are set
        WRITE_PERI_REG(SLC_INT_CLR, 0xffffffff);
        ///enable DMA intr in cpu
        ets_isr_unmask(1<<ETS_SLC_INUM);

        //Start transmission
        SET_PERI_REG_MASK(SLC_TX_LINK, SLC_TXLINK_START);
        SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_START);

        //Init pins to i2s functions
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_I2SO_DATA);

        //Enable clock to i2s subsystem
        i2c_writeReg_Mask_def(i2c_bbpll, i2c_bbpll_en_audio_clock_out, 1);

        //Reset I2S subsystem
        CLEAR_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);
        SET_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);
        CLEAR_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);

        //Select 16bits per channel (FIFO_MOD=0), no DMA access (FIFO only)
        CLEAR_PERI_REG_MASK(I2S_FIFO_CONF, I2S_I2S_DSCR_EN|(I2S_I2S_RX_FIFO_MOD<<I2S_I2S_RX_FIFO_MOD_S)|(I2S_I2S_TX_FIFO_MOD<<I2S_I2S_TX_FIFO_MOD_S));

        //Enable DMA in i2s subsystem
        SET_PERI_REG_MASK(I2S_FIFO_CONF, I2S_I2S_DSCR_EN);

        //tx/rx binaureal
        CLEAR_PERI_REG_MASK(I2SCONF_CHAN, (I2S_TX_CHAN_MOD<<I2S_TX_CHAN_MOD_S)|(I2S_RX_CHAN_MOD<<I2S_RX_CHAN_MOD_S));

        //Clear int
        SET_PERI_REG_MASK(I2SINT_CLR,   I2S_I2S_TX_REMPTY_INT_CLR|I2S_I2S_TX_WFULL_INT_CLR|
                          I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
        CLEAR_PERI_REG_MASK(I2SINT_CLR, I2S_I2S_TX_REMPTY_INT_CLR|I2S_I2S_TX_WFULL_INT_CLR|
                            I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);

        //trans master&rece slave,MSB shift,right_first,msb right
        CLEAR_PERI_REG_MASK(I2SCONF, I2S_TRANS_SLAVE_MOD|(I2S_BITS_MOD<<I2S_BITS_MOD_S)|(I2S_BCK_DIV_NUM <<I2S_BCK_DIV_NUM_S)|
                            (I2S_CLKM_DIV_NUM<<I2S_CLKM_DIV_NUM_S));

        SET_PERI_REG_MASK(I2SCONF, I2S_RIGHT_FIRST|I2S_MSB_RIGHT|I2S_RECE_SLAVE_MOD|I2S_RECE_MSB_SHIFT|I2S_TRANS_MSB_SHIFT|
                          ((WS_I2S_BCK&I2S_BCK_DIV_NUM )<<I2S_BCK_DIV_NUM_S)|((WS_I2S_DIV&I2S_CLKM_DIV_NUM)<<I2S_CLKM_DIV_NUM_S));

        //No idea if ints are needed...
        SET_PERI_REG_MASK(I2SINT_CLR,   I2S_I2S_TX_REMPTY_INT_CLR|I2S_I2S_TX_WFULL_INT_CLR|
                          I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
        CLEAR_PERI_REG_MASK(I2SINT_CLR,   I2S_I2S_TX_REMPTY_INT_CLR|I2S_I2S_TX_WFULL_INT_CLR|
                            I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
        //enable int
        SET_PERI_REG_MASK(I2SINT_ENA,   I2S_I2S_TX_REMPTY_INT_ENA|I2S_I2S_TX_WFULL_INT_ENA|
                          I2S_I2S_RX_REMPTY_INT_ENA|I2S_I2S_TX_PUT_DATA_INT_ENA|I2S_I2S_RX_TAKE_DATA_INT_ENA);

        //Start transmission
        SET_PERI_REG_MASK(I2SCONF,I2S_I2S_TX_START);
}

void ICACHE_FLASH_ATTR videostop() {
        //Reset I2S subsystem
        CLEAR_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);
        SET_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);
        CLEAR_PERI_REG_MASK(I2SCONF,I2S_I2S_RESET_MASK);

        ///disable DMA intr in cpu
        ets_isr_mask(1<<ETS_SLC_INUM);

        //Clear int
        SET_PERI_REG_MASK(I2SINT_CLR,   I2S_I2S_TX_REMPTY_INT_CLR|I2S_I2S_TX_WFULL_INT_CLR|
                          I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);
        CLEAR_PERI_REG_MASK(I2SINT_CLR, I2S_I2S_TX_REMPTY_INT_CLR|I2S_I2S_TX_WFULL_INT_CLR|
                            I2S_I2S_RX_WFULL_INT_CLR|I2S_I2S_PUT_DATA_INT_CLR|I2S_I2S_TAKE_DATA_INT_CLR);

        //Reset DMA
        SET_PERI_REG_MASK(SLC_CONF0, SLC_RXLINK_RST|SLC_TXLINK_RST);
        CLEAR_PERI_REG_MASK(SLC_CONF0, SLC_RXLINK_RST|SLC_TXLINK_RST);

        //Clear DMA int flags
        SET_PERI_REG_MASK(SLC_INT_CLR,  0xffffffff);
        CLEAR_PERI_REG_MASK(SLC_INT_CLR,  0xffffffff);
}


#define BASEFREQ (160000000L)
