#include <stdio.h>
#include "pico/stdlib.h"
#include "hw_config.h"
#include "sd_card.h"
#include "ff.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0   // PIN 0  for UART Transmission
#define UART_RX_PIN 1   // PIN 1  for UART Receive

int main() {
    FRESULT fr;
    FATFS fs;
    FIL fil;
    int ret;
    char buf[100];
    char filename[] = "test02.jpg";

    // Initialize chosen serial port
    stdio_usb_init();
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Wait for user to press 'C' or 'D' to continue
    printf("\r\nSD card test. Enter 'C' or 'D' to console to start.\r\n");
    while (true)
    {
        buf[0] = getchar();
        if ((buf[0] == 'C') || (buf[0] == 'D'))
        {
            break;
        }
    }

     // Initialize SD card
    if (!sd_init_driver()) {
        printf("ERROR: Could not initialize SD card\r\n");
        while (true);
    }

    // flush uart first character
    if(uart_is_readable(UART_ID)){
        printf("UART FLUSH - %02x\n", uart_getc(UART_ID));
    }

    // Mount drive
    fr = f_mount(&fs, "0:", 0);
    if (fr != FR_OK)
    {
        printf("ERROR: Could not mount filesystem (%d)\r\n", fr);
        while (true);
    }

    // Open file for writing ()
    fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK)
    {
        printf("ERROR: Could not open file for writing (%d)\r\n", fr);
        while (true);
    } 

    DWORD buff[FF_MAX_SS]; /* Working buffer (4 sector in size) */
    BYTE *pbuff = (BYTE *)buff; // buffer to store the carved file bytes
    UINT bw;
    UINT n = 0;
    while (true) 
    {   
        if(uart_is_readable(UART_ID))
        {
            // Carved bytes received from uart will be stored in pbuff buffer byte by byte
            pbuff[n] = uart_getc(UART_ID); 
            if(n == 2047)
            {
                // Carved bytes will be written to the SD card in batches of 2048 bytes
                ret = f_write(&fil, pbuff, (n + 1), &bw);
            }

            // Stops when the carved byes ff d9 are detected, ff d9 indicates the end of a jpg file
            if(pbuff[n - 1] == 0xff && pbuff[n] == 0xd9){
                // Performs a write operation of the remaining less than 
                // 2048 carved bytes up to the ff d9 bytes to the SD card
                ret = f_write(&fil, pbuff, (n + 1), &bw);
                printf("\n%02x", pbuff[n-1]);
                printf("\n%02x", pbuff[n]);
                break;
            }
            n++;
            if(n == 2048){
                n = 0;
            }         
        }
    }

    // Close file
    fr = f_close(&fil);
    if (fr != FR_OK)
    {
        printf("ERROR: Could not close file (%d)\r\n", fr);
        while (true);
    }

    // Unmount drive
    f_unmount("0:");

    printf("\nEND OF OPERATION\n");
    // Loop forever doing nothing
    while (true)
    {   
        sleep_ms(1000);
    }
}
