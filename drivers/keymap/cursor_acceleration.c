/* SPDX-License-Identifier: MIT */
#define DT_DRV_COMPAT cornix_input_processor_cursor_acceleration

#include <limits.h>
#include <stdint.h>

#include <drivers/input_processor.h>
#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <zephyr/sys/util.h>
#include <zmk/keymap.h>

struct cursor_acceleration_config {
    uint8_t mouse_layer;
    uint16_t start_delta;
    uint16_t max_delta;
    uint8_t max_multiplier;
};

static int cursor_acceleration_handle_event(const struct device *dev,
                                            struct input_event *event,
                                            uint32_t param1, uint32_t param2,
                                            struct zmk_input_processor_state *state) {
    ARG_UNUSED(param1);
    ARG_UNUSED(param2);
    ARG_UNUSED(state);

    if (event->type != INPUT_EV_REL ||
        (event->code != INPUT_REL_X && event->code != INPUT_REL_Y) ||
        event->value == 0) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    const struct cursor_acceleration_config *config = dev->config;
    if (zmk_keymap_highest_layer_active() != config->mouse_layer) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    const int64_t value = event->value;
    const int64_t magnitude = value < 0 ? -value : value;
    uint32_t multiplier_milli = 1000;

    if (magnitude >= config->max_delta) {
        multiplier_milli = (uint32_t)config->max_multiplier * 1000U;
    } else if (magnitude > config->start_delta &&
               config->max_delta > config->start_delta) {
        const uint32_t span = config->max_delta - config->start_delta;
        const uint32_t boost = ((uint32_t)config->max_multiplier - 1U) * 1000U;
        multiplier_milli += boost * (uint32_t)(magnitude - config->start_delta) / span;
    }

    int64_t scaled = value * multiplier_milli;
    scaled = scaled >= 0 ? (scaled + 500) / 1000 : (scaled - 500) / 1000;
    if (scaled > INT32_MAX) {
        scaled = INT32_MAX;
    } else if (scaled < INT32_MIN) {
        scaled = INT32_MIN;
    }
    event->value = (int32_t)scaled;

    return ZMK_INPUT_PROC_CONTINUE;
}

static const struct zmk_input_processor_driver_api cursor_acceleration_api = {
    .handle_event = cursor_acceleration_handle_event,
};

#define CURSOR_ACCELERATION_INST(n)                                                       \
    BUILD_ASSERT(DT_INST_PROP(n, start_delta) > 0, "Cursor acceleration start must be positive"); \
    BUILD_ASSERT(DT_INST_PROP(n, max_delta) > DT_INST_PROP(n, start_delta), \
                 "Cursor acceleration max delta must exceed start delta"); \
    BUILD_ASSERT(DT_INST_PROP(n, max_multiplier) >= 1 && DT_INST_PROP(n, max_multiplier) <= 8, \
                 "Cursor acceleration multiplier must be from 1 to 8"); \
    static const struct cursor_acceleration_config cursor_acceleration_config_##n = {     \
        .mouse_layer = DT_INST_PROP(n, mouse_layer),                                      \
        .start_delta = DT_INST_PROP(n, start_delta),                                      \
        .max_delta = DT_INST_PROP(n, max_delta),                                          \
        .max_multiplier = DT_INST_PROP(n, max_multiplier),                               \
    };                                                                                    \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, &cursor_acceleration_config_##n,           \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                \
                          &cursor_acceleration_api);

DT_INST_FOREACH_STATUS_OKAY(CURSOR_ACCELERATION_INST)
