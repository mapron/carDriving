#ifndef CARTELEMETRY_H
#define CARTELEMETRY_H

#include <QPointF>
/**
 * \brief Данные телеметрии для анализа
 */
struct CarTelemetry {
    bool m_performingTurn;  //!< Производится поворот руля.
    float m_angle;          //!< Текущий абсолютный угол
    float m_sideRatio;      //!< Тип положения авто.  -1 - движение по левой полосе, 1 - по правой, > 1 , <-1 - обочина, 0.0 - ровно по разделительной полосе.
    float m_splitCollision; //!< Относительное значеие наезда на сплошную линию.
    QPointF m_pos;          //!< положеие авто
    float m_speed;          //!< скорость, пикселей в секунду.
    CarTelemetry();
};


#endif // CARTELEMETRY_H
