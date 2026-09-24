#pragma once

#include <stdint.h>

class ApogeeDetector {
public:
    static constexpr float DROP_THRESHOLD_METERS = 3.0f;
    static constexpr uint8_t REQUIRED_DROP_SAMPLES = 5;
    static constexpr uint32_t BURNOUT_LOCKOUT_MS = 2500;

    bool update(float currentAltitude, float launchAltitude, uint32_t flightElapsedMs) {
        if (flightElapsedMs < BURNOUT_LOCKOUT_MS) {
            return false;
        }

        const float relativeAltitude = currentAltitude - launchAltitude;
        if (relativeAltitude > peakAltitude_) {
            peakAltitude_ = relativeAltitude;
            dropSamples_ = 0;
            return false;
        }

        if (relativeAltitude < peakAltitude_ - DROP_THRESHOLD_METERS) {
            ++dropSamples_;
            return dropSamples_ >= REQUIRED_DROP_SAMPLES;
        }

        // A sample near the peak breaks the consecutive descent confirmation sequence.
        dropSamples_ = 0;
        return false;
    }

    void reset() {
        peakAltitude_ = 0.0f;
        dropSamples_ = 0;
    }

private:
    float peakAltitude_ = 0.0f;
    uint8_t dropSamples_ = 0;
};
