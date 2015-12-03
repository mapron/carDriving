#include "TelemetryAnalysis.h"
#include <math.h>
#include <QVector2D>
#define RIGHT_LANE_ANGLE 270.0
#define LEFT_LANE_ANGLE 90.0

TelemetryAnalysis::TelemetryAnalysis(QSettings *settings)
    : m_currentAngle(0.0)
    , m_performingTurn(false)
{
    m_params.minTurningAngle = settings->value("minTurningAngle", 50.0).toFloat();
    m_params.minTurningLength = settings->value("minTurningLength", 20.0).toFloat();

    m_params.splitLaneHitSensibility =   settings->value("splitLaneHitSensibility", 0.5).toFloat();
    m_params.splitLaneHitIntersectionRate = settings->value("splitLaneHitIntersectionRate", 0.2).toFloat();

    m_params.minOppositeDrivingAngle =  settings->value("minOppositeDrivingAngle", 30.0).toFloat();

    m_params.turnFinishedPathLength =  settings->value("turnFinishedPathLength", 200.0).toFloat();

    m_isTurnFinished = false;
    m_isRealTurn = false;

}

inline float vectorLength(QPointF p1, QPointF p2) {
    return QVector2D(p2- p1 ).length();
}

void TelemetryAnalysis::update(const CarTelemetry &telemetry)
{
    bool isTurnStarted = !m_performingTurn && telemetry.m_performingTurn;
    m_performingTurn = telemetry.m_performingTurn;
    m_currentAngle = telemetry.m_angle;
    m_currentPos = telemetry.m_pos;

    if (isTurnStarted) {
        m_startTurnPos = m_currentPos;
        m_startTurnAngle = m_currentAngle;
    }
    bool isRealTurnPrev = m_isRealTurn;
    m_isRealTurn = false;
    // реально мы делаем разворот, когда произведен непрерывный поворот
    // на достаточный угол и машина проехала достаточное расстояние.
    if (m_performingTurn) {
        if (   fabs(m_startTurnAngle-m_currentAngle) > m_params.minTurningAngle
            && vectorLength(m_startTurnPos, m_currentPos) > m_params.minTurningLength
        ) {
            m_isRealTurn = true;
            m_isTurnFinished = false;
        }
    }

    // поворот закончен, когда флаг "поворот" перестал быть выставлен.
    if (isRealTurnPrev && !m_isRealTurn) {
        m_isTurnFinished = true;
        m_endTurnPos = m_currentPos;
    }

    if (!m_isRealTurn && vectorLength(m_endTurnPos, m_currentPos) > m_params.turnFinishedPathLength) {
        m_isTurnFinished = false;
    }

    // выезд на встречную полосу - если отклонение угла от стандартных RIGHT_LANE_ANGLE, LEFT_LANE_ANGLE -
    // превышает допустимое.
    m_isOppositeDriving = false;
    if (telemetry.m_sideRatio == 1.0) {
        if (   telemetry.m_angle < RIGHT_LANE_ANGLE - m_params.minOppositeDrivingAngle
            || telemetry.m_angle > RIGHT_LANE_ANGLE + m_params.minOppositeDrivingAngle
        ) {
            m_isOppositeDriving = true;
        }
    }
    if (telemetry.m_sideRatio == -1.0) {
        if (   telemetry.m_angle < LEFT_LANE_ANGLE - m_params.minOppositeDrivingAngle
            || telemetry.m_angle > LEFT_LANE_ANGLE + m_params.minOppositeDrivingAngle
        ) {
            m_isOppositeDriving = true;
        }
    }
    // если пересечение с разделительной линией выше заданной чувствительности, мы наехали на сплошную.
    m_isSplitLaneHit = false;
    if (    fabs(telemetry.m_sideRatio) < m_params.splitLaneHitSensibility
        &&  telemetry.m_splitCollision > m_params.splitLaneHitIntersectionRate
    ) {
        m_isSplitLaneHit = true;
    }

    // в кювете мы если заведомо за пределами полос.
    m_isDitchSlid = fabs(telemetry.m_sideRatio) > 1;

}

bool TelemetryAnalysis::isTurn() const
{
    return m_isRealTurn;
}

bool TelemetryAnalysis::isTurnFinished() const
{
    return m_isTurnFinished;
}

bool TelemetryAnalysis::isOppositeDriving() const
{
    return m_isOppositeDriving;
}

bool TelemetryAnalysis::isSplitLaneHit() const
{
    return m_isSplitLaneHit;
}

bool TelemetryAnalysis::isDitchSlid() const
{
    return m_isDitchSlid;
}

