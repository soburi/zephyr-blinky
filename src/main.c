/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/sys_io.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000
#if 0
#define GPIO2_NODE DT_NODELABEL(gpio2)
#define GPIO2_PHYS_BASE DT_REG_ADDR(GPIO2_NODE)
#define GPIO2_INOUTSEL_OFFSET 0x04
#define GPIO2_OUTDT_OFFSET    0x08
#define GPIO2_INDT_OFFSET     0x0c
#define GPIO2_OUTDTSEL_OFFSET 0x40
#define GPIO2_PIN_COUNT       20

static mm_reg_t gpio2_base;

static void map_gpio2_registers(void)
{
	device_map(&gpio2_base, GPIO2_PHYS_BASE, DT_REG_SIZE(GPIO2_NODE),
		   K_MEM_CACHE_NONE);
}

static void print_gpio2_registers(const char *reason)
{
	uint32_t inoutsel = sys_read32(gpio2_base + GPIO2_INOUTSEL_OFFSET);
	uint32_t outdt = sys_read32(gpio2_base + GPIO2_OUTDT_OFFSET);
	uint32_t indt = sys_read32(gpio2_base + GPIO2_INDT_OFFSET);
	uint32_t outdtsel = sys_read32(gpio2_base + GPIO2_OUTDTSEL_OFFSET);

	printf("GPIO2 %s: INOUTSEL=0x%08x OUTDT=0x%08x "
	       "INDT=0x%08x OUTDTSEL=0x%08x\n",
	       reason, inoutsel, outdt, indt, outdtsel);

	for (unsigned int pin = 0; pin < GPIO2_PIN_COUNT; pin++) {
		printf("GPIO2_%02u: dir=%u out=%u in=%u\n", pin,
		       (inoutsel >> pin) & 1U,
		       (outdt >> pin) & 1U,
		       (indt >> pin) & 1U);
	}
}
#endif

/* The devicetree node identifiers for the GPIO2 LED aliases. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define LED4_NODE DT_ALIAS(led4)
#define LED5_NODE DT_ALIAS(led5)
#define LED6_NODE DT_ALIAS(led6)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec leds[] = {
	GPIO_DT_SPEC_GET(LED0_NODE, gpios),
	GPIO_DT_SPEC_GET(LED1_NODE, gpios),
	GPIO_DT_SPEC_GET(LED2_NODE, gpios),
	GPIO_DT_SPEC_GET(LED3_NODE, gpios),
	GPIO_DT_SPEC_GET(LED4_NODE, gpios),
	GPIO_DT_SPEC_GET(LED5_NODE, gpios),
	GPIO_DT_SPEC_GET(LED6_NODE, gpios),
};

int main(void)
{
	int ret;
	bool led_state = true;

	for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
		if (!gpio_is_ready_dt(&leds[i])) {
			return 0;
		}

		ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_ACTIVE);
		if (ret < 0) {
			return 0;
		}
	}

#if 0
	map_gpio2_registers();
	print_gpio2_registers("configured");
#endif

	while (1) {
		for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
			ret = gpio_pin_toggle_dt(&leds[i]);
			if (ret < 0) {
				return 0;
			}
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
#if 0
		print_gpio2_registers("toggled");
#endif
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
