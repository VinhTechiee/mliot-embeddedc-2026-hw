// HW04 - GPIO on STM32F103C8T6 (Blue Pill)

#include "stm32f1xx_hal.h"

#define DEMO_BLINK   1U
#define DEMO_BUTTON  2U
#define DEMO_KEYPAD  3U

// Change this value to run each task.
#define DEMO_MODE    DEMO_KEYPAD

#define LED_PORT     GPIOC
#define LED_PIN      GPIO_PIN_13

#define BUTTON_PORT  GPIOA
#define BUTTON_PIN   GPIO_PIN_0

#define ROW_PORT     GPIOB
#define ROW_1_PIN    GPIO_PIN_0
#define ROW_2_PIN    GPIO_PIN_1

#define COL_PORT     GPIOB
#define COL_1_PIN    GPIO_PIN_10
#define COL_2_PIN    GPIO_PIN_11

#define KEYPAD_LED_PORT  GPIOA
#define KEYPAD_LED_PIN   GPIO_PIN_8

#define KEY_NONE     0U

static void delay_ms(uint32_t time_ms);
static void gpio_init(void);
static void led_set(uint8_t is_on);
static uint8_t button_pressed_event(void);
static uint8_t keypad_read(void);

int main(void)
{
    uint8_t led_state = 0U;
    uint8_t key;

    HAL_Init();
    gpio_init();

    while (1)
    {
#if DEMO_MODE == DEMO_BLINK
        led_set(1U);
        delay_ms(500U);
        led_set(0U);
        delay_ms(500U);

#elif DEMO_MODE == DEMO_BUTTON
        if (button_pressed_event() != 0U)
        {
            led_state = !led_state;
            led_set(led_state);
        }

#elif DEMO_MODE == DEMO_KEYPAD
        key = keypad_read();

        // Firmware feedback: PA8 is on while key 1 is held.
        HAL_GPIO_WritePin(KEYPAD_LED_PORT, KEYPAD_LED_PIN,
                          (key == 1U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        delay_ms(10U);

#else
#error "Invalid DEMO_MODE"
#endif
    }
}

static void gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    // Clock must be enabled before configuring the GPIO ports.
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // PC13 onboard LED is active-low.
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    gpio.Pin = LED_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &gpio);

    gpio.Pin = BUTTON_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_PORT, &gpio);

    HAL_GPIO_WritePin(ROW_PORT, ROW_1_PIN | ROW_2_PIN, GPIO_PIN_SET);
    gpio.Pin = ROW_1_PIN | ROW_2_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ROW_PORT, &gpio);

    gpio.Pin = COL_1_PIN | COL_2_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COL_PORT, &gpio);

    HAL_GPIO_WritePin(KEYPAD_LED_PORT, KEYPAD_LED_PIN, GPIO_PIN_RESET);
    gpio.Pin = KEYPAD_LED_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(KEYPAD_LED_PORT, &gpio);
}

static void led_set(uint8_t is_on)
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN,
                      (is_on != 0U) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static uint8_t button_pressed_event(void)
{
    static uint8_t was_pressed = 0U;
    uint8_t is_pressed;

    is_pressed = (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET);

    if ((is_pressed != 0U) && (was_pressed == 0U))
    {
        delay_ms(20U);
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET)
        {
            was_pressed = 1U;
        }
    }
    else if ((is_pressed == 0U) && (was_pressed != 0U))
    {
        delay_ms(20U);
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_SET)
        {
            was_pressed = 0U;
            return 1U;
        }
    }

    return 0U;
}

static uint8_t keypad_read(void)
{
    uint8_t key = KEY_NONE;

    // Scan row 1. The inactive row stays high.
    HAL_GPIO_WritePin(ROW_PORT, ROW_1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ROW_PORT, ROW_2_PIN, GPIO_PIN_SET);
    delay_ms(1U);

    if (HAL_GPIO_ReadPin(COL_PORT, COL_1_PIN) == GPIO_PIN_RESET)
    {
        key = 1U;
    }
    else if (HAL_GPIO_ReadPin(COL_PORT, COL_2_PIN) == GPIO_PIN_RESET)
    {
        key = 2U;
    }

    // Scan row 2 only if no key was found in row 1.
    if (key == KEY_NONE)
    {
        HAL_GPIO_WritePin(ROW_PORT, ROW_1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW_2_PIN, GPIO_PIN_RESET);
        delay_ms(1U);

        if (HAL_GPIO_ReadPin(COL_PORT, COL_1_PIN) == GPIO_PIN_RESET)
        {
            key = 3U;
        }
        else if (HAL_GPIO_ReadPin(COL_PORT, COL_2_PIN) == GPIO_PIN_RESET)
        {
            key = 4U;
        }
    }

    HAL_GPIO_WritePin(ROW_PORT, ROW_1_PIN | ROW_2_PIN, GPIO_PIN_SET);
    return key;
}

static void delay_ms(uint32_t time_ms)
{
    HAL_Delay(time_ms);
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}