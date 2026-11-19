#include"vibration_analyser.hpp"
#include<cmath>

void vibration_analyser::addSample(float x, float y, float z){
    x_samples[sample_count]=x;
    y_samples[sample_count]=y;
    z_samples[sample_count]=z;

    write_index=(write_index+1)%WINDOW_SIZE;

    if(sample_count<WINDOW_SIZE){
        sample_count++;
    }
}

bool vibration_analyser::isReady() const
{
    return sample_count == WINDOW_SIZE;
}

float vibration_analyser::calculateRMS() const
{
    if (!isReady()) {
        return 0.0f;
    }

    float mean_x = 0.0f;
    float mean_y = 0.0f;
    float mean_z = 0.0f;

    for (size_t i = 0; i < WINDOW_SIZE; i++) {
        mean_x += x_samples[i];
        mean_y += y_samples[i];
        mean_z += z_samples[i];
    }

    mean_x /= WINDOW_SIZE;
    mean_y /= WINDOW_SIZE;
    mean_z /= WINDOW_SIZE;

    float sum = 0.0f;

    for (size_t i = 0; i < WINDOW_SIZE; i++) {
        float dx = x_samples[i] - mean_x;
        float dy = y_samples[i] - mean_y;
        float dz = z_samples[i] - mean_z;

        sum += dx * dx;
        sum += dy * dy;
        sum += dz * dz;
    }

    return std::sqrt(sum / (WINDOW_SIZE * 3.0f));
}

void vibration_analyser::reset()
{
    write_index = 0;
    sample_count = 0;

    for (size_t i = 0; i < WINDOW_SIZE; i++) {
        x_samples[i] = 0.0f;
        y_samples[i] = 0.0f;
        z_samples[i] = 0.0f;
    }
}