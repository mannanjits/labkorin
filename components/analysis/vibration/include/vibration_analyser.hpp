#pragma once

#include <cstddef>

class vibration_analyser{
    private:
    static constexpr size_t WINDOW_SIZE =100;
    size_t write_index=0;
    size_t sample_count=0;

    float x_samples[WINDOW_SIZE]{};
    float y_samples[WINDOW_SIZE]{};
    float z_samples[WINDOW_SIZE]{};

    public:
    void addSample(float x, float y, float z);

    bool isReady() const;

    float calculateRMS() const;

    void reset();
};
