set(zclaw_srcs
    "main.c"
    "agent.c"
    "agent_commands.c"
    "agent_prompt.c"
    "channel.c"
    "local_admin.c"
    "llm.c"
    "llm_auth.c"
    "http_gate.c"
    "tools.c"
    "tools_common.c"
    "gpio_policy.c"
    "tools_dht.c"
    "tools_memory.c"
    "tools_persona.c"
    "tools_cron.c"
    "tools_system.c"
    "tools_cores3.c"
    "memory.c"
    "json_util.c"
    "telegram.c"
    "telegram_http_diag.c"
    "telegram_poll_policy.c"
    "telegram_targets.c"
    "telegram_chat_ids.c"
    "telegram_token.c"
    "cron.c"
    "memory_keys.c"
    "telegram_update.c"
    "text_buffer.c"
    "security.c"
    "cron_utils.c"
    "ratelimit.c"
    "ota.c"
    "boot_guard.c"
    "user_tools.c"
    "wifi_credentials.c"
    "cores3/app.c"
    "cores3/cores3_board.c"
    "cores3/cores3_io_extender.c"
    "cores3/cores3_power_mgmt.c"
    "cores3/cores3_touch.c"
    "cores3/gui_app.c"
    "cores3/display/cores3_display.c"
    "cores3/graphics/bitmap_icon.c"
    "cores3/graphics/bmf_reader.c"
    "cores3/graphics/display_surface.c"
    "cores3/graphics/render_support.c"
    "cores3/graphics/text_renderer.c"
)

if(CONFIG_ZCLAW_ENABLE_GPIO_TOOLS)
    list(APPEND zclaw_srcs "tools_gpio.c")
endif()

if(CONFIG_ZCLAW_ENABLE_I2C_TOOLS)
    list(APPEND zclaw_srcs "tools_i2c.c")
endif()

idf_component_register(
    SRCS ${zclaw_srcs}
    INCLUDE_DIRS "."
    REQUIRES
        nvs_flash
        esp_wifi
        esp_http_client
        esp_netif
        esp-tls
        esp_timer
        json
        driver
        lwip
        esp_driver_usb_serial_jtag
        app_update
        axp2101
        aw9523b
        ft6x36
        igpio
        ii2c
        ili9342
        ispi
)

set(ZCLAW_RELEASE_VERSION "dev")
if(DEFINED PROJECT_VER AND NOT PROJECT_VER STREQUAL "")
    set(ZCLAW_RELEASE_VERSION "${PROJECT_VER}")
endif()
target_compile_definitions(${COMPONENT_LIB} PRIVATE ZCLAW_VERSION="${ZCLAW_RELEASE_VERSION}")