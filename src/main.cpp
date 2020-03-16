#include <nrf_delay.h>
#include <nrf_gpio.h>
#include <libraries/button/app_button.h>
#include <nrfx_spim.h>

#include "Fonts/Ubuntu24Font.h"
#include "Hardware/Screen/ST7789.h"

#include "Widgets/Container.h"
#include "Widgets/Text.h"

void buttonHandler(uint8_t pinNo, uint8_t buttonAction)
{

}

int main()
{
    // Set backlight pins as output
    nrf_gpio_cfg_output(14);
    nrf_gpio_cfg_output(22);
    nrf_gpio_cfg_output(23);

    // Set battery charge indication pin to input
    nrf_gpio_cfg_input(12, NRF_GPIO_PIN_NOPULL);

    // Set the vibrator pin to output
    nrf_gpio_cfg_output(16);
    nrf_gpio_pin_set(16);

    // Initialize the button
    nrf_gpio_cfg_output(15);
    nrf_gpio_pin_set(15);

    app_button_cfg_t pinetimeButton = {
        13,
        APP_BUTTON_ACTIVE_HIGH,
        NRF_GPIO_PIN_PULLDOWN,
        &buttonHandler
    };

    app_button_init(&pinetimeButton, 1, 10);
    app_button_enable();

    // Reset backlight pins
    nrf_gpio_pin_clear(14);
    nrf_gpio_pin_set(22);
    nrf_gpio_pin_set(23);

    // Instantiate a new Screen
    Hardware::Screen::ST7789 lcd({240, 240}, 3, 4, 2, 25, 18, 26);
    lcd.clearFramebuffer({0, 0, 0});

    // Test the Container widget
    Widget::Container ctr(nullptr, {16, 16}, {208, 64}, {0, 63, 0});
    ctr.draw(lcd);

    Widget::Text txt(&ctr, "Hi!", &ubuntu_24ptFontInfo, {0, 0}, {31, 0, 0}, {0, 0, 31});
    txt.draw(lcd);

    nrf_delay_ms(500);
    ctr.setPosition({16, 128});
    ctr.draw(lcd);

    nrf_delay_ms(500);
    txt.setText("Hello!");
    txt.setPosition({16, 16});
    txt.draw(lcd);

    nrf_delay_ms(500);
    ctr.setPosition({32, 96});
    ctr.setSize({192, 48});
    ctr.draw(lcd);

    while (true)
    {
        // Dim the screen if the battery is charging
        uint32_t isCharging = nrf_gpio_pin_read(12);
        if (!isCharging) {
            nrf_gpio_pin_clear(22);
            nrf_gpio_pin_clear(23);
        } else {
            nrf_gpio_pin_set(22);
            nrf_gpio_pin_set(23);
        }

        // Exit the loop if the button is pressed
        if (app_button_is_pushed(0)) {
            break;
        }

        // Wait 500ms
        nrf_delay_ms(500);
    }

    // Shutdown the screen backlight
    nrf_gpio_pin_set(14);
    nrf_gpio_pin_set(22);
    nrf_gpio_pin_set(23);

    // Completely shutdown the device
    sd_power_system_off();
}
