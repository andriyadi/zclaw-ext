#include "tools_handlers.h"
#include "cores3/app.h"
#include <axp2101/axp2101.h>
#include <stdio.h>
#include <string.h>

bool tools_set_display_text_handler(const cJSON *input, char *result, size_t result_len)
{
    const cJSON *text_json = cJSON_GetObjectItemCaseSensitive(input, "text");

    if (!text_json || !cJSON_IsString(text_json)) {
        snprintf(result, result_len, "Error: 'text' parameter required (string)");
        return false;
    }

    const char *text = text_json->valuestring;
    if (!text || text[0] == '\0') {
        snprintf(result, result_len, "Error: 'text' cannot be empty");
        return false;
    }

    int32_t err = cores3_app_set_main_text_content(text);
    if (err != 0) {
        snprintf(result, result_len, "Error: failed to set display text (err=%ld)", (long)err);
        return false;
    }

    snprintf(result, result_len, "Display text updated: %s", text);
    return true;
}

bool tools_set_display_brightness_handler(const cJSON *input, char *result, size_t result_len)
{
    const cJSON *brightness_json = cJSON_GetObjectItemCaseSensitive(input, "brightness");

    if (!brightness_json || !cJSON_IsNumber(brightness_json)) {
        snprintf(result, result_len, "Error: 'brightness' parameter required (integer 0-100)");
        return false;
    }

    int brightness = (int)cJSON_GetNumberValue(brightness_json);
    if (brightness < 0 || brightness > 100) {
        snprintf(result, result_len, "Error: 'brightness' must be between 0 and 100");
        return false;
    }

    int32_t err = cores3_app_display_brightness_set((uint8_t)brightness);
    if (err != 0) {
        snprintf(result, result_len, "Error: failed to set display brightness (err=%ld)", (long)err);
        return false;
    }

    snprintf(result, result_len, "Display brightness set to %d%%", brightness);
    return true;
}

bool tools_get_battery_percentage_handler(const cJSON *input, char *result, size_t result_len)
{
    (void)input;
    uint8_t percent = 0;
    int32_t err = cores3_app_battery_percentage_get(&percent);
    if (err != AXP2101_ERR_NONE) {
        snprintf(result, result_len, "Error: failed to read battery percentage (%s)", axp2101_err_to_name(err));
        return false;
    }
    snprintf(result, result_len, "Battery: %u%%", (unsigned)percent);
    return true;
}

bool tools_get_battery_voltage_handler(const cJSON *input, char *result, size_t result_len)
{
    (void)input;
    uint16_t voltage_mv = 0;
    int32_t err = cores3_app_battery_voltage_get(&voltage_mv);
    if (err != AXP2101_ERR_NONE) {
        snprintf(result, result_len, "Error: failed to read battery voltage (%s)", axp2101_err_to_name(err));
        return false;
    }
    snprintf(result, result_len, "Battery voltage: %u mV", (unsigned)voltage_mv);
    return true;
}

bool tools_get_display_brightness_handler(const cJSON *input, char *result, size_t result_len)
{
    (void)input;
    uint8_t brightness = 0;
    int32_t err = cores3_app_display_brightness_get(&brightness);
    if (err != AXP2101_ERR_NONE) {
        snprintf(result, result_len, "Error: failed to read display brightness (%s)", axp2101_err_to_name(err));
        return false;
    }
    snprintf(result, result_len, "Display brightness: %u%%", (unsigned)brightness);
    return true;
}