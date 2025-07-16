#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
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
        .frequency    = 868300000,
        .bandwidth    = BW_125_KHZ,
        .datarate     = SF_7,
        .preamble_len = 8,
        .coding_rate  = CR_4_5,
        .tx_power     = 14,
        .tx           = true
    };

    int ret = lora_config(lora_dev, &config);
    if (ret < 0) {
        LOG_ERR("LoRa config failed: %d", ret);
        return 0;
    }

    LOG_INF("LoRa config done. Performing dummy receive...");
    uint8_t dummy_buf[1];
    lora_recv(lora_dev, dummy_buf, sizeof(dummy_buf), K_NO_WAIT, NULL, NULL);

    k_sleep(K_MSEC(1500));

    char msg[] = "Hello from nRF54L15 + SX1261!";
    while (1) {
        ret = lora_send(lora_dev, (uint8_t *)msg, strlen(msg));
        if (ret == -EBUSY) {
            LOG_WRN("SX1261 busy. BUSY pin = %d. Retrying in 1s...", gpio_pin_get(busy_gpio, busy_pin));
        } else if (ret < 0) {
            LOG_ERR("lora_send() failed: %d. BUSY pin = %d", ret, gpio_pin_get(busy_gpio, busy_pin));
        } else {
            LOG_INF("Message sent: %d bytes. BUSY pin = %d", ret, gpio_pin_get(busy_gpio, busy_pin));
            gpio_pin_toggle_dt(&led);
            // LOG_INF("Message sent!");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
