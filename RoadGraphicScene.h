#ifndef ROADGRAPHICSCENE_H
#define ROADGRAPHICSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "CarMovement.h"
#include "GraphicsItems.h"
#include "CarTelemetry.h"

/**
 * \brief Графическая сцена дороги
 *
 * Создает графическое представление автомобиля, дороги, бордюра.
 * Позволяет управлять движенем с помощью клавиатуры.
 *
 * Сам расчет движения - см. CarMovement
 * Отрисовку элементов см. в CarItem, RoadSplitItem.
 *
 * Основной метод сцены - updateScene(), вызывает функции расчета движения,
 * а также делает emit сигнала updateTelemetry() с телеметрией машины.
 */

class RoadGraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit RoadGraphicScene(const QList<float> &patternConfiguration,
                              float sceneW,
                              float sceneH,
                              QObject *parent = 0);
    ~RoadGraphicScene();

signals:
    void updateTelemetry(CarTelemetry telemetrics); //!< Обновление данных положения машины.

protected:
    void keyPressEvent(QKeyEvent* keyEvent);
    void keyReleaseEvent(QKeyEvent* keyEvent);
    void timerEvent(QTimerEvent*);

private:
    void updateScene(double secondsElapsed); //!< Основной расчет параметров положения.
    //! Вызывается с интервалом, который прошел с предыдущего запуска метода.


    CarItem *_car;              //!< Графический элемент авто.
    RoadSplitItem *_roadSplit;  //!< Разделительная линия (прерывистая)
    LaneSideItem *_leftSide;    //!< Левый бордюр.
    LaneSideItem *_rightSide;   //!< Правый бордюр.
    /**
     * \brief Флаги управления, на которые влияет пользователь. Движение и поворот.
     */
    struct CarControl {
        bool m_throttleOn;  //!< Газ
        bool m_brakeOn;     //!< Тормоз
        bool m_reverseMove; //!< Задний ход
        bool m_leftRotate;  //!< Поворот влево
        bool m_rightRotate; //!< Поворот вправо
        CarControl();
    } m_control;
    CarMovement m_movement;

    qint64 m_lastFrameTime; //!< Время последнего обработанного фрейма.
    float m_splitHeight;    //!< Полная высота разделительной линии.
    float m_sceneH;         //!< Высота сцены.
    float m_worldOffset;    //!< Смещение объектов сцены при приближении к краям.
    float m_laneWidth;      //!< Ширина полосы.
    QPointF m_splitPosInitial;//!< Начальное положение разделительной линии.


};

#endif // ROADGRAPHICSCENE_H
