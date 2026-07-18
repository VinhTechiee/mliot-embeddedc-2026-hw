cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(hw04_gpio C ASM)

if(NOT DEFINED STM32CUBE_F1_PATH)
    message(FATAL_ERROR
        "Configure with -DSTM32CUBE_F1_PATH=<path to STM32CubeF1>")
endif()

set(HAL_DIR ${STM32CUBE_F1_PATH}/Drivers/STM32F1xx_HAL_Driver)
set(CMSIS_DIR ${STM32CUBE_F1_PATH}/Drivers/CMSIS)

add_executable(${PROJECT_NAME}.elf
    main.c
    ${HAL_DIR}/Src/stm32f1xx_hal.c
    ${HAL_DIR}/Src/stm32f1xx_hal_gpio.c
    ${HAL_DIR}/Src/stm32f1xx_hal_cortex.c
    ${CMSIS_DIR}/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
    ${CMSIS_DIR}/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s
)

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    .
    ${HAL_DIR}/Inc
    ${CMSIS_DIR}/Include
    ${CMSIS_DIR}/Device/ST/STM32F1xx/Include
)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F103xB
    USE_HAL_DRIVER
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8TX_FLASH.ld
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    --specs=nano.specs
    --specs=nosys.specs
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary
            $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMENT "Creating ${PROJECT_NAME}.bin"
)
