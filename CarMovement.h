#ifndef CARMOVEMENT_H
#define CARMOVEMENT_H
#include <QPointF>

/**
 * \brief Расчет параметров движения авто.
 */
class CarMovement {
    float m_angle;//!< угол поворота в градусах
    float m_speed;//!< скорость, пикселей в секунду.

    float m_x; //!< координаты в пикселях.
    float m_y;
    public:
    CarMovement();
    enum VelocityMode {vmNeutral, vmThrottle, vmBrake }; //!< режим движения. нейтральный, газ, тормоз.
    enum RotationMode {rmNeutral, rmOrdinal, rmLeft, rmRight };//!< режим поворота. нейтральный (в случае есть есть движение),
    /// выравнивание по горизонтали/вертикали и повороты.

    void applyVelocity(VelocityMode velMode); //!< производит применение ускорение, меняя скорость.
    void applyVelocity(float velocity);
    void applyRotation(RotationMode rotMode, double seconds); //!< производит применение вращение. Влияет на скорость.
    void applySpeed(double seconds, bool isReverseMove); //!< производит применение скорости, меняет координаты.
    float carSpeed() const;
    bool isMoving() const;

    QPointF carPos() const;
    float carAngle() const;


};

#endif // CARMOVEMENT_H
