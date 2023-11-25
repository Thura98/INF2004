add_executable(Pico2_FileStorage
        Pico2_FileStorage.c
        )

# Tell CMake where to find other source code
add_subdirectory(../Project_Libraries/no-OS-FatFS-SD-SPI-RPi-Pico-master/FatFs_SPI2 build)

target_link_libraries(Pico2_FileStorage
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
        FatFs_SPI2
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(Pico2_FileStorage 1)
pico_enable_stdio_uart(Pico2_FileStorage 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(Pico2_FileStorage)

# add url via pico_set_program_url
example_auto_set_url(Pico2_FileStorage)
