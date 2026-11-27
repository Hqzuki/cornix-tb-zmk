/* SPDX-License-Identifier: MIT
 * AML gate: only arm the auto-mouse layer (ZMK's temp-layer processor) once the
 * ball has really moved, so micro-vibration (a finger brushing the ball while
 * typing on the keys next to it) does not switch to the MOUSE layer.
 * The cursor itself is not affected: every event continues down the chain.
 */
#define DT_DRV_COMPAT custom_input_processor_aml_gate

#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/input/input.h>
#include <drivers/input_processor.h>
#include <zmk/keymap.h>

struct aml_gate_config {
    const struct device *temp_layer;
    uint8_t layer;
    uint32_t timeout_ms;
    uint16_t threshold;
    uint16_t window_ms;
};

struct aml_gate_data {
    int32_t travel;
    int64_t window_start;
};

static int aml_gate_handle(const struct device *dev, struct input_event *event,
                           uint32_t param1, uint32_t param2,
                           struct zmk_input_processor_state *state) {
    ARG_UNUSED(param1); ARG_UNUSED(param2);
    const struct aml_gate_config *cfg = dev->config;
    struct aml_gate_data *data = dev->data;

    if (event->type != INPUT_EV_REL ||
        (event->code != INPUT_REL_X && event->code != INPUT_REL_Y)) {
        return ZMK_INPUT_PROC_CONTINUE;
    }
    /* Already in AML: keep feeding the temp layer so it behaves as before. */
    if (zmk_keymap_layer_active(cfg->layer)) {
        data->travel = 0;
        zmk_input_processor_handle_event(cfg->temp_layer, event, cfg->layer, cfg->timeout_ms, state);
        return ZMK_INPUT_PROC_CONTINUE;
    }
    int64_t now = k_uptime_get();
    if (now - data->window_start > cfg->window_ms) {
        data->window_start = now;
        data->travel = 0;
    }
    data->travel += abs(event->value);
    if (data->travel >= cfg->threshold) {
        data->travel = 0;
        zmk_input_processor_handle_event(cfg->temp_layer, event, cfg->layer, cfg->timeout_ms, state);
    }
    return ZMK_INPUT_PROC_CONTINUE;
}

static const struct zmk_input_processor_driver_api aml_gate_api = {
    .handle_event = aml_gate_handle,
};

#define AML_GATE_INST(n)                                                                         \
    static const struct aml_gate_config aml_gate_config_##n = {                                  \
        .temp_layer = DEVICE_DT_GET(DT_INST_PHANDLE(n, processor)),                               \
        .layer = DT_INST_PROP(n, layer),                                                          \
        .timeout_ms = DT_INST_PROP(n, timeout_ms),                                                \
        .threshold = DT_INST_PROP(n, threshold),                                                  \
        .window_ms = DT_INST_PROP(n, window_ms),                                                  \
    };                                                                                           \
    static struct aml_gate_data aml_gate_data_##n;                                               \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, &aml_gate_data_##n, &aml_gate_config_##n, POST_KERNEL,  \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &aml_gate_api);

DT_INST_FOREACH_STATUS_OKAY(AML_GATE_INST)
