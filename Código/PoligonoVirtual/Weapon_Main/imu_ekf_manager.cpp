#include "imu_ekf_manager.h"
#include "config_weapon.h"
#include <Wire.h>
#include <BasicLinearAlgebra.h>
#include "BoschSensorClass.h"

using namespace BLA;

// Buffer histórico
OrientationRecord history_buffer[HISTORY_BUFFER_SIZE];
uint16_t history_head = 0;
unsigned long last_imu_time = 0;

// Estado del EKF: cuaternión q = [qw, qx, qy, qz]^T
BLA::Matrix<4, 1> x = {1.0, 0.0, 0.0, 0.0}; 
BLA::Matrix<4, 4> P = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}; // Covarianza
BLA::Matrix<4, 4> Q = {0.001,0,0,0, 0,0.001,0,0, 0,0,0.001,0, 0,0,0,0.001}; // Ruido del proceso
BLA::Matrix<3, 3> R = {0.1,0,0, 0,0.1,0, 0,0,0.1}; // Ruido de medición del acelerómetro

void imu_ekf_init() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    IMU.begin(BOSCH_ACCEL_AND_MAGN); // Inicia BMI270 según la librería
}

void imu_ekf_update() {
    unsigned long current_time = millis();
    if (current_time - last_imu_time >= IMU_SAMPLE_DELAY_MS) {
        float dt = (current_time - last_imu_time) / 1000.0f;
        last_imu_time = current_time;

        float ax, ay, az, gx, gy, gz;
        if (IMU.readAcceleration(ax, ay, az) && IMU.readGyroscope(gx, gy, gz)) {
            
            // 1. PREDICCIÓN (Giroscopio)
            gx *= (PI / 180.0f); gy *= (PI / 180.0f); gz *= (PI / 180.0f); // Rad/s
            
            BLA::Matrix<4, 4> Omega = {
                 0,  -gx, -gy, -gz,
                 gx,   0,  gz, -gy,
                 gy, -gz,   0,  gx,
                 gz,  gy, -gx,   0
            };
            
            BLA::Matrix<4, 4> I = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
            BLA::Matrix<4, 4> F = I + (Omega * (dt / 2.0f));
            
            x = F * x; // Predecir estado
            P = F * P * (~F) + Q; // Predecir covarianza
            
            // Normalizar cuaternión
            float norm = sqrt(x(0)*x(0) + x(1)*x(1) + x(2)*x(2) + x(3)*x(3));
            x(0)/=norm; x(1)/=norm; x(2)/=norm; x(3)/=norm;

            // 2. ACTUALIZACIÓN (Acelerómetro)
            float a_norm = sqrt(ax*ax + ay*ay + az*az);
            ax/=a_norm; ay/=a_norm; az/=a_norm;
            BLA::Matrix<3, 1> z_meas = {ax, ay, az};

            float qw = x(0), qx = x(1), qy = x(2), qz = x(3);
            
            // Gravedad esperada
            BLA::Matrix<3, 1> h = {
                2.0f*(qx*qz - qw*qy),
                2.0f*(qy*qz + qw*qx),
                qw*qw - qx*qx - qy*qy + qz*qz
            };

            // Jacobiana H
            BLA::Matrix<3, 4> H = {
                -2.0f*qy,  2.0f*qz, -2.0f*qw,  2.0f*qx,
                 2.0f*qx,  2.0f*qw,  2.0f*qz,  2.0f*qy,
                 2.0f*qw, -2.0f*qx, -2.0f*qy,  2.0f*qz
            };

            BLA::Matrix<3, 3> S = H * P * (~H) + R;
            BLA::Matrix<4, 3> K = P * (~H) * Inverse(S);

            x = x + K * (z_meas - h);
            
            // Re-normalizar
            norm = sqrt(x(0)*x(0) + x(1)*x(1) + x(2)*x(2) + x(3)*x(3));
            x(0)/=norm; x(1)/=norm; x(2)/=norm; x(3)/=norm;
            
            P = (I - K * H) * P;

            // 3. Guardar en Buffer Histórico
            history_buffer[history_head] = {current_time, x(0), x(1), x(2), x(3)};
            history_head = (history_head + 1) % HISTORY_BUFFER_SIZE;
        }
    }
}

bool imu_get_historical_orientation(uint64_t target_timestamp, OrientationRecord* out_record) {
    // Búsqueda simple del más cercano hacia atrás (simplificado para O(1) asumiendo frecuencia constante)
    uint16_t offset_idx = (TRIGGER_JERK_OFFSET_MS / IMU_SAMPLE_DELAY_MS);
    int16_t best_idx = history_head - 1 - offset_idx;
    if (best_idx < 0) best_idx += HISTORY_BUFFER_SIZE;
    
    *out_record = history_buffer[best_idx];
    return true;
}