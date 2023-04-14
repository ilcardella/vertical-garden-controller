
#include <hardware/clocks.h>
#include <hardware/rosc.h>
#include <hardware/rtc.h>
#include <hardware/structs/scb.h>
#include <pico/sleep.h>
#include <pico/stdlib.h>

// Time in minutes that we want to keep the relay on
const uint RELAY_ON_INTERVAL = 15;
// Time in minutes that we want to keep the relay off
const uint RELAY_OFF_INTERVAL = 45;

// LED GPIO number
const uint LED_PIN = PICO_DEFAULT_LED_PIN;
// Relay GPIO number
const uint RELAY_PIN = 6;

// Define a starting date. It does not matter what day/time it is
static datetime_t reference_datetime = {
    .year = 2023, .month = 01, .day = 01, .dotw = 1, .hour = 00, .min = 00, .sec = 00};

// Control the relay status after the sleep. Start with the relay on
static bool turn_relay_on = true;

// Reset the ROSC clock
void rosc_enable(void)
{
    uint32_t tmp = rosc_hw->ctrl;
    tmp &= (~ROSC_CTRL_ENABLE_BITS);
    tmp |= (ROSC_CTRL_ENABLE_VALUE_ENABLE << ROSC_CTRL_ENABLE_LSB);
    rosc_write(&rosc_hw->ctrl, tmp);
    // Wait for stable
    while ((rosc_hw->status & ROSC_STATUS_STABLE_BITS) != ROSC_STATUS_STABLE_BITS)
        ;
}

void enable_relay()
{
    gpio_put(LED_PIN, 1);
    gpio_put(RELAY_PIN, 1);
}

void disable_relay()
{
    gpio_put(LED_PIN, 0);
    gpio_put(RELAY_PIN, 0);
}

static void sleep_callback()
{
    // After the sleep period, switch the relay status
    turn_relay_on = !turn_relay_on;
}

static void rtc_sleep(const int8_t &minute_to_sleep_to)
{
    // This datetime matches the reference time, plus the specified minutes
    datetime_t t_alarm = {.year = 2023,
                          .month = 01,
                          .day = 01,
                          .dotw = 1,
                          .hour = 00,
                          .min = minute_to_sleep_to,
                          .sec = 00};

    sleep_goto_sleep_until(&t_alarm, &sleep_callback);
}

bool setup()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);

    rtc_init();

    return true;
}

void loop()
{
    if (turn_relay_on)
    {
        enable_relay();
    }
    else
    {
        disable_relay();
    }

    sleep_run_from_xosc();

    rtc_set_datetime(&reference_datetime);

    if (turn_relay_on)
    {
        rtc_sleep(RELAY_ON_INTERVAL);
    }
    else
    {
        rtc_sleep(RELAY_OFF_INTERVAL);
    }

    rosc_enable();
    clocks_init();
}

int main()
{
    if (not setup())
    {
        return 1;
    }

    while (true)
    {
        loop();
    }
    return 0;
}
