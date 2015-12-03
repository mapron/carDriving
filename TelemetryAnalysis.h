#ifndef TELEMETRYANALYSIS_H
#define TELEMETRYANALYSIS_H
#include "CarTelemetry.h"
#include <QSettings>

/**
 * \brief Класс анализа телеметрии
 *
 * Анализируя параметры движения, выдает те или иные сообщения.
 */
class TelemetryAnalysis
{
    QPointF m_currentPos;       //!< Текущая позиция
    float m_currentAngle;       //!< Текущий угол, градусы. (вверх=270)

    QPointF m_startTurnPos;     //!< Позиция в момент начала поворота.
    float m_startTurnAngle;     //!< Угол в момент начала поворота.
    bool m_performingTurn;      //!< Выполняется поворот.

    QPointF m_endTurnPos;       //!< Позиция в момент окончания поворота.

    bool m_isRealTurn;          //!< Отображение разворота для пользователя.
    bool m_isTurnFinished;      //!< Разворот завершен.
    bool m_isOppositeDriving;   //!< Выезд на встречную.
    bool m_isSplitLaneHit;      //!< Наезд на сплошную.
    bool m_isDitchSlid;         //!< Съезд в кювет.

    struct Parameters {
        float minTurningAngle;              //!< Мин. угол, чтобы засчитать поворот, градусы.
        float minTurningLength;             //!< Мин. расстояние, которое нужно проехать, для разворота.
        float splitLaneHitSensibility;      //!< чувствительность наезда на сплошную 0.0...1.0
        float splitLaneHitIntersectionRate; //!< чувствительность площади пересечения сплошной 0.0...1.0
        float minOppositeDrivingAngle;      //!< мин угол для засчета выезда на встречную полосу, градусы.
        float turnFinishedPathLength;       //!< Когда считать что сообщение "поворот закончен" уже не выводить.
    } m_params;

public:
    TelemetryAnalysis(QSettings* settings);
    void update(const CarTelemetry& telemetry);

    bool isTurn() const;
    bool isTurnFinished() const;
    bool isOppositeDriving() const;
    bool isSplitLaneHit() const;
    bool isDitchSlid() const;
};

#endif // TELEMETRYANALYSIS_H
