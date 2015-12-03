#include "GraphicsItems.h"
#include <QBrush>

CarItem::CarItem(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
   static const float carLength = 100.0;
   static const float carWidgth = 50.0;
   static const float rotationPointLengthRatio = 0.45;

   // рисуем прямоугольник - машину.
   QGraphicsRectItem *rect =   new QGraphicsRectItem(this);
   rect->setRect(0,0, carLength, carWidgth);
   rect->setBrush(QBrush(Qt::white));
   addToGroup(rect);


   // следующие три вызова - рисуем в прямоугольнике стрелочку из трех линий.
   addToGroup(new QGraphicsLineItem(
                  QLineF(
                      QPointF(carLength * 0.9, carWidgth / 2),
                      QPointF(carLength * 0.5, carWidgth / 2)
                      ),
                  this));

   addToGroup(new QGraphicsLineItem(
                  QLineF(
                      QPointF(carLength * 0.9, carWidgth / 2),
                      QPointF(carLength * 0.7, carWidgth * 0.75)
                      ),
                  this));

   addToGroup(new QGraphicsLineItem(
                  QLineF(
                      QPointF(carLength * 0.9, carWidgth / 2),
                      QPointF(carLength * 0.7, carWidgth * 0.25)
                      ),
                  this));

   // стоп-сигнал. Когда мы едем назад.
   _backLights =  new QGraphicsRectItem(this);
   _backLights->setVisible(false);
   _backLights->setRect(0, carWidgth* 0.1, carLength*0.05, carWidgth*0.8);
   _backLights->setBrush(QBrush(QColor(Qt::darkRed)));

   setTransformOriginPoint(carLength* rotationPointLengthRatio, carWidgth / 2);
}

void CarItem::setReverseMode(bool reverse)
{
    _backLights->setVisible(reverse);
}

RoadSplitItem::RoadSplitItem(const QList<float> &patternConfiguration, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent), m_patternRepeatCount(3)
{
    bool isSplitLine = true;
    static const float splitLineWidth = 5.0;
    float offset=0;
    for (int i=0; i<m_patternRepeatCount; ++i) {
        foreach (float length, patternConfiguration) {
            // проходим по всей конфигурации, и создаем прямоугольники если это линия, а не разрыв.
            if (isSplitLine) {
                QGraphicsRectItem *rect =   new QGraphicsRectItem(this);
                QRectF splitRect(0,offset, splitLineWidth, length );
                rect->setRect(splitRect);
                rect->setBrush(QBrush(QColor(Qt::black)));
                addToGroup(rect);
                m_splitLineRects << splitRect;
            }
            isSplitLine = !isSplitLine;
            offset += length;
        }
    }
    m_patternLength = offset / m_patternRepeatCount;
}

float RoadSplitItem::patternLength() const
{
    return m_patternLength;
}

float RoadSplitItem::fullLength() const
{
    return m_patternLength * m_patternRepeatCount;
}

float RoadSplitItem::calculateIntersectionRatio(QRectF boundingRect)
{
    foreach (QRectF myRect, m_splitLineRects) {
        // пройдем по всем прямоугольникам сплошной линии, и если есть пересечение,
        // рассчитаем его отношение к проекции авто (boundingRect).
        myRect.translate(this->pos());
        QRectF intersection = myRect.intersected(boundingRect);
        if (intersection.height() > 0 && intersection.width() > 4) {
            return intersection.height() / boundingRect.height();
        }
    }

    return 0.0;
}

LaneSideItem::LaneSideItem(float length, QGraphicsItem *parent)
    : QGraphicsRectItem(QRectF(0, 0, 2.0, length),parent)
{
    setBrush(QBrush(QColor(Qt::black)));
}
