add_executable(atari_pico)

# by default the header is generated into the build dir
pico_generate_pio_header(atari_pico ${CMAKE_CURRENT_LIST_DIR}/atari_pico.pio)
# however, alternatively you can choose to generate it somewhere else (in this case in the source tree for check in)
#pico_generate_pio_header(pio_blink ${CMAKE_CURRENT_LIST_DIR}/blink.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR})

target_sources(atari_pico PRIVATE atari_pico.c)

target_link_libraries(atari_pico PRIVATE pico_stdlib hardware_pio hardware_dma)
pico_add_extra_outputs(atari_pico)

# add url via pico_set_program_url
example_auto_set_url(atari_pico)

pico_enable_stdio_usb(atari_pico 1)
pico_enable_stdio_uart(atari_pico 0)