# SPDX-License-Identifier: MIT
# The short-name behavior exists in normal and reset images; reset is a no-op.
target_sources(app PRIVATE sync_behavior.c)
if(CONFIG_CORNIX_LED_STATUS)
  if(CONFIG_CORNIX_RIGHT_CENTRAL OR CONFIG_CORNIX_LEFT_PERIPHERAL OR CONFIG_CORNIX_TB_TRACKBALL)
    target_sources(app PRIVATE controller_tb.c)
    if(CONFIG_CORNIX_TB_TRACKBALL)
      # The XIAO has no WS2812 strip: its onboard RGB LED (PWM) is the inner LED.
      target_sources(app PRIVATE backend_none.c)
    else()
      # Both Cornix halves drive the two-pixel WS2812 strip.
      target_sources(app PRIVATE backend_strip.c)
    endif()
  else()
    target_sources(app PRIVATE controller.c backend_strip.c)
  endif()
endif()
