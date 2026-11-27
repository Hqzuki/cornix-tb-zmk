/* SPDX-License-Identifier: MIT
 * Hold-to-slow pointer: a behavior (bound to Space on the MOUSE layer) that is
 * active while held, and an input processor that halves pointer motion while
 * the behavior is held. No extra keymap layer is needed.
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/input/input.h>
#include <drivers/behavior.h>
#include <drivers/input_processor.h>
#include <zmk/behavior.h>

static atomic_t precision_held;

/* ---- behavior: custom,behavior-precision ---- */
#define DT_DRV_COMPAT custom_behavior_precision

static int precision_pressed(struct zmk_behavior_binding *binding,
                             struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding); ARG_UNUSED(event);
    atomic_set(&precision_held, 1);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int precision_released(struct zmk_behavior_binding *binding,
                              struct zmk_behavior_binding_event event) {
    ARG_UNUSED(binding); ARG_UNUSED(event);
    atomic_set(&precision_held, 0);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api precision_behavior_api = {
    .binding_pressed = precision_pressed,
    .binding_released = precision_released,
};

#define PRECISION_BEHAVIOR_INST(n)                                                               \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,                              \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &precision_behavior_api);
DT_INST_FOREACH_STATUS_OKAY(PRECISION_BEHAVIOR_INST)
#undef DT_DRV_COMPAT

/* ---- input processor: custom,input-processor-precision ---- */
#define DT_DRV_COMPAT custom_input_processor_precision

static int32_t remainder_x, remainder_y;

static int precision_handle(const struct device *dev, struct input_event *event,
                            uint32_t param1, uint32_t param2,
                            struct zmk_input_processor_state *state) {
    ARG_UNUSED(dev); ARG_UNUSED(param1); ARG_UNUSED(param2); ARG_UNUSED(state);
    if (event->type != INPUT_EV_REL ||
        (event->code != INPUT_REL_X && event->code != INPUT_REL_Y)) {
        return ZMK_INPUT_PROC_CONTINUE;
    }
    int32_t *rem = event->code == INPUT_REL_X ? &remainder_x : &remainder_y;
    if (!atomic_get(&precision_held)) {
        *rem = 0;
        return ZMK_INPUT_PROC_CONTINUE;
    }
    /* Half speed, carrying the odd count so slow motion is not lost. */
    int32_t total = event->value + *rem;
    event->value = total / 2;
    *rem = total - event->value * 2;
    return ZMK_INPUT_PROC_CONTINUE;
}

static const struct zmk_input_processor_driver_api precision_processor_api = {
    .handle_event = precision_handle,
};

#define PRECISION_PROCESSOR_INST(n)                                                              \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,                                \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &precision_processor_api);
DT_INST_FOREACH_STATUS_OKAY(PRECISION_PROCESSOR_INST)
