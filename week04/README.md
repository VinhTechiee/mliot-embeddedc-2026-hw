# HW04 - STM32F103 GPIO

## Pin connection

| Function | Blue Pill pin | Connection |
|---|---|---|
| Onboard LED | PC13 | Already on the board, active-low |
| Push button | PA0 | Button between PA0 and GND |
| Keypad row 1 | PB0 | Row 1 |
| Keypad row 2 | PB1 | Row 2 |
| Keypad column 1 | PB10 | Column 1 |
| Keypad column 2 | PB11 | Column 2 |
| Keypad feedback LED | PA8 | PA8 -> 220-330 ohm resistor -> LED -> GND |

The keypad map is:

```text
        Col 1   Col 2
Row 1     1       2
Row 2     3       4
```

`main.c` contains all three practical tasks. Change `DEMO_MODE` near the top of the
file to `DEMO_BLINK`, `DEMO_BUTTON`, or `DEMO_KEYPAD` before building. The default is
`DEMO_KEYPAD`. In the keypad demo, pressing key 1 turns on the LED connected to PA8.

## Build

Download STM32CubeF1, then give CMake its extracted folder:

```bat
cmake -S . -B build -G Ninja -DSTM32CUBE_F1_PATH="D:/path/to/STM32CubeF1"
ninja -C build
```

The output file for flashing is `build/hw04_gpio.bin` at address `0x08000000`.

The simple delay function assumes the reset-default 8 MHz HSI clock. It avoids adding
timer or SysTick setup to this GPIO exercise.
