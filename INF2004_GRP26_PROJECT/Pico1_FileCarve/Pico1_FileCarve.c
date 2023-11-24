#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hw_config.h"
#include "sd_card.h"
#include "ff.h"
#include "diskio.h"


#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

BYTE bytearr[2048];

int read_disk_sectors(
    BYTE pdrv,   /* Physical drive number to be checked (all data on the drive will be lost) */
    DWORD *buff /* Pointer to the working buffer */
)
{
    UINT m;
    UINT n;
    DWORD lba = 1;
    BYTE *pbuff = (BYTE *)buff;
    DSTATUS ds;
    DRESULT dr;

    // FRESULT fr;
    // FATFS fs;
    // FIL fil;
    // int ret;
    // char filename[] = "test1001.jpg";
    // char buf[100];

    printf(" disk_initalize(%u)", pdrv);
    ds = disk_initialize(pdrv);
    if (ds & STA_NOINIT)
    {
        printf(" - failed.\n");
        return 2;
    }
    else
    {
        printf(" - ok.\n");
    }

    // // Mount drive
    // fr = f_mount(&fs, "0:", 0);
    // if (fr != FR_OK)
    // {
    //     printf("ERROR: Could not mount filesystem (%d)\r\n", fr);
    //     while (true);
    // }

    // // Open file for writing ()
    // fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    // if (fr != FR_OK)
    // {
    //     printf("ERROR: Could not open file for writing (%d)\r\n", fr);
    //     while (true);
    // } 

    /* Single sector read test */
    printf("**** Read from disk sector ****\n");
    lba = 16640;
    printf(" disk_read(%u, 0x%X, %lu, 4)", pdrv, (UINT)pbuff, lba);
    printf("\n");
    for(m = 0 ; m < (61/4) + 2 ; m ++){
        dr = disk_read(pdrv, pbuff, lba, 4);
        for (n = 0; n < 2048; n++)
        {
            printf("%02x ", pbuff[n]);
            uart_putc(UART_ID, pbuff[n]);
            if(pbuff[n-1] == 0xff && pbuff[n] == 0xd9){
                break;
            }
        }

        if(pbuff[n-1] == 0xff && pbuff[n] == 0xd9){
            break;
        }

        // UINT test;
        // ret = f_write(&fil, pbuff, 2048, &test);
        // if (ret < 0)
        // {
        //     printf("ERROR: Could not write to file (%d)\r\n", ret);
        //     f_close(&fil);
        //     while (true);
        // }

        lba = lba + 4;
    }

    // // Close file
    // fr = f_close(&fil);
    // if (fr != FR_OK)
    // {
    //     printf("ERROR: Could not close file (%d)\r\n", fr);
    //     while (true);
    // }

    // // Unmount drive
    // f_unmount("0:");

    return 0;
}

int main()
{
    char buf[100];

    // Initialize chosen serial port
    stdio_usb_init();
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Wait for user to press 'enter' to continue
    printf("\r\nSD card test. Enter 'A' or 'B' to console to start.\r\n");
    while (true)
    {
        buf[0] = getchar();
        if ((buf[0] == 'A') || (buf[0] == 'B'))
        {
            break;
        }
    }

    if(FF_FS_READONLY == 0 && FF_FS_LOCK > 0){
        printf("WRITE BLOCK HAS BEEN TAMPERED WITH\n");
        printf("EXITING....\n");
        return 0;
    }

    if(FF_FS_READONLY == 1 && FF_FS_LOCK == 0){
        printf("FF_FS_READONLY = %d\n", FF_FS_READONLY);
        printf("FF_FS_LOCK = %d\n", FF_FS_LOCK);
        printf("============= WRITE BLOCK HAS BEEN ENABLED ON THIS DEVICE =============\n");
    }

    // Initialize SD card
    if (!sd_init_driver())
    {
        printf("ERROR: Could not initialize SD card\r\n");
        while (true);
    }

    int rc;
    DWORD buff[FF_MAX_SS]; /* Working buffer (4 sector in size) */

    /* Read disk sectors of the physical drive #0 */
    rc = read_disk_sectors(0, buff);

    // Loop forever doing nothing
    while (true)
    {   
        sleep_ms(1000);
    }
}