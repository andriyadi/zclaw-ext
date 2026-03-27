/*
 * Host tests for local admin WiFi runtime reconnect helpers.
 */

#include <stdio.h>
#include <stdint.h>

#include "boot_guard.h"
#include "config.h"
#include "local_admin.h"
#include "mock_memory.h"

#define TEST(name) static int test_##name(void)
#define ASSERT(cond) do { \
    if (!(cond)) { \
        printf("  FAIL: %s (line %d)\n", #cond, __LINE__); \
        return 1; \
    } \
} while(0)

TEST(runtime_retry_delay_starts_at_base_and_caps)
{
    ASSERT(local_admin_test_runtime_retry_delay_ms(0) == WIFI_RUNTIME_RETRY_BASE_MS);
    ASSERT(local_admin_test_runtime_retry_delay_ms(1) == WIFI_RUNTIME_RETRY_BASE_MS);
    ASSERT(local_admin_test_runtime_retry_delay_ms(2) >= WIFI_RUNTIME_RETRY_BASE_MS);
    ASSERT(local_admin_test_runtime_retry_delay_ms(2) <= WIFI_RUNTIME_RETRY_MAX_MS);
    ASSERT(local_admin_test_runtime_retry_delay_ms(3) >= local_admin_test_runtime_retry_delay_ms(2));
    ASSERT(local_admin_test_runtime_retry_delay_ms(10) == WIFI_RUNTIME_RETRY_MAX_MS);
    ASSERT(local_admin_test_runtime_retry_delay_ms(100) == WIFI_RUNTIME_RETRY_MAX_MS);
    return 0;
}

TEST(runtime_reboot_budget_honors_attempt_limit)
{
    ASSERT(!local_admin_test_runtime_reboot_budget_exhausted(1, 0));
    ASSERT(!local_admin_test_runtime_reboot_budget_exhausted(WIFI_RUNTIME_MAX_ATTEMPTS,
                                                             WIFI_RUNTIME_REBOOT_AFTER_MS - 1U));
    ASSERT(local_admin_test_runtime_reboot_budget_exhausted(WIFI_RUNTIME_MAX_ATTEMPTS + 1U, 0));
    return 0;
}

TEST(runtime_reboot_budget_honors_outage_limit)
{
    ASSERT(!local_admin_test_runtime_reboot_budget_exhausted(1, WIFI_RUNTIME_REBOOT_AFTER_MS - 1U));
    ASSERT(local_admin_test_runtime_reboot_budget_exhausted(1, WIFI_RUNTIME_REBOOT_AFTER_MS));
    ASSERT(local_admin_test_runtime_reboot_budget_exhausted(1, WIFI_RUNTIME_REBOOT_AFTER_MS + 1U));
    return 0;
}

TEST(runtime_reboot_refunds_boot_count_by_one_with_floor)
{
    mock_memory_reset();
    ASSERT(local_admin_test_runtime_refund_boot_count() == ESP_OK);
    ASSERT(boot_guard_get_persisted_count() == 0);

    ASSERT(boot_guard_set_persisted_count(3) == ESP_OK);
    ASSERT(local_admin_test_runtime_refund_boot_count() == ESP_OK);
    ASSERT(boot_guard_get_persisted_count() == 2);

    mock_memory_fail_next_set(ESP_FAIL);
    ASSERT(local_admin_test_runtime_refund_boot_count() == ESP_FAIL);
    ASSERT(boot_guard_get_persisted_count() == 2);
    return 0;
}

int test_local_admin_wifi_runtime_all(void)
{
    int failures = 0;

    printf("\nLocal Admin WiFi Runtime Tests:\n");

    printf("  runtime_retry_delay_starts_at_base_and_caps... ");
    if (test_runtime_retry_delay_starts_at_base_and_caps() == 0) {
        printf("OK\n");
    } else {
        failures++;
    }

    printf("  runtime_reboot_budget_honors_attempt_limit... ");
    if (test_runtime_reboot_budget_honors_attempt_limit() == 0) {
        printf("OK\n");
    } else {
        failures++;
    }

    printf("  runtime_reboot_budget_honors_outage_limit... ");
    if (test_runtime_reboot_budget_honors_outage_limit() == 0) {
        printf("OK\n");
    } else {
        failures++;
    }

    printf("  runtime_reboot_refunds_boot_count_by_one_with_floor... ");
    if (test_runtime_reboot_refunds_boot_count_by_one_with_floor() == 0) {
        printf("OK\n");
    } else {
        failures++;
    }

    return failures;
}
