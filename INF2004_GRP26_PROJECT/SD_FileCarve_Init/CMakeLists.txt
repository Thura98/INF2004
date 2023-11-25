add_executable(SD_FileCarve_Init
    SD_FileCarve_Init.c
        )

# Tell CMake where to find other source code
add_subdirectory(../Project_Libraries/no-OS-FatFS-SD-SPI-RPi-Pico-master/FatFs_SPI3 build)

target_link_libraries(SD_FileCarve_Init
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        FatFs_SPI3
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(SD_FileCarve_Init 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(SD_FileCarve_Init)

# add url via pico_set_program_url
example_auto_set_url(SD_FileCarve_Init)
