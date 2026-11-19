/* SPDX-License-Identifier: MIT
 * The XIAO central has no Cornix WS2812 strip. Its onboard RGB LED is used
 * for the inner status color (battery / charging / low battery). Hardware
 * PWM limits its brightness to match the Cornix WS2812 status LEDs.
 */
#include <errno.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include "backend.h"

static const struct pwm_dt_spec led_red = PWM_DT_SPEC_GET(DT_NODELABEL(xiao_status_red));
static const struct pwm_dt_spec led_green = PWM_DT_SPEC_GET(DT_NODELABEL(xiao_status_green));
static const struct pwm_dt_spec led_blue = PWM_DT_SPEC_GET(DT_NODELABEL(xiao_status_blue));

int status_backend_init(void) {
    if (!pwm_is_ready_dt(&led_red) || !pwm_is_ready_dt(&led_green) ||
        !pwm_is_ready_dt(&led_blue)) {
        return -ENODEV;
    }
    int rc = pwm_set_pulse_dt(&led_red, 0);
    if (rc < 0) { return rc; }
    rc = pwm_set_pulse_dt(&led_green, 0);
    if (rc < 0) { return rc; }
    return pwm_set_pulse_dt(&led_blue, 0);
}

int status_backend_write(struct status_frame frame, int64_t now) {
    (void)now;
    /* The Cornix WS2812 ceiling is 13/255; use that same absolute RGB scale. */
    int rc = pwm_set_pulse_dt(&led_red, led_red.period * frame.inner.r / 255U);
    if (rc < 0) { return rc; }
    rc = pwm_set_pulse_dt(&led_green, led_green.period * frame.inner.g / 255U);
    if (rc < 0) { return rc; }
    return pwm_set_pulse_dt(&led_blue, led_blue.period * frame.inner.b / 255U);
}
