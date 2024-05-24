#include "kalmanfilter.h"

KalmanFilter::KalmanFilter(double initialEstimate, double initialErrorEstimate, double processNoise, double measurementNoise)
    : xEstimate(initialEstimate), errorEstimate(initialErrorEstimate), processNoise(processNoise), measurementNoise(measurementNoise)
{
}

double KalmanFilter::update(double measurement)
{
    // Prediction step
    predict();

    // Correction step
    correct(measurement);

    return xEstimate;
}

void KalmanFilter::predict()
{
    // Predict the next state estimate
    xEstimate = xEstimate;
    errorEstimate += processNoise;
}

void KalmanFilter::correct(double measurement)
{
    // Calculate Kalman gain
    double kalmanGain = errorEstimate / (errorEstimate + measurementNoise);

    // Update the state estimate based on measurement
    xEstimate += kalmanGain * (measurement - xEstimate);

    // Update the error estimate
    errorEstimate *= (1.0 - kalmanGain);
}
