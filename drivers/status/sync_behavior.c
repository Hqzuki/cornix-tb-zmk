/* SPDX-License-Identifier: MIT
 * Uses ZMK's existing encrypted split behavior channel; no extra BLE link.
 */
#define DT_DRV_COMPAT cornix_led_sync
#include <errno.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include "backend.h"

/* Only the images whose devicetree has the led_sync node (both Cornix halves
 * and the XIAO trackball) define the behavior. The central (right half) sends
 * it to every peripheral slot; peripherals apply it, the central rejects it. */
#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)
static int sync_pressed(struct zmk_behavior_binding *binding,
                        struct zmk_behavior_binding_event event) {
    (void)event;
#if IS_ENABLED(CONFIG_CORNIX_LED_STATUS) && !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    int rc = cornix_status_receive(binding->param1, binding->param2);
    return rc < 0 ? rc : ZMK_BEHAVIOR_OPAQUE;
#else
    (void)binding;
    return -ENOTSUP;
#endif
}
static int sync_released(struct zmk_behavior_binding *binding,
                         struct zmk_behavior_binding_event event) {
    (void)binding; (void)event; return ZMK_BEHAVIOR_OPAQUE;
}
static const struct behavior_driver_api sync_api = {
    .binding_pressed = sync_pressed, .binding_released = sync_released,
};
BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &sync_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
