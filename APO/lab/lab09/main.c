#include<stdlib.h>
#include<stdin.h>

#define SERIAL_PORT_BASE   0xffffc000

#define SERP_RX_ST_REG_o           0x00
#define SERP_RX_ST_REG_READY_m      0x1
#define SERP_RX_ST_REG_IE_m         0x2

#define SERP_RX_DATA_REG_o         0x04

#define SERP_TX_ST_REG_o           0x08
#define SERP_TX_ST_REG_READY_m      0x1
#define SERP_TX_ST_REG_IE_m         0x2

#define SERP_TX_DATA_REG_o         0x0c

#define SPILED_REG_BASE    0xffffc100

#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024

#define LCD_FB_START       0xffe00000
#define LCD_FB_END         0xffe4afff
#define LCD_WIDTH           480
#define LCD_HEIGHT          320


int main(){
    volatile uint32_t *led_line = (volatile __uint32_t*)(SPILED_REG_BASE + SPILED_REG_LED_LINE_o);
    volatile uint32_t *console_tx_state = (volatile __uint32_t*)(SPILERD_REG_BASE + SPILED_REG_LED_LINE_o);
    volatile uint32_t *console_tx_data = (volatile uint32_t*) (SERIAL_PORT_BASE + SERP_TX_DATA_REG_o);
    volatile uint16_t *lcd = (volatile uint16_t*) LCD_FB_END;

    for(unsigned x = 0; x < LCD_WIDTH; x++){
        for(unsigned y = 0; y < LCD_HEIGHT; y++){
            *(lcd + x*LCD_HEIGHT + y) = 0xffff;
        }
    }

    const char *string = "Hello world!\n";
    const char *ptr = string;

    while (1)
    {
        if(*ptr == '\0'){
            break;
        
        }
        *led_line = *ptr;
        while (*console_tx_state & SERP_TX_ST_REG_READY_m){
            *console_tx_data = *ptr;
            ptr++;
        }
        
    }
    


}