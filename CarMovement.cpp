#include "CarMovement.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <QList>
CarMovement::CarMovement()
    : m_angle(270.0)
    , m_speed(0.0)
    , m_x(10.0)
    , m_y(0.0)
{

}

void CarMovement::applyVelocity(CarMovement::VelocityMode velMode)
{
    static const float frictionForceVelocityPerM_S = -0.01;
    static const float throttleVelocity = 6.0;
    static const float brakeVelocity = -10.0;

    float velocity = 0.0;
    switch (velMode) {
        case vmNeutral:
            velocity = frictionForceVelocityPerM_S * m_speed;
            if (velocity > -1) {
                velocity = -1;
            }
        break;
        case vmThrottle:
            velocity = throttleVelocity;
        break;
        case vmBrake:
            velocity = brakeVelocity;
        break;
    }
    applyVelocity(velocity);
}

void CarMovement::applyVelocity(float velocity)
{
    static const float maxSpeed = 500.0;
    static const float midSpeed = 250.0;
    static const float minSpeed = 1.0;
    if (m_speed > midSpeed) velocity /=2;
    m_speed += velocity;
    if (m_speed < minSpeed) m_speed = 0;
    if (m_speed > maxSpeed) m_speed = maxSpeed;
}

void CarMovement::applyRotation(CarMovement::RotationMode rotMode, double seconds)
{
    static const float frictionSideWheelMax = 80.0;
    static const float frictionSideWheelInDegreesPerM_S = 0.7;
    static const float sideWheelVelocity = -4;
    static const QList<float> ordinalAngles = QList<float> () << 0.0 << 90.0 << 180.0 << 270.0;
    static const float ordinalAnglesEpsilon = 2.0;
    float directionMult = 1.0;
    float angleDelta;

    switch (rotMode) {
        case rmNeutral:
        break;
        case rmOrdinal:
            foreach (float ordinalAngle, ordinalAngles) {
                if (fabs(ordinalAngle-m_angle) < ordinalAnglesEpsilon) {
                    m_angle = ordinalAngle;
                    break;//foreach
                }
            }
        break;
        case rmLeft:
            directionMult = -1.0;
            // идем дальше, break не нужен.
        case rmRight:
            applyVelocity(sideWheelVelocity);
            angleDelta = frictionSideWheelInDegreesPerM_S * m_speed ;
            if (angleDelta > frictionSideWheelMax)
                angleDelta = frictionSideWheelMax;
            m_angle +=  directionMult * angleDelta * seconds;
        break;
    }
    // приводим в [0; 360).
    while (m_angle < 0)    m_angle += 360.0;
    while (m_angle >= 360) m_angle -= 360.0;
}

inline double deg2rad(double degrees) {
    return degrees / (180.0 / M_PI);
}
void CarMovement::applySpeed(double seconds, bool isReverseMove)
{
    static const float reverseSpeedFactor = 0.5;
    float speedX = cos(deg2rad(m_angle)) * m_speed * seconds;
    float speedY = sin(deg2rad(m_angle)) * m_speed * seconds;
    if (isReverseMove) {
        speedX = -speedX * reverseSpeedFactor;
        speedY = -speedY * reverseSpeedFactor;
    }
    m_x += speedX;
    m_y += speedY;
}

float CarMovement::carSpeed() const
{
    return m_speed;
}

bool CarMovement::isMoving() const
{
    return m_speed > 0; // проверка на epsilon у нас в applyVelocity.
}

QPointF CarMovement::carPos() const
{
    return QPointF(m_x, m_y);
}

float CarMovement::carAngle() const
{
    return m_angle;
}
