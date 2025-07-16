#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define ADC_NODE        DT_NODELABEL(adc)
#define ADC_CHANNEL_ID  5  // AIN5 = P1.12
#define ADC_RESOLUTION  12
#define ADC_GAIN        ADC_GAIN_1
#define ADC_REFERENCE   ADC_REF_INTERNAL
#define ADC_ACQ_TIME    ADC_ACQ_TIME_DEFAULT
#define ADC_INPUT_POS   SAADC_CH_PSELP_PSELP_AnalogInput5

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static int16_t sample_buffer;

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

float read_sensor_voltage(const struct device *adc_dev)
{
    struct adc_channel_cfg channel_cfg = {
        .gain             = ADC_GAIN,
        .reference        = ADC_REFERENCE,
        .acquisition_time = ADC_ACQ_TIME,
        .channel_id       = ADC_CHANNEL_ID,
        .input_positive   = ADC_INPUT_POS,
    };

    adc_channel_setup(adc_dev, &channel_cfg);

    struct adc_sequence sequence = {
        .channels    = BIT(ADC_CHANNEL_ID),
        .buffer      = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution  = ADC_RESOLUTION,
    };

    int ret = adc_read(adc_dev, &sequence);
    if (ret < 0) {
        LOG_ERR("ADC read failed: %d", ret);
        return -1.0f;
    }

    float voltage = (sample_buffer / 4095.0f) * 3.6f;
    return voltage;
}

int main(void)
{
    const struct device *lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
    const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);

    const struct device *reset_gpio = DEVICE_DT_GET(DT_GPIO_CTLR(DT_ALIAS(lora0), reset_gpios));
    gpio_pin_t reset_pin = DT_GPIO_PIN(DT_ALIAS(lora0), reset_gpios);
    const struct device *busy_gpio = DEVICE_DT_GET(DT_GPIO_CTLR(DT_ALIAS(lora0), busy_gpios));
    gpio_pin_t busy_pin = DT_GPIO_PIN(DT_ALIAS(lora0), busy_gpios);

    if (!device_is_ready(lora_dev) || !device_is_ready(adc_dev)) {
        LOG_ERR("Devices not ready");
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

    char msg[64];
    while (1) {
        float voltage = read_sensor_voltage(adc_dev);
        snprintf(msg, sizeof(msg), "Sensor Voltage: %.2f V", voltage);

        LOG_INF("Sending: %s", msg);
        int busy_before = gpio_pin_get(busy_gpio, busy_pin);
        ret = lora_send(lora_dev, (uint8_t *)msg, strlen(msg));
        int busy_after = gpio_pin_get(busy_gpio, busy_pin);

        LOG_INF("lora_send() returned: %d, BUSY before: %d, after: %d", ret, busy_before, busy_after);
        if (ret == -EBUSY) {
            LOG_WRN("SX1261 busy. Retrying in 1s...");
        } else if (ret < 0) {
            LOG_ERR("lora_send() failed: %d", ret);
        } else {
            LOG_INF("Message sent!");
            gpio_pin_toggle_dt(&led);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
