#include <unity.h>

#include "ApogeeDetector.h"

namespace {
constexpr float LAUNCH_ALTITUDE = 120.0f;
constexpr uint32_t AFTER_BURNOUT_MS = ApogeeDetector::BURNOUT_LOCKOUT_MS + 1;

void test_ignores_altitude_loss_during_motor_burn() {
    ApogeeDetector detector;

    TEST_ASSERT_FALSE(detector.update(220.0f, LAUNCH_ALTITUDE, 1000));
    TEST_ASSERT_FALSE(detector.update(200.0f, LAUNCH_ALTITUDE, 2000));
}

void test_confirms_apogee_after_five_consecutive_drops() {
    ApogeeDetector detector;
    TEST_ASSERT_FALSE(detector.update(320.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));

    for (uint8_t sample = 0; sample < ApogeeDetector::REQUIRED_DROP_SAMPLES - 1; ++sample) {
        TEST_ASSERT_FALSE(detector.update(315.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
    }

    TEST_ASSERT_TRUE(detector.update(315.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
}

void test_does_not_confirm_without_a_drop_larger_than_threshold() {
    ApogeeDetector detector;
    TEST_ASSERT_FALSE(detector.update(320.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));

    for (uint8_t sample = 0; sample < ApogeeDetector::REQUIRED_DROP_SAMPLES + 1; ++sample) {
        TEST_ASSERT_FALSE(detector.update(317.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
    }
}

void test_peak_recovery_resets_descent_confirmation() {
    ApogeeDetector detector;
    TEST_ASSERT_FALSE(detector.update(320.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));

    TEST_ASSERT_FALSE(detector.update(315.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
    TEST_ASSERT_FALSE(detector.update(315.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
    TEST_ASSERT_FALSE(detector.update(319.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));

    for (uint8_t sample = 0; sample < ApogeeDetector::REQUIRED_DROP_SAMPLES - 1; ++sample) {
        TEST_ASSERT_FALSE(detector.update(315.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
    }
    TEST_ASSERT_TRUE(detector.update(315.0f, LAUNCH_ALTITUDE, AFTER_BURNOUT_MS));
}
}  // namespace

void setUp() {}
void tearDown() {}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_ignores_altitude_loss_during_motor_burn);
    RUN_TEST(test_confirms_apogee_after_five_consecutive_drops);
    RUN_TEST(test_does_not_confirm_without_a_drop_larger_than_threshold);
    RUN_TEST(test_peak_recovery_resets_descent_confirmation);
    return UNITY_END();
}
