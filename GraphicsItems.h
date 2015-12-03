#ifndef GRAPHICSITEMS_H
#define GRAPHICSITEMS_H

#include <QGraphicsItemGroup>

/**
 * \brief Графический элемент авто.
 *
 * Состоит из прямоугольника, трех линий-стрелочки,
 * а также красного прямоугольника заднего хода.
 */
class CarItem : public QGraphicsItemGroup
{
    QGraphicsRectItem* _backLights;
public:
    explicit CarItem(QGraphicsItem* parent=0);
    void setReverseMode(bool reverse);  //!< отображает сигналы заднего хода.
};

/**
 * \brief Разделительная полоса.
 *
 * Состоит из 3 повторов набора сегментов. Конфигурация идет следующим образом:
 * -четные элементы (с 0) - сплошная линия
 * -нечетные элементы (с 1) - разрыв.
 */
class RoadSplitItem : public QGraphicsItemGroup
{
    float m_patternLength;
    const int m_patternRepeatCount;
    QList<QRectF> m_splitLineRects;
public:
    explicit RoadSplitItem(const QList<float> &patternConfiguration, QGraphicsItem* parent=0);
    float patternLength() const;
    float fullLength() const;
    float calculateIntersectionRatio(QRectF boundingRect); //!< Возвращает отношение пересечения авто и разделительной линии
    //! к проекции авто на разделительную полосу

};
/**
 * \brief Бордюр полосы. Несет просто декоративную функцию.
 */
class LaneSideItem : public QGraphicsRectItem
{
public:
    explicit LaneSideItem(float length, QGraphicsItem* parent=0);

};

#endif // GRAPHICSITEMS_H
