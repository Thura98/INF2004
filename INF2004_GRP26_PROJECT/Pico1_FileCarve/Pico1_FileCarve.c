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

#define UART_TX_PIN 0  // PIN 0  for UART Transmission
#define UART_RX_PIN 1  // PIN 1  for UART Receive

int read_disk_sectors(
    BYTE pdrv,   /* Physical drive number to be checked (all data on the drive will be lost) */
    DWORD *buff /* Pointer to the working buffer */
)
{
    UINT m;
    UINT n;
    DWORD lba = 1; // variable to store the sector number
    BYTE *pbuff = (BYTE *)buff;
    DSTATUS ds;
    DRESULT dr;

    // Initialize SD Card disk
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

    // Disk read operation is performed in batches of 2048 bytes, which is 4 sectors by 4 sectors
    // 1 sector contains 512 bytes
    // The pico will crash if more than 2048 bytes of data is processed
    printf("**** Read from disk sector ****\n");
    lba = 16639; // Start reading from disk sector 16639
    printf(" disk_read(%u, 0x%X, %lu, 4)", pdrv, (UINT)pbuff, lba);
    printf("\n");
    // To read 61 sectors, the disk read operation will be performed (61/4) times
    // Added 2 due to receiving wrong results with just a (61/4)
    for(m = 0 ; m < (61/4) + 2 ; m ++){
        // Performs a disk read operation
        dr = disk_read(pdrv, pbuff, lba, 4);
        for (n = 0; n < 2048; n++)
        {
            // Starts sending bytes to the second pico when the bytes ff d8 ff e0 are detected
            // ff d8 ff e0 is the file signature for jpg image files
            // File carving begins here
            if(pbuff[n] == 0xe0 && pbuff[n-1] == 0xff && pbuff[n-2] == 0xd8 && pbuff[n-3] == 0xff){
                uart_putc(UART_ID, pbuff[n - 3]);
                uart_putc(UART_ID, pbuff[n - 2]);
                uart_putc(UART_ID, pbuff[n - 1]);

                printf("%02x ", pbuff[n-3]);
                printf("%02x ", pbuff[n-2]);
                printf("%02x ", pbuff[n-1]);

                // Continuously sends bytes over to the second pico untill the bytes ff d9 are detected 
                while(true)
                {  
                    uart_putc(UART_ID, pbuff[n]);
                    printf("%02x ", pbuff[n]);
                    // Stops when the carved byes ff d9 are detected
                    if(pbuff[n-1] == 0xff && pbuff[n] == 0xd9){
                        break;
                    }
                    n++;
                    if(n == 2048){
                        lba = lba + 4;
                        disk_read(pdrv, pbuff, lba, 4);
                        n = 0;
                    }
                }
            }
             // Stops when the carved byes ff d9 are detected
            if(pbuff[n-1] == 0xff && pbuff[n] == 0xd9){
                break;
            }
        }

        // Stops when the carved byes ff d9 are detected
        if(pbuff[n-1] == 0xff && pbuff[n] == 0xd9){
            break;
        }

        // Increase the sector to read from by 4
        lba = lba + 4;
    }

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

    // Wait for user to press 'A' or 'B' to continue
    printf("\r\nSD card test. Enter 'A' or 'B' to console to start.\r\n");
    while (true)
    {
        buf[0] = getchar();
        if ((buf[0] == 'A') || (buf[0] == 'B'))
        {
            break;
        }
    }

    // Program exits if write block have been tampered with
    if(FF_FS_READONLY == 0 && FF_FS_LOCK > 0){
        printf("WRITE BLOCK HAS BEEN TAMPERED WITH\n");
        printf("EXITING....\n");
        return 0;
    }

    // Verifies that write block is enabled by setting the
    // Write block done by setting the FF_FS_READONLY variable to 1 and FF_FS_LOCK variable to 0
    // In the ffconf.h file
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

    /* Read disk sectors of the physical drive #0 for file varving */
    rc = read_disk_sectors(0, buff);

    // Loop forever doing nothing
    while (true)
    {   
        sleep_ms(1000);
    }
}
