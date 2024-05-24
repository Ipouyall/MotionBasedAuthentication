#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter
{
public:
    KalmanFilter(double initialEstimate, double initialErrorEstimate, double processNoise, double measurementNoise);

    // Update the filter with a new measurement
    double update(double measurement);

private:
    double xEstimate; // Current state estimate
    double errorEstimate; // Current estimate error
    double processNoise; // Process noise covariance
    double measurementNoise; // Measurement noise covariance

    // Prediction step: predict the next state estimate
    void predict();

    // Correction step: correct the estimate using the measurement
    void correct(double measurement);
};

#endif // KALMANFILTER_H
