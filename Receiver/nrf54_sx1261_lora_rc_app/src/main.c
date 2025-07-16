#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

void sx1261_hardware_reset(const struct device *reset_gpio,
                           gpio_pin_t reset_pin,
                           const struct device *busy_gpio,
                           gpio_pin_t busy_pin)
{
    gpio_pin_set(reset_gpio, reset_pin, 0);
    k_msleep(10);
    gpio_pin_set(reset_gpio, reset_pin, 1);

    int wait_count = 0;
    while (gpio_pin_get(busy_gpio, busy_pin) == 1 && wait_count++ < 1000) {
        k_msleep(1);
    }

    LOG_INF("Reset complete. BUSY wait = %d ms", wait_count);
}

int main(void)
{
    const struct device *lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));

    const struct device *reset_gpio = DEVICE_DT_GET(DT_GPIO_CTLR(DT_ALIAS(lora0), reset_gpios));
    gpio_pin_t reset_pin = DT_GPIO_PIN(DT_ALIAS(lora0), reset_gpios);
    const struct device *busy_gpio = DEVICE_DT_GET(DT_GPIO_CTLR(DT_ALIAS(lora0), busy_gpios));
    gpio_pin_t busy_pin = DT_GPIO_PIN(DT_ALIAS(lora0), busy_gpios);

    if (!device_is_ready(lora_dev)) {
        LOG_ERR("LoRa device not ready");
        return 0;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(reset_gpio, reset_pin,
        GPIO_OUTPUT_ACTIVE | DT_GPIO_FLAGS(DT_NODELABEL(lora0), reset_gpios));
    gpio_pin_configure(busy_gpio, busy_pin,
        GPIO_INPUT | DT_GPIO_FLAGS(DT_NODELABEL(lora0), busy_gpios));

    sx1261_hardware_reset(reset_gpio, reset_pin, busy_gpio, busy_pin);

    struct lora_modem_config config = {
        .frequency    = 868000000,
        .bandwidth    = BW_125_KHZ,
        .datarate     = SF_7,
        .preamble_len = 8,
        .coding_rate  = CR_4_5,
        .tx_power     = 0,     // Not used in RX
        .tx           = false  // RX mode
    };

    int ret = lora_config(lora_dev, &config);
    if (ret < 0) {
        LOG_ERR("LoRa config failed: %d", ret);
        return 0;
    }

    LOG_INF("LoRa RX config done. Waiting for packets...");

    uint8_t rx_buf[128];
    int16_t rssi;
    int8_t snr;

    while (1) {
        int len = lora_recv(lora_dev, rx_buf, sizeof(rx_buf) - 1, K_MSEC(5000), &rssi, &snr);

        if (len > 0) {
            rx_buf[len] = '\0';
            LOG_INF("Packet received: '%s'", rx_buf);
            LOG_INF("RSSI: %d dBm | SNR: %d dB", rssi, snr);
            gpio_pin_toggle_dt(&led);  // Blink LED when message is received
        } else {
            LOG_INF("No packet received.");
        }

        k_sleep(K_SECONDS(2));
    }

    return 0;
}
