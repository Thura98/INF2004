add_executable(Pico1_FileCarve
    Pico1_FileCarve.c
        )

# Tell CMake where to find other source code
add_subdirectory(../Project_Libraries/no-OS-FatFS-SD-SPI-RPi-Pico-master/FatFs_SPI build)

# pull in common dependencies
target_link_libraries(Pico1_FileCarve pico_stdlib 
                    pico_cyw43_arch_none 
                    FatFs_SPI)

# enable usb output, disable uart output
pico_enable_stdio_usb(Pico1_FileCarve 1)
pico_enable_stdio_uart(Pico1_FileCarve 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(Pico1_FileCarve)

# add url via pico_set_program_url
example_auto_set_url(Pico1_FileCarve)
