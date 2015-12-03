#include "RoadGraphicScene.h"

#include <QKeyEvent>
#include <QDateTime>
#include <QDebug>

RoadGraphicScene::RoadGraphicScene(const QList<float> &patternConfiguration,
                                   float sceneW,
                                   float sceneH,
                                   QObject *parent)
    : QGraphicsScene(parent)
    , m_sceneH(sceneH)
    , m_worldOffset(0.0)
{
    const int sceneFrameTimerIntervalMS = 33;

    // элемент авто
    _car = new CarItem();
    _car->setZValue(10);
    this->addItem(_car);

    // разделительная
    _roadSplit = new RoadSplitItem(patternConfiguration);

    m_splitHeight = _roadSplit->fullLength();
    this->addItem(_roadSplit);
    _roadSplit->setY(-m_splitHeight/2);
    m_splitPosInitial = _roadSplit->pos();

    // бордюры
    _leftSide= new LaneSideItem(m_splitHeight);
    _rightSide= new LaneSideItem(m_splitHeight);
    m_laneWidth= sceneW / 6;

    _leftSide->setY(-m_splitHeight/2);
    _rightSide->setY(-m_splitHeight/2);
    _leftSide->setX(-m_laneWidth);
    _rightSide->setX(+m_laneWidth);

    this->addItem(_leftSide);
    this->addItem(_rightSide);

    // таймер который будет производить расчеты сцены
    startTimer(sceneFrameTimerIntervalMS);

    m_lastFrameTime = QDateTime::currentMSecsSinceEpoch();

    QRectF sceneR(-sceneW/2, -sceneH/2, sceneW, sceneH);
    sceneR.adjust(1,1,-2,-2); // избежим скролбаров во view при точной подгонке.
    this->setSceneRect(sceneR);
}

RoadGraphicScene::~RoadGraphicScene()
{
    delete _car;
    delete _roadSplit;
    delete _leftSide;
    delete _rightSide;
}

void RoadGraphicScene::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key()){
        case Qt::Key_Up:
            m_control.m_throttleOn = true;
            break;
        case Qt::Key_Down:
           m_control.m_brakeOn = true;
           break;
        case Qt::Key_Left:
           m_control.m_leftRotate = true;
           break;
        case Qt::Key_Right:
           m_control.m_rightRotate = true;
           break;
        case Qt::Key_R:
            if (!m_movement.isMoving()) { // epsilon-сравнение делается в функции обработки движения.
                m_control.m_reverseMove = !m_control.m_reverseMove;
            }
           break;
        default:
            break;
    }

   // мы не будем вызывать родительский метод keyPressEvent
}

void RoadGraphicScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key()){
        case Qt::Key_Up:
            m_control.m_throttleOn = false;
            break;
        case Qt::Key_Down:
           m_control.m_brakeOn = false;
           break;
        case Qt::Key_Left:
           m_control.m_leftRotate = false;
           break;
        case Qt::Key_Right:
           m_control.m_rightRotate = false;
           break;
        default:
            break;
    }

   // мы не будем вызывать родительский метод keyReleaseEvent
}

void RoadGraphicScene::timerEvent(QTimerEvent *)
{
    qint64 frameTime = QDateTime::currentMSecsSinceEpoch();
    qint64 msecsElapsed = frameTime - m_lastFrameTime;
    m_lastFrameTime = frameTime;
    updateScene(msecsElapsed / 1000.);
}

void RoadGraphicScene::updateScene(double secondsElapsed)
{
    // сперва расчитаем реальное действие от нажатий кнопок.
    // например, одновременно нажатые "вправо" и "влево" не несут эффекта.
    CarMovement::VelocityMode velMode = CarMovement::vmNeutral;
    CarMovement::RotationMode rotMode = CarMovement::rmOrdinal;
    if (m_control.m_throttleOn && !m_control.m_brakeOn) {
        velMode = CarMovement::vmThrottle;
        rotMode = CarMovement::rmNeutral;
    }else if (!m_control.m_throttleOn &&  m_control.m_brakeOn) {
        velMode = CarMovement::vmBrake;
        rotMode = CarMovement::rmNeutral;
    }
    if (m_control.m_leftRotate && !m_control.m_rightRotate) {
        rotMode = CarMovement::rmLeft;
    }else if (!m_control.m_leftRotate &&  m_control.m_rightRotate) {
        rotMode = CarMovement::rmRight;
    }

    // теперь сделаем расчеты в объекте движения автомобиля.
    m_movement.applyVelocity(velMode);
    m_movement.applyRotation(rotMode, secondsElapsed);
    m_movement.applySpeed(secondsElapsed, m_control.m_reverseMove);


    // код ниже осуществляет сдвиг сцены вверх либо вниз, при приближении к краям.
    float patternLength = _roadSplit->patternLength();
    QPointF carPos      = m_movement.carPos();
    QPointF carPosOrig  = carPos;
    float yUnderflow    = -m_sceneH*0.4 - carPos.y();
    float yOverflow     = carPos.y() - m_sceneH*0.4;

    yUnderflow -= m_worldOffset;
    yOverflow += m_worldOffset;
    if (yUnderflow > 0) {
        m_worldOffset += yUnderflow;
    }
    if (yOverflow > 0) {
        m_worldOffset -= yOverflow;
    }

    carPos.ry() += m_worldOffset;
    if (m_worldOffset > patternLength) {
        m_worldOffset -= patternLength;
    }
    if (m_worldOffset < -patternLength) {
        m_worldOffset += patternLength;
    }

    // выставление видимой позиции авто.
    _car->setPos(carPos);
    _car->setRotation(m_movement.carAngle());
    _car->setReverseMode(m_control.m_reverseMove);

    // выставление позиции разделительной линии с учетом смещения.
    QPointF splitPos = m_splitPosInitial;
    splitPos.ry() += m_worldOffset;
    _roadSplit->setPos(splitPos);

    // расчет реального положения авто с учетом поворота.
    QRectF carRect = _car->boundingRect();
    QTransform transform;
    transform.rotate(m_movement.carAngle(),Qt::ZAxis);
     carRect = transform.mapRect(carRect);
    carRect.moveTo(carPos);

    // расчет показателей телеметрии - положения авто относительно окружающих объектов.
    CarTelemetry telemetrics;
    telemetrics.m_performingTurn = CarMovement::rmLeft == rotMode ||CarMovement::rmRight == rotMode  ;
    telemetrics.m_angle = m_movement.carAngle();
    if (m_control.m_reverseMove) {
        telemetrics.m_angle = -telemetrics.m_angle;
    }
    telemetrics.m_sideRatio = 0.0;

    if (carRect.left() >= 0 && carRect.right() < m_laneWidth) {
        telemetrics.m_sideRatio = 1.0; // правая полоса.
    }else if (carRect.right() <= 0 && carRect.left() > -m_laneWidth) {
        telemetrics.m_sideRatio = -1.0; // левая полоса.
    }else if ( carRect.right() >= m_laneWidth) {
        telemetrics.m_sideRatio = 2.0; // обочина, не будем вычислять конкретную долю.
    }else if ( carRect.left() <= -m_laneWidth) {
        telemetrics.m_sideRatio = -2.0; // обочина, не будем вычислять конкретную долю.
    }else {
        float centerPosInBoundsRatio = -carRect.left() / carRect.width();
        telemetrics.m_sideRatio = 1 - centerPosInBoundsRatio * 2;
    }
    telemetrics.m_splitCollision = _roadSplit->calculateIntersectionRatio(carRect);
    telemetrics.m_pos = carPosOrig;
    telemetrics.m_speed = m_movement.carSpeed();
    emit updateTelemetry(telemetrics);

}

RoadGraphicScene::CarControl::CarControl()
    : m_throttleOn(false)
    , m_brakeOn(false)
    , m_reverseMove(false)
    , m_leftRotate(false)
    , m_rightRotate(false)
{

}
